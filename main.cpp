#include "vxrendermain.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	vxRenderMain w;
	w.show();
	
	return a.exec();
}
