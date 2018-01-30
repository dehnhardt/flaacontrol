#ifndef DRAGGABLEBUTTON_H
#define DRAGGABLEBUTTON_H

#include <QPushButton>
#include <QMouseEvent>

class DraggableButton : public QPushButton
{
	Q_OBJECT

public:
	DraggableButton(const QString &sText, const QIcon &icon, QWidget *pParent);
	DraggableButton();

protected:
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;

signals:
	void buttonDragStart(DraggableButton *me, QMouseEvent *event);

};

#endif // DRAGGABLEBUTTON_H
