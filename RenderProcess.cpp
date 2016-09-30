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

#define SINGLERAY 1
#if SINGLERAY
#define PIXEL_X 530
#define PIXEL_Y 600
#endif

#ifdef _DEBUG
#define USE_THREADS 0
#else
#define USE_THREADS 1
#endif

using timePoint = std::chrono::time_point<std::chrono::system_clock>;
using render = vxCompute::vxRenderProcess;


namespace vxCompute 
{

vxRenderProcess::vxRenderProcess(ImagePropertiesHandle &prop, 
								 unsigned int samples)
	:	m_properties(prop)
	,	m_imageData(prop)
	,	m_contactBuffer(prop->numPixels())
	,	m_samples{samples}
{
	setNMaxThreads(100);
}

unsigned int vxRenderProcess::samples() const
{
	return m_samples;
}

void vxRenderProcess::setSamples(unsigned int samples)
{
	m_samples = samples;
}

std::shared_ptr<const ImageProperties> vxRenderProcess::properties() const
{
	return m_properties;
}

void vxRenderProcess::setProperties(const std::shared_ptr<const ImageProperties> &properties)
{
	m_properties = properties;
}


unsigned int vxRenderProcess::giSamples() const
{
	return m_giSamples;
}

void vxRenderProcess::setGISamples(unsigned int giSamples)
{
	m_giSamples = giSamples;
}

scalar vxRenderProcess::giMultiplier() const
{
	return m_giMultiplier;
}

void vxRenderProcess::setGIMultiplier(const scalar &giMultiplier)
{
	m_giMultiplier = giMultiplier;
}

unsigned int vxRenderProcess::nMaxThreads() const
{
	return m_nThreads;
}

void vxRenderProcess::setNMaxThreads(unsigned int nMaxThreads)
{
	m_nThreads = std::min(std::thread::hardware_concurrency(), nMaxThreads);
}

unsigned int vxRenderProcess::visSamples() const
{
	return m_samples;
}

void vxRenderProcess::setVisSamples(unsigned int visSamples)
{
	m_samples = visSamples;
	m_c_invSamples = 1.0/(scalar)m_samples;
}

unsigned int vxRenderProcess::reflectionSamples() const
{
	return m_reflectionSamples;
}

void vxRenderProcess::setReflectionSamples(unsigned int reflectionSamples)
{
	m_reflectionSamples = reflectionSamples;
}


Status vxRenderProcess::setDatabase(std::shared_ptr<SceneParser> )
{
	Status st;
	
	if(m_scene!=nullptr)
	{
		/*st = m_scene->build(scn)*/;
	}
	
	return st;
}

Status::code vxRenderProcess::preProcess(Process *p)
{
	if(p!=nullptr)
	{
		p->execute();
	}
	
	return Status::code::kSuccess;
}

Status::code vxRenderProcess::postProcess(Process *p)
{
	if(p!=nullptr)
	{
		p->execute();
	}
	
	return Status::code::kSuccess;
}


Status::code vxRenderProcess::execute()
{
	timePoint start = std::chrono::system_clock::now();
	m_finished = false;
	m_contactBuffer.reserve(m_properties->numPixels());
	
#if USE_THREADS
	const auto& minUpdateInterval = 1; //seconds
	unsigned int customUpdateInterval = minUpdateInterval;
	
	std::cout << "Using " << m_nThreads << " threads" << std::endl;
	std::vector<std::thread> threads;
	
	for(unsigned int i=0;i<m_nThreads; i++)
	{
		auto&& th = std::thread([this,i]{(this->render(m_nThreads,i));});
		ThreadPool::threadInfo(std::this_thread::get_id());
		threads.emplace_back(std::move(th));
	}
	
	scalar accelerationRatio{1.0};
	scalar prevProgress{-1};
	while(!m_finished)
	{
		scalar dProgress = progress();
		if((dProgress-prevProgress)<accelerationRatio)
		{
			customUpdateInterval+=1;
		}
		
		std::this_thread::sleep_for(std::chrono::seconds((int)customUpdateInterval));
		std::cout << "\t\t(" << customUpdateInterval
				  << ") progress update: " 
				  << std::setprecision(2) 
				  << dProgress << "%"<< std::endl;
		
		prevProgress = dProgress;
	}
	
	for(auto &&th: threads)
	{
		if(th.joinable())
		{
			th.join();
		}
	}
	
#else
	render(1,0);
#endif
	
	std::cout << "Render Process done, took: " << TimeUtils::decorateTime(start, 2);
	return Status::code::kSuccess;
}

scalar vxRenderProcess::progress() const
{
	return  100.0 * m_progress.load() / (m_properties->ry()*m_nThreads);
}

Color vxRenderProcess::computeLight(const Ray &ray, Collision &col)
{
	Color retColor = Color::zero;
	m_scene->throwRay(ray, col);
	
	if(col.isValid())
	{
		Shader* sh{nullptr};
		if(col.m_geo!=nullptr)
		{
			sh = col.m_geo->shader();
		}
		else
		{
			sh = m_scene->defaultShader().get();
		}
		
		retColor = sh->getIlluminatedColor(ray,col);
	}
	else
	{
		m_scene->domeThrowRay(ray, col);
		retColor = col.color();
	}
	
	return retColor;
}

Status::code vxRenderProcess::render(unsigned int by, unsigned int offset)
{
	assert(offset<this->imageProperties()->rx());
	const auto&& rCamera = scene()->camera();
	Sampler sampler(m_samples);
	
#if SINGLERAY
	(void)by;
	(void)offset;
	// moving to start point.
	unsigned int itH = PIXEL_X;
	unsigned int itV = PIXEL_Y;
	
	//TODO: return this to smart pointer.
	Collision collision;
	const v2s hitCoordinates(
				itV/(scalar)m_properties->ry(),
				itH/(scalar)m_properties->rx());
	
	const auto ray = rCamera->ray(hitCoordinates, sampler);
	
	computeLight(ray, collision);
	
	const auto id = itH  + (itV * m_properties->rx());
	//TODO:check ranges here, it might fail.
	m_contactBuffer.pixel(id) = Color::white;
	
	
#else
	
	// moving to start point.
	unsigned int itH {offset};
	unsigned int itV {0u};
	
	const auto dome = m_scene->dome();
	
	// on eachpixel.
	while(!(itV>=(m_properties->ry())))
	{
		auto firstHitColor{Color::zero};
		const v2s hitCoordinates(
					itV/(scalar)m_properties->ry(),
					itH/(scalar)m_properties->rx());
		
		for(auto s=0u;s<m_samples;s++)
		{
			Collision collision;
			auto&& ray = rCamera->ray(hitCoordinates, sampler);
			
			//compute the shader
			firstHitColor += computeLight(ray,collision);
			
			if(collision.isValid())
			{
				Color reflection = Color::zero;
				{
					const auto& n = collision.normal();
					Collision refxCollision;
					for(unsigned int k = 0u;k<m_reflectionSamples;k++)
					{
						v3s invV = ((n * ray.direction().dot(n) * scalar(-2.0))
									+ ray.direction());
						
						invV+=MU::getSolidSphereRand3(0.3141592);
						
						const auto &&reflexRay =
								Ray(collision.position() + n.tiny(), invV);
						
						reflection = computeLight(reflexRay, refxCollision);
					}
					
					reflection*=(0.002f/(scalar)m_reflectionSamples);
					firstHitColor+= (reflection);
				}
				
				Color globalIlm;
				{
					Color baseColor = m_scene->defaultShader()->getColor(ray,collision);
					const auto n = m_reflectionSamples;
					const auto colorRatio = m_giMultiplier*.5/(scalar)n;
					for(auto i=0u; i<n; i++)
					{
						const auto&& r = MU::getHollowHemisphereRand(1.0, collision.normal());
						const Ray giRay(collision.position()
										  +collision.normal().tiny(), 
										  r.inverted());
						
						auto rayIncidence = giRay.incidence(collision.normal());
						
						Collision giColl;
						
						m_scene->throwRay(giRay, giColl);
						if(giColl.isValid())
						{
							Color gi(m_scene->defaultShader()->getIlluminatedColor(ray,giColl));
							globalIlm.mixSumm(baseColor * gi * rayIncidence, colorRatio);
						}
						else
						{
							if(dome!=nullptr)
							{
								dome->throwRay(giRay, giColl);
								auto domeColor = giColl.color();
								domeColor.applyCurve(dome->gamma(), dome->gain());
								
								globalIlm.mixSumm((baseColor * domeColor) * rayIncidence, colorRatio);
							}
						}
						
					}
					firstHitColor+= globalIlm;
				}
			}
			
			sampler.next();
		}
		
		sampler.resetIterator();
		firstHitColor*=m_c_invSamples;
		
		const auto id = itH  + (itV * m_properties->rx());
		m_contactBuffer.pixel(id) = firstHitColor;
		
		itH+=by;
		if(itH >= m_properties->rx())
		{
			itV+=1;
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

Status::code vxRenderProcess::preConditions()
{
	return Status::code::kSuccess;
}


const unsigned char *
vxRenderProcess::generateImage()
{
	static_assert(sizeof(float)==4, "scalar is no 32bits");
	static_assert(sizeof(double)==8, "scalar is no 64bits");
	static_assert(sizeof(unsigned char)==1, "unsigned char is no 8bits");
	
	const auto&& prop = imageProperties();
	
	auto buff = m_imageData.initialise();
	
	int k = 0;
	auto pixel = m_imageData.m_pc.get();
	
	for(auto&& px:m_contactBuffer.m_pxs)
	{
		px.m_color.setToGamma(1.0);
		px.m_color.toRGBA8888(pixel);
		k++;
		pixel+=4;
	}
	
	return buff;
}

std::shared_ptr<const ImageProperties> vxRenderProcess::imageProperties() const
{
	return m_properties;
}

void vxRenderProcess::setImageProperties(std::shared_ptr<const ImageProperties> imageProperties)
{
	m_properties = imageProperties;
}

std::shared_ptr<Scene> vxRenderProcess::scene() const
{
	return m_scene;
}

void vxRenderProcess::setScene(const std::shared_ptr<Scene> &scene)
{
	m_scene = scene;
}



}
