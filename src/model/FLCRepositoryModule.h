#ifndef FLCREPOSITORYMODULE_H
#define FLCREPOSITORYMODULE_H

#include "FLModuleDefs.h"

#include <QObject>

class FLCRepositoryModule
{
public:
	FLCRepositoryModule(int moduleType, int dataType, std::string sModuleTypeName, std::string sFunctionalName, std::string sDescription);

public: //getter
	flaarlib::MODULE_TYPE moduleType() const;
	flaarlib::DATA_TYPE dataType() const;
	std::string moduleTypeName() const;
	std::string functionalName() const;
	std::string description() const;

protected:
	flaarlib::MODULE_TYPE m_moduleType;
	flaarlib::DATA_TYPE m_dataType;
	std::string m_sModuleTypeName;
	std::string m_sFunctionalName;
	std::string m_sDescription;
};

#endif // FLCREPOSITORYMODULE_H
