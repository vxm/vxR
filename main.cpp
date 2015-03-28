#include <QImage>

#include "vxRenderMain.h"
#include <QApplication>
#include <vxRenderProcess.h>
#include <ImageProperties.h>

int main(int argc, char *argv[])
{
	using render = vxCompute::vxRenderProcess;

	QApplication a(argc, argv);
	vxRenderMain w;
	w.show();
	
	// if buffer is created it will then be used to store 
	// the render while rendering.
	std::shared_ptr<vxStorage::ImageProperties> 
			imgDesc(new vxStorage::ImageProperties(10, 10));

	// create the render process
	render rp(imgDesc);
	
	// creates the bucket list (vector) using the img 
	// description.
	rp.createBucketList();
	
	// executes the render.
	rp.execute();

	// generates an image buffer and fills it 
	// with the render results. Buffer properties
	// are based on ImageProperties stored in the 
	// render process object.
	auto bff = rp.generateImage();

	// storing an image from the buffer obtained.
	if (bff!=nullptr)
	{
		QImage img( bff, 
					imgDesc->rx(), 
					imgDesc->ry(), 
					QImage::Format_RGBA8888);

		img.save(QString("image.bmp"),"BMP");
	}
	
	return a.exec();
}
