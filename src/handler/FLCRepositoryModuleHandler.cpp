#include "FLCRepositoryModuleHandler.h"

#include "../Flaacontrol.h"
#include "../model/FLCRepositoryModule.h"
#include "../osc/oscsender.h"

#include <QDebug>

FLCRepositoryModuleHandler::FLCRepositoryModuleHandler() :
	OscHandler ("/ws/repository/module")
{
}

bool FLCRepositoryModuleHandler::handle(UdpSocket *socket __attribute__((unused)), Message *message )

{
	std::string moduleTypeName;
	int	type;
	std::string functionalName;
	std::string description;

	if(message->arg().popStr(moduleTypeName).popInt32(type).popStr(functionalName).popStr(description).isOk())
	{
		qDebug() << "recieced " << moduleTypeName.c_str();
		qDebug() << "\ttype: " << type;
		qDebug() << "\tfunctionalName " << functionalName.c_str();
		qDebug() << "\tdescription " << description.c_str();
		FLCRepositoryModule *m = new FLCRepositoryModule(type, moduleTypeName, functionalName, description);
		addToRepository(m);
	}
	return true;
}

std::map<flaarlib::MODULE_TYPE, std::vector<FLCRepositoryModule *> *> FLCRepositoryModuleHandler::getRepository()
{
	return m_flcModulesMap;
}

void FLCRepositoryModuleHandler::addToRepository(FLCRepositoryModule *flcModule)
{
	std::vector<FLCRepositoryModule *> *moduleVector = m_flcModulesMap[flcModule->moduleType()];
	if( !moduleVector )
		moduleVector = new std::vector<FLCRepositoryModule *>();
	moduleVector->push_back(flcModule);
	m_flcModulesMap[flcModule->moduleType()] = moduleVector;
}

void FLCRepositoryModuleHandler::requestRepository()
{
	OscSender *sender = Flaacontrol::instance()->pUdpSender();
	Message msg("/ws/repository/get");
	sender->enqueuMessage(msg);
}

