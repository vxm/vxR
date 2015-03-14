#include "vxrendermain.h"
#include <QApplication>
#include <vxrenderprocess.h>
#include <QImage>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	vxRenderMain w;
	w.show();
	
	vxCompute::vxRenderProcess rp;
	rp.execute();
	
	QImage img(1200, 900, QImage::Format_RGB32);
	img.save(QString("image.bmp"),"BMP");

	return a.exec();
}
