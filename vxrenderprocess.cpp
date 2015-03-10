#include "vxrenderprocess.h"

vxRenderProcess::vxRenderProcess()
{
}

vxStatus::code vxRenderProcess::execute()
{
	QImage qtImage;

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
	vxPixel color;

	while(!cam.rayIsDone())
	{
		/*vxCollision collide;

		mat.throwRay(cam.nextRay(), collide );

		vxColor color;

		if (collide.isValid()) 

			color = collide.getColor();

		else

			color.set(14,34,27);
	
		qtImage.setPixel( cam.getXCoord() , cam.getYCoord(), qRgba( color.getR(), color.getG() , color.getB(), 255));
*/
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
  /*

	QImage qtImage;

	qtImage.create(QSize(800,600), 32, 0, QImage::IgnoreEndian);

	vxCamera cam;

	cam.set(vxVector3d (0,0,0), vxVector3d (0,0,1), 1, 1.1, 1.333 );
 
	cam.setResolution(800,600);
	cam.setSamples(32);

	int visto=0;

	int posPixX=0;
	int posPixY=0;

	cam.resetRay();

	vxBoxN *caj = vxGlobal::getBox(entrX->value() ,entrY->value() ,entrZ->value(), 4);
	
	vxPixel color;

	while( !cam.rayIsDone() )
	{
		color.set(0,0,0);

		vxCollision collide;

		cam.resetPixel();

		while( !cam.pixIsDone() )
		{
			caj->throwRay( cam.nextRay(), collide);
			
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
		
		caj->initialize();
	}

	delete caj;

	actualizeImage(qtImage);

	qtImage.save(QString("c:/only_one.bmp"),"BMP");*/

}

