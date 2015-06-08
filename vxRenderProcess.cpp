#include <climits>
#include <cassert>
#include <thread>
#include <future>

#include "vxRenderProcess.h"
#include "vxCamera.h"
#include "vxGrid.h"
#include "vxPixel.h"
#include "ImageProperties.h"
#ifdef _DEBUG
#include <iostream>
#endif


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

vxStatus::code vxRenderProcess::execute()
{
	unsigned int by = 1u;
	unsigned int offset = 0u;
	
//#ifdef _DEBUG
	auto npx = 0u;
//#endif

	vxColor color;
	auto rCamera = scene()->defaultCamera();
	unsigned int nSamples = rCamera->getPixelSamples();
	const double invSamples = 1.0/(double)nSamples;
	const auto nTh = 1;//std::min(std::thread::hardware_concurrency(),
						//m_nMaxThreads);

	std::vector<vxCollision> collisions(nTh);
	vxColor c;
	// camera throwing rays.
	rCamera->next(offset);
	while(!rCamera->rayIsDone())
	{
//#ifdef _DEBUG
		npx++;
		if(npx%57333==0 || npx==m_imageProperties->numPixels())
		{
			auto pct = 100.0 * npx / m_imageProperties->numPixels();
			std::cout << pct << "% done. -- ray " << ((npx*nSamples)+scene()->dRays) << " on " << (m_imageProperties->numPixels()) << " pixels" << std::endl;
		}
//#endif

		auto coords = rCamera->coords();
		//vxVector2d coords(.83,.5);

		//TODO: return this to smart pointer.
		auto bk = m_bucketList.getBucket(coords.x(), coords.y());
		rCamera->resetSampler();
		// on eachpixel.
		c.reset();
		for(unsigned int s=0;s<nSamples;s++)
		{
			if(vxScene::throwRay(scene().get(), //shared?
									rCamera->nextSampleRay(),
									std::ref(collisions[0])))
			{
				c.add(collisions[0].color());
			}
		}
		bk->append(c*invSamples, coords);

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
