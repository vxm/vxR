#include <iostream>
#include <climits>
#include <cassert>
#include <thread>
#include <future>

#include "vxRenderProcess.h"
#include "vxCamera.h"
#include "vxGrid.h"
#include "vxPixel.h"
#include "ImageProperties.h"
#include "TimeUtils.h"
#include "vxSampler.h"
#include "vxThreadPool.h"

#define SINGLERAY 0

#ifdef _DEBUG
#define USE_THREADS 0
#else
#define USE_THREADS 1
#endif

using timePoint = std::chrono::time_point<std::chrono::system_clock>;
using render = vxCompute::vxRenderProcess;


namespace vxCompute 
{

vxRenderProcess::vxRenderProcess(std::shared_ptr<ImageProperties> &prop, 
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


vxStatus vxRenderProcess::setDatabase(std::shared_ptr<vxSceneParser> )
{
	vxStatus st;
	
	if(m_scene!=nullptr)
	{
		/*st = m_scene->build(scn)*/;
	}
	
	return st;
}

vxStatus::code vxRenderProcess::preProcess(vxProcess *p)
{
	if(p!=nullptr)
	{
		p->execute();
	}
	
	return vxStatus::code::kSuccess;
}

vxStatus::code vxRenderProcess::postProcess(vxProcess *p)
{
	if(p!=nullptr)
	{
		p->execute();
	}
	
	return vxStatus::code::kSuccess;
}


vxStatus::code vxRenderProcess::execute()
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
		vxThreadPool::threadInfo(std::this_thread::get_id());
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
	return vxStatus::code::kSuccess;
}

scalar vxRenderProcess::progress() const
{
	return  100.0 * m_progress.load() / (m_properties->ry()*m_nThreads);
}

vxColor vxRenderProcess::computeLight(const vxRay &ray, vxCollision &col)
{
	auto retColor{vxColor::zero};
	m_scene->throwRay(ray, col);
	
	if(col.isValid())
	{
		retColor = m_scene->defaultShader()->getIlluminatedColor(ray,col);
	}
	else
	{
		m_scene->domeThrowRay(ray, col);
		retColor = col.color();
	}
	
	return retColor;
}

vxStatus::code vxRenderProcess::render(unsigned int by, unsigned int offset)
{
	assert(offset<this->imageProperties()->rx());
	const auto&& rCamera = scene()->camera();
	vxSampler sampler(m_samples);
	
#if SINGLERAY
	(void)by;
	(void)offset;
	// moving to start point.
	unsigned int itH = 636;
	unsigned int itV = 550;
	
	//TODO: return this to smart pointer.
	vxCollision collision;
	const v2 hitCoordinates(
				itV/(scalar)m_properties->ry(),
				itH/(scalar)m_properties->rx());
	
	const auto ray = rCamera->ray(hitCoordinates, sampler);
	
	auto&& color = computeLight(ray, collision);
	
	const auto id = itH  + (itV * m_properties->rx());
	//TODO:check ranges here, it might fail.
	m_contactBuffer.pixel(id) = vxColor::white;
	
	
#else
	
	// moving to start point.
	unsigned int itH {offset};
	unsigned int itV {0u};
	
	const auto dome = m_scene->dome();
	
	// on eachpixel.
	while(!(itV>=(m_properties->ry())))
	{
		auto pixelColor{vxColor::zero};
		const v2 hitCoordinates(
					itV/(scalar)m_properties->ry(),
					itH/(scalar)m_properties->rx());
		
		for(auto s=0u;s<m_samples;s++)
		{
			vxCollision collision;
			const auto ray = rCamera->ray(hitCoordinates, sampler);
			
			//compute the shader
			pixelColor += computeLight(ray,collision);
			
			if(collision.isValid())
			{
				//Reflection
				if(m_reflectionSamples!=0)
				{
					auto reflection{vxColor::zero};
					{
						const auto& n = collision.normal();
						vxCollision refxCollision;
						for(unsigned int k = 0u;k<m_reflectionSamples;k++)
						{
							v3 invV = ((n * ray.direction().dot(n) * -2.0)
									   + ray.direction());
<<<<<<< HEAD
							invV+=MU::getSolidSphereRand3(0.1);
							
							const auto &&reflexRay= 
									vxRay(collision.position() + n.tiny(), invV);
							
=======
							invV+=MU::getSolidSphereRand3(0.5);
							const auto &&reflexRay = vxRay(collision.position()
														   +(n/10000),
														   invV);
>>>>>>> master
							reflection = computeLight(reflexRay, refxCollision);
						}
						
						reflection*=(1.f/(float)m_reflectionSamples);
						pixelColor+= (reflection/5.f);
					}
				}
				
				//GI
				if(m_giSamples!=0)
				{
					vxColor globalIlm;
					{
						vxColor baseColor = m_scene->defaultShader()->getColor(ray,collision);
						const auto n = m_giSamples;
						const auto colorRatio = m_giMultiplier/(scalar)n;
						for(auto i=0u; i<n; i++)
						{
							const auto&& r = MU::getHollowHemisphereRand(1.0, collision.normal());
							const vxRay giRay(collision.position()
											  +collision.normal().tiny(), 
											  r.inverted());
							
							auto rayIncidence = giRay.incidence(collision.normal());
							
							vxCollision giColl;
							
							m_scene->throwRay(giRay, giColl);
							if(giColl.isValid())
							{
								vxColor gi(m_scene->defaultShader()->getIlluminatedColor(ray,giColl));
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
						pixelColor+= globalIlm;
					}
				}
			}
			
			sampler.next();
		}
		
		sampler.resetIterator();
		pixelColor*=m_c_invSamples;
		
		const auto id = itH  + (itV * m_properties->rx());
		m_contactBuffer.pixel(id) = pixelColor;
		
		itH+=by;
		if(itH >= m_properties->rx())
		{
			itV+=1;
			itH %= m_properties->rx();
			
			m_progress.store(m_progress.load() + 1);
		}
	}
#endif
	
	auto thInfo = vxThreadPool::threadInfo(std::this_thread::get_id());
	std::cout << "Finished task on thread ::" << thInfo.id << "::" << std::endl;
	
	m_finished = true;
	
	return vxStatus::code::kSuccess;
}

vxStatus::code vxRenderProcess::preConditions()
{
	return vxStatus::code::kSuccess;
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
		px.m_color.setToGamma(2.2);
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

std::shared_ptr<vxScene> vxRenderProcess::scene() const
{
	return m_scene;
}

void vxRenderProcess::setScene(const std::shared_ptr<vxScene> &scene)
{
	m_scene = scene;
}



}
