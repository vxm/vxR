
#include <memory>

#include <cassert>
#include <future>
#include <thread>

#include "BroadPhase.h"
#include "Light.h"
#include "Scene.h"

namespace vxCore
{

class Scene;

Scene::Scene(ImagePropertiesHandle prop) : m_properties(std::move(prop))
{
	m_broadPhase = std::make_unique<BroadPhase>();
}

void Scene::build(std::shared_ptr<SceneParser> nodeDB)
{
	m_nodeDB = std::move(nodeDB);

	buildDefaultShader();

	buildShaders();

	buildClocks();

	buildImages();

	buildCameras();

	buildPlanes();

	buildGeometries();

    buildGrids();

	buildCylinders();

	buildSpheres();

	buildDomes();

	buildLights();

	// Update caches
	updateCache();

	std::cout << " -- Finished building process scene -- " << std::endl;
}

std::vector<vxShaderHandle> Scene::shaders() const { return m_shaders; }

void Scene::setShaders(const std::vector<vxShaderHandle> &shaders)
{
	m_shaders = shaders;
}

std::vector<std::shared_ptr<Grid>> &Scene::grids() { return m_grids; }

void Scene::buildImages()
{
	for (const auto &node : m_nodeDB->getNodesByType("vxImage"))
	{
		auto &&gain = node->getScalar("gain");
		auto &&gamma = node->getScalar("gamma");

		auto img = createImage(FileUtils::path + node->getString("imagePath"), gain,
							   gamma);

		img->load();

		node->bind(img);
	}
}

void Scene::buildLights()
{
	for (const auto &node : m_nodeDB->getNodesByType("vxDirectLight"))
	{
		auto direct = createDirectLight();
		direct->setIntensity(node->getScalar("intensity"));
		direct->setColor(Color::lookup256(node->getColor("color"s)));
		direct->setSamples(node->getInt("samples"s));

		direct->setOrientation(node->getVector3d("orientation"s));

		std::string &&cast = node->getString("castShadows"s);
		direct->setComputeShadows(cast == "true"s);

		node->bind(direct);
	}

	for (const auto &node : m_nodeDB->getNodesByType("vxIBLight"))
	{
		auto env = createIBLight(FileUtils::path + node->getString("filePath"));
		env->setIntensity(node->getScalar("intensity"));
		env->setColor(Color::lookup256(node->getColor("color")));

		env->setSamples(node->getInt("samples"));
		env->setRadius(node->getScalar("radius"));
		env->setGain(node->getScalar("gain"));
		env->setGamma(node->getScalar("gamma"));
		env->setLowThreshold(node->getScalar("lowThreshold"));

		node->bind(env);
	}

	for (const auto &node : m_nodeDB->getNodesByType("vxAmbientLight"))
	{
		auto ambient = createAmbientLight();
		ambient->setIntensity(node->getScalar("intensity"));
		ambient->setColor(Color::lookup256(node->getColor("color")));
		ambient->setSamples(node->getInt("samples"));

		const auto transform = node->getMatrix("transform");
		ambient->setTransform(transform);

		node->bind(ambient);
	}

	for (const auto &node : m_nodeDB->getNodesByType("vxPointLight"))
	{
		auto point = createPointLight();
		point->setIntensity(node->getScalar("intensity"));
		point->setColor(Color::lookup256(node->getColor("color")));
		point->setSamples(node->getInt("samples"));

		std::string &&cast = node->getString("castShadows"s);
		point->setComputeShadows(cast == "true"s);

		point->setRadius(node->getScalar("radius"));

		const auto transform = node->getMatrix("transform");
		point->setTransform(transform);
		node->bind(point);
	}

	for (const auto &node : m_nodeDB->getNodesByType("vxSphereLight"))
	{
		auto point = createSphereLight();
		point->setIntensity(node->getScalar("intensity"));
		point->setColor(Color::lookup256(node->getColor("color")));
		point->setSamples(node->getInt("samples"));

		std::string &&cast = node->getString("castShadows"s);
		point->setComputeShadows(cast == "true"s);

		point->setRadius(node->getScalar("radius"));

		const auto transform = node->getMatrix("transform");
		point->setTransform(transform);
		node->bind(point);
	}

	for (const auto &node : m_nodeDB->getNodesByType("vxSunLight"))
	{
		auto sun = createSunLight();
		sun->setIntensity(node->getScalar("intensity"));
		sun->setOrientation(node->getVector3d("orientation"));
		sun->setColor(Color::lookup256(node->getColor("color")));
		sun->setSamples(node->getInt("samples"));

		std::string &&cast = node->getString("castShadows"s);
		sun->setComputeShadows(cast == "true"s);
		sun->setRadiusMultiplier(node->getScalar("radiusMultiplier"));

		const auto transform = node->getMatrix("transform");
		sun->setTransform(transform);
		node->bind(sun);
	}

	for (const auto &node : m_nodeDB->getNodesByType("vxAreaLight"))
	{
		auto area = createAreaLight();
		area->setIntensity(node->getScalar("intensity"));
		area->setColor(Color::lookup256(node->getColor("color")));
		area->setSamples(node->getInt("samples"));

		area->setMinX(node->getScalar("minX"));
		area->setMinY(node->getScalar("minY"));

		area->setMaxX(node->getScalar("maxX"));
		area->setMaxY(node->getScalar("maxY"));

		const auto transform = node->getMatrix("transform");
		area->setTransform(transform);

		node->bind(area);
	}
}

void Scene::buildCameras()
{
	for (const auto &node : m_nodeDB->getNodesByType("vxCamera"))
	{
		m_camera = std::make_shared<Camera>(m_properties);

		const auto pRadius = node->getScalar("pixelRadius");
		const auto fDistance = node->getScalar("focusDistance");
		const auto focalLength = node->getScalar("focalLength");
		const auto lensSize = node->getScalar("lensSize");
		const auto hAperture = node->getScalar("horizontalAperture");
		const auto vAperture = node->getScalar("verticalAperture");
		const auto transform = node->getMatrix("transform");

		m_camera->set(v3s::zero, v3s::constZ, fDistance, hAperture, vAperture);

		m_camera->setFocalLength(focalLength);
		m_camera->setLensSize(lensSize);
		m_camera->setPixelRadius(pRadius);
		m_camera->setTransform(transform);
		node->bind(m_camera);
	}
}

void Scene::buildGrids()
{
	for (const auto &node : m_nodeDB->getNodesByType("vxGrid"))
	{
		const auto transform = node->getMatrix("transform");
		const auto size = node->getScalar("size");
		auto grid = std::make_shared<Grid>(transform.origin(), size);

		auto &&shaderNodeName = node->getString("shader");
		auto shaderNode = m_nodeDB->getNodeByName(shaderNodeName);
		if (!shaderNode)
		{
			std::cerr << "shader node name " << shaderNodeName
					  << " does not exist in database." << std::endl;
			static_assert(true);
		}

		grid->setShader(std::static_pointer_cast<Shader>(shaderNode->node()));

		const auto color = Color::lookup256(node->getColor("color"));
		const auto resolution = (long)node->getInt("resolution");

		grid->setResolution(resolution);
		grid->setTransform(transform);

		grid->setColor(color);
		m_grids.emplace_back(grid);
		m_geometries.emplace_back(grid);
		m_broadPhase->addVisible(grid);

		for (const auto &nodeGeo : m_nodeDB->getNodesByType("vxGridGeometry"))
		{
			const auto path = FileUtils::path + nodeGeo->getString("filePath");
			const auto transform = nodeGeo->getMatrix("transform");

			auto grid_geo = std::make_shared<TriangleMesh>();
			grid_geo->setTransform(transform);
			grid_geo->setConstructionPath(path);

			auto plyReader = std::make_shared<PLYImporter>(grid_geo);
			plyReader->processPLYFile(path);
			grid->addGeometry(grid_geo);
		}

		// grid->createGround(3, (unsigned char)4u);
		// grid->createEdges((unsigned char)12u);
		// grid->createRandom(.3,-0.64);
		// grid->fill();
		// grid->createRandom(.03,-0.6);
		// grid->createRandom(.0003,-0.5);

		grid->createGround(-0.63, 2);
		grid->createRandom(.5, -0.62);
		grid->createRandom(.2, -0.60);
		grid->createRandom(.1, -0.59);
        grid->legolizeColors();
        //grid->createBox(BoundingBox(-0.03, -2.3, -0.03, 0.03, 2.1, 0.03), 4);
		// grid->dumpFileInMemory("/home/mario/Downloads/xyzrgb_statuette_1.ply");
		// grid->createEdges((unsigned char)12u);

		auto na = grid->numActiveVoxels();
		auto totals = grid->getNumberOfVoxels();
		std::cout << "Number of active voxels " << na << " of " << totals
				  << std::endl;
		node->bind(grid);
	}
}

void Scene::buildDomes()
{
	for (const auto &node : m_nodeDB->getNodesByType("vxDome"))
	{
		auto imageNodeName = node->getString("imageNode");
		auto imageLightNode = node->getString("lightImageNode");

		auto dome = createDome(imageNodeName, imageLightNode);
		dome->setRadius(node->getScalar("radius"));
		node->bind(dome);
	}
}

void Scene::buildPlanes()
{
	for (const auto &node : m_nodeDB->getNodesByType("vxPlane"))
	{
		const auto planeTypeName = node->getString("planeType");
		Plane::type planeType = Plane::type::kFree;
		if (planeTypeName == "X"s)
		{
			planeType = Plane::type::kX;
		}
		if (planeTypeName == "Y"s)
		{
			planeType = Plane::type::kY;
		}
		if (planeTypeName == "z"s)
		{
			planeType = Plane::type::kZ;
		}
		if (planeTypeName == "free"s)
		{
			planeType = Plane::type::kFree;
		}
		auto plane = createPlane(planeType);

		// geometry color.
		plane->setColor(Color::lookup256(node->getColor("color")));

		plane->setPointA(node->getVector3d("pointA"));
		plane->setPointB(node->getVector3d("pointB"));
		plane->setPointC(node->getVector3d("pointC"));

		plane->setX(node->getScalar("x"));
		plane->setY(node->getScalar("y"));
		plane->setZ(node->getScalar("z"));
		node->bind(plane);
	}
}

void Scene::buildCylinders()
{
	for (const auto &node : m_nodeDB->getNodesByType("vxCylinder"))
	{
		const auto transform = node->getMatrix("transform");

		auto cylinderGeo = createCylinder();

		cylinderGeo->setColor(Color::lookup256(node->getColor("color")));

		auto &&shaderNodeName = node->getString("shader");
		auto shaderNode = m_nodeDB->getNodeByName(shaderNodeName);
		if (!shaderNode)
		{
			std::cerr << "shader node name " << shaderNodeName
					  << " does not exist in database." << std::endl;

			static_assert(true);
		}

		cylinderGeo->setShader(
			std::static_pointer_cast<Shader>(shaderNode->node()));
		cylinderGeo->setTransform(transform);

		auto radius = node->getScalar("radius");
		auto height = node->getScalar("height");

		cylinderGeo->setRadius(radius);
		cylinderGeo->setHeight(height);

		cylinderGeo->boundingBox()->applyTransform(transform);
		m_geometries.emplace_back(cylinderGeo);
		m_broadPhase->addVisible(cylinderGeo);

		node->bind(cylinderGeo);
	}
}

void Scene::buildSpheres()
{
	for (const auto &node : m_nodeDB->getNodesByType("vxSphere"))
	{
		const auto transform = node->getMatrix("transform");

		auto sphereGeo = createSphere();

		sphereGeo->setColor(Color::lookup256(node->getColor("color")));

		auto &&shaderNodeName = node->getString("shader");
		auto shaderNode = m_nodeDB->getNodeByName(shaderNodeName);
		if (!shaderNode)
		{
			std::cerr << "shader node name " << shaderNodeName
					  << " does not exist in database." << std::endl;

			static_assert(true);
		}

		sphereGeo->setShader(std::static_pointer_cast<Shader>(shaderNode->node()));
		sphereGeo->setTransform(transform);

		auto radius = node->getScalar("radius");

		sphereGeo->setRadius(radius);

		m_geometries.emplace_back(sphereGeo);
		m_broadPhase->addVisible(sphereGeo);

		node->bind(sphereGeo);
	}
}

void Scene::buildGeometries()
{
	for (const auto &node : m_nodeDB->getNodesByType("vxGeometry"))
	{
		const auto path = FileUtils::path + node->getString("filePath");

		const auto transform = node->getMatrix("transform");

		auto geo = createGeometry(path, transform);
		geo->setColor(node->getColor("color"));

		auto &&shaderNodeName = node->getString("shader");
		auto shaderNode = m_nodeDB->getNodeByName(shaderNodeName);
		if (!shaderNode)
		{
			std::cerr << "shader node name " << shaderNodeName
					  << " does not exist in database." << std::endl;
			static_assert(true);
		}

		geo->setShader(std::static_pointer_cast<Shader>(shaderNode->node()));
		geo->setTransform(transform);

		auto plyReader = std::make_shared<PLYImporter>(geo);
		if (plyReader->processPLYFile(path))
		{
			geo->updateBoundingBox();
			node->bind(geo);
		}
	}
}

void Scene::buildClocks()
{
	for (const auto &node : m_nodeDB->getNodesByType("vxClock"))
	{
		Clock::setStart(node->getScalar("start"));
		Clock::setEnd(node->getScalar("end"));
		Clock::setStep(node->getScalar("step"));
	}
}

void Scene::buildShaders()
{
	for (const auto &node : m_nodeDB->getNodesByType("vxShader"))
	{
		auto shader = createShader();

		shader->setDiffuseColor(Color::lookup256(node->getColor("diffuseColor")));
		shader->setDiffuseCoeficent(node->getScalar("diffuseCoeficent"));
		shader->setGiCoeficent(node->getScalar("giCoeficent"));
		shader->setGiColorMultiplier(
			Color::lookup256(node->getColor("giColorMultiplier")));
		shader->setRayDepth((unsigned int)node->getInt("rayDepth"));
		shader->setReflectionRadius(node->getScalar("reflectionRadius"));
		shader->setReflectionCoefficent(node->getScalar("reflectionCoefficent"));
		shader->setReflectionColorMultiplier(
			Color::lookup256(node->getColor("reflectionColorMultiplier")));
		shader->setRefractionSamples(
			(unsigned int)node->getInt("refractionSamples"));
		shader->setRefractionRadius(node->getScalar("refractionRadius"));
		shader->setRefractionCoefficent(node->getScalar("refractionCoefficent"));
		shader->setRefractionColorMultiplier(
			Color::lookup256(node->getColor("refractionColorMultiplier")));
		shader->setSscSamples((unsigned int)node->getInt("sscSamples"));
		shader->setSscRadius(node->getScalar("sscRadius"));
		shader->setSscCoefficent(node->getScalar("sscCoefficent"));
		shader->setSscColorMultiplier(
			Color::lookup256(node->getColor("sscColorMultiplier")));

		node->bind(shader);
	}
}

void Scene::updateCache()
{
	// std::cout << " -- Start caching -- " << std::endl;

	m_broadPhase->updateCache();

	// std::cout << " -- End cache computation -- " << std::endl;
}

vxShaderHandle Scene::createShader()
{
	auto lambert = std::make_shared<Lambert>();
	m_shaders.emplace_back(lambert);
	lambert->setScene(this);
	lambert->setLights(&m_lights);
	return lambert;
}

void Scene::setShader(const vxShaderHandle &shader) { m_shader = shader; }
void Scene::setCamera(const std::shared_ptr<Camera> &camera)
{
	m_camera = camera;
}

void Scene::buildDefaultShader()
{
	m_shader = std::make_shared<Lambert>();
	m_shader->setLights(&m_lights);
	m_shader->setScene(this);
}

void Scene::addLight(LightHandle lh)
{
	m_lights.emplace_back(lh);
	lh->setScene(this);
	m_broadPhase->addVisible(lh);
}

AreaLightHandle Scene::createAreaLight()
{
	auto area = std::make_shared<AreaLight>();
	m_areaLights.emplace_back(area);
	addLight(area);
	return area;
}

PointLightHandle Scene::createPointLight()
{
	auto pl1 = std::make_shared<PointLight>();
	m_pointLights.emplace_back(pl1);
	addLight(pl1);
	return pl1;
}

SphereLightHandle Scene::createSphereLight()
{
	auto pl1 = std::make_shared<SphereLight>();
	m_SphereLights.emplace_back(pl1);
	addLight(pl1);
	return pl1;
}

SunLightHandle Scene::createSunLight()
{
	auto pl1 = std::make_shared<SunLight>();
	m_sunLights.emplace_back(pl1);
	addLight(pl1);
	return pl1;
}

IBLightHandle Scene::createIBLight(const std::string &path)
{
	auto ibl1 = std::make_shared<IBLight>(1.0, path);
	m_IBLights.emplace_back(ibl1);
	addLight(ibl1);
	return ibl1;
}

DirectLightHandle Scene::createDirectLight()
{
	auto dl1 = std::make_shared<DirectLight>();
	m_directLights.emplace_back(dl1);
	addLight(dl1);
	return dl1;
}

AmbientLightHandle Scene::createAmbientLight()
{
	auto al1 = std::make_shared<AmbientLight>();
	m_ambientLights.emplace_back(al1);
	addLight(al1);
	return al1;
}

vxDomeHandle Scene::createDome(const std::string &imagePath,
							   const std::string &lightImagePath)
{
	auto imageNode = m_nodeDB->getNodeByName(imagePath);

	if (!imageNode)
	{
		std::cerr << "image node name " << imagePath
				  << " does not exist in database." << std::endl;
		static_assert(true);
	}

	auto image = getImage(imageNode);

	auto lightImageNode = m_nodeDB->getNodeByName(lightImagePath);

	if (!lightImageNode)
	{
		std::cerr << "image node name " << lightImagePath
				  << " does not exist in database." << std::endl;
		static_assert(true);
	}

	auto lightImage = getImage(lightImageNode);

	auto dome = std::make_shared<Dome>(image);
	dome->setLightImage(lightImage);

	m_domes.emplace_back(dome);
	return dome;
}

std::shared_ptr<Plane> Scene::createPlane(Plane::type type)
{
	auto plane = std::make_shared<Plane>(type);
	m_planes.emplace_back(plane);
	return plane;
}

ImageHandle Scene::getImage(const vxNodeHandle &node)
{
	ImageHandle ret;

	if (node->type() != "vxImage"s)
	{
		std::cerr << "Node " << node->name() << " is not an image" << std::endl;
	}

	auto path = FileUtils::path + node->getString("imagePath");
	auto gain = node->getScalar("gain");
	auto gamma = node->getScalar("gamma");

	for (const auto &img : m_bitMaps)
	{
		if (img->path() == path && img->gain() == gain && img->gamma() == gamma)
		{
			ret = img;
		}
	}

	if (!ret)
	{
		std::cerr << "Image with path " << path << " gain " << gain << " and gamma "
				  << gamma << " could not be found" << std::endl;
	}

	return ret;
}

ImageHandle Scene::createImage(const std::string &path, const scalar gain,
							   const scalar gamma)
{
	// looking for previouly opened images.

	auto image = std::make_shared<Image>(path);
	image->setGamma(gamma);
	image->setGain(gain);

	m_bitMaps.emplace_back(image);
	return image;
}

vxTriangleMeshHandle Scene::createGeometry(const std::string &path,
										   const Matrix44 &transform)
{
	// looking for previouly processed meshes.
	for (const auto &geo : m_triangleMeshes)
	{
		if (geo->constructionPath() == path && geo->transform() == transform)
		{
			return geo;
		}
	}

	// auto found = std::find_first_of(m_geometries.begin(), m_geometries.end(),
	//	[&path](vxGeometryHandle g){ return g->constructionPath()==path;});

	auto geo = std::make_shared<TriangleMesh>();
	geo->setTransform(transform);
	geo->setConstructionPath(path);
	m_broadPhase->addVisible(geo);

	m_geometries.emplace_back(geo);

	return geo;
}

CylinderHandle Scene::createCylinder()
{
	auto geo = std::make_shared<Cylinder>();
	return geo;
}

SphereHandle Scene::createSphere()
{
	auto geo = std::make_shared<Sphere>();
	return geo;
}

ImagePropertiesHandle Scene::imageProperties() const { return m_properties; }

void Scene::setImageProperties(const ImagePropertiesHandle &prop)
{
	m_properties = prop;
}

ImagePropertiesHandle Scene::properties() const { return m_properties; }

void Scene::setProperties(const ImagePropertiesHandle &properties)
{
	m_properties = properties;
}

std::shared_ptr<Camera> Scene::camera() const { return m_camera; }

vxDomeHandle Scene::dome() const
{
	if (!m_domes.empty())
		return m_domes[0];

	return vxDomeHandle();
}

bool Scene::throwRay(const Ray &ray) const
{
	Collision col;
	return throwRay(ray, col) == 1;
}

int Scene::domeThrowRay(const Ray &ray, Collision &collide) const
{
	if (!m_domes.empty())
	{
		m_domes[0]->throwRay(ray, collide);
		return 1;
	}

	return 0;
}

int Scene::domeComputeLight(const Ray &ray, Collision &collide) const
{
	if (!m_domes.empty())
	{
		m_domes[0]->computeLight(ray, collide);
		return 1;
	}

	return 0;
}

int Scene::throwRay(const Ray &ray, Collision &collide) const
{
	collide.setValid(false);

	if (m_broadPhase->throwRay(ray, collide))
	{
		collide.setValid(true);

		return 1;
	}

	collide.setValid(false);

	return 0;
}

bool Scene::hasCollision(const Ray &ray) const
{
	return m_broadPhase->hasCollision(ray);
	/*Collision collide;
	if (m_broadPhase->throwRay(ray, collide))
	{
	  return collide.isValid();
	}

	return false;*/
}

vxShaderHandle Scene::defaultShader() const { return m_shader; }
} // namespace vxCore
