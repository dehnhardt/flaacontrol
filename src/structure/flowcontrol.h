#ifndef FLOWCONTROL_H
#define FLOWCONTROL_H

#include "draggablebutton.h"
#include "FLModuleDefs.h"
#include "../model/FLCRepositoryModule.h"

#include <QDialog>
#include <QMoveEvent>
#include <map>
#include <vector>

namespace Ui
{
class FlowControl;
}

class FlowControl : public QDialog
{
	Q_OBJECT

public: // Methods
	explicit FlowControl(QWidget *parent = 0);
	~FlowControl();

public slots:
	void buttonDragStart(DraggableButton *draggableButton, QMouseEvent *event);

protected:
	void dragEnterEvent(QDragEnterEvent *event) override;
	void dragMoveEvent(QDragMoveEvent *event) override;
	void dropEvent(QDropEvent *event) override;

private: // Methods
	void setupUi();
	void setupDraggableObjects();
	void getRepositoryModules();

private: // Members
	Ui::FlowControl *m_pUi;
	QString m_sMimeType = "application/x-flowcontrolobject";
	std::map<flaarlib::MODULE_TYPE, std::vector<FLCRepositoryModule *>*> m_flcModulesMap;

};

#endif // FLOWCONTROL_H
