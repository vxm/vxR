#include <climits>
#include "vxRenderProcess.h"
#include "vxCamera.h"
#include "vxGrid.h"
#include <vxPixel.h>
#include <cassert>
#include <ImageProperties.h>
#ifdef _DEBUG
#include <iostream>
#endif


namespace vxCompute 
{
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
//#ifdef _DEBUG
	auto npx = 0u;
//#endif
	vxColor color;
	auto cam = scene()->defaultCamera();
	unsigned int nSamples = cam->getPixelSamples();

	vxColor c;
	// camera throwing rays.
	while(!cam->rayIsDone())
	{
//#ifdef _DEBUG
		npx++;
		if(npx%187333==0 || npx==m_imageProperties->numPixels())
		{
			std::cout << "Number of rays " << (npx*nSamples) << " of " << (m_imageProperties->numPixels() * nSamples)<< std::endl;
		}
//#endif
		auto coord = cam->getCoords();

		//TODO: return this to smart pointer.
		auto bk = m_bucketList.getBucket(coord.x(), coord.y());
		vxCollision collide;

		cam->resetPixel();
		// on eachpixel.
		c.reset();
		for(int i=0;i<nSamples;i++)
		{
			if (scene()->throwRay(cam->nextRay(), collide))
			{
				c = c + collide.getColor();
			}
		}
		cam->next();
		bk->append(c/(double)nSamples, coord);
	}// end camera loop
	
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
	
	if(m_pc==nullptr)
	{
		std::cout << "unique ptr is empty now" << std::endl;
	}
	
	// on each bucket
	for(uint i=0;i<m_bucketList.size();i++)
	{
		std::vector<Hit> *bk = m_bucketList[i].m_pb.getHits();
		auto sz = bk->size();
		
		unsigned int dist;
		// for every of their render Hit.
		//for(auto it = begin(bk);it!=end(bk);++it)
		for(uint j=0;j<sz;j++)
		{
			Hit &h = (*bk)[j];
			unsigned int compX = (h.m_xyCoef.y() * (prop->rx()-1));
			unsigned int compY = (h.m_xyCoef.x() * (prop->ry()-1));
			dist = (compX + (compY * prop->rx())) * prop->numChannels();
			//assert(dist && dist<=numElements);
			h.m_px.toRGBA8888(buff + dist);
		}
	}

	return m_pc.get();
}



}
