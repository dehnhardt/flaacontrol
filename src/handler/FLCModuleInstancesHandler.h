#ifndef FLCMODULEINSTANCESHANDLER_H
#define FLCMODULEINSTANCESHANDLER_H

#include "../osc/oschandler.h"

#include <QObject>

class FLCModuleInstance;

class FLCModuleInstancesHandler : public QObject, public OscHandler
{
	Q_OBJECT

public slots:
	bool addModuleInstance( FLCModuleInstance *module);

public:
	FLCModuleInstancesHandler();
	bool handle(oscpkt::UdpSocket *socket, oscpkt::Message *message) override;

};

#endif // FLCMODULEINSTANCESHANDLER_H
