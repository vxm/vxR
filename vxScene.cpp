#include "vxScene.h"


std::shared_ptr<vxShader> vxScene::defaultShader() const
{
	return m_shader;
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
std::shared_ptr<vxLight> vxScene::defaultLight() const
{
	return m_light;
}

void vxScene::setLight(const std::shared_ptr<vxLight> &light)
{
	m_light = light;
}
