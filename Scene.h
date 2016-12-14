#ifndef _VXSCENEMC_
#define _VXSCENEMC_

#include<math.h>
#include<memory>
#include<vector>
#include<future>

#include "Clock.h"
#include "BroadPhase.h"
#include "SceneParser.h"
#include "Ray.h"
#include "Plane.h"
#include "Matrix44.h"
#include "Light.h"
#include "Grid.h"
#include "Camera.h"
#include "ImageProperties.h"
#include "Shader.h"
#include "PLYImporter.h"
#include "Image.h"
#include "Dome.h"
#include "TriangleMesh.h"
#include "Cylinder.h"

namespace vxCore {

class Shader;
class Grid;
class Light;
class IBLight;
class AreaLight;
class PointLight;
class DirectLight;
class AmbientLight;
class Cylinder;

class Scene: public std::enable_shared_from_this<Scene>
{
protected:
	bool m_defaultLight = {true};
	bool m_defaultShader = {true};
	
	std::shared_ptr<SceneParser> m_nodeDB;
	
	std::vector<vxShaderHandle> m_shaders;
	std::vector<AreaLightHandle> m_areaLights;
	std::vector<IBLightHandle> m_IBLights;
	std::vector<PointLightHandle> m_pointLights;
	std::vector<DirectLightHandle> m_directLights;
	std::vector<AmbientLightHandle> m_ambientLights;
	
	std::vector<vxGeometryHandle> m_geometries;
	std::vector<std::shared_ptr<TriangleMesh>> m_triangleMeshes;
	std::vector<std::shared_ptr<Plane>> m_planes;
	
	std::vector<std::shared_ptr<Light>> m_lights;
	
	std::vector<ImageHandle> m_bitMaps;
	std::vector<vxDomeHandle> m_domes;
	
	vxShaderHandle m_shader = {nullptr};
	std::shared_ptr<Camera> m_camera = {nullptr};
	
	std::vector<std::shared_ptr<Grid>> m_grids;
	
	std::vector<Camera> m_cameras;
	ImagePropertiesHandle m_properties = {nullptr};
	
	vxBroadPhaseHandle m_broadPhase;
	//TODO: private and getter setter.
	long long dRays{0};
	
public:
	///
	/// \brief vxScene
	/// \param imageProperties
	///
	Scene(ImagePropertiesHandle imageProperties);
	~Scene();
	
	///
	/// \brief build
	/// \param nodeDB
	/// It will register the database and call every build 
	/// process for each of specific types.
	void build(std::shared_ptr<SceneParser> nodeDB);
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
	/// \brief buildCylinder
	///
	void buildCylinders();
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
	vxShaderHandle createShader();
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
	std::shared_ptr<Light> defaultLight() const;
	///
	/// \brief setLight
	/// \param defaultLight
	///
	void setLight(const std::shared_ptr<Light> &defaultLight);
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
	std::shared_ptr<Camera> camera() const;
	///
	/// \brief setCamera
	/// \param defaultCamera
	///
	void setCamera(const std::shared_ptr<Camera> &defaultCamera);
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
	AreaLightHandle createAreaLight();
	///
	/// \brief createPointLight
	/// \return 
	///
	PointLightHandle createPointLight();
	///
	/// \brief createDirectLight
	/// \return 
	///
	DirectLightHandle createDirectLight();
	///
	/// \brief createIBLight
	/// \param path
	/// \return 
	///
	IBLightHandle createIBLight(const std::string path);
	///
	/// \brief createAmbientLight
	/// \return 
	///
	AmbientLightHandle createAmbientLight();
	///
	/// \brief createDom
	/// \param path
	/// \return 
	///
	vxDomeHandle createDome(const std::string &imageName, const std::__cxx11::string &lightImagePath);
	///
	/// \brief createPlane
	/// \param type
	/// \return 
	///
	std::shared_ptr<Plane> createPlane(Plane::type type);
	///
	/// \brief createImage
	/// \param path
	/// \return 
	///
	ImageHandle createImage(const std::string &path,
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
	/// \brief createCylinder
	/// \return 
	///
	CylinderHandle createCylinder();
	///
	/// \brief grids
	/// \return 
	///
	std::vector<std::shared_ptr<Grid> > &grids();
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
	int domeThrowRay(const Ray &ray, Collision &collide) const;
	///
	/// \brief getImage
	/// \param node
	/// \return 
	///
	ImageHandle getImage(vxNodeHandle node);
	
	///
	/// \brief domeComputeLight
	/// \param ray
	/// \param collide
	/// \return 
	///
	int domeComputeLight(const Ray &ray, Collision &collide) const;
	
	//renderable interface
	bool throwRay(const Ray &ray) const;
	int throwRay(const Ray &ray, Collision &collide) const;
	bool hasCollision(const Ray &ray) const;
};

}
#endif
