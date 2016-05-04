#include<memory>

#include<thread>
#include<future>

#include "vxScene.h"

namespace vxCore{

class vxScene;

vxScene::vxScene(std::shared_ptr<ImageProperties> prop)
	: m_properties(prop)
{
	m_broadPhase = std::make_shared<vxBroadPhase>();
}

vxScene::~vxScene()
{
}

std::vector<std::shared_ptr<vxGrid> >& vxScene::grids()
{
	return m_grids;
}

void vxScene::build(std::shared_ptr<vxSceneParser> nodeDB)
{
	buildDefaultShader();
	
	for(const auto node: nodeDB->getNodesByType("vxDirectLight"))
	{
		auto direct = createDirectLight();
		direct->setIntensity(node->getFloatAttribute("intensity"));
		direct->setColor(vxColor::lookup256(node->getColorAttribute("color")));
		direct->setSamples(node->getIntAttribute("samples"));
		
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
		ambient->setSamples(node->getIntAttribute("samples"));
		
		const auto transform = node->getMatrixAttribute("transform");
		ambient->setTransform(transform);
	}
	
	for(const auto node: nodeDB->getNodesByType("vxCamera"))
	{
		m_camera = std::make_shared<vxCamera>(m_properties);
		
		const auto fDistance = node->getFloatAttribute("focusDistance");
		const auto hAperture = node->getFloatAttribute("horizontalAperture");
		const auto vAperture = node->getFloatAttribute("verticalAperture");
		
		m_camera->set(v3s::zero,
					  v3s::constZ,
					  fDistance,
					  hAperture,
					  vAperture);
	}
	
	for(const auto node: nodeDB->getNodesByType("vxPointLight"))
	{
		auto point = createPointLight();
		point->setIntensity(node->getFloatAttribute("intensity"));
		point->setColor(vxColor::lookup256(node->getColorAttribute("color")));
		point->setSamples(node->getIntAttribute("samples"));
		point->setComputeShadows(node->getBoolAttribute("castShadows"));
		
		const auto transform = node->getMatrixAttribute("transform");
		point->setTransform(transform);
	}
	
	for(const auto node: nodeDB->getNodesByType("vxAreaLight"))
	{
		auto area = createAreaLight();
		area->setIntensity(node->getFloatAttribute("intensity"));
		area->setColor(vxColor::lookup256(node->getColorAttribute("color")));
		area->setSamples(node->getIntAttribute("samples"));
		
		area->setMinX(node->getFloatAttribute("minX"));
		area->setMinY(node->getFloatAttribute("minY"));
		
		area->setMaxX(node->getFloatAttribute("maxX"));
		area->setMaxY(node->getFloatAttribute("maxY"));
		
		const auto transform = node->getMatrixAttribute("transform");
		area->setTransform(transform);
	}
	
	for(const auto node: nodeDB->getNodesByType("vxGrid"))
	{
		const auto resolution = node->getIntAttribute("resolution");
		const auto transform = node->getMatrixAttribute("transform");
		const auto color = vxColor::lookup256(node->getColorAttribute("color"));
		const auto size = node->getFloatAttribute("size");
		
		auto grid = std::make_shared<vxGrid>(transform.getOrigin(), size);
		grid->setResolution(resolution);
		grid->setTransform(transform);
		
		grid->setBaseColor(color);
		m_grids.emplace_back(grid);
		m_geometries.emplace_back(grid);
		m_broadPhase->addGeometry(grid);
		
		for(const auto nodeGeo: nodeDB->getNodesByType("vxGridGeometry"))
		{
			const auto path = nodeGeo->getStringAttribute("filePath");
			const auto transform = nodeGeo->getMatrixAttribute("transform");
			
			auto grid_geo = std::make_shared<vxTriangleMesh>();
			grid_geo->setTransform(transform);
			grid_geo->setConstructionPath(path);
			
			auto plyReader = std::make_shared<vxPLYImporter>(grid_geo);
			plyReader->processPLYFile(path);
			grid->addGeometry(grid_geo);
		}
		
		//grid->createGround(0, (unsigned char)4u);
		//grid->createEdges((unsigned char)12u);
		//grid->createRandom(.004,0.85);
		//grid->dumpFileInMemory("/home/mario/Downloads/xyzrgb_statuette_1.ply");
		
		auto na = grid->numActiveVoxels();
		auto totals = grid->getNumberOfVoxels();
		std::cout << "Number of active voxels " << na << " of " << totals << std::endl;
	}
	
	for(const auto node: nodeDB->getNodesByType("vxDome"))
	{
		auto dome = createDom(node->getStringAttribute("imagePath"));
		dome->setGain(node->getFloatAttribute("gain"));
		dome->setGamma(node->getFloatAttribute("gamma"));
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
	
	for(const auto node: nodeDB->getNodesByType("vxGeometry"))
	{
		const auto path = node->getStringAttribute("filePath");
		const auto transform = node->getMatrixAttribute("transform");
		
		auto geo = createGeometry(path, transform);
		geo->setBaseColor(vxColor::lookup256(node->getColorAttribute("color")));
		geo->setTransform(transform);
		
		auto plyReader = std::make_shared<vxPLYImporter>(geo);
		plyReader->processPLYFile(path);
	}
	
	for(const auto node: nodeDB->getNodesByType("vxClock"))
	{
		vxClock::setStart( node->getFloatAttribute("start"));
		vxClock::setEnd( node->getFloatAttribute("end"));
		vxClock::setStep( node->getFloatAttribute("step"));
	}
	
	updateCache();
	//Update caches
	std::cout << " -- Finished building process scene -- " << std::endl;
}

void vxScene::updateCache()
{
	std::cout << " -- Start caching -- " << std::endl;

	m_broadPhase->updateCache();

	std::cout << " -- End cache computation -- " << std::endl;
}

void vxScene::setShader(std::shared_ptr<vxShader> shader)
{
	m_shader = shader;
}

void vxScene::setCamera(const std::shared_ptr<vxCamera> &camera)
{
	m_camera = camera;
}

void vxScene::buildDefaultShader()
{
	m_shader = std::make_shared<vxLambert>();
	m_shader->setLights(&m_lights);
	m_shader->setScene(shared_from_this());
}

std::shared_ptr<vxAreaLight> vxScene::createAreaLight()
{
	auto area = std::make_shared<vxAreaLight>();
	m_areaLights.emplace_back(area);
	m_lights.emplace_back(area);
	area->setScene(shared_from_this());
	return area;
	
}

std::shared_ptr<vxPointLight> vxScene::createPointLight()
{
	auto pl1 = std::make_shared<vxPointLight>(1.0, vxColor::white);
	m_pointLights.emplace_back(pl1);
	m_lights.emplace_back(pl1);
	pl1->setScene(shared_from_this());
	return pl1;
}

std::shared_ptr<vxIBLight> vxScene::createIBLight(const std::string path)
{
	auto ibl1 = std::make_shared<vxIBLight>(1.0, path);
	m_IBLights.emplace_back(ibl1);
	m_lights.emplace_back(ibl1);
	ibl1->setScene(shared_from_this());
	return ibl1;
}

std::shared_ptr<vxDirectLight> vxScene::createDirectLight()
{
	auto dl1 = std::make_shared<vxDirectLight>(1.0, vxColor::white);
	m_directLights.emplace_back(dl1);
	m_lights.emplace_back(dl1);
	dl1->setScene(shared_from_this());
	return dl1;
}

std::shared_ptr<vxAmbientLight> vxScene::createAmbientLight()
{
	auto al1 = std::make_shared<vxAmbientLight>(1.0, vxColor::white);
	m_ambientLights.emplace_back(al1);
	m_lights.emplace_back(al1);
	al1->setScene(shared_from_this());
	return al1;
}

std::shared_ptr<vxDome> vxScene::createDom(const std::__cxx11::string path)
{
	auto image = createImage(path);
	auto dom = std::make_shared<vxDome>(image);
	m_domes.emplace_back(dom);
	return dom;
}

std::shared_ptr<vxPlane> vxScene::createPlane(vxPlane::type type)
{
	auto plane = std::make_shared<vxPlane>(type);
	m_planes.emplace_back(plane);
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
	m_bitMaps.emplace_back(image);
	return image;
}

vxTriangleMeshHandle vxScene::createGeometry(const std::string &path, const Matrix44 &transform)
{
	// looking for previouly processed meshes.
	for(const auto& geo: m_triangleMeshes)
	{
		if(geo->constructionPath()==path && geo->transform()==transform)
		{
			return geo;
		}
	}
	
	//auto found = std::find_first_of(m_geometries.begin(), m_geometries.end(),
	//	[&path](vxGeometryHandle g){ return g->constructionPath()==path;});
	
	auto geo = std::make_shared<vxTriangleMesh>();
	geo->setTransform(transform);
	geo->setConstructionPath(path);
	m_broadPhase->addGeometry(geo);
	
	m_geometries.emplace_back(geo);
	
	return geo;
}

std::shared_ptr<ImageProperties> vxScene::imageProperties() const
{
	return m_properties;
}

void vxScene::setImageProperties(const std::shared_ptr<ImageProperties> &prop)
{
	m_properties = prop;
}

std::shared_ptr<ImageProperties> vxScene::properties() const
{
	return m_properties;
}

void vxScene::setProperties(const std::shared_ptr<ImageProperties> &properties)
{
	m_properties = properties;
}

std::shared_ptr<vxCamera> vxScene::camera() const
{
	return m_camera;
}

std::shared_ptr<vxDome> vxScene::dome() const
{
	if(m_domes.size())
		return m_domes[0];

	return std::shared_ptr<vxDome>();
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
	if(m_broadPhase->throwRay(ray,collide))
	{
		collide.setValid(true);
		
		return 1;
	}
	
	collide.setValid(false);
	return 0;
}

bool vxScene::hasCollision(const vxRay &ray) const
{
	vxCollision col;
	return throwRay(ray, col)==1;
}

std::shared_ptr<vxShader> vxScene::defaultShader() const
{
	return m_shader;
}



}
