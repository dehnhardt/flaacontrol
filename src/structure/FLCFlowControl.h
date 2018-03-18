#ifndef FLCFLOWCONTROL_H
#define FLCFLOWCONTROL_H

#include "FLModuleDefs.h"
#include "../model/FLCRepositoryModule.h"
#include "../model/FLCRepositoryModuleModel.h"
#include "../model/FLCModuleInstanceAttributesModel.h"

#include <QDialog>
#include <QMoveEvent>
#include <QUuid>
#include <map>
#include <vector>

class FLCModuleInstancesModel;
class FLCModuleInstancesHandler;
class FLOModuleInstanceDAO;
class FLCModuleWidget;

namespace Ui
{
class FlowControl;
}

class FLCFlowControl : public QDialog
{
	Q_OBJECT

public: // Methods
	explicit FLCFlowControl(QWidget *parent = 0);
	~FLCFlowControl();

	QIcon iconForModule(flaarlib::MODULE_TYPE moduleType, flaarlib::DATA_TYPE dataType);
	std::map<flaarlib::MODULE_TYPE, FLCRepositoryModuleModel *> getFlcModulesModelMap() const;
	void setAttributesTableModel(FLCModuleInstanceAttributesModel *moduleInstanceAttributeModel);

private: // Methods
	void connectSlots();
	void setupUi();
	void setupStructureObjects();
	void getRepositoryModules();
	void clearModuleMap();
	FLCModuleWidget *createModuleWidget(FLOModuleInstanceDAO *module);
	void deleteModuleWidget(FLCModuleWidget *moduleWidget);

private slots:
	void saveStructure();

private: // Members
	bool m_bDataLoaded = false;
	Ui::FlowControl *m_pUi;
	std::map<flaarlib::MODULE_TYPE, FLCRepositoryModuleModel *> m_flcModulesModelMap;
	FLCModuleInstancesModel *m_pModel = 0;
	std::map<QUuid, FLCModuleWidget *> m_flcWidgetMap;

};

#endif // FLOWCONTROL_H
