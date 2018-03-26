#ifndef FLCMODULECONNECTION_H
#define FLCMODULECONNECTION_H

#include <QObject>
#include <QUuid>

class FLCModuleWidget;

class FLCModuleConnection
{
public:
	FLCModuleConnection();

	QUuid outputUuid = "";
	QUuid inputUuid = "";
	FLCModuleWidget *outputWidget = 0;
	FLCModuleWidget *inputWidget = 0;

	int outputPortNumber;
	int inputPortNumber;
};

#endif // FLCMODULECONNECTION_H
