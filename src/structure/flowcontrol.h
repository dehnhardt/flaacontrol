#ifndef FLOWCONTROL_H
#define FLOWCONTROL_H

#include "draggablebutton.h"
#include "FLModuleDefs.h"
#include "../model/FLCRepositoryModule.h"
#include "../model/FLCRepositoryModuleModel.h"

#include <QDialog>
#include <QMoveEvent>
#include <QUuid>
#include <map>
#include <vector>

class FLCModuleInstancesModel;
class FLCModuleInstance;
class FLCModuleWidget;

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

	void setModel(FLCModuleInstancesModel *model);
	FLCModuleInstancesModel *model() const;
	QIcon iconForModule(flaarlib::MODULE_TYPE moduleType, flaarlib::DATA_TYPE dataType);

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

private slots:
	void saveStructure();
	void readStructure();
	void addModuleWidget( FLCModuleInstance *module);
	void removeModuleWidget( QUuid uuid);

private: // Members
	bool m_bDataLoaded = false;
	Ui::FlowControl *m_pUi;
	std::map<flaarlib::MODULE_TYPE, FLCRepositoryModuleModel *> m_flcModulesModelMap;
	FLCModuleInstancesModel *m_pModel = 0;
	std::map<QUuid, FLCModuleWidget *> m_flcWidgetMap;

};

#endif // FLOWCONTROL_H
