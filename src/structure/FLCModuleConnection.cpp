#include "FLCModuleConnection.h"

FLCModuleConnection::FLCModuleConnection() :
	inputPortNumber(0),
	inputUuid(""),
	inputWidget(0),
	outputPortNumber(0),
	outputUuid(""),
	outputWidget(0)
{

}

FLCModuleConnection::FLCModuleConnection(FLCModuleConnection *fLCModuleConnection) :
	inputPortNumber(fLCModuleConnection->inputPortNumber),
	inputUuid(fLCModuleConnection->inputUuid),
	inputWidget(fLCModuleConnection->inputWidget),
	outputPortNumber(fLCModuleConnection->outputPortNumber),
	outputUuid(fLCModuleConnection->outputUuid),
	outputWidget(fLCModuleConnection->outputWidget)
{

}
