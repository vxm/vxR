#include "vxrenderprocess.h"

vxRenderProcess::vxRenderProcess()
{
}

vxStatus::code vxRenderProcess::execute()
{
/*	QImage qtImage;

	qtImage.create(QSize(1200,900), 32, 0, QImage::IgnoreEndian);

	vxCamera cam;

	cam.set(vxVector3d (0,0,0), vxVector3d (0,0,1), 1, 1.1, 1.333 );
 
	cam.setResolution(1200,900);

	cam.setSamples(2);

	int visto=0;

	int posPixX=0;
	int posPixY=0;

	cam.resetRay();

	vxBoxMatrix mat(entrX->value() ,entrY->value() ,entrZ->value(), 6);
	mat.createSphere(entrX->value() ,entrY->value() ,entrZ->value(), 2);
//	mat.createRandom();

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
		qtImage.setPixel( posPixX , posPixY , qRgba( color.getR(), color.getG() , color.getB(), 255));
	}


	actualizeImage(qtImage);

	qtImage.save(QString("c:/image.bmp"),"BMP");
*/
}
	
