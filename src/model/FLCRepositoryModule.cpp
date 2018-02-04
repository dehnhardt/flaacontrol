#include "FLCRepositoryModule.h"

FLCRepositoryModule::FLCRepositoryModule(int moduleType, int dataType, std::string sModuleTypeName, std::string sFunctionalName, std::string sDescription) :
	m_moduleType(flaarlib::MODULE_TYPE(moduleType)),
	m_dataType(flaarlib::DATA_TYPE(dataType)),
	m_sModuleTypeName(sModuleTypeName),
	m_sFunctionalName(sFunctionalName),
	m_sDescription( sDescription)
{

}

flaarlib::MODULE_TYPE FLCRepositoryModule::moduleType() const
{
	return m_moduleType;
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

flaarlib::DATA_TYPE FLCRepositoryModule::dataType() const
{
	return m_dataType;
}
