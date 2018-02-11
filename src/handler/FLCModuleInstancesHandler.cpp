#include "FLCModuleInstancesHandler.h"
#include "../handler/FLCModuleInstancesHandler.h"
#include "logging/FLLog.h"
#include "../osc/oscsender.h"
#include "../Flaacontrol.h"

#include <QDebug>

FLCModuleInstancesHandler::FLCModuleInstancesHandler():
	QObject(),
	OscHandler ("/ws/instances/modules")

{

}

bool FLCModuleInstancesHandler::addModuleInstance(FLCModuleInstance *module)
{
	std::string path = prefix() + "/add/";
	qDebug("start sending module Repository (path: %s)", path.c_str());
	OscSender *sender = Flaacontrol::instance()->udpSender();
	Message msg(path);
	sender->enqueuMessage(msg);
}

bool FLCModuleInstancesHandler::handle(UdpSocket *socket, Message *message)
{

}
