#include <iostream>
#include <climits>
#include <cassert>
#include <thread>
#include <future>

#include "RenderProcess.h"
#include "Camera.h"
#include "Grid.h"
#include "Pixel.h"
#include "ImageProperties.h"
#include "TimeUtils.h"
#include "Sampler.h"
#include "ThreadPool.h"

#define SINGLERAY 0
#if SINGLERAY
#define PIXEL_X 275
#define PIXEL_Y 119
#endif

#ifdef _DEBUG
#define USE_THREADS 0
#else
#define USE_THREADS 1
#endif

using timePoint = std::chrono::time_point<std::chrono::system_clock>;
using render = vxCompute::RenderProcess;

namespace vxCompute
{

unsigned int RenderProcess::lightBounces() const { return m_lightBounces; }

void RenderProcess::setLightBounces(unsigned int lightBounces)
{
	m_lightBounces = lightBounces;
}

RenderProcess::RenderProcess(ImagePropertiesHandle &prop, unsigned int samples)
    : m_properties(prop), m_imageData(prop), m_contactBuffer(prop->numPixels()),
      m_samples{samples}
{
	setNMaxThreads(100);
}

unsigned int RenderProcess::samples() const { return m_samples; }

void RenderProcess::setSamples(unsigned int samples) { m_samples = samples; }

std::shared_ptr<const ImageProperties> RenderProcess::properties() const
{
	return m_properties;
}

void RenderProcess::setProperties(
    const std::shared_ptr<const ImageProperties> &properties)
{
	m_properties = properties;
}

unsigned int RenderProcess::giSamples() const { return m_giSamples; }

void RenderProcess::setGISamples(unsigned int giSamples)
{
	m_giSamples = giSamples;
}

scalar RenderProcess::giMultiplier() const { return m_giMultiplier; }

void RenderProcess::setGIMultiplier(const scalar &giMultiplier)
{
	m_giMultiplier = giMultiplier;
}

unsigned int RenderProcess::nMaxThreads() const { return m_nThreads; }

void RenderProcess::setNMaxThreads(unsigned int nMaxThreads)
{
	m_nThreads = std::min(std::thread::hardware_concurrency(), nMaxThreads);
}

unsigned int RenderProcess::visSamples() const { return m_samples; }

void RenderProcess::setVisSamples(unsigned int visSamples)
{
	m_samples = visSamples;
	m_c_invSamples = 1.0 / (scalar)m_samples;
}

unsigned int RenderProcess::reflectionSamples() const
{
	return m_reflectionSamples;
}

void RenderProcess::setReflectionSamples(unsigned int reflectionSamples)
{
	m_reflectionSamples = reflectionSamples;
}

Status RenderProcess::setDatabase(std::shared_ptr<SceneParser>)
{
	Status st;

	if (m_scene != nullptr)
	{
		/*st = m_scene->build(scn)*/;
	}

	return st;
}

Status::code RenderProcess::execute()
{
	timePoint start = std::chrono::system_clock::now();
	m_finished = false;
	m_contactBuffer.reserve(m_properties->numPixels());

#if USE_THREADS
	const auto &minUpdateInterval = 1; // seconds
	unsigned int customUpdateInterval = minUpdateInterval;

	std::cout << "Using " << m_nThreads << " threads" << std::endl;
	std::vector<std::thread> threads;

	for (unsigned int i = 0; i < m_nThreads; i++)
	{
		auto &&th = std::thread([this, i]
		                        {
			                        (this->render(m_nThreads, i));
			                      });
		ThreadPool::threadInfo(std::this_thread::get_id());
		threads.emplace_back(std::move(th));
	}

	scalar accelerationRatio{1.0};
	scalar prevProgress{-1};
	while (!m_finished)
	{
		scalar dProgress = progress();
		if ((dProgress - prevProgress) < accelerationRatio)
		{
			customUpdateInterval += 1;
		}

		std::this_thread::sleep_for(
		    std::chrono::seconds((int)customUpdateInterval));
		std::cout << "\t\t(" << customUpdateInterval
		          << ") progress update: " << std::setprecision(2) << dProgress
		          << "%" << std::endl;

		prevProgress = dProgress;
	}

	for (auto &&th : threads)
	{
		if (th.joinable())
		{
			th.join();
		}
	}

#else
	render(1, 0);
#endif

	std::cout << "Render Process done, took: "
	          << TimeUtils::decorateTime(start, 2);
	return Status::code::kSuccess;
}

scalar RenderProcess::progress() const
{
	return 100.0 * m_progress.load() / (m_properties->ry() * m_nThreads);
}

Shader *RenderProcess::getShader(const Collision &col)
{
	Shader *sh;

	if (col.m_geo != nullptr && col.m_geo->shader() != nullptr)
	{
		sh = col.m_geo->shader().get();
	}
	else
	{
		sh = m_scene->defaultShader().get();
	}

	return sh;
}

Color RenderProcess::computeLight(const Ray &ray, Collision &col)
{
	Color retColor = Color::zero;

	m_scene->throwRay(ray, col);

	if (col.isValid())
	{
		retColor = getShader(col)->getIlluminatedColor(ray, col);
	}
	else
	{
		m_scene->domeThrowRay(ray, col);

		retColor = col.color();
	}

	return retColor;
}

Color RenderProcess::computeReflection(unsigned int iter, const Ray &ray,
                                       Collision &col)
{
	auto sh = getShader(col);

	// Decrease one iteration now.
	if (iter == 0)
	{
		return Color::zero;
	}

	iter--;

	Color reflection = Color::zero;

	const auto &n = col.normal();

	Collision refxCollision;

	// Reflected direction
	v3s invV = ((n * ray.direction().dot(n) * scalar(-2.0)) + ray.direction());

	// Noise Sphere
	invV += MU::getSolidSphereRand3(sh->getReflectionRadius() * 3.141592);

	auto reflexRay = Ray(col.position() + n.small(), invV);

	reflection = computeLight(reflexRay, refxCollision);

	reflection.applyCurve(1.0, 0.0);
	//	Collision nextRound = refxCollision;

	//	reflection+= computeReflection(iter-1, ray, nextRound)
	//			* (scalar(iter) * colorRatio/scalar(m_lightBounces));

	return reflection * 0.1;
}

Color RenderProcess::computeGI(unsigned int iter, Collision &col)
{
	auto sh = getShader(col);

	const auto ray = Ray();

	auto baseColor = sh->getColor(ray, col);

	auto lumm = sh->getLightLoop(ray, col);

	Color globalIlm = baseColor * lumm;

	if (iter == 0)
	{
		return globalIlm;
	}

	iter--;

	const auto &&r = MU::getHollowHemisphereRand(1.0, col.normal());

	const Ray giRay(col.position() + col.normal().small(), r.inverted());

	const auto treeLevel = scalar(iter) / scalar(m_lightBounces * 2);

	auto &&rayIncidence = giRay.incidence(col.normal()) * treeLevel;

	if (rayIncidence < 0.001)
	{
		return globalIlm;
	}

	Collision nextRound = col;

	m_scene->throwRay(giRay, nextRound);
	
	if (!nextRound.m_geo->shader()->hasGI())
	{
		return globalIlm;
	}
	
	if (nextRound.isValid())
	{
		const auto &&nextColor = computeGI(iter, nextRound);

		globalIlm += nextColor * baseColor;
	}
	else if (m_scene->domeComputeLight(giRay, nextRound))
	{
		globalIlm += nextRound.color() * baseColor;
	}

	return globalIlm;
}

Color RenderProcess::computeEnergyAndColor(const Ray &ray, Collision &col)
{
	Color retColor;

	m_scene->throwRay(ray, col);

	if (col.isValid())
	{
		auto sh = getShader(col);
		// Compute reflection
		if (sh->hasReflection())
		{
			retColor += computeReflection(m_lightBounces, ray, col);
		}

		// Compute Global Illumination
		if (sh->hasGI())
		{
			retColor += computeGI(m_lightBounces, col);
		}
		/*else
		{
			return Color::white;
		}*/
	}
	else
	{
		m_scene->domeThrowRay(ray, col);

		retColor = col.color();
	}

	retColor.setA(1.0);

	return retColor;
}

Status::code RenderProcess::render(unsigned int by, unsigned int offset)
{
	assert(offset < this->imageProperties()->rx());
	const auto &&rCamera = scene()->camera();
	Sampler sampler(m_samples);

#if SINGLERAY
	(void)by;
	(void)offset;
	// moving to start point.
	unsigned int itH = PIXEL_X;
	unsigned int itV = PIXEL_Y;

	// TODO: return this to smart pointer.
	Collision collision;
	const v2s hitCoordinates(itV / (scalar)m_properties->ry(),
	                         itH / (scalar)m_properties->rx());

	const auto ray = rCamera->ray(hitCoordinates, sampler);

	computeEnergyAndColor(ray, collision);

	const auto id = itH + (itV * m_properties->rx());

	if (m_properties->numPixels() <= id)
	{
		std::cerr << "Error: trying to write a pixel which doesnt exist"
		          << std::endl;
	}
	else
	{
		m_contactBuffer.pixel(id) = Color::white;
	}

#else

	// moving to start point.
	unsigned int itH{offset};
	unsigned int itV{0u};

	// on eachpixel.
	while (!(itV >= (m_properties->ry())))
	{
		Color firstHitColor(Color::zero);
		const v2s hitCoordinates(itV / (scalar)m_properties->ry(),
		                         itH / (scalar)m_properties->rx());

		for (auto s = 0u; s < m_samples; s++)
		{
			Collision col;
			auto &&ray = rCamera->ray(hitCoordinates, sampler);

			/// TODO: this crashes with 0
			firstHitColor += computeEnergyAndColor(ray, col);

			sampler.next();
		}

		sampler.resetIterator();
		firstHitColor *= m_c_invSamples;

		const auto id = itH + (itV * m_properties->rx());
		m_contactBuffer.pixel(id) = firstHitColor;

		itH += by;

		if (itH >= m_properties->rx())
		{
			itV++;
			itH %= m_properties->rx();

			m_progress.store(m_progress.load() + 1);
		}
	}
#endif

	auto thInfo = ThreadPool::threadInfo(std::this_thread::get_id());

	std::cout << "Finished task on thread ::" << thInfo.id << "::" << std::endl;

	m_finished = true;

	return Status::code::kSuccess;
}

const unsigned char *RenderProcess::generateImage()
{
	static_assert(sizeof(float) == 4, "scalar is no 32bits");
	static_assert(sizeof(double) == 8, "scalar is no 64bits");
	static_assert(sizeof(unsigned char) == 1, "unsigned char is no 8bits");

	auto buff = m_imageData.initialise();

	int k = 0;
	auto pixel = m_imageData.m_pc.get();

	for (auto &&px : m_contactBuffer.m_pxs)
	{
		px.m_color.setToGamma(1.0);
		px.m_color.toRGBA8888(pixel);
		k++;
		pixel += 4;
	}

	return buff;
}

std::shared_ptr<const ImageProperties> RenderProcess::imageProperties() const
{
	return m_properties;
}

void RenderProcess::setImageProperties(
    std::shared_ptr<const ImageProperties> imageProperties)
{
	m_properties = imageProperties;
}

std::shared_ptr<Scene> RenderProcess::scene() const { return m_scene; }

void RenderProcess::setScene(const std::shared_ptr<Scene> &scene)
{
	m_scene = scene;
}
}
