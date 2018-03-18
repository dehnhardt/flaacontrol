#ifndef FLCMODULEINSTANCESPANEL_H
#define FLCMODULEINSTANCESPANEL_H

#include "FLModuleDefs.h"

#include <QWidget>
#include <QMoveEvent>
#include <QUuid>

class FLCFlowControl;
class FLCModuleInstancesModel;
class FLCModuleInstancesHandler;
class FLOModuleInstanceDAO;
class FLCModuleWidget;
class FLCRepositoryModuleModel;

class FLCModuleInstancesPanel : public QWidget
{
	Q_OBJECT

public: //methods
	explicit FLCModuleInstancesPanel(QWidget *parent = nullptr);
	~FLCModuleInstancesPanel();

	void setModel(FLCModuleInstancesModel *model);
	void setHandler(FLCModuleInstancesHandler *handler);
	FLCModuleInstancesModel *model() const;
	QIcon iconForModule(flaarlib::MODULE_TYPE moduleType, flaarlib::DATA_TYPE dataType);

protected: // events
	void mousePressEvent(QMouseEvent *event) override;
	void dragEnterEvent(QDragEnterEvent *event) override;
	void dragMoveEvent(QDragMoveEvent *event) override;
	void dropEvent(QDropEvent *event) override;

protected: // methods
	void setupUi();
	void initFomModel();
	FLCModuleWidget *createModuleWidget(FLOModuleInstanceDAO *module);
	void deleteModuleWidget(FLCModuleWidget *moduleWidget);
	QPoint snapToGrid(QPoint position, QPoint offset);

signals:
	void addModuleInstance(FLOModuleInstanceDAO *module);
	void modifyModuleInstance(FLOModuleInstanceDAO *module);
	void removeModuleInstance( QUuid uuid);

protected slots:
	void moduleWidgetAdded(FLOModuleInstanceDAO *module);
	void moduleWidgetModified(FLOModuleInstanceDAO *module);
	void removeModuleWidget( QUuid uuid);
	void moduleWidgetRemoved( QUuid uuid);


protected: // members
	double snap = 10;
	FLCFlowControl *m_pParent = 0;
	bool m_bDataLoaded = false;
	FLCModuleInstancesModel *m_pModel = 0;
	FLCModuleInstancesHandler *m_pHandler = 0;
	std::map<QUuid, FLCModuleWidget *> m_flcWidgetMap;

};

#endif // MODULEINSTANCESPANEL_H
