#ifndef _VXSCENEMC_
#define _VXSCENEMC_

#include<math.h>
#include<memory>
#include<vector>
#include<future>

#include "vxClock.h"
#include "vxBroadPhase.h"
#include "vxSceneParser.h"
#include "vxRay.h"
#include "vxPlane.h"
#include "Matrix44.h"
#include "vxLight.h"
#include "vxGrid.h"
#include "vxCamera.h"
#include "ImageProperties.h"
#include "vxShader.h"
#include "vxPLYImporter.h"
#include "vxImage.h"
#include "vxDome.h"
#include "vxTriangleMesh.h"

namespace vxCore {

class vxShader;
class vxGrid;
class vxLight;
class vxIBLight;
class vxAreaLight;
class vxPointLight;
class vxDirectLight;
class vxAmbientLight;

class vxScene: public std::enable_shared_from_this<vxScene>
{
protected:
	bool m_defaultLight = {true};
	bool m_defaultShader = {true};
	
	std::shared_ptr<vxSceneParser> m_nodeDB;
	
	std::vector<vxShaderHandle> m_shaders;
	std::vector<vxAreaLightHandle> m_areaLights;
	std::vector<vxIBLightHandle> m_IBLights;
	std::vector<vxPointLightHandle> m_pointLights;
	std::vector<vxDirectLightHandle> m_directLights;
	std::vector<vxAmbientLightHandle> m_ambientLights;
	
	std::vector<vxGeometryHandle> m_geometries;
	std::vector<std::shared_ptr<vxTriangleMesh>> m_triangleMeshes;
	std::vector<std::shared_ptr<vxPlane>> m_planes;
	
	std::vector<std::shared_ptr<vxLight>> m_lights;
	
	std::vector<vxImageHandle> m_bitMaps;
	std::vector<vxDomeHandle> m_domes;
	
	vxShaderHandle m_shader = {nullptr};
	std::shared_ptr<vxCamera> m_camera = {nullptr};
	
	std::vector<std::shared_ptr<vxGrid>> m_grids;
	
	std::vector<vxCamera> m_cameras;
	ImagePropertiesHandle m_properties = {nullptr};
	
	vxBroadPhaseHandle m_broadPhase;
	//TODO: private and getter setter.
	long long dRays{0};
	
public:
	///
	/// \brief vxScene
	/// \param imageProperties
	///
	vxScene(ImagePropertiesHandle imageProperties);
	~vxScene();
	
	///
	/// \brief build
	/// \param nodeDB
	/// It will register the database and call every build 
	/// process for each of specific types.
	void build(std::shared_ptr<vxSceneParser> nodeDB);
	///
	/// \brief buildShaders
	///
	void buildShaders();
	///
	/// \brief buildClocks
	///
	void buildClocks();
	///
	/// \brief buildGeometries
	///
	void buildGeometries();
	///
	/// \brief buildPlanes
	///
	void buildPlanes();
	///
	/// \brief buildDomes
	///
	void buildDomes();
	///
	/// \brief buildGrids
	///
	void buildGrids();
	///
	/// \brief buildCameras
	///
	void buildCameras();
	///
	/// \brief buildLights
	///
	void buildLights();
	///
	/// \brief buildImages
	///
	void buildImages();
	///
	/// \brief updateCache
	/// After all builds this process will ensure the scene is ready 
	/// and optimized for render.
	void updateCache();
	///
	/// \brief createShader
	/// \return 
	///
	vxShaderHandle createShader(const std::__cxx11::string &name);
	///
	/// \brief imageProperties
	/// \return 
	///
	std::shared_ptr<vxCore::ImageProperties> imageProperties() const;
	///
	/// \brief setImageProperties
	/// \param imageProperties
	///
	void setImageProperties(const std::shared_ptr<vxCore::ImageProperties> &imageProperties);
	///
	/// \brief defaultLight
	/// \return 
	///
	std::shared_ptr<vxLight> defaultLight() const;
	///
	/// \brief setLight
	/// \param defaultLight
	///
	void setLight(const std::shared_ptr<vxLight> &defaultLight);
	///
	/// \brief defaultShader
	/// \return 
	///
	vxShaderHandle defaultShader() const;
	///
	/// \brief setShader
	/// \param defaultShader
	///
	void setShader(vxShaderHandle defaultShader);
	///
	/// \brief camera
	/// \return 
	///
	std::shared_ptr<vxCamera> camera() const;
	///
	/// \brief setCamera
	/// \param defaultCamera
	///
	void setCamera(const std::shared_ptr<vxCamera> &defaultCamera);
	///
	/// \brief properties
	/// \return 
	///
	ImagePropertiesHandle properties() const;
	///
	/// \brief setProperties
	/// \param properties
	///
	void setProperties(const ImagePropertiesHandle &properties);
	///
	/// \brief dome
	/// \return 
	///
	vxDomeHandle dome() const;
	///
	/// \brief buildDefaultShader
	///
	void buildDefaultShader();
	///
	/// \brief createAreaLight
	/// \return 
	///
	vxAreaLightHandle createAreaLight();
	///
	/// \brief createPointLight
	/// \return 
	///
	vxPointLightHandle createPointLight();
	///
	/// \brief createDirectLight
	/// \return 
	///
	vxDirectLightHandle createDirectLight();
	///
	/// \brief createIBLight
	/// \param path
	/// \return 
	///
	vxIBLightHandle createIBLight(const std::string path);
	///
	/// \brief createAmbientLight
	/// \return 
	///
	vxAmbientLightHandle createAmbientLight();
	///
	/// \brief createDom
	/// \param path
	/// \return 
	///
	vxDomeHandle createDome(const std::string &imageName);
	///
	/// \brief createPlane
	/// \param type
	/// \return 
	///
	std::shared_ptr<vxPlane> createPlane(vxPlane::type type);
	///
	/// \brief createImage
	/// \param path
	/// \return 
	///
	vxImageHandle createImage(const std::string &path,
							  const scalar gain,
							  const scalar gamma);
	///
	/// \brief createGeometry
	/// \param path
	/// \param transform
	/// \return 
	///
	vxTriangleMeshHandle createGeometry(const std::string &path, 
										const Matrix44 &transform);
	///
	/// \brief grids
	/// \return 
	///
	std::vector<std::shared_ptr<vxGrid> > &grids();
	///
	/// \brief shaders
	/// \return 
	///
	std::vector<vxShaderHandle > shaders() const;
	///
	/// \brief setShaders
	/// \param shaders
	///
	void setShaders(const std::vector<vxShaderHandle > &shaders);
	///
	/// \brief domeThrowRay
	/// \param ray
	/// \param collide
	/// \return 
	///
	int domeThrowRay(const vxRay &ray, vxCollision &collide) const;
	///
	/// \brief getImage
	/// \param node
	/// \return 
	///
	vxImageHandle getImage(vxNodeHandle node);
	///
	/// \brief getShader
	/// \param node
	/// \return 
	///
	vxShaderHandle getShader(vxNodeHandle node);
	
	//renderable interface
	bool throwRay(const vxRay &ray) const;
	int throwRay(const vxRay &ray, vxCollision &collide) const;
	bool hasCollision(const vxRay &ray) const;
};

}
#endif
