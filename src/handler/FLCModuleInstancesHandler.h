#ifndef FLCMODULEINSTANCESHANDLER_H
#define FLCMODULEINSTANCESHANDLER_H

#include "../flaaoscsdk/oschandler.h"

#include <QObject>
#include <QUuid>

class FLOModuleInstanceDAO;
class FLCModuleInstancesModel;
class FLCModuleInstanceAttributesModel;

class FLCModuleInstancesHandler : public QObject, public OscHandler
{
	Q_OBJECT

public:
	FLCModuleInstancesHandler();
	bool handle(oscpkt::UdpSocket *socket, oscpkt::Message *message) override;
	void setModel(FLCModuleInstancesModel *moduleInstancesModel);

signals:
	void moduleInstanceAdded(FLOModuleInstanceDAO *module);
	void moduleInstanceModified(FLOModuleInstanceDAO *module);
	void moduleInstanceRemoved(QUuid uuid);
	void moduleInstanceAttributeModified(FLOModuleInstanceDAO *module);

public slots:
	bool requestSave();
	bool requestStructure();
	bool addModuleInstance(FLOModuleInstanceDAO *module);
	bool modifyModuleInstance(FLOModuleInstanceDAO *module);
	bool removeModuleInstance(QUuid uuid);

private: // methods
	void initFomModel();

private: // mebers
	FLCModuleInstancesModel *m_pModuleInstancesModel = 0;
	FLCModuleInstanceAttributesModel *m_pModuleInstanceAttributesModel = 0;

};

#endif // FLCMODULEINSTANCESHANDLER_H
