#include<memory>

#include<thread>
#include<future>

#include "vxScene.h"

namespace vxCore{
class vxScene;

#define RESL 60
#define PX resl * 2
#define PY resl * 0
#define PZ resl * 2

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
	, m_environment("/home/john/code/vxR/Ditch_River/Ditch-River_TMap.jpg") //TODO: if string is empty program crashes, fix it.
{}

vxScene::~vxScene()
{}

void vxScene::build(std::shared_ptr<vxSceneParser> scn)
{
	//const auto sunCoords = vxVector2d{-13.022000, -10.1000};
	int nLightSamples{20};
	std::cout << " -- Building scene process -- " << std::endl;
	
	for(auto dlNode: scn->getNodesByType("vxDirectLight"))
	{
		auto directLight = createDirectLight();

		const auto intensity = dlNode->getFloatAttribute("intensity");
		directLight->setIntensity(intensity);
		
		const auto color = dlNode->getColorAttribute("color");
		directLight->setColor(vxColor::lookup256(color));
		
		const auto&& orienentation = dlNode->getVector3dAttribute("orientation");
		directLight->setOrientation(orienentation);
	}

	for(auto iblNode: scn->getNodesByType("vxIBLight"))
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

	auto ambLights = scn->getNodesByType("vxAmbientLight");
	for(auto ambLNode: ambLights)
	{
		auto ambLight = createAmbientLight();

		const auto intensity = ambLNode->getFloatAttribute("intensity");
		ambLight->setIntensity(intensity);

		const auto color = ambLNode->getColorAttribute("color");
		ambLight->setColor(vxColor::lookup256(color));
	}
	
	for(auto cmNode: scn->getNodesByType("vxCamera"))
	{
		m_camera = std::make_shared<vxCamera>(m_prop);
		m_camera->set( vxVector3d::zero,
						vxVector3d::constZ,
						2.4,
						hAperture,
						vAperture);
	}
	
	createCamera(vxMatrix{});
	
	m_shader = std::make_shared<vxLambert>();
	m_shader->setLights(&m_lights);
	m_shader->setScene(shared_from_this());

	//TODO: remove this debug code
	const double resl = RESL;
	vxVector3d p{PX, PY, PZ};

	
	
	createGrid();
	
	auto plyReader = std::make_shared<vxPLYImporter>();
	plyReader->processPLYFile("/home/john/Downloads/dragon_1.ply");
//	plyReader->processPLYFile("/home/john/Downloads/vmilo_0.ply");
	loadFromFile(plyReader);

	//m_grids[0]->createSphere(p.x(), p.y(), p.z(),  resl/2.0); 
	auto iRadius = 12.0;
	auto distSph = (resl/3.3);

//	m_grids[0]->fill();
//	m_grids[0]->createSphere(p.x()-distSph, p.y()+distSph, p.z()+distSph, (resl/iRadius)); // Position, radius
//	m_grids[0]->createSphere(p.x()-distSph, p.y()+distSph, p.z()-distSph, (resl/iRadius)); // Position, radius
//	m_grids[0]->createSphere(p.x()-distSph, p.y()-distSph, p.z()+distSph, (resl/(7.0))); // Position, radius
//	m_grids[0]->createSphere(p.x()-distSph, p.y()-distSph, p.z()-distSph, (resl/iRadius)); // Position, radius
//	m_grids[0]->createSphere(p.x()+distSph, p.y()+distSph, p.z()+distSph, (resl/iRadius)); // Position, radius
//	m_grids[0]->createSphere(p.x()+distSph, p.y()+distSph, p.z()-distSph, (resl/iRadius)); // Position, radius
//	m_grids[0]->createSphere(p.x()+distSph, p.y()-distSph, p.z()+distSph, (resl/iRadius)); // Position, radius
//	m_grids[0]->createSphere(p.x()+distSph, p.y()-distSph, p.z()-distSph, (resl/5.0)); // Position, radius
//	m_grids[0]->createEdges(); // of the grid
	m_grids[0]->createGround(0);
//	m_grids[0]->createGround(3);
//	m_grids[0]->createRoof();

	//m_grids[0]->dumpFileInMemory("/home/john/code/build-vxR-Desktop-Release/vxR");
	//m_grids[0]->dumpFileInMemory("/home/john/code/build-vxR-Desktop-Release/image.0000002.tif");
	
	auto na = m_grids[0]->numActiveVoxels();
	auto totals = m_grids[0]->getNumberOfVoxels();
	std::cout << "Number of active voxels " << na << " of " << totals << std::endl;
	std::cout << " -- Finished building process scene -- " << std::endl;

}

std::shared_ptr<vxCamera> 
vxScene::createCamera(const vxMatrix &,
						double hAperture,
						double vAperture)
{


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

std::shared_ptr<vxGrid> vxScene::createGrid()
{
	// this is the grid object
	const double resl = RESL;
	vxVector3d p{PX, PY, PZ};
	
	// this is a hardcode program to test the rays. 
	//TODO:get rid of this hard-coded values.
	m_grids.push_back(std::make_shared<vxGrid>(p.x(), p.y(), p.z(), resl));
	m_grids[0]->setResolution(resl);
	
	return m_grids[0];
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
	const double resl = RESL;
	const auto& vts = importer->getPointCloud();
	m_grids[0]->addVertices(vts,
							vxVector3d{PX,PY-(resl/2.0),PZ},
//							vxVector3d{resl, resl, resl});
							vxVector3d{resl*0.75, resl*0.75, resl*0.75});
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
//	auto p = MathUtils::rectAndYPlane(ray.direction(),  - RESL/2.0);
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
	else
	{
		collide.setUV(MathUtils::normalToCartesian(ray.direction()));
		auto environmentColor = m_environment.compute(collide);
		
		collide.setValid(false);
		collide.setColor( environmentColor );
		return 1;
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
