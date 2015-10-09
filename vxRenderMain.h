#ifndef VXRENDERMAIN_H
#define VXRENDERMAIN_H

#include <QMainWindow>

namespace Ui {
class vxRenderMain;
}

class vxRenderMain : public QMainWindow
{
		Q_OBJECT
		
	public:
		explicit vxRenderMain(QWidget *parent = 0);
		~vxRenderMain();

	private:
		Ui::vxRenderMain *ui;
};

#endif // VXRENDERMAIN_H
