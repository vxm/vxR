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

using timePoint = std::chrono::time_point<std::chrono::system_clock>;
using render = vxCompute::vxRenderProcess;

namespace vxCompute 
{

unsigned int vxRenderProcess::nMaxThreads() const
{
	return m_nMaxThreads;
}

void vxRenderProcess::setNMaxThreads(unsigned int nMaxThreads)
{
	m_nMaxThreads = nMaxThreads;
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

	const auto &updateInterval = 2; //seconds
#if USE_THREADS
	const auto nTh = std::min(std::thread::hardware_concurrency(), m_nMaxThreads);
	std::thread a([&]{this->render(nTh,0);});
	std::thread b([&]{this->render(nTh,1);});
#else
	std::thread a([&]{this->render(1,0);});
	std::thread b([&]{});
#endif
	while(!m_finished)
	{
		std::this_thread::sleep_for(std::chrono::seconds(updateInterval));
		std::cout << "(" << updateInterval << ") progress update: " << std::setprecision(2) << progress() << std::endl;
	}

	a.join();
	b.join();
	std::cout << "Render Process done, took: " << TimeUtils::decorateTime(start, 2);

	return vxStatus::code::kSuccess;
}

double vxRenderProcess::progress() const
{
	return 100.0 * m_progress.load()  / m_prop->numPixels();
}

vxStatus::code vxRenderProcess::render(unsigned int by, unsigned int offset)
{
	assert(offset<this->imageProperties()->rx());
	unsigned int nSamples = 4;
	const auto& rCamera = scene()->defaultCamera();
	const double invSamples = 1.0/(double)nSamples;
	vxSampler sampler(nSamples);
	vxCollision collision;

	// moving to start point.
	unsigned int itH {offset};
	unsigned int itV {0u};

	// on eachpixel.
	while(!(itV>=(m_prop->ry())))
	{
		vxColor c;
		vxVector2d cors(itH/(double)m_prop->rx(),
						itV/(double)m_prop->ry());

		//TODO: return this to smart pointer.
		auto bk = m_bucketList.getBucket(cors);
		for(unsigned int s=0;s<nSamples;s++)
		{
			const auto& ray = rCamera->ray(cors, sampler);
			if(m_scene->throwRay(ray, collision))
			{
				c.add(collision.color());
			}
			sampler.next();
		}
		sampler.resetIterator();
		//vxColor fCol = (c*invSamples);
		bk->append(c, cors);

		itH+=by;
		if(itH >= m_prop->rx())
		{
			itV+=1;
			itH %= m_prop->rx();

			m_progress.store(m_progress.load() + (double)itV);
		}
	}

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

	const auto& prop = imageProperties();
	size_t numElements = prop->numElements();
	switch(prop->format())
	{
		case ImageProperties::ImgFormat::k8:
			m_pc.reset(new unsigned char[numElements]);
		break;
		case ImageProperties::ImgFormat::k32:
			m_pf.reset(new float[numElements]);
		break;
		case ImageProperties::ImgFormat::k64:
			m_pd.reset(new double[numElements]);
		break;
		default:
			m_pc.reset(new unsigned char[numElements]);
		break;
	}
	
	// hardcode buffer type!!
	unsigned char *buff = m_pc.get();
	unsigned char *tmpp = buff;
	for(;tmpp<buff+numElements;tmpp++)
	{
		*tmpp = '\0';
	}

	if(m_pc==nullptr)
	{
		std::cout << "unique ptr is empty now" << std::endl;
	}
	
	// on each bucket
	for(unsigned int i=0;i<m_bucketList.size();i++)
	{
		std::vector<Hit> *bk = m_bucketList[i].m_pb.getHits();
		auto sz = m_bucketList[i].m_pb.hitsCount();

		unsigned int dist;
		for(unsigned int j=0;j<sz;j++)
		{
			Hit &h = (*bk)[j];

			// Pixel postprocess
		//	h.m_px.setToGamma(2.2);


			unsigned int compX = h.m_xyCoef.y() * (prop->rx()+1);
			unsigned int compY = h.m_xyCoef.x() * (prop->ry()+1);
			dist = (compX + (compY * prop->rx())) * prop->numChannels();
			if(dist>numElements)
			{
				dist = numElements;
			}
			h.m_px.toRGBA8888(buff + dist);
		}
	}

	return m_pc.get();
}



}
