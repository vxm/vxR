#include "vxrenderprocess.h"
#include "vxCamera.h"
#include "vxBoxMatrix.h"
#include <vxpixel.h>
#include <imageproperties.h>

namespace vxCompute 
{



ImageProperties vxRenderProcess::imageProperties() const
{
	return m_imageProperties;
}

void vxRenderProcess::setImageProperties(const ImageProperties &imageProperties)
{
	m_imageProperties = imageProperties;
}

vxRenderProcess::vxRenderProcess()
	: vxProcess()
{
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

	cam.set(vxVector3d (0,0,0), vxVector3d (0,0,1), 1, 1.1, 1.333 );
	cam.setPixelSamples(1);
	
	auto visto	=	0;
	auto posPixX=	0;
	auto posPixY=	0;
	
	cam.resetRay();
	
	// this is the grid object
	vxBoxMatrix mat(12, 4, 16,   6.0); // Position, size
	mat.createSphere(12, 4, 16,   140.0); // Position, radius
	auto na = mat.numActiveVoxels();

	vxPixel color;

	// camera throwing rays.
	while(!cam.rayIsDone())
	{
		color.set(0.0,0.0,0.0);

		vxCollision collide;

		cam.resetPixel();

		// on eachpixel.
		while( !cam.pixIsDone() )
		{
			mat.throwRay(cam.nextRay(), collide );
			
			if (collide.isValid())
			{
				color.add( collide.getColor() );
			}
			else
			{
				color.add(0,0,0);
			}
		}

		// this should get double
		posPixX = cam.getXCoord();
		posPixY = cam.getYCoord();
	
		double posHitX = (double) posPixX;
		double posHitY = (double) posPixY;
		
		color.setResult();
		
		auto bk = m_bList->getBucket(posHitX, posHitY);
		//m_pb->append(color, posHitX, posHitY);
	}// end camera loop
	
	
	return vxStatus::code::kSuccess;
}

vxStatus::code vxRenderProcess::preConditions()
{
	return vxStatus::code::kSuccess;
}

const unsigned char *
vxRenderProcess::createBucketList()
{
	static_assert(sizeof(float)==4, "float is no 32bits");
	static_assert(sizeof(double)==8, "double is no 64bits");
	static_assert(sizeof(unsigned char)==1, "unsigned char is no 8bits");

	unsigned char *buff{nullptr};
	size_t numElements = imageProperties().numElements();
	switch(imageProperties().format())
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
		std::cout << "unique ptr is empty now" << std::endl;
		
	return buff;
}
	
}
