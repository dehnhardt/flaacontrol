#include "FLCModuleInstancesHandler.h"
#include "../handler/FLCModuleInstancesHandler.h"
#include "logging/FLLog.h"
#include "../flaaoscsdk/oscsender.h"
#include "../Flaacontrol.h"
#include "../model/FLCModuleInstancesModel.h"
#include "../flaaoscsdk/FLOModuleInstanceDAO.h"

#include <QDebug>

FLCModuleInstancesHandler::FLCModuleInstancesHandler():
	QObject(),
	OscHandler ("/ws/instances/modules")

{
	m_sHandlerName="FLCModuleInstancesHandler";
}

bool FLCModuleInstancesHandler::addModuleInstance(FLOModuleInstanceDAO *module)
{
	std::string path = prefix() + "/add";
	qDebug("start sending module (path: %s)", path.c_str());
	OscSender *sender = Flaacontrol::instance()->udpSender();
	Message msg(path);
	module->serialize(&msg);
	sender->enqueuMessage(msg);
	return(true);
}

bool FLCModuleInstancesHandler::removeModuleInstance(QUuid uuid)
{
	std::string path = prefix() + "/remove";
	qDebug("start sending module (path: %s)", path.c_str());
	OscSender *sender = Flaacontrol::instance()->udpSender();
	Message msg(path);
	msg.pushStr(uuid.toString());
	sender->enqueuMessage(msg);
	return(true);
}

bool FLCModuleInstancesHandler::requestSave()
{
	std::string path = prefix() + "/save";
	qDebug("saving instances (path: %s)", path.c_str());
	OscSender *sender = Flaacontrol::instance()->udpSender();
	Message msg(path);
	sender->enqueuMessage(msg);
	return(true);
}

bool FLCModuleInstancesHandler::requestStructure()
{
	std::string path = prefix() + "/structure";
	qDebug("requesting all module instances (path: %s)", path.c_str());
	OscSender *sender = Flaacontrol::instance()->udpSender();
	Message msg(path);
	sender->enqueuMessage(msg);
	return(true);
}

bool FLCModuleInstancesHandler::handle(UdpSocket *socket, Message *message)
{
	Q_UNUSED(socket)
	std::string function = lastPathToken(message->addressPattern());
	qDebug( "Function String: %s", function.c_str());
	if( function == "added")
	{
		FLOModuleInstanceDAO *moduleInstance = new FLOModuleInstanceDAO();
		moduleInstance->deserialize(message);
		qDebug() << "About to add module instance " << moduleInstance->uuid().toString();
		emit( moduleInstanceAdded(moduleInstance) );
		return true;
	}
	if( function == "removed")
	{
		QString uuid;
		FLOModuleInstanceDAO *moduleInstance = new FLOModuleInstanceDAO();
		message->arg().popStr(uuid);
		qDebug() << "About to add module instance " << moduleInstance->uuid().toString();
		emit( moduleInstanceRemoved(QUuid(uuid)) );
		return true;
	}
	qDebug("Function not handled: %s", function.c_str());
	return false;
}

void FLCModuleInstancesHandler::initFomModel()
{
	if( m_pModuleInstancesModel)
	{
		QMap<QUuid, FLOModuleInstanceDAO *> map = m_pModuleInstancesModel->getModuleInstancesMap();
		for( auto module : map)
			addModuleInstance(module);
	}
}


void FLCModuleInstancesHandler::setModel(FLCModuleInstancesModel *moduleInstancesModel)
{
	m_pModuleInstancesModel = moduleInstancesModel;
	connect(this, &FLCModuleInstancesHandler::moduleInstanceAdded, m_pModuleInstancesModel, &FLCModuleInstancesModel::addFLCModuleInstance);
	connect(this, &FLCModuleInstancesHandler::moduleInstanceRemoved, m_pModuleInstancesModel, &FLCModuleInstancesModel::removeFLCModuleInstance );
}
