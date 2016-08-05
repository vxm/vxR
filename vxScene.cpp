
#include<memory>

#include<cassert>
#include<thread>
#include<future>

#include "vxScene.h"

namespace vxCore{

class vxScene;

vxScene::vxScene(ImagePropertiesHandle prop)
	: m_properties(prop)
{
	m_broadPhase = std::make_shared<vxBroadPhase>();
}

vxScene::~vxScene()
{
}

void vxScene::build(std::shared_ptr<vxSceneParser> nodeDB)
{
	m_nodeDB = nodeDB;
	
	buildDefaultShader();
	
	buildClocks();
	
	buildImages();
	
	buildCameras();
	
	buildGrids();
	
	buildPlanes();
	
	buildShaders();
	
	buildGeometries();
	
	buildDomes();
	
	buildLights();
	
	//Update caches
	updateCache();
	
	std::cout << " -- Finished building process scene -- " << std::endl;
}

std::vector<vxShaderHandle > vxScene::shaders() const
{
	return m_shaders;
}

void vxScene::setShaders(const std::vector<vxShaderHandle > &shaders)
{
	m_shaders = shaders;
}

std::vector<std::shared_ptr<vxGrid> >& vxScene::grids()
{
	return m_grids;
}

void vxScene::buildImages()
{
	for(const auto node: m_nodeDB->getNodesByType("vxImage"))
	{
		auto&& gain = node->getFloat("gain");
		auto&& gamma = node->getFloat("gamma");
		
		auto img = createImage(node->getString("imagePath"), 
							   gain, 
							   gamma);
		
		img->load();
		
		node->m_object = img.get();
	}
}

void vxScene::buildLights()
{
	for(const auto node: m_nodeDB->getNodesByType("vxDirectLight"))
	{
		auto direct = createDirectLight();
		direct->setIntensity(node->getFloat("intensity"));
		direct->setColor(vxColor::lookup256(node->getColor("color"s)));
		direct->setSamples(node->getInt("samples"s));
		
		direct->setOrientation(node->getVector3d("orientation"s));
		
		std::string&& cast = node->getString("castShadows"s);
		direct->setComputeShadows(cast == "true"s);

		node->m_object = direct.get();
	}
	
	for(const auto node: m_nodeDB->getNodesByType("vxIBLight"))
	{
		auto env = createIBLight(node->getString("filePath"));
		env->setIntensity(node->getFloat("intensity"));
		env->setColor(vxColor::lookup256(node->getColor("color")));
		
		env->setSamples(node->getInt("samples"));
		env->setRadius(node->getFloat("radius"));
		env->setGain(node->getFloat("gain"));
		env->setGamma(node->getFloat("gamma"));
		env->setLowThreshold(node->getFloat("lowThreshold"));

		node->m_object = env.get();
	}
	
	for(const auto node: m_nodeDB->getNodesByType("vxAmbientLight"))
	{
		auto ambient = createAmbientLight();
		ambient->setIntensity(node->getFloat("intensity"));
		ambient->setColor(vxColor::lookup256(node->getColor("color")));
		ambient->setSamples(node->getInt("samples"));
		
		const auto transform = node->getMatrix("transform");
		ambient->setTransform(transform);

		node->m_object = ambient.get();	
	}
	
	for(const auto node: m_nodeDB->getNodesByType("vxPointLight"))
	{
		auto point = createPointLight();
		point->setIntensity(node->getFloat("intensity"));
		point->setColor(vxColor::lookup256(node->getColor("color")));
		point->setSamples(node->getInt("samples"));
		point->setComputeShadows(node->getBool("castShadows"));
		
		const auto transform = node->getMatrix("transform");
		point->setTransform(transform);
		node->m_object = point.get();
	}
	
	for(const auto node: m_nodeDB->getNodesByType("vxAreaLight"))
	{
		auto area = createAreaLight();
		area->setIntensity(node->getFloat("intensity"));
		area->setColor(vxColor::lookup256(node->getColor("color")));
		area->setSamples(node->getInt("samples"));
		
		area->setMinX(node->getFloat("minX"));
		area->setMinY(node->getFloat("minY"));
		
		area->setMaxX(node->getFloat("maxX"));
		area->setMaxY(node->getFloat("maxY"));
		
		const auto transform = node->getMatrix("transform");
		area->setTransform(transform);

		node->m_object = area.get();
	}
}

void vxScene::buildCameras()
{
	for(const auto node: m_nodeDB->getNodesByType("vxCamera"))
	{
		m_camera = std::make_shared<vxCamera>(m_properties);
		
		const auto pRadius = node->getFloat("pixelRadius");
		const auto fDistance = node->getFloat("focusDistance");
		const auto hAperture = node->getFloat("horizontalAperture");
		const auto vAperture = node->getFloat("verticalAperture");
		const auto transform = node->getMatrix("transform");
		
		m_camera->set(v3s::zero,
					  v3s::constZ,
					  fDistance,
					  hAperture,
					  vAperture);
		
		m_camera->setPixelRadius(pRadius);
		m_camera->setTransform(transform);
		node->m_object = m_camera.get();
	}
}


void vxScene::buildGrids()
{
	for(const auto node: m_nodeDB->getNodesByType("vxGrid"))
	{
		const auto resolution = node->getInt("resolution");
		const auto transform = node->getMatrix("transform");
		const auto color = vxColor::lookup256(node->getColor("color"));
		const auto size = node->getFloat("size");
		
		auto grid = std::make_shared<vxGrid>(transform.getOrigin(), size);
		grid->setResolution(resolution);
		grid->setTransform(transform);
		
		grid->setBaseColor(color);
		m_grids.emplace_back(grid);
		m_geometries.emplace_back(grid);
		m_broadPhase->addGeometry(grid);
		
		for(const auto nodeGeo: m_nodeDB->getNodesByType("vxGridGeometry"))
		{
			const auto path = nodeGeo->getString("filePath");
			const auto transform = nodeGeo->getMatrix("transform");
			
			auto grid_geo = std::make_shared<vxTriangleMesh>();
			grid_geo->setTransform(transform);
			grid_geo->setConstructionPath(path);
			
			auto plyReader = std::make_shared<vxPLYImporter>(grid_geo);
			plyReader->processPLYFile(path);
			grid->addGeometry(grid_geo);
		}
		
		//grid->createGround(0, (unsigned char)4u);
		//grid->createEdges((unsigned char)12u);
		//grid->createRandom(.004,0.85);
		//grid->dumpFileInMemory("/home/mario/Downloads/xyzrgb_statuette_1.ply");
		
		auto na = grid->numActiveVoxels();
		auto totals = grid->getNumberOfVoxels();
		std::cout << "Number of active voxels " << na << " of " << totals << std::endl;
		node->m_object = grid.get();
	}
	
}

void vxScene::buildDomes()
{
	for(const auto node: m_nodeDB->getNodesByType("vxDome"))
	{
		auto dome = createDome(node->getString("imageNode"));
		dome->setRadius(node->getFloat("radius"));
		node->m_object = dome.get();
	}
}

void vxScene::buildPlanes()
{
	for(const auto node: m_nodeDB->getNodesByType("vxPlane"))
	{
		const auto planeTypeName = node->getString("planeType");
		vxPlane::type planeType = vxPlane::type::kFree;
		if(planeTypeName=="X"s)
		{
			planeType = vxPlane::type::kX;
		}
		if(planeTypeName=="Y"s)
		{
			planeType = vxPlane::type::kY;
		}
		if(planeTypeName=="z"s)
		{
			planeType = vxPlane::type::kZ;
		}
		if(planeTypeName=="free"s)
		{
			planeType = vxPlane::type::kFree;
			
		}
		auto plane = createPlane(planeType);
		
		//geometry color.
		plane->setColor(vxColor::lookup256(node->getColor("color")));
		
		plane->setPointA(node->getVector3d("pointA"));
		plane->setPointB(node->getVector3d("pointB"));
		plane->setPointC(node->getVector3d("pointC"));
		
		plane->setX(node->getFloat("x"));
		plane->setY(node->getFloat("y"));
		plane->setZ(node->getFloat("z"));
		node->m_object = plane.get();
	}
}

void vxScene::buildGeometries()
{
	for(const auto node: m_nodeDB->getNodesByType("vxGeometry"))
	{
		
		const auto path = node->getString("filePath");
		const auto transform = node->getMatrix("transform");
		
		auto geo = createGeometry(path, transform);
		geo->setBaseColor(vxColor::lookup256(node->getColor("color")));
		
		auto&& shaderNodeName = node->getString("shader");
		auto shaderNode = m_nodeDB->getNodeByName(shaderNodeName);
		if(!shaderNode)
		{
			std::cerr << "shader node name " 
					  << shaderNodeName 
					  << " does not exist in database." 
					  << std::endl;
			assert(true);
		}
		
		geo->setShader((vxShader*)shaderNode->m_object);
		
		geo->setTransform(transform);
		
		auto plyReader = std::make_shared<vxPLYImporter>(geo);
		plyReader->processPLYFile(path);
		
		node->m_object = geo.get();
	}
}

void vxScene::buildClocks()
{
	for(const auto node: m_nodeDB->getNodesByType("vxClock"))
	{
		vxClock::setStart( node->getFloat("start"));
		vxClock::setEnd( node->getFloat("end"));
		vxClock::setStep( node->getFloat("step"));
	}
}

void vxScene::buildShaders()
{
	for(const auto node: m_nodeDB->getNodesByType("vxShader"))
	{
		auto shader = createShader(node->name());
		
		shader->setDiffuseColor(vxColor::lookup256(node->getColor("diffuseColor")));
		shader->setDiffuseCoeficent(node->getFloat("diffuseCoeficent"));
		shader->setGiSamples(node->getInt("giSamples"));
		shader->setGiCoeficent(node->getFloat("giCoeficent"));
		shader->setGiColorMultiplier(vxColor::lookup256(node->getColor("giColorMultiplier")));
		shader->setReflectionSamples(node->getInt("reflectionSamples"));
		shader->setReflectionRadius(node->getFloat("reflectionRadius"));
		shader->setReflectionCoefficent(node->getFloat("reflectionCoefficent"));
		shader->setReflectionColorMultiplier(vxColor::lookup256(node->getColor("reflectionColorMultiplier")));
		shader->setRefractionSamples(node->getInt("refractionSamples"));
		shader->setRefractionRadius(node->getFloat("refractionRadius"));
		shader->setRefractionCoefficent(node->getFloat("refractionCoefficent"));
		shader->setRefractionColorMultiplier(vxColor::lookup256(node->getColor("refractionColorMultiplier")));
		shader->setSscSamples(node->getInt("sscSamples"));
		shader->setSscRadius(node->getFloat("sscRadius"));
		shader->setSscCoefficent(node->getFloat("sscCoefficent"));
		shader->setSscColorMultiplier(vxColor::lookup256(node->getColor("sscColorMultiplier")));
		
		node->m_object = shader.get();
	}
}

void vxScene::updateCache()
{
	std::cout << " -- Start caching -- " << std::endl;
	
	m_broadPhase->updateCache();
	
	std::cout << " -- End cache computation -- " << std::endl;
}

vxShaderHandle vxScene::createShader(const std::string &name)
{
	auto lambert = std::make_shared<vxLambert>();
	m_shaders.emplace_back(lambert);
	lambert->setScene(shared_from_this());
	lambert->setLights(&m_lights);
	return lambert;
}

void vxScene::setShader(vxShaderHandle shader)
{
	m_shader = shader;
}

void vxScene::setCamera(const std::shared_ptr<vxCamera> &camera)
{
	m_camera = camera;
}

void vxScene::buildDefaultShader()
{
	m_shader = std::make_shared<vxLambert>();
	m_shader->setLights(&m_lights);
	m_shader->setScene(shared_from_this());
}

vxAreaLightHandle vxScene::createAreaLight()
{
	auto area = std::make_shared<vxAreaLight>();
	m_areaLights.emplace_back(area);
	m_lights.emplace_back(area);
	area->setScene(shared_from_this());
	return area;
	
}

vxPointLightHandle vxScene::createPointLight()
{
	auto pl1 = std::make_shared<vxPointLight>(1.0, vxColor::white);
	m_pointLights.emplace_back(pl1);
	m_lights.emplace_back(pl1);
	pl1->setScene(shared_from_this());
	return pl1;
}

vxIBLightHandle vxScene::createIBLight(const std::string path)
{
	auto ibl1 = std::make_shared<vxIBLight>(1.0, path);
	m_IBLights.emplace_back(ibl1);
	m_lights.emplace_back(ibl1);
	ibl1->setScene(shared_from_this());
	return ibl1;
}

vxDirectLightHandle vxScene::createDirectLight()
{
	auto dl1 = std::make_shared<vxDirectLight>(1.0, vxColor::white);
	m_directLights.emplace_back(dl1);
	m_lights.emplace_back(dl1);
	dl1->setScene(shared_from_this());
	return dl1;
}

vxAmbientLightHandle vxScene::createAmbientLight()
{
	auto al1 = std::make_shared<vxAmbientLight>(1.0, vxColor::white);
	m_ambientLights.emplace_back(al1);
	m_lights.emplace_back(al1);
	al1->setScene(shared_from_this());
	return al1;
}

vxDomeHandle vxScene::createDome(const std::string &imagePath)
{
	auto imageNode = m_nodeDB->getNodeByName(imagePath);
	
	if(!imageNode)
	{
		std::cerr << "image node name " 
				  << imagePath 
				  << " does not exist in database." 
				  << std::endl;
		assert(true);
	}
	
	auto image = getImage(imageNode);
	
	auto dome = std::make_shared<vxDome>(image);
	m_domes.emplace_back(dome);
	return dome;
}

std::shared_ptr<vxPlane> vxScene::createPlane(vxPlane::type type)
{
	auto plane = std::make_shared<vxPlane>(type);
	m_planes.emplace_back(plane);
	return plane;
}


vxShaderHandle vxScene::getShader(vxNodeHandle node)
{
	vxShaderHandle ret;
	
	return ret;
}

vxImageHandle vxScene::getImage(vxNodeHandle node)
{
	vxImageHandle ret;
	
	if(node->type()!="vxImage"s)
	{
		std::cerr << "Node " 
				  << node->name() 
				  << " is not an image" 
				  << std::endl;
	}
	
	auto&& path = node->getString("imagePath");
	auto gain = node->getFloat("gain");
	auto gamma = node->getFloat("gamma");
	
	for(const auto& img: m_bitMaps)
	{
		if(img->path() == path
				&& img->gain() == gain
				&& img->gamma() == gamma)
		{
			ret = img;
		}
	}
	
	if(!ret)
	{
		std::cerr << "Image with path " 
				  << path 
				  << " gain " 
				  << gain 
				  << " and gamma " 
				  << gamma 
				  << " could not be found" 
				  << std::endl;
	}
	
	return ret;
}

vxImageHandle vxScene::createImage(const std::string &path,
								   const scalar gain,
								   const scalar gamma)
{
	// looking for previouly opened images.
	
	auto image = std::make_shared<vxImage>(path);
	image->setGamma(gamma);
	image->setGain(gain);
	
	m_bitMaps.emplace_back(image);
	return image;
}

vxTriangleMeshHandle vxScene::createGeometry(const std::string &path, const Matrix44 &transform)
{
	// looking for previouly processed meshes.
	for(const auto& geo: m_triangleMeshes)
	{
		if(geo->constructionPath()==path && geo->transform()==transform)
		{
			return geo;
		}
	}
	
	//auto found = std::find_first_of(m_geometries.begin(), m_geometries.end(),
	//	[&path](vxGeometryHandle g){ return g->constructionPath()==path;});
	
	auto geo = std::make_shared<vxTriangleMesh>();
	geo->setTransform(transform);
	geo->setConstructionPath(path);
	m_broadPhase->addGeometry(geo);
	
	m_geometries.emplace_back(geo);
	
	return geo;
}

ImagePropertiesHandle vxScene::imageProperties() const
{
	return m_properties;
}

void vxScene::setImageProperties(const ImagePropertiesHandle &prop)
{
	m_properties = prop;
}

ImagePropertiesHandle vxScene::properties() const
{
	return m_properties;
}

void vxScene::setProperties(const ImagePropertiesHandle &properties)
{
	m_properties = properties;
}

std::shared_ptr<vxCamera> vxScene::camera() const
{
	return m_camera;
}

vxDomeHandle vxScene::dome() const
{
	if(m_domes.size())
		return m_domes[0];
	
	return vxDomeHandle();
}

bool vxScene::throwRay(const vxRay &ray) const
{
	vxCollision col;
	return throwRay(ray, col)==1;
}

int vxScene::domeThrowRay(const vxRay &ray, vxCollision &collide) const
{
	if(m_domes.size())
	{
		m_domes[0]->throwRay(ray, collide);
	}
	
	return 1;
}

int vxScene::throwRay(const vxRay &ray, vxCollision &collide) const
{
	if(m_broadPhase->throwRay(ray,collide))
	{
		collide.setValid(true);
		
		return 1;
	}
	
	collide.setValid(false);
	return 0;
}

bool vxScene::hasCollision(const vxRay &ray) const
{
	vxCollision col;
	return throwRay(ray, col)==1;
}

vxShaderHandle vxScene::defaultShader() const
{
	return m_shader;
}



}
