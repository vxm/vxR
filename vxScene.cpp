#include "vxScene.h"

vxScene::vxScene(std::shared_ptr<ImageProperties> prop)
	: m_prop(prop)
{
	m_shader = std::make_shared<vxLambert>();
	m_light = std::make_shared<vxPointLight>();
	m_light->setPosition(44, 183, -3);
	
	createCamera(vxMatrix(), 1);
	createGrid();
}

std::shared_ptr<vxCamera> vxScene::createCamera(const vxMatrix &transform, unsigned int samples, double hAperture, double vAperture)
{
	m_camera = std::make_shared<vxCamera>(m_prop);
	m_camera->set(	vxVector3d(0,0,0),
					vxVector3d(0,0,1),
					1.83);
	m_camera->setPixelSamples(samples);
	return m_camera;
}

std::shared_ptr<vxShader> vxScene::defaultShader() const
{
	static std::shared_ptr<vxShader> sLambert;
	if(sLambert!=nullptr)
	{
		return sLambert;
	}
	sLambert = std::make_shared<vxLambert>();
	sLambert->setLight(defaultLight());
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
	const double resl = 5.0;
	
	vxVector3d p{resl/1.2, 0.0, resl*2.20};
	
	// this is a hardcode program to test the rays. 
	//TODO:get rid of this hard-coded values.
	m_grids.push_back(std::make_shared<vxGrid>(p.x(), p.y(), p.z(), resl));
	//m_grids[0]->setResolution(resl);
	/*m_grids[0]->createSphere(p.x(), p.y(), p.z(),  (resl/3.0)); // Position, radius		
	m_grids[0]->createSphere(p.x()+(resl/3.0), p.y(), p.z(),  (resl/6.0)); // Position, radius
	m_grids[0]->createSphere(p.x()-(resl/3.0), p.y(), p.z(),  (resl/6.0)); // Position, radius
	m_grids[0]->createSphere(p.x(), p.y()+(resl/3.0), p.z(),  (resl/6.0)); // Position, radius
	m_grids[0]->createSphere(p.x(), p.y()-(resl/3.0), p.z(),  (resl/6.0)); // Position, radius
	m_grids[0]->createSphere(p.x(), p.y(), p.z()+(resl/3.0),  (resl/6.0)); // Position, radius
	m_grids[0]->createSphere(p.x(), p.y(), p.z()-(resl/3.0),  (resl/6.0)); // Position, radius
	m_grids[0]->createRandom(0.0007);
	m_grids[0]->createGround();
	m_grids[0]->createEdges(); // of the grid
	//#ifdef _DEBUG
	*/
	//m_grids[0]->activate(3,3,1);
	//m_grids[0]->activate(4,0,3);
	m_grids[0]->activate(3,0,3);
	
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
	if(!m_grids.size())
	{
		return 0;
	}
	
	m_grids[0]->throwRay(ray,collide);
	
	if(collide.isValid())
	{
		defaultShader()->getColor(collide);
		return 1;
	}
	
	return 0;
}

std::shared_ptr<vxLight> vxScene::defaultLight() const
{
	return m_light;
}

void vxScene::setLight(const std::shared_ptr<vxLight> &light)
{
	m_light = light;
}
