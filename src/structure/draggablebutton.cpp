#include "draggablebutton.h"
#include <QApplication>
#include <QDebug>

DraggableButton::DraggableButton() :
	QPushButton()

{
	setStyleSheet("backround-opacity: 0; border-style: none");
}

DraggableButton::DraggableButton(const QString &sText, const QIcon &icon, QWidget *pParent) :
	QPushButton(icon, sText, pParent)

{
	setStyleSheet("text-align: left; padding-left: 5");
}

void DraggableButton::mousePressEvent(QMouseEvent *event)
{
	qDebug() << "mouse press";
	//QApplication::sendEvent(parentWidget(), event);
}

void DraggableButton::mouseMoveEvent(QMouseEvent *event)
{
	qDebug() << "mouse move";
	emit( buttonDragStart( this, event));
}
