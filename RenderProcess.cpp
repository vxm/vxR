#include <cassert>
#include <climits>
#include <future>
#include <iostream>
#include <thread>

#include "Camera.h"
#include "Grid.h"
#include "ImageProperties.h"
#include "Pixel.h"
#include "RenderProcess.h"
#include "Sampler.h"
#include "ThreadPool.h"
#include "TimeUtils.h"

#define SINGLERAY 0
#if SINGLERAY
#define PIXEL_X 430
#define PIXEL_Y 132
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
    : m_properties(prop), m_imageData(prop),
      m_contactBuffer(prop->numPixels()), m_samples{samples}
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
		auto &&th = std::thread([this, i] { (this->render(m_nThreads, i)); });
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

Color RenderProcess::computeReflection(const Ray &ray, Collision &col)
{
	Color reflection = Color::zero;

	const auto &n = col.normal();

	// Reflected direction
	auto invV = ((n * ray.direction().dot(n) * scalar(-2.0)) + ray.direction());

	// Noise Sphere
	auto sh = getShader(col);
	auto invVRand = MU::getSolidSphereRand3(sh->getReflectionRadius());
	auto ratio = invV.dot(invVRand);
	invV += invVRand;

	auto reflexRay = Ray(col.position() + n.small(), invV, VisionType::kAll);

	Collision refxCollision;
	reflection = computeLight(reflexRay, refxCollision);
	reflection *= sh->getReflectionCoefficent() - fabs(ratio);

	reflection.applyCurve(1.2, 0.0);

	return reflection;
}

Color RenderProcess::computeGI(Collision &col)
{
	const auto &&r = MU::getHollowHemisphereRand(1.0, col.normal());

	const Ray giRay(col.position() + col.normal().small(), r.inverted(),
	                VisionType::kOpaque);

	auto ratio = giRay.direction().dot(col.normal());

	Collision nextRound;

	Color hitColor = computeLight(giRay, nextRound);

	Color globalIlm = hitColor * (1.0 - fabs(ratio));

	return globalIlm;
}

Color RenderProcess::computeEnergyAndColor(const Ray &ray, Collision &col,
                                           unsigned int giBounces,
                                           unsigned int rflBounces)
{
	Color firstHitColor = computeLight(ray, col);

	if (col.isValid())
	{
		auto sh = getShader(col);

		// Compute Global Illumination
		if (sh->hasGI() && giBounces)
		{
			firstHitColor += computeGI(col);
		}

		// Compute reflection
		if (sh->hasReflection() && rflBounces)
		{
			// firstHitColor *= std::max(0.0, 1.0-sh->getReflectionCoefficent());
			firstHitColor += computeReflection(ray, col);
		}
	}
	else
	{
		m_scene->domeThrowRay(ray, col);

		firstHitColor = col.color();
	}

	firstHitColor.setA(1.0);

	return firstHitColor;
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

	computeEnergyAndColor(ray, collision, 0, 0);

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

			firstHitColor += computeEnergyAndColor(ray, col, 1, 1);

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
