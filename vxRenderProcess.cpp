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
						  1.333);

	cam.setPixelSamples(1);
	
	// this is the grid object
	vxGrid mat(-6, -5, 28,   5.0); // Position, size
	mat.setResolution(10);
	mat.createSphere(-6, -5, 28,  3.0); // Position, radius
	auto na = mat.numActiveVoxels();

	vxColor color;

	// camera throwing rays.
	while(!cam.rayIsDone())
	{
		// this should get double
		auto xCoord = cam.getXCoord();
		auto yCoord = cam.getYCoord();
		
		vxBucket *bk = m_bucketList.getBucket(xCoord, yCoord);
		vxCollision collide;

		cam.resetPixel();
		// on eachpixel.
		while( !cam.pixIsDone() )
		{
			auto ray = cam.nextRay();
			mat.throwRay(ray, collide );
			
			if (collide.isValid())
			{
				bk->append(collide.getColor(), xCoord, yCoord);
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
	unsigned char *buff =m_pc.get();
	
	
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
			unsigned int compX = (h.m_xcoef * (prop->rx()-1));
			unsigned int compY = (h.m_ycoef * (prop->ry()-1));
			unsigned int dist = (compX + (compY * (prop->rx())))*4;
			//assert(dist && dist<=numElements);
	
			unsigned char *tbuff = (buff) + dist;
			*tbuff= char(h.m_px.getR()*255);
			tbuff++;
			*tbuff= char(h.m_px.getG()*255);
			tbuff++;
			*tbuff= char(h.m_px.getB()*255);
		}
	}	
	return m_pc.get();
}
	


}
