#include "vxrenderprocess.h"
#include "vxCamera.h"
#include "vxBoxMatrix.h"
#include <vxpixel.h>

namespace vxCompute 
{


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
	m_pb.reset(new vxPxBuffer);
	vxCamera cam;

	cam.set(vxVector3d (0,0,0), vxVector3d (0,0,1), 1, 1.1, 1.333 );
	cam.setResolution(1200,900);
	cam.setSamples(2);
	auto visto	=	0;
	auto posPixX=	0;
	auto posPixY=	0;
	cam.resetRay();
	vxBoxMatrix mat(0, 0, 0, 6); // Position, size
	mat.createSphere(0, 0, 0, 2); // Position, radius

	vxPixel color;

	while(!cam.rayIsDone())
	{
		color.set(0,0,0);

		vxCollision collide;

		cam.resetPixel();

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

		posPixX=cam.getXCoord();
		posPixY=cam.getYCoord();
	
		color.setResult();
		//qtImage.setPixel( posPixX , posPixY , qRgba( color.getR(), color.getG() , color.getB(), 255));
		m_pb->append(color, posHitX, posHitY);
	}

	actualizeImage(qtImage);

	return vxStatus::code::kSuccess;
}

vxStatus::code vxRenderProcess::preConditions()
{
	return vxStatus::code::kSuccess;
}
	
}
