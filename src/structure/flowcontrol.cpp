#include "flowcontrol.h"
#include "ui_flowcontrol.h"

FlowControl::FlowControl(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::FlowControl)
{
	ui->setupUi(this);
}

FlowControl::~FlowControl()
{
	delete ui;
}

void FlowControl::mousePressEvent(QMouseEvent *event)
{
	QPushButton *child = static_cast<QPushButton *>(childAt(event->pos()));
	if (!child)
		return;

}
