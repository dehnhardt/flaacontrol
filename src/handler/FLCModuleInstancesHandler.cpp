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
	qDebug("start sending module Repository (path: %s)", path.c_str());
	OscSender *sender = Flaacontrol::instance()->udpSender();
	Message msg(path);
	module->serialize(&msg);
	sender->enqueuMessage(msg);
	return(true);
}

bool FLCModuleInstancesHandler::handle(UdpSocket *socket, Message *message)
{
	Q_UNUSED(socket)
	Q_UNUSED(message)
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
	initFomModel();
	connect(m_pModuleInstancesModel, &FLCModuleInstancesModel::moduleAdded, this, &FLCModuleInstancesHandler::addModuleInstance);
}
