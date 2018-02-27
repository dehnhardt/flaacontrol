#ifndef FLCMODULEINSTANCESHANDLER_H
#define FLCMODULEINSTANCESHANDLER_H

#include "../flaaoscsdk/oschandler.h"

#include <QObject>

class FLOModuleInstanceDAO;
class FLCModuleInstancesModel;

class FLCModuleInstancesHandler : public QObject, public OscHandler
{
	Q_OBJECT

public:
	FLCModuleInstancesHandler();
	bool handle(oscpkt::UdpSocket *socket, oscpkt::Message *message) override;
	void setModel(FLCModuleInstancesModel *moduleInstancesModel);

signals:
	void moduleInstanceAdded(FLOModuleInstanceDAO *module);

public slots:
	bool requestSave();
	bool requestStructure();
	bool addModuleInstance(FLOModuleInstanceDAO *module);

private: // methods
	void initFomModel();

private: // mebers
	FLCModuleInstancesModel *m_pModuleInstancesModel = 0;

};

#endif // FLCMODULEINSTANCESHANDLER_H
