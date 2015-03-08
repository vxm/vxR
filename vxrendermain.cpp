#include "vxrendermain.h"
#include "ui_vxrendermain.h"

vxRenderMain::vxRenderMain(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::vxRenderMain)
{
	ui->setupUi(this);
}

vxRenderMain::~vxRenderMain()
{
	delete ui;
}
