#include "FLCModuleInstance.h"

FLCModuleInstance::FLCModuleInstance(QObject *parent) : QObject(parent)
{
}

QUuid FLCModuleInstance::uuid() const
{
	return m_uuid;
}

void FLCModuleInstance::setUuid(const QUuid &uuid)
{
	m_uuid = uuid;
}

std::string FLCModuleInstance::sModuleTypeName() const
{
	return m_sModuleTypeName;
}

void FLCModuleInstance::setSModuleTypeName(const std::string &sModuleTypeName)
{
	m_sModuleTypeName = sModuleTypeName;
}

std::string FLCModuleInstance::sModuleName() const
{
	return m_sModuleName;
}

void FLCModuleInstance::setSModuleName(const std::string &sModuleName)
{
	m_sModuleName = sModuleName;
}

std::string FLCModuleInstance::sDescription() const
{
	return m_sDescription;
}

void FLCModuleInstance::setSDescription(const std::string &sDescription)
{
	m_sDescription = sDescription;
}

flaarlib::MODULE_TYPE FLCModuleInstance::moduleType() const
{
	return m_moduleType;
}

void FLCModuleInstance::setModuleType(const flaarlib::MODULE_TYPE &moduleType)
{
	m_moduleType = moduleType;
}

flaarlib::DATA_TYPE FLCModuleInstance::dataType() const
{
	return m_dataType;
}

void FLCModuleInstance::setDataType(const flaarlib::DATA_TYPE &dataType)
{
	m_dataType = dataType;
}
