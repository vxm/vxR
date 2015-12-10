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
	:	m_prop(prop)
	,	m_imageData(prop)
	,	m_contactBuffer(prop->numPixels())
	,	m_visSamples{samples}
{
	m_scene = std::make_shared<vxScene>(prop);
	setNMaxThreads(100);
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
	return m_visSamples;
}

void vxRenderProcess::setVisSamples(unsigned int visSamples)
{
	m_visSamples = visSamples;
	m_c_invSamples = 1.0/(scalar)m_visSamples;
}

unsigned int vxRenderProcess::reflectionSamples() const
{
	return m_reflectionSamples;
}

void vxRenderProcess::setReflectionSamples(unsigned int reflectionSamples)
{
	m_reflectionSamples = reflectionSamples;
}


vxStatus vxRenderProcess::setDatabase(std::shared_ptr<vxSceneParser> scn)
{
	vxStatus st;
	
	if(m_scene!=nullptr)
	{
		/*st = */m_scene->build(scn);
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
	m_contactBuffer.reserve(m_prop->numPixels());
	
#if USE_THREADS
	const auto& minUpdateInterval = 1; //seconds
	unsigned int customUpdateInterval = minUpdateInterval;
	
	std::cout << "Using " << m_nThreads << " threads" << std::endl;
	std::vector<std::thread> threads;
	for(unsigned int i=0;i<m_nThreads; i++)
	{
		auto&& th = std::thread([this,i]{this->render(m_nThreads,i);});
		vxThreadPool::threadInfo(std::this_thread::get_id());
		threads.push_back(std::move(th));
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
		std::cout << "(" << customUpdateInterval
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
	return  100.0 * m_progress.load() / (m_prop->ry()*m_nThreads);
}

vxStatus::code vxRenderProcess::render(unsigned int by, unsigned int offset)
{
	assert(offset<this->imageProperties()->rx());
	const auto&& rCamera = scene()->defaultCamera();
	vxSampler sampler(m_visSamples);
	
#if SINGLERAY
	(void)by;
	(void)offset;
	// moving to start point.
	unsigned int itH = 270;
	unsigned int itV = 227;
	vxColor color;
	
	//TODO: return this to smart pointer.
	vxCollision collision;
	const v2 hitCoordinates(
				itV/(scalar)m_prop->ry(),
				itH/(scalar)m_prop->rx());
	
	const auto ray = rCamera->ray(hitCoordinates, sampler);
	
	if(m_scene->throwRay(ray, collision))
	//if(m_scene->domeThrowRay(ray, collision))
	{
		const auto&& baseColor = collision.color();
		color.add(baseColor);
	}
	
	const auto id = itH  + (itV * m_prop->rx());
	//TODO:check ranges here, it might fail.
	m_contactBuffer.pixel(id) = vxColor::white;
	
	
#else
	
	// moving to start point.
	unsigned int itH {offset};
	unsigned int itV {0u};
	
	// on eachpixel.
	while(!(itV>=(m_prop->ry())))
	{
		vxColor pixelColor;
		const v2 hitCoordinates(
					itV/(scalar)m_prop->ry(),
					itH/(scalar)m_prop->rx());
		for(auto s=0u;s<m_visSamples;s++)
		{
			vxCollision collision;
			vxCollision refxCollision;
			vxColor reflection;
			const auto ray = rCamera->ray(hitCoordinates, sampler);
			if(m_scene->throwRay(ray, collision) && collision.isValid())
			{
				const auto& n = collision.normal();
				for(unsigned int k = 0u;k<m_reflectionSamples;k++)
				{
					v3 invV = ((n * ray.direction().dot(n) * -2.0)
									 + ray.direction());
					invV+=MU::getSolidSphereRand3(0.25);
					const auto &&reflexRay = vxRay(collision.position()
												   +(n/10000),
												   invV);
					if(m_scene->throwRay(reflexRay, refxCollision))
					{
						reflection.mixSumm(refxCollision.color(), 
										   (1.0/m_reflectionSamples));
					}
					else
					{
						m_scene->domeThrowRay(reflexRay, refxCollision);
						reflection.mixSumm(refxCollision.color(),
										   (1.0/m_reflectionSamples));
					}
				}

				//compute the shader
				vxColor col(m_scene->defaultShader()->getColor(ray,collision));
				collision.setColor( col );
				pixelColor+= MU::lerp(reflection, collision.color(), 0.96);
			}
			else
			{
				m_scene->domeThrowRay(ray, collision);
				pixelColor+=collision.color();
			}
			
			sampler.next();
		}
		
		sampler.resetIterator();
		pixelColor*=m_c_invSamples;
		
		const auto id = itH  + (itV * m_prop->rx());
		m_contactBuffer.pixel(id) = pixelColor;
		
		itH+=by;
		if(itH >= m_prop->rx())
		{
			itV+=1;
			itH %= m_prop->rx();
			
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
	return m_prop;
}

void vxRenderProcess::setImageProperties(std::shared_ptr<const ImageProperties> imageProperties)
{
	m_prop = imageProperties;
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
