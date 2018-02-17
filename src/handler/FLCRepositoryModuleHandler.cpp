#include "FLCRepositoryModuleHandler.h"

#include "../Flaacontrol.h"
#include "../model/FLCRepositoryModule.h"
#include "../model/FLCRepositoryModuleModel.h"

#include "../flaaoscsdk/oscsender.h"

#include <QDebug>

FLCRepositoryModuleHandler::FLCRepositoryModuleHandler() :
	OscHandler ("/ws/repository/module")
{
	m_sHandlerName="FLCRepositoryModuleHandler";
}

bool FLCRepositoryModuleHandler::handle(UdpSocket *socket __attribute__((unused)), Message *message )

{
	std::string moduleTypeName;
	int	type;
	int dataType;
	std::string functionalName;
	std::string description;

	if(message->arg().popStr(moduleTypeName).popInt32(type).popInt32(dataType).popStr(functionalName).popStr(description).isOk())
	{
		qDebug() << "recieced " << moduleTypeName.c_str();
		qDebug() << "\ttype: " << type;
		qDebug() << "\tdataType: " << dataType;
		qDebug() << "\tfunctionalName " << functionalName.c_str();
		qDebug() << "\tdescription " << description.c_str();
		FLCRepositoryModule *m = new FLCRepositoryModule(type, dataType, moduleTypeName, functionalName, description);
		addToModel(m);
	}
	return true;
}

void FLCRepositoryModuleHandler::setModel(flaarlib::MODULE_TYPE moduleType, FLCRepositoryModuleModel *repositoryModuleModel)
{
	m_flcModuleModels[moduleType] = repositoryModuleModel;
}

void FLCRepositoryModuleHandler::addToModel(FLCRepositoryModule *flcModule)
{
	FLCRepositoryModuleModel *moduleModel = m_flcModuleModels[flcModule->moduleType()];
	if( moduleModel )
		moduleModel->addModule(flcModule);
}

void FLCRepositoryModuleHandler::requestRepository()
{
	OscSender *sender = Flaacontrol::instance()->udpSender();
	Message msg("/ws/repository/get");
	sender->enqueuMessage(msg);
}

