#ifndef FLCMODULEINSTANCESHANDLER_H
#define FLCMODULEINSTANCESHANDLER_H

#include "../flaaoscsdk/oschandler.h"

#include <QObject>

class FLOModuleInstanceDAO;
class FLOModuleInstancesModel;

class FLCModuleInstancesHandler : public QObject, public OscHandler
{
	Q_OBJECT

public:
	FLCModuleInstancesHandler();
	bool handle(oscpkt::UdpSocket *socket, oscpkt::Message *message) override;
	void setModel(FLOModuleInstancesModel *moduleInstancesModel);

public slots:
	bool addModuleInstance( FLOModuleInstanceDAO *module);

private: // methods
	void initFomModel();

private: // mebers
	FLOModuleInstancesModel *m_pModuleInstancesModel = 0;

};

#endif // FLCMODULEINSTANCESHANDLER_H
