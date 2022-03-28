#ifndef VXRMAINUI_H
#define VXRMAINUI_H

#include <QMainWindow>

namespace Ui {
class vxRMainUI;
}

class vxRMainUI : public QMainWindow
{
		Q_OBJECT
		
	public:
		explicit vxRMainUI(QWidget *parent = 0);
		~vxRMainUI();
		
	private:
		Ui::vxRMainUI *ui;
};

#endif // VXRMAINUI_H
