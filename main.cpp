#include "vxrendermain.h"
#include <QApplication>
#include <vxrenderprocess.h>
#include <QImage>

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
	auto b = rp.createPixelBuffer(120, 90,
								render::ImgFormat::k8,
								render::ImgChannels::kRGBA);

	rp.execute();

	if (b!=nullptr)
	{
		QImage img( b, 120, 90, QImage::Format_RGBA8888);

		img.save(QString("image.bmp"),"BMP");
	}
	
	return a.exec();
}
