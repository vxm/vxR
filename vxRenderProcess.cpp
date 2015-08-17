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

using timePoint = std::chrono::time_point<std::chrono::system_clock>;
using render = vxCompute::vxRenderProcess;

namespace vxCompute 
{

unsigned int vxRenderProcess::nMaxThreads() const
{
	return m_nThreads;
}

void vxRenderProcess::setNMaxThreads(unsigned int nMaxThreads)
{
	m_nThreads = std::min(std::thread::hardware_concurrency(), nMaxThreads);
}

vxRenderProcess::vxRenderProcess(std::shared_ptr<ImageProperties> &prop)
	:	m_bucketList(prop, 10)
	,	m_prop(prop)
	,	m_imageData(prop)
{
	m_scene = std::make_shared<vxScene>(prop);
	m_scene->build();
	setNMaxThreads(100);
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


#define USE_THREADS 1
vxStatus::code vxRenderProcess::execute()
{
	timePoint start = std::chrono::system_clock::now();
	m_finished = false;

#if USE_THREADS
	const auto& minUpdateInterval = 1; //seconds
	unsigned int customUpdateInterval = minUpdateInterval;
	
	std::cout << "Using " << m_nThreads << " threads" << std::endl;
	std::vector<std::thread> threads;
	
	for(unsigned int i=0;i<m_nThreads; i++)
	{
		auto&& th = std::thread([this,i]{this->render(m_nThreads,i);});
		threads.push_back(std::move(th));
	}
	
	double accelerationRatio{1.0};
	double prevProgress{-1};
	while(!m_finished)
	{
		double dProgress = progress();
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

double vxRenderProcess::progress() const
{
	return  100.0 * m_progress.load() / (m_prop->ry()*m_nThreads);
}

#define SINGLERAY 0
vxStatus::code vxRenderProcess::render(unsigned int by, unsigned int offset)
{
	assert(offset<this->imageProperties()->rx());
	const unsigned int visSamples {3u};
	const unsigned int rfxSamples {4u};
	const auto&& rCamera = scene()->defaultCamera();
	const auto&& invSamples = 1.0/(double)visSamples;
	vxSampler sampler(visSamples);

	// moving to start point.
	unsigned int itH {offset};
	unsigned int itV {0u};


#if SINGLERAY
	vxColor color;
	vxVector2d hitCoordinates(.55, .25);

//TODO: return this to smart pointer.
	auto bk = m_bucketList.getBucket(hitCoordinates);
	vxCollision collision;
	const auto&& ray = rCamera->ray(hitCoordinates, sampler);
	if(m_scene->throwRay(ray, collision))
	{
		const auto&& baseColor = collision.color();
		color.add(baseColor);
	}
	
	bk->append(color*invSamples, hitCoordinates);

#else	
	// on eachpixel.
	while(!(itV>=(m_prop->ry())))
	{
		vxColor color;
		vxVector2d hitCoordinates(itH/(double)m_prop->rx(),
									itV/(double)m_prop->ry());

		//TODO: return this to smart pointer.
		auto bk = m_bucketList.getBucket(hitCoordinates);
		for(unsigned int s=0;s<visSamples;s++)
		{
			vxCollision collision;
			const auto&& ray = rCamera->ray(hitCoordinates, sampler);
			if(m_scene->throwRay(ray, collision))
			{
				if(collision.isValid())
				{
					const auto&& N = collision.normal();
					auto incidence = ray.incidence(N);
					for(int k = 0;k<rfxSamples;k++)
					{
						vxVector3d&& invV = vxVector3d( N[0]==0 ? 1 : -1,
														N[1]==0 ? 1 : -1,
														N[2]==0 ? 1 : -1);
						const auto range=0.2;
						invV+=vxVector3d(MathUtils::getRand(range)-(range/2.0),
										 MathUtils::getRand(range)-(range/2.0),
										 MathUtils::getRand(range)-(range/2.0));
						vxCollision refxCollision;
						const auto&& reflexRay = vxRay(collision.position()
														+N/10000,
														ray.direction()*invV);
						if(m_scene->throwRay(reflexRay, refxCollision))
						{
							color.mixSumm(refxCollision.color(), 0.25 * (1.0/rfxSamples) );
						}
					}
				}
				color.add(collision.color());
			}
			sampler.next();
		}
		sampler.resetIterator();
		
		bk->append(color*invSamples, hitCoordinates);

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

void vxRenderProcess::createBucketList()
{
	m_bucketList.reset(m_prop,10);
}

const unsigned char *
vxRenderProcess::generateImage()
{
	static_assert(sizeof(float)==4, "double is no 32bits");
	static_assert(sizeof(double)==8, "double is no 64bits");
	static_assert(sizeof(unsigned char)==1, "unsigned char is no 8bits");

	const auto&& prop = imageProperties();

	auto buff = m_imageData.initialise();
	
	// on each bucket
	for(unsigned int i=0;i<m_bucketList.size();i++)
	{
		std::vector<Hit> *bk = m_bucketList[i].m_pb.getHits();
		auto sz = m_bucketList[i].m_pb.hitsCount();
		for(unsigned int j=0;j<sz;j++)
		{
			Hit &h = (*bk)[j];
			auto pixel = m_imageData.get(h.m_xyCoef);

			// Pixel postprocess
			h.m_px.setToGamma(2.2);
			h.m_px.toRGBA8888(pixel);
		}
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
