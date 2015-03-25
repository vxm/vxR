#include "vxRenderProcess.h"
#include "vxCamera.h"
#include "vxGrid.h"
#include <vxPixel.h>
#include <ImageProperties.h>

namespace vxCompute 
{



std::shared_ptr<const ImageProperties> vxRenderProcess::imageProperties() const
{
	return m_imageProperties;
}

void vxRenderProcess::setImageProperties(std::shared_ptr<const ImageProperties> imageProperties)
{
	m_imageProperties = imageProperties;
}

vxStatus::code vxRenderProcess::preProcess(vxProcess *p)
{
	
	return vxStatus::code::kSuccess;
}

vxStatus::code vxRenderProcess::postProcess(vxProcess *p)
{

	return vxStatus::code::kSuccess;
}

vxStatus::code vxRenderProcess::execute()
{
	vxCamera cam(imageProperties());

	cam.set(vxVector3d	(0,0,0), 
			vxVector3d	(0,0,1), 
						1, 
						1.1, 
						1.333 );

	cam.setPixelSamples(1);
	
	cam.resetRay();
	
	// this is the grid object
	vxGrid mat(6, 4, 16,   5.0); // Position, size
	mat.setResolution(5);
	mat.createSphere(6, 4, 16,  40.0); // Position, radius
	auto na = mat.numActiveVoxels();

	vxColor color;

	// camera throwing rays.
	while(!cam.rayIsDone())
	{
		// this should get double
		auto xCoord = cam.getXCoord();
		auto yCoord = cam.getYCoord();
		
		auto bk = m_bucketList.getBucket(xCoord, yCoord);
		
		color.reset();

		vxCollision collide;

		cam.resetPixel();
		// on eachpixel.
		while( !cam.pixIsDone() )
		{
			auto ray = cam.nextRay();
			mat.throwRay(ray, collide);
			
			if (collide.isValid())
			{
				bk.append(collide.getColor(),
									xCoord,
									yCoord);
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
	unsigned char *buff{nullptr};
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
	
	buff = m_pc.get();
	
	if(m_pc==nullptr)
	{
		std::cout << "unique ptr is empty now" << std::endl;
	}

	// on each bucket
	for(int i=0;i<m_bucketList.size();i++)
	{
		auto&& bk = m_bucketList[i].m_pb.getHits();
		
		// for every of their render Hit.
		for(auto it = begin(bk);it!=end(bk);++it)
		{
			auto tbuff = buff;
			const Hit &h = *it;
			int dist = (h.m_xcoef) + (h.m_ycoef*prop->rx());
			
			if(dist<=numElements)
			{
				tbuff+=dist;
				*tbuff=222;
				tbuff++;
				*tbuff=22;
				tbuff++;
				*tbuff=2;
			}
		}
	}
	
	return buff;
}
	


}
