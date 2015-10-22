#ifndef _VXSCENEMC_
#define _VXSCENEMC_

#include<math.h>
#include<memory>
#include<vector>
#include<future>

#include "vxSceneParser.h"
#include "vxRay.h"
#include "vxPlane.h"
#include "vxMatrix.h"
#include "vxLight.h"
#include "vxGrid.h"
#include "vxCamera.h"
#include "ImageProperties.h"
#include "vxShader.h"
#include "vxPLYImporter.h"
#include "vxBitMap2d.h"
#include "vxDome.h"

namespace vxCore {

class vxShader;
class vxGrid;
class vxLight;
class vxIBLight;
class vxPointLight;
class vxDirectLight;
class vxAmbientLight;

class vxScene: public std::enable_shared_from_this<vxScene>
{
protected:
	bool m_defaultLight = {true};
	bool m_defaultShader = {true};

	std::vector<std::shared_ptr<vxIBLight>> m_IBLights;
	std::vector<std::shared_ptr<vxPointLight>> m_pointLights;
	std::vector<std::shared_ptr<vxDirectLight>> m_directLights;
	std::vector<std::shared_ptr<vxAmbientLight>> m_ambientLights;
	std::vector<std::shared_ptr<vxGeometry>> m_geometries;
	std::vector<std::shared_ptr<vxPlane>> m_planes;

	std::vector<std::shared_ptr<vxLight>> m_lights;

	std::vector<std::shared_ptr<vxBitMap2d>> m_bitMaps;
	std::vector<std::shared_ptr<vxDome>> m_domes;

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

	void build(std::shared_ptr<vxSceneParser> nodeDB);

	std::shared_ptr<vxCamera>
		createCamera(const vxMatrix &,
					double hAperture = 0.0,
					double vAperture = 0.0);

	std::shared_ptr<vxGrid> createGrid();
	std::shared_ptr<vxCore::ImageProperties> imageProperties() const;
	
	void setImageProperties(const std::shared_ptr<vxCore::ImageProperties> &imageProperties);

	
	
	std::shared_ptr<vxLight> defaultLight() const;
	void setLight(const std::shared_ptr<vxLight> &defaultLight);
	
	std::shared_ptr<vxShader> defaultShader() const;
	void setShader(std::shared_ptr<vxShader> defaultShader);
	
	std::shared_ptr<vxCamera> defaultCamera() const;
	void setCamera(const std::shared_ptr<vxCamera> &defaultCamera);

	std::shared_ptr<vxPointLight> createPointLight();
	std::shared_ptr<vxDirectLight> createDirectLight();
	std::shared_ptr<vxIBLight> createIBLight(const std::string path);
	std::shared_ptr<vxAmbientLight> createAmbientLight();
	std::shared_ptr<vxDome> createDom(const std::string path);
	std::shared_ptr<vxPlane> createPlane(vxPlane::type type);
	std::shared_ptr<vxBitMap2d> createImage(const std::string path);
	std::shared_ptr<vxGeometry> createGeometry(const std::__cxx11::string &path);

	//renderable interface
	bool throwRay(const vxRay &ray) const;
	int throwRay(const vxRay &ray, vxCollision &collide) const;
	bool hasCollision(const vxRay &ray) const;
};

}
#endif
