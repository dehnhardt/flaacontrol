#ifndef FLOWCONTROL_H
#define FLOWCONTROL_H

#include <QDialog>
#include <QMoveEvent>

namespace Ui
{
class FlowControl;
}

class FlowControl : public QDialog
{
	Q_OBJECT

public:
	explicit FlowControl(QWidget *parent = 0);
	~FlowControl();

protected:
	void mousePressEvent(QMouseEvent *event) override;

private:
	Ui::FlowControl *ui;
};

#endif // FLOWCONTROL_H
