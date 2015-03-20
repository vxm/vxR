#include "vxrMainUi.h"
#include "ui_vxrmainui.h"

vxRMainUI::vxRMainUI(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::vxRMainUI)
{
	ui->setupUi(this);
}

vxRMainUI::~vxRMainUI()
{
	delete ui;
}
