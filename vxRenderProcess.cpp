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
	vxColor color;

	auto cam = scene()->defaultCamera();
	// camera throwing rays.
	while(!cam->rayIsDone())
	{
		auto coord = cam->getCoords();

		//TODO: return this to smart pointer.
		auto bk = m_bucketList.getBucket(coord.x(), coord.y());
		vxCollision collide;

		cam->resetPixel();
		// on eachpixel.
		while( !cam->pixIsDone() )
		{
			scene()->throwRay(cam->nextRay(), collide);
			
			if (collide.isValid())
			{
				bk->append(collide.getColor(), coord);
			}
		}
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
	static_assert(sizeof(float)==4, "float is no 32bits");
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
		
		// for every of their render Hit.
		//for(auto it = begin(bk);it!=end(bk);++it)
		for(uint j=0;j<sz;j++)
		{
			Hit &h = (*bk)[j];
			unsigned int compX = (h.m_xyCoef.y() * (prop->rx()-1));
			unsigned int compY = (h.m_xyCoef.x() * (prop->ry()-1));
			unsigned int dist = (compX + (compY * prop->rx())) * prop->numChannels();
			//assert(dist && dist<=numElements);
			h.m_px.toRGBA8888(buff + dist);
		}
	}

	return m_pc.get();
}



}
