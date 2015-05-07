#ifndef _VXSCENEMC_
#define _VXSCENEMC_

#include<memory>
#include<vector>

#include "vxObject.h"
#include "vxMatrix.h"
#include "vxShader.h"
#include "vxLight.h"
#include "vxGrid.h"
#include "vxCamera.h"
#include "ImageProperties.h"

namespace vxStorage {


class vxScene:public vxObject
{
protected:

	bool m_defaultLight = {true};
	bool m_defaultShader = {true};

	std::vector<vxPointLight> m_lights;
	
	std::shared_ptr<vxShader> m_shader = {nullptr};
	std::shared_ptr<vxCamera> m_camera = {nullptr};

	std::vector<std::shared_ptr<vxGrid>> m_grids;
	std::vector<vxGrid> m_cameras;
	std::shared_ptr<ImageProperties> m_prop = {nullptr};

public:

	vxScene(std::shared_ptr<ImageProperties> prop);
	
	~vxScene()
	{
	}

	std::shared_ptr<vxCamera>
		createCamera(const vxMatrix &transform,
					unsigned int samples = 1,
					double hAperture = 0.0,
					double vAperture = 0.0);

	std::shared_ptr<vxGrid> createGrid();
	
	std::shared_ptr<vxStorage::ImageProperties> prop() const;
	
	void setProp(const std::shared_ptr<vxStorage::ImageProperties> &prop);

	// devuelve 0 si no le da a la caja
	// 1 si da y 2 y el resultado es optimo
	int throwRay(const vxVector3d &ray, vxCollision &collide);
	
	std::shared_ptr<vxLight> defaultLight() const;
	void setLight(const std::shared_ptr<vxLight> &defaultLight);
	
	std::shared_ptr<vxShader> defaultShader();
	void setShader(const std::shared_ptr<vxShader> &defaultShader);
	
	std::shared_ptr<vxCamera> defaultCamera() const;
	void setCamera(const std::shared_ptr<vxCamera> &defaultCamera);
};

}
#endif
