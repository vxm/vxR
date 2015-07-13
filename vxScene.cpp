#include<memory>

#include<thread>
#include<future>

#include "vxScene.h"

namespace vxCore{
class vxScene;

#define RESL 30
#define PX resl/1.1
#define PY 0.0
#define PZ resl*2.60
///home/john/code/vxR/EtniesPark_Center/Etnies_Park_Center_8k.jpg
///home/john/code/vxR/Ditch_River/Ditch-River_TMap.jpg
///home/john/code/vxR/Basketball_Court/BasketballCourt_8k.jpg 
///home/john/code/vxR/Milkyway/Milkyway_BG.jpg

vxScene::vxScene(std::shared_ptr<ImageProperties> prop)
	: m_prop(prop)
	, m_environment("/home/john/code/vxR/Ditch_River/Ditch-River_TMap.jpg")
{}

vxScene::~vxScene()
{}

void vxScene::build()
{
	int nLightSamples = 50;
	const double sunIntensity{1.5};
	const auto sunCoords = vxVector2d{	2 * 0.172000 * MathUtils::PI,
										 0.224000  };
	const auto sunColor = vxColor::lookup256(255,240,241);

	m_shader = std::make_shared<vxLambert>();
	m_shader->setLights(&m_lights);
	m_shader->setScene(shared_from_this());

	//TODO: remove this debug code
	const double resl = RESL;
	vxVector3d p{PX, PY, PZ};

	//Environment tint.
	auto l3 = createIBLight(m_environment.path());
	l3->setSamples(nLightSamples);
	l3->setRadius(0.97);
	l3->setIntensity(1.0);

	//This simulates the sun.
	auto l4 = createDirectLight();
	l4->setSamples(5);
	l4->setRadius(0.1);
	l4->setOrientation(MathUtils::cartesianToNormal(sunCoords));
	l4->setIntensity(sunIntensity);
	l4->setColor(sunColor);
	
	
	createCamera(vxMatrix{});
	createGrid();
	
	auto plyReader = std::make_shared<vxPLYImporter>();
	//plyReader->processPLYFile("../vxR/juan_0.ply");
	plyReader->processPLYFile("/home/john/Downloads/statue_and_dog_1.ply");
	//plyReader->processPLYFile("/home/john/Downloads/vmilo_0.ply");
	loadFromFile(plyReader);
	
	//m_grids[0]->createSphere(p.x(), p.y(), p.z(),  resl/2.0); 
	auto iRadius = 5.0;
	auto distSph = (resl/3.0);
	m_grids[0]->createSphere(p.x(), p.y()-(resl/2.0), p.z(),  (resl/iRadius)); // Position, radius
	m_grids[0]->createSphere(p.x()+distSph, p.y()+distSph, p.z()+distSph,  (resl/iRadius)); // Position, radius
	m_grids[0]->createSphere(p.x()+distSph, p.y()+distSph, p.z()-distSph,  (resl/iRadius)); // Position, radius
	m_grids[0]->createSphere(p.x()+distSph, p.y()-distSph, p.z()+distSph,  (resl/iRadius)); // Position, radius
	m_grids[0]->createSphere(p.x()+distSph, p.y()-distSph, p.z()-distSph,  (resl/iRadius)); // Position, radius
	m_grids[0]->createSphere(p.x()-distSph, p.y()+distSph, p.z()+distSph,  (resl/iRadius)); // Position, radius
	//m_grids[0]->createSphere(p.x()-distSph, p.y()+distSph, p.z()-distSph,  (resl/iRadius)); // Position, radius
	m_grids[0]->createSphere(p.x()-distSph, p.y()-distSph, p.z()+distSph,  (resl/iRadius)); // Position, radius
	//m_grids[0]->createSphere(p.x()-distSph, p.y()-distSph, p.z()-distSph,  (resl/iRadius)); // Position, radius
	//m_grids[0]->createEdges(); // of the grid
	m_grids[0]->createGround();

	auto na = m_grids[0]->numActiveVoxels();
	auto totals = m_grids[0]->getNumberOfVoxels();
	std::cout << "Number of active voxels " << na << " of " << totals << std::endl;
}

std::shared_ptr<vxCamera> 
vxScene::createCamera(const vxMatrix &,
						double hAperture,
						double vAperture)
{
	m_camera = std::make_shared<vxCamera>(m_prop);
	m_camera->set( vxVector3d(0,0,0),
					vxVector3d(0,0,1),
					2.3,
					hAperture,
					vAperture);

	return m_camera;
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
							vxVector3d(PX,PY,PZ),
							vxVector3d(resl*1.5, resl*1.5, resl*1.5));
	return true;
}

int vxScene::throwRay(const vxRay &ray,
						vxCollision &collide) const
{
	if(m_grids[0]->throwRay(ray, collide))
	{
		vxColor col(defaultShader()->getColor(collide));
		collide.setColor( col );
		return 1;
	}

	//TODO:take this to a dommo object or something like..
//	auto p = MathUtils::rectAndYPlane(ray, (-1) - RESL/2.0);
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
		const auto&& cart = MathUtils::normalToCartesian(ray.direction());
		collide.setUV(cart);
		auto environmentColor = m_environment.compute(collide);
		
		collide.setValid();
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
