#include "vxScene.h"

vxScene::vxScene(std::shared_ptr<ImageProperties> prop)
	: m_prop(prop)
{
	m_shader = std::make_shared<vxLambert>();
	
	vxPointLight l1(1.0, vxColor::white);
	l1.setPosition(-1, 80, -1);
	l1.setIntensity(0.7);
	m_lights.push_back(l1);

	vxPointLight l2(1.0, vxColor::white);
	l2.setPosition(-11, 70, 212);
	l2.setIntensity(0.9);
	m_lights.push_back(l2);
	
	createCamera(vxMatrix(), 3);
	createGrid();
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
					2.0);
	m_camera->setPixelSamples(samples);
	return m_camera;
}

std::shared_ptr<vxShader> vxScene::defaultShader()
{
	static std::shared_ptr<vxShader> sLambert;
	if(sLambert!=nullptr)
	{
		return sLambert;
	}

	sLambert = std::make_shared<vxLambert>();
	sLambert->setLights(&m_lights);
	return sLambert;
}

void vxScene::setShader(const std::shared_ptr<vxShader> &shader)
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

std::shared_ptr<vxGrid> vxScene::createGrid()
{
	// this is the grid object
	const double resl = 125.0;
	
	vxVector3d p{resl/1.2, 0.0, resl*2.20};
	
	// this is a hardcode program to test the rays. 
	//TODO:get rid of this hard-coded values.
	m_grids.push_back(std::make_shared<vxGrid>(p.x(), p.y(), p.z(), resl));
	m_grids[0]->setResolution(resl);
	auto iRadius = 8.0;
	//m_grids[0]->createSphere(p.x(), p.y(), p.z(),  (resl/3.0)); // Position, radius		
	m_grids[0]->createSphere(p.x()+(resl/3.0), p.y(), p.z(),  (resl/iRadius)); // Position, radius
	m_grids[0]->createSphere(p.x()-(resl/3.0), p.y(), p.z(),  (resl/iRadius)); // Position, radius
	m_grids[0]->createSphere(p.x(), p.y()+(resl/3.0), p.z(),  (resl/iRadius)); // Position, radius
	m_grids[0]->createSphere(p.x(), p.y()-(resl/3.0), p.z(),  (resl/iRadius)); // Position, radius
	m_grids[0]->createSphere(p.x(), p.y(), p.z()+(resl/3.0),  (resl/iRadius)); // Position, radius
	m_grids[0]->createSphere(p.x(), p.y(), p.z()-(resl/3.0),  (resl/iRadius)); // Position, radius
	//m_grids[0]->createRandom(0.0007);
	m_grids[0]->createEdges(); // of the grid
	//#ifdef _DEBUG
	
	//m_grids[0]->activate(3,3,1);
	//m_grids[0]->createCorners();
	m_grids[0]->createGround();
	int n = 0;
	m_grids[0]->activate(1,0,1);
	//m_grids[0]->activate(n+2,0,1);
	//m_grids[0]->activate(1,0,2);
	m_grids[0]->activate(n+2,0,2);

	/*m_grids[0]->activate(1,1,1);
	m_grids[0]->activate(n+2,1,1);
	m_grids[0]->activate(1,2,1);
	m_grids[0]->activate(n+2,2,1);
	m_grids[0]->activate(1,3,1);
	m_grids[0]->activate(n+2,3,1);
	m_grids[0]->activate(1,4,1);
	m_grids[0]->activate(n+2,4,1);
	m_grids[0]->activate(1,4,2);
	m_grids[0]->activate(n+2,4,2);
	*/
	
	
	auto na = m_grids[0]->numActiveVoxels();
	auto totals = m_grids[0]->getNumberOfVoxels();
	std::cout << "Number of active voxels " << na << " of " << totals << std::endl;
//#endif

	return m_grids[0];
}

std::shared_ptr<ImageProperties> vxScene::prop() const
{
	return m_prop;
}

void vxScene::setProp(const std::shared_ptr<ImageProperties> &prop)
{
	m_prop = prop;
}

int vxScene::throwRay(const vxVector3d &ray, vxCollision &collide)
{ 
	m_grids[0]->throwRay(ray,collide);
	
	if(collide.isValid())
	{
		vxColor col(defaultShader()->getColor(collide));
		collide.setColor( col );
		return 1;
	}
	
	return 0;
}

