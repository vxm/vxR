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

#ifdef _DEBUG
#include <iostream>
#endif

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
	std::cout << "Starting render AT: " << TimeUtils::decorateTime(start) << std::endl;

	const auto nTh = std::min(std::thread::hardware_concurrency(), m_nMaxThreads);

#if USE_THREADS
	std::thread a([&]{this->render(1,0);});
	std::thread b([&]{});
#else
	std::thread a([&]{this->render(nTh,0);});
	std::thread b([&]{this->render(nTh,1);});
#endif


	a.join();
	b.join();
	std::cout << "Finish Render Process: " << TimeUtils::decorateTime(start) << std::endl;

	return vxStatus::code::kSuccess;
}

vxStatus::code vxRenderProcess::render(unsigned int by, unsigned int offset)
{
	const auto& rCamera = scene()->defaultCamera();
	unsigned int nSamples = rCamera->getPixelSamples();
	const double invSamples = 1.0/(double)nSamples;

	vxCollision collision;
	vxColor c;

	// moving to start point.
	rCamera->resetRay();
	rCamera->next(offset);
	// on eachpixel.
	while(!rCamera->rayIsDone())
	{
		auto coords = rCamera->coords();

		//TODO: return this to smart pointer.
		auto bk = m_bucketList.getBucket(coords.x(), coords.y());
		rCamera->resetSampler();
		for(unsigned int s=0;s<nSamples;s++)
		{
			if(vxScene::throwRay(scene(), //shared?
									rCamera->nextSampleRay(),
									std::ref(collision)))
			{
				c.add(collision.color());
			}
		}
		bk->append(c*invSamples, coords);
		c.reset();
		rCamera->next(by);
	}

	return vxStatus::code::kSuccess;
}

vxStatus::code vxRenderProcess::preConditions()
{
	return vxStatus::code::kSuccess;
}

void vxRenderProcess::createBucketList()
{
	m_bucketList.reset(m_imageProperties,10);
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
		// for every of their render Hit.
		//for(auto it = begin(bk);it!=end(bk);++it)
		for(unsigned int j=0;j<sz;j++)
		{
			Hit &h = (*bk)[j];
			unsigned int compX = h.m_xyCoef.y() * (prop->rx()+1);
			unsigned int compY = h.m_xyCoef.x() * (prop->ry()+1);
			dist = (compX + (compY * prop->rx())) * prop->numChannels();
			if(dist>numElements)
				dist = numElements;
			h.m_px.toRGBA8888(buff + dist);
		}
	}

	return m_pc.get();
}



}
