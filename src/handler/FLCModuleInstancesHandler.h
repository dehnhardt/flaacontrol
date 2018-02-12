#ifndef FLCMODULEINSTANCESHANDLER_H
#define FLCMODULEINSTANCESHANDLER_H

#include "../osc/oschandler.h"

#include <QObject>

class FLCModuleInstance;
class FLCModuleInstancesModel;

class FLCModuleInstancesHandler : public QObject, public OscHandler
{
	Q_OBJECT

public:
	FLCModuleInstancesHandler();
	bool handle(oscpkt::UdpSocket *socket, oscpkt::Message *message) override;
	void setModel(FLCModuleInstancesModel *moduleInstancesModel);

public slots:
	bool addModuleInstance( FLCModuleInstance *module);

private: // methods
	void initFomModel();

private: // mebers
	FLCModuleInstancesModel *m_pModuleInstancesModel = 0;

};

#endif // FLCMODULEINSTANCESHANDLER_H
