#ifndef _VXSCENEMC_
#define _VXSCENEMC_

#include<math.h>
#include<memory>
#include<vector>
#include<future>

#include "vxObject.h"
#include "vxMatrix.h"
#include "vxLight.h"
#include "vxGrid.h"
#include "vxCamera.h"
#include "ImageProperties.h"
#include "vxShader.h"
#include "vxPLYImporter.h"

namespace vxCore {

class vxShader;
class vxGrid;
class vxLight;
class vxIBLight;
class vxPointLight;
class vxDirectLight;

class vxScene: public std::enable_shared_from_this<vxScene>
{
protected:

	bool m_defaultLight = {true};
	bool m_defaultShader = {true};

	std::vector<std::shared_ptr<vxIBLight>> m_IBLights;
	std::vector<std::shared_ptr<vxPointLight>> m_pointLights;
	std::vector<std::shared_ptr<vxDirectLight>> m_directLights;
	std::vector<std::shared_ptr<vxLight>> m_lights;

	std::shared_ptr<vxShader> m_shader = {nullptr};
	std::shared_ptr<vxCamera> m_camera = {nullptr};

	std::vector<std::shared_ptr<vxGrid>> m_grids;
	std::vector<vxGrid> m_cameras;
	std::shared_ptr<ImageProperties> m_prop = {nullptr};

public:
	//TODO: private and getter setter.
	long long dRays{0};

	vxScene(std::shared_ptr<ImageProperties> imageProperties);
	
	~vxScene();

	void build();

	std::shared_ptr<vxCamera>
		createCamera(const vxMatrix &,
					double hAperture = 0.0,
					double vAperture = 0.0);

	
	std::shared_ptr<vxGrid> createGrid();
	std::shared_ptr<vxCore::ImageProperties> imageProperties() const;
	
	void setImageProperties(const std::shared_ptr<vxCore::ImageProperties> &imageProperties);

	// Loads from a file using an importer object.
	bool loadFromFile(std::shared_ptr<vxImporter> importer);
	
	// devuelve 0 si no le da a la caja
	// 1 si da y 2 y el resultado es optimo
	int throwRay( const vxRayXYZ &ray,
						 vxCollision &collide) const;

	bool hasCollision(const vxVector3d &origin, const vxRayXYZ &ray);
	
	std::shared_ptr<vxLight> defaultLight() const;
	void setLight(const std::shared_ptr<vxLight> &defaultLight);
	
	std::shared_ptr<vxShader> defaultShader() const;
	void setShader(std::shared_ptr<vxShader> defaultShader);
	
	std::shared_ptr<vxCamera> defaultCamera() const;
	void setCamera(const std::shared_ptr<vxCamera> &defaultCamera);

	std::shared_ptr<vxPointLight> createPointLight();
	std::shared_ptr<vxDirectLight> createDirectLight();
	std::shared_ptr<vxIBLight> createIBLight();
};

}
#endif
