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

	render rp;
	rp.execute();

	const char* b = rp.getPixelBuffer(1200, 900, 
									  render::ImgFormat::k32,
									  render::ImgChannels::kRGBA);

	QImage img( 1200, 900, QImage::Format_RGB32);

	
	img.save(QString("image.bmp"),"BMP");

	return a.exec();
}
