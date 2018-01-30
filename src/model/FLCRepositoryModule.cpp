#include "FLCRepositoryModule.h"

FLCRepositoryModule::FLCRepositoryModule(int moduleType, std::string sModuleTypeName, std::string sFunctionalName, std::string sDescription) :
	m_ModuleType(flaarlib::MODULE_TYPE(moduleType)),
	m_sModuleTypeName(sModuleTypeName),
	m_sFunctionalName(sFunctionalName),
	m_sDescription( sDescription)
{

}

flaarlib::MODULE_TYPE FLCRepositoryModule::moduleType() const
{
	return m_ModuleType;
}

std::string FLCRepositoryModule::moduleTypeName() const
{
	return m_sModuleTypeName;
}

std::string FLCRepositoryModule::functionalName() const
{
	return m_sFunctionalName;
}

std::string FLCRepositoryModule::description() const
{
	return m_sDescription;
}
