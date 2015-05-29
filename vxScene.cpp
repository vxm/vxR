#include<memory>
#include<thread>
#include<future>

#include "vxScene.h"

namespace vxCore{
class vxScene;

#define RESL 107
#define PX resl/1.2
#define PY 0.0
#define PZ resl*2.20

vxScene::vxScene(std::shared_ptr<ImageProperties> prop)
	: m_prop(prop)
{

}

vxScene::~vxScene()
{
}

void vxScene::build()
{
	m_shader = std::make_shared<vxLambert>();
	m_shader->setLights(&m_lights);
	m_shader->setScene(shared_from_this());

	//TODO: remove this debug code
	const double resl = RESL;
	vxVector3d p{PX, PY, PZ};
	//

	auto l1 = createPointLight();
	l1->setColor(vxColor(.8,.6,.5));
	l1->setSamples(22);
	l1->setRadius(3.0);
	l1->setPosition(-3, PY+20, PZ+resl);
	l1->setIntensity(0.12);

	auto l2 = createPointLight();
	l2->setColor(vxColor(.3,.4,.8));
	l2->setSamples(42);
	l2->setRadius(12);
	l2->setPosition(PX+resl, PY+resl, -3);
	l2->setIntensity(0.14);

	auto l3 = createIBLight();
	l3->setColor(vxColor(1,1,1));
	l3->setSamples(65);
	l3->setRadius(3186);
	l3->setIntensity(4.2);

	//	auto l3 = createDirectLight();
	//	l3->set(vxVector3d(0,-1,0), true);
	//	l3->setIntensity(1.0);

	createCamera(vxMatrix(), 2);
	createGrid();
	
	auto plyReader = std::make_shared<vxPLYImporter>();
	plyReader->processPLYFile("/home/john/Downloads/happy_recon/happy_vrip.ply");
//	plyReader->processPLYFile("/home/john/Downloads/dragon_backdrop/dragonBk1_0.ply");

//	other files
//	plyReader->processPLYFile("/home/john/Downloads/dragon_backdrop/dragonClearSpace1_0.ply");
//	plyReader->processPLYFile("/home/john/Downloads/dragon_backdrop/dragonClearSpace2_0.ply");
//	plyReader->processPLYFile("/home/john/Downloads/dragon_backdrop/dragonClearSpace3_0.ply");

	loadFromFile(plyReader);
}

std::shared_ptr<vxCamera> 
vxScene::createCamera(const vxMatrix &transform,
						unsigned int samples,
						double hAperture,
						double vAperture)
{
	m_camera = std::make_shared<vxCamera>(m_prop);
	m_camera->set(	vxVector3d(0,0,0),
					vxVector3d(0,0,1),
					2.3);
	m_camera->setPixelSamples(samples);
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

std::shared_ptr<vxIBLight> vxScene::createIBLight()
{
	auto ibl1 = std::make_shared<vxIBLight>(1.0, vxColor::white);
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
	
	/*
	auto iRadius = 4.0;
	auto distSph = (resl/3.0);
	m_grids[0]->createSphere(p.x(), p.y(), p.z(),  distSph); // Position, radius
	m_grids[0]->createSphere(p.x()+distSph, p.y()+distSph, p.z()+distSph,  (resl/iRadius)); // Position, radius
	m_grids[0]->createSphere(p.x()+distSph, p.y()+distSph, p.z()-distSph,  (resl/iRadius)); // Position, radius
	m_grids[0]->createSphere(p.x()+distSph, p.y()-distSph, p.z()+distSph,  (resl/iRadius)); // Position, radius
	m_grids[0]->createSphere(p.x()+distSph, p.y()-distSph, p.z()-distSph,  (resl/iRadius)); // Position, radius
	m_grids[0]->createSphere(p.x()-distSph, p.y()+distSph, p.z()+distSph,  (resl/iRadius)); // Position, radius
	m_grids[0]->createSphere(p.x()-distSph, p.y()+distSph, p.z()-distSph,  (resl/iRadius)); // Position, radius
	m_grids[0]->createSphere(p.x()-distSph, p.y()-distSph, p.z()+distSph,  (resl/iRadius)); // Position, radius
	m_grids[0]->createSphere(p.x()-distSph, p.y()-distSph, p.z()-distSph,  (resl/iRadius)); // Position, radius

	m_grids[0]->createSphere(p.x(), p.y(), p.z(),  (resl/iRadius)); // Position, radius
	m_grids[0]->createEdges(); // of the grid
	*/
	//m_grids[0]->createRandom(0.0007);
	//#ifdef _DEBUG
	
	//m_grids[0]->activate(3,3,1);
	//m_grids[0]->createCorners();
	m_grids[0]->createGround();
	int n = 2;
	//m_grids[0]->activate(6,2,6);
	//m_grids[0]->activate(n+2,0,1);
	//m_grids[0]->activate(1,0,2);
	//m_grids[0]->activate(n+2,2,6);

	//m_grids[0]->activate(n+2,2,1);
	//m_grids[0]->activate(1,2,1);
	//m_grids[0]->activate(1,1,1);
	//m_grids[0]->activate(n+2,1,1);
	//m_grids[0]->activate(1,2,1);
	//m_grids[0]->activate(n+2,3,1);
	//m_grids[0]->activate(1,4,1);
	//m_grids[0]->activate(n+2,4,1);
	//m_grids[0]->activate(1,4,2);
	//m_grids[0]->activate(n+2,4,2);

	auto na = m_grids[0]->numActiveVoxels();
	auto totals = m_grids[0]->getNumberOfVoxels();
	std::cout << "Number of active voxels " << na << " of " << totals << std::endl;
//#endif

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
							vxVector3d(resl*-5, resl*5, resl*-5));
}

bool vxScene::throwRay(const vxScene * const sc, 
					   const vxRayXYZ &ray, 
					   vxCollision &collide,
					   std::promise<int>&& pr)
{ 
	sc->m_grids[0]->throwRay(ray,collide);
	
	if(collide.isValid())
	{
		vxColor col(sc->defaultShader()->getColor(collide));
		
		collide.setColor( col );
		pr.set_value(true);
		return true;
	}
	
	//TODO:take this to a dommo object or something like..
	auto p = MathUtils::rectAndYPlane(ray, -RESL/2.0);
	
	if(!std::signbit(p.z()))
	{
		collide.setNormal(vxVector3d::constY);
		collide.setPosition(p);
		collide.setU(fmod(p.x(),1.0));
		collide.setV(fmod(p.z(),1.0));
		
		vxColor col(sc->defaultShader()->getColor(collide));
		collide.setColor( col );
		collide.setValid();
		pr.set_value(true);
		return true;
	}
	else
	{
		collide.setValid();	
		collide.setColor( vxColor::white );
		pr.set_value(true);
		return true;
	}
	
	// 
	pr.set_value(false);
	return false;
}

bool vxScene::hasCollision(const vxVector3d &origin, const vxRayXYZ &ray)
{
	return m_grids[0]->hasCollision(origin, ray);
}

std::shared_ptr<vxShader> vxScene::defaultShader() const
{
	return m_shader;
}

}
