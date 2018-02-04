#ifndef FLOWCONTROL_H
#define FLOWCONTROL_H

#include "draggablebutton.h"
#include "FLModuleDefs.h"
#include "../model/FLCRepositoryModule.h"
#include "../model/FLCRepositoryModuleModel.h"

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
	//void buttonDragStart(DraggableButton *draggableButton, QMouseEvent *event);

private slots:
	void repositoryItemClicked(const QModelIndex &index);

protected:
	void mousePressEvent(QMouseEvent *event) override;
	void dragEnterEvent(QDragEnterEvent *event) override;
	void dragMoveEvent(QDragMoveEvent *event) override;
	void dropEvent(QDropEvent *event) override;

private: // Methods
	void connectSlots();
	void setupUi();
	void setupStructureObjects();
	void getRepositoryModules();
	void clearModuleMap();

private: // Members
	bool m_bDataLoaded = false;
	Ui::FlowControl *m_pUi;
	QString m_sMimeType = "application/x-flowcontrolobject";
	//std::map<flaarlib::MODULE_TYPE, std::vector<FLCRepositoryModule *>*> m_flcModulesMap;
	std::map<flaarlib::MODULE_TYPE, FLCRepositoryModuleModel *> m_flcModulesModelMap;


};

#endif // FLOWCONTROL_H
