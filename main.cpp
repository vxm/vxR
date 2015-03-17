#include "vxrendermain.h"
#include <QApplication>
#include <vxrenderprocess.h>
#include <QImage>
#include <imageproperties.h>

int main(int argc, char *argv[])
{
	using render = vxCompute::vxRenderProcess;

	QApplication a(argc, argv);
	vxRenderMain w;
	w.show();

	// create the render process
	render rp;
	
	// if buffer is created it will then be used to store 
	// the render while rendering.
	vxStorage::ImageProperties imgDesc(120, 90);
	
	auto bff = rp.createBucketList(imgDesc);

	// executes the render.
	rp.execute();

	// storing an image from the buffer obtained.
	if (bff!=nullptr)
	{
		QImage img( bff, imgDesc.rx(), imgDesc.ry(), QImage::Format_RGBA8888);

		img.save(QString("image.bmp"),"BMP");
	}
	
	return a.exec();
}
