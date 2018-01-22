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
	QLabel *child = static_cast<QLabel *>(childAt(event->pos()));
	if (!child)
		return;

	QPixmap pixmap = *child->pixmap();

	QByteArray itemData;
	QDataStream dataStream(&itemData, QIODevice::WriteOnly);
	dataStream << pixmap << QPoint(event->pos() - child->pos());
}
