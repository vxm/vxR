#include "vxScene.h"


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
int vxScene::throwRay(const vxVector3d &ray, vxCollision &collide)
{ 
	if(!m_grids.size())
	{
		return 0;
	}
	
	m_grids[0]->throwRay(ray,collide);
	
	if (collide.isValid())
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
