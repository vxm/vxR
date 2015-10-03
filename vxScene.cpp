#include<memory>

#include<thread>
#include<future>

#include "vxScene.h"

namespace vxCore{
class vxScene;

//plyReader->processPLYFile("../vxR/juan_0.ply");
//plyReader->processPLYFile("/home/john/Downloads/statue_and_dog_1.ply");
//plyReader->processPLYFile("/home/john/Downloads/dragon_1.ply");
//plyReader->processPLYFile("/home/john/Downloads/vmilo_0.ply");
//plyReader->processPLYFile("/home/john/Downloads/lucyTop_1.ply");
///home/john/code/vxR/EtniesPark_Center/Etnies_Park_Center_8k.jpg
///home/john/code/vxR/Ditch_River/Ditch-River_TMap.jpg
///home/john/code/vxR/Basketball_Court/BasketballCourt_8k.jpg 
///home/john/code/vxR/Milkyway/Milkyway_BG.jpg

vxScene::vxScene(std::shared_ptr<ImageProperties> prop)
	: m_prop(prop)
{}

vxScene::~vxScene()
{}

void vxScene::build(std::shared_ptr<vxSceneParser> nodeDB)
{
	//const auto sunCoords = vxVector2d{-13.022000, -10.1000};
	int nLightSamples{20};

	for(const auto dlNode: nodeDB->getNodesByType("vxDirectLight"))
	{
		auto directLight = createDirectLight();

		const auto intensity = dlNode->getFloatAttribute("intensity");
		directLight->setIntensity(intensity);
		
		const auto color = dlNode->getColorAttribute("color");
		directLight->setColor(vxColor::lookup256(color));
		
		const auto&& orienentation = dlNode->getVector3dAttribute("orientation");
		directLight->setOrientation(orienentation);
	}

	for(const auto iblNode: nodeDB->getNodesByType("vxIBLight"))
	{
		const auto path = iblNode->getStringAttribute("filePath");

		//Environment tint.
		auto envLight = createIBLight( path );

		const auto intensity = iblNode->getFloatAttribute("intensity");
		envLight->setIntensity(intensity);

		const auto color = iblNode->getColorAttribute("color");
		envLight->setColor(vxColor::lookup256(color));

		const auto samples = iblNode->getIntAttribute("samples");
		envLight->setSamples(nLightSamples);
		
		const auto radius = iblNode->getFloatAttribute("radius");
		envLight->setRadius(radius);
	}

	for(const auto ambLNode: nodeDB->getNodesByType("vxAmbientLight"))
	{
		auto ambLight = createAmbientLight();

		const auto intensity = ambLNode->getFloatAttribute("intensity");
		ambLight->setIntensity(intensity);

		const auto color = ambLNode->getColorAttribute("color");
		ambLight->setColor(vxColor::lookup256(color));
	}
	
	for(const auto cameraNode: nodeDB->getNodesByType("vxCamera"))
	{
		m_camera = std::make_shared<vxCamera>(m_prop);

		const auto fDistance = cameraNode->getFloatAttribute("focusDistance");
		const auto hAperture = cameraNode->getFloatAttribute("horizontalAperture");
		const auto vAperture = cameraNode->getFloatAttribute("verticalAperture");

		m_camera->set(vxVector3d::zero,
						vxVector3d::constZ,
						fDistance,
						hAperture,
						vAperture);
	}
	
	for(const auto gridNode: nodeDB->getNodesByType("vxGrid"))
	{
		const auto resolution = gridNode->getIntAttribute("resolution");
		const auto pos = gridNode->getVector3dAttribute("position");
		const auto size = gridNode->getIntAttribute("size");

		// this is a hardcode program to test the rays. 
		//TODO:get rid of this hard-coded values.
		m_grids.push_back(std::make_shared<vxGrid>(pos, size));
		m_grids[0]->setResolution(resolution);

		for(const auto gridGeometry: nodeDB->getNodesByType("vxGridGeometry"))
		{
			const auto path = gridGeometry->getStringAttribute("filePath");
			const auto pos = gridGeometry->getVector3dAttribute("position");
			const auto sf = gridGeometry->getFloatAttribute("scaleFactor");
			
			auto plyReader = std::make_shared<vxPLYImporter>();
			plyReader->processPLYFile(path);
			const auto& vts = plyReader->getPointCloud();
			m_grids[0]->addVertices(vts, pos, vxVector3d(resolution * sf,
											   resolution * sf,
											   resolution * sf) );
			m_grids[0]->createGround();
		}
		
		auto na = m_grids[0]->numActiveVoxels();
		auto totals = m_grids[0]->getNumberOfVoxels();
		std::cout << "Number of active voxels " << na << " of " << totals << std::endl;
	}

	for(const auto domNode: nodeDB->getNodesByType("vxDom"))
	{
		const auto path = domNode->getStringAttribute("imagePath");
		createDom(path);
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

std::shared_ptr<vxDom> vxScene::createDom(const std::__cxx11::string path)
{
	auto image = createImage(path);
	auto dom = std::make_shared<vxDom>(image);

	m_doms.push_back(dom);
	return dom;
}

std::shared_ptr<vxBitMap2d> vxScene::createImage(const std::__cxx11::string path)
{
	for(auto img: m_bitMaps)
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

std::shared_ptr<ImageProperties> vxScene::imageProperties() const
{
	return m_prop;
}

void vxScene::setImageProperties(const std::shared_ptr<ImageProperties> &prop)
{
	m_prop = prop;
}

bool vxScene::loadFromFile(std::shared_ptr<vxImporter> importer)
{
	return true;
}

int vxScene::throwRay(const vxRay &ray) const
{
	return m_grids[0]->throwRay(ray);
}


int vxScene::throwRay(const vxRay &ray,
						vxCollision &collide) const
{
	if(m_grids[0]->throwRay(ray, collide))
	{
		vxColor col(defaultShader()->getColor(collide));
		collide.setColor( col );
		collide.setValid(true);
		return 1;
	}

//	TODO:take this to a dommo object or something like..
//	auto p = MathUtils::rectAndYPlane(ray.direction(), -110.0);
//	if(!std::signbit(p.z()))
//	{
//		collide.setNormal(vxVector3d::constY);
//		collide.setPosition(p);
//		collide.setU(fmod(p.x(),1.0));
//		collide.setV(fmod(p.z(),1.0));

//		vxColor col(defaultShader()->getColor(collide));
//		collide.setColor( col );
//		collide.setValid();
//		return 1;
//	}
//	else
	{
		if(m_doms.size())
		{
			m_doms[0]->throwRay(ray, collide);
			return 1;
		}
	}
	
	return 0;
}

bool vxScene::hasCollision(const vxRay &ray)
{
	dRays++;
	return m_grids[0]->hasCollision(ray);
}

std::shared_ptr<vxShader> vxScene::defaultShader() const
{
	return m_shader;
}

}
