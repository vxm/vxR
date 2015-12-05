#include<memory>

#include<thread>
#include<future>

#include "vxScene.h"

namespace vxCore{
class vxScene;


vxScene::vxScene(std::shared_ptr<ImageProperties> prop)
	: m_prop(prop)
{}

vxScene::~vxScene()
{}

void vxScene::build(std::shared_ptr<vxSceneParser> nodeDB)
{
	for(const auto node: nodeDB->getNodesByType("vxDirectLight"))
	{
		auto direct = createDirectLight();
		direct->setIntensity(node->getFloatAttribute("intensity"));
		direct->setColor(vxColor::lookup256(node->getColorAttribute("color")));
		direct->setOrientation(node->getVector3dAttribute("orientation"));
		
		std::string cast = node->getStringAttribute("castShadows");
		direct->setComputeShadows(cast == "true"s);
	}

	for(const auto node: nodeDB->getNodesByType("vxIBLight"))
	{
		auto env = createIBLight(node->getStringAttribute("filePath"));
		env->setIntensity(node->getFloatAttribute("intensity"));
		env->setColor(vxColor::lookup256(node->getColorAttribute("color")));

		env->setSamples(node->getIntAttribute("samples"));
		env->setRadius(node->getFloatAttribute("radius"));
		env->setGain(node->getFloatAttribute("gain"));
		env->setGamma(node->getFloatAttribute("gamma"));
		env->setLowThreshold(node->getFloatAttribute("lowThreshold"));
	}

	for(const auto node: nodeDB->getNodesByType("vxAmbientLight"))
	{
		auto ambient = createAmbientLight();
		ambient->setIntensity(node->getFloatAttribute("intensity"));
		ambient->setColor(vxColor::lookup256(node->getColorAttribute("color")));
	}
	
	for(const auto node: nodeDB->getNodesByType("vxCamera"))
	{
		m_camera = std::make_shared<vxCamera>(m_prop);

		const auto fDistance = node->getFloatAttribute("focusDistance");
		const auto hAperture = node->getFloatAttribute("horizontalAperture");
		const auto vAperture = node->getFloatAttribute("verticalAperture");

		m_camera->set(v3::zero,
						v3::constZ,
						fDistance,
						hAperture,
						vAperture);
	}
	
	for(const auto node: nodeDB->getNodesByType("vxPointLight"))
	{
		auto point = createPointLight();
		point->setIntensity(node->getFloatAttribute("intensity"));
		point->setColor(vxColor::lookup256(node->getColorAttribute("color")));
		const auto transform = node->getMatrixAttribute("transform");
		point->setTransform(transform);
	}
	
	for(const auto node: nodeDB->getNodesByType("vxGrid"))
	{
		const auto resolution = node->getIntAttribute("resolution");
		const auto pos = node->getVector3dAttribute("position");
		const auto size = node->getFloatAttribute("size");

		// this is a hardcode program to test the rays.
		//TODO:get rid of this hard-coded values.
		m_grids.push_back(std::make_shared<vxGrid>(pos, size));
		m_grids[0]->setResolution(resolution);

		for(const auto nodeGeo: nodeDB->getNodesByType("vxGridGeometry"))
		{
			const auto path = nodeGeo->getStringAttribute("filePath");
			const auto pos = nodeGeo->getVector3dAttribute("position");
			const auto sf = nodeGeo->getFloatAttribute("scaleFactor");
			const auto transform = nodeGeo->getMatrixAttribute("transform");
			
			auto geo = createGeometry(path, transform);
			auto plyReader = std::make_shared<vxPLYImporter>(geo);
			plyReader->processPLYFile(path);
			m_grids[0]->addGeometry(geo,
									pos,
									v3(resolution * sf,
									   resolution * sf,
									   resolution * sf) );
			m_grids[0]->createGround();
			//m_grids[0]->createEdges();

		}
		
		auto na = m_grids[0]->numActiveVoxels();
		auto totals = m_grids[0]->getNumberOfVoxels();
		std::cout << "Number of active voxels " << na << " of " << totals << std::endl;
	}

	for(const auto node: nodeDB->getNodesByType("vxDome"))
	{
		createDom(node->getStringAttribute("imagePath"));
	}
	
	for(const auto node: nodeDB->getNodesByType("vxPlane"))
	{
		const auto planeTypeName = node->getStringAttribute("planeType");
		vxPlane::type planeType = vxPlane::type::kFree;
		if(planeTypeName=="X"s)
		{
			planeType = vxPlane::type::kX;
		}
		if(planeTypeName=="Y"s)
		{
			planeType = vxPlane::type::kY;
		}
		if(planeTypeName=="z"s)
		{
			planeType = vxPlane::type::kZ;
		}
		if(planeTypeName=="free"s)
		{
			planeType = vxPlane::type::kFree;
			
		}
		auto plane = createPlane(planeType);
		
		//geometry color.
		plane->setColor(vxColor::lookup256(node->getColorAttribute("color")));

		plane->setPointA(node->getVector3dAttribute("pointA"));
		plane->setPointB(node->getVector3dAttribute("pointB"));
		plane->setPointC(node->getVector3dAttribute("pointC"));
		
		plane->setX(node->getFloatAttribute("x"));
		plane->setY(node->getFloatAttribute("y"));
		plane->setZ(node->getFloatAttribute("z"));
	}

	for(const auto nodeGeo: nodeDB->getNodesByType("vxGeometry"))
	{
		const auto path = nodeGeo->getStringAttribute("filePath");
		const auto transform = nodeGeo->getMatrixAttribute("transform");

		auto geo = createGeometry(path, transform);
		geo->setTransform(transform);

		auto plyReader = std::make_shared<vxPLYImporter>(geo);
		plyReader->processPLYFile(path);
	}
	
	m_shader = std::make_shared<vxLambert>();
	m_shader->setLights(&m_lights);
	m_shader->setScene(shared_from_this());
	
	std::cout << " -- Finished building process scene -- " << std::endl;
}

void vxScene::setShader(std::shared_ptr<vxShader> shader)
{
	m_shader = shader;
}

std::shared_ptr<vxCamera> vxScene::defaultCamera() const
{
	return m_camera;
}

void vxScene::setCamera(const std::shared_ptr<vxCamera> &camera)
{
	m_camera = camera;
}

std::shared_ptr<vxPointLight> vxScene::createPointLight()
{
	auto pl1 = std::make_shared<vxPointLight>(1.0, vxColor::white);
	m_pointLights.push_back(pl1);
	m_lights.push_back(pl1);
	pl1->setScene(shared_from_this());
	return pl1;
}

std::shared_ptr<vxIBLight> vxScene::createIBLight(const std::string path)
{
	auto ibl1 = std::make_shared<vxIBLight>(1.0, path);
	m_IBLights.push_back(ibl1);
	m_lights.push_back(ibl1);
	ibl1->setScene(shared_from_this());
	return ibl1;
}

std::shared_ptr<vxDirectLight> vxScene::createDirectLight()
{
	auto dl1 = std::make_shared<vxDirectLight>(1.0, vxColor::white);
	m_directLights.push_back(dl1);
	m_lights.push_back(dl1);
	dl1->setScene(shared_from_this());
	return dl1;
}

std::shared_ptr<vxAmbientLight> vxScene::createAmbientLight()
{
	auto al1 = std::make_shared<vxAmbientLight>(1.0, vxColor::white);
	m_ambientLights.push_back(al1);
	m_lights.push_back(al1);
	al1->setScene(shared_from_this());
	return al1;
}

std::shared_ptr<vxDome> vxScene::createDom(const std::__cxx11::string path)
{
	auto image = createImage(path);
	auto dom = std::make_shared<vxDome>(image);
	m_domes.push_back(dom);
	return dom;
}

std::shared_ptr<vxPlane> vxScene::createPlane(vxPlane::type type)
{
	auto plane = std::make_shared<vxPlane>(type);
	m_planes.push_back(plane);
	return plane;
}

std::shared_ptr<vxBitMap2d> vxScene::createImage(const std::__cxx11::string path)
{
	// looking for previouly opened images.
	for(const auto& img: m_bitMaps)
	{
		if(img->path()==path)
		{
			return img;
		}
	}
	
	auto image = std::make_shared<vxBitMap2d>(path);
	m_bitMaps.push_back(image);
	return image;
}

vxGeometryHandle vxScene::createGeometry(const std::string &path, const vxMatrix &transform)
{
	// looking for previouly opened images.
	for(const auto& geo: m_geometries)
	{
		if(geo->constructionPath()==path && geo->transform()==transform)
		{
			return geo;
		}
	}
	
	//auto found = std::find_first_of(m_geometries.begin(), m_geometries.end(),
	//	[&path](vxGeometryHandle g){ return g->constructionPath()==path;});
	
	auto geo = std::make_shared<vxGeometry>();
	geo->setTransform(transform);
	m_geometries.push_back(geo);
	m_broadPhase.addGeometry(geo);
	geo->setConstructionPath(path);
	
	
	return geo;
}

std::shared_ptr<ImageProperties> vxScene::imageProperties() const
{
	return m_prop;
}

void vxScene::setImageProperties(const std::shared_ptr<ImageProperties> &prop)
{
	m_prop = prop;
}



bool vxScene::throwRay(const vxRay &ray) const
{
	vxCollision col;
	return throwRay(ray, col)==1;
}

int vxScene::domeThrowRay(const vxRay &ray, vxCollision &collide) const
{
	if(m_domes.size())
	{
		m_domes[0]->throwRay(ray, collide);
	}
	
	return 1;
}

int vxScene::throwRay(const vxRay &ray, vxCollision &collide) const
{
	if(m_broadPhase.throwRay(ray,collide))
	{
		vxColor col(defaultShader()->getColor(ray,collide));
		collide.setColor( col );
		collide.setValid(true);

		return 1;
	}
	
	return 0;
}

bool vxScene::hasCollision(const vxRay &ray) const
{
	if(m_broadPhase.hasCollision(ray))
	{
		return true;
	}
	
	return false;
}

std::shared_ptr<vxShader> vxScene::defaultShader() const
{
	return m_shader;
}



}
