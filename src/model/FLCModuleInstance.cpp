#include "FLCModuleInstance.h"
#include "FLCRepositoryModule.h"

FLCModuleInstance::FLCModuleInstance(QObject *parent)
	: QObject(parent),
	  m_uuid(QUuid::createUuid())
{

}

FLCModuleInstance::FLCModuleInstance(FLCRepositoryModule *module) :
	FLCModuleInstance()
{
	setModuleType(module->moduleType());
	setDataType(module->dataType());
	setModuleFunctionalName(module->functionalName().c_str());
	setModuleTypeName(module->moduleTypeName().c_str());
}

void FLCModuleInstance::serialize( QXmlStreamWriter &xmlWriter )
{
	xmlWriter.writeStartElement(moduleFunctionalName());
	xmlWriter.writeAttribute("uuid", uuid().toString());
	xmlWriter.writeAttribute("name", moduleName());
	xmlWriter.writeStartElement("moduleType");
	xmlWriter.writeAttribute("moduleTypeId", QString(static_cast<int>(moduleType())));
	xmlWriter.writeCharacters(moduleTypeName());
	xmlWriter.writeEndElement();
	xmlWriter.writeStartElement("moduleDataType");
	xmlWriter.writeAttribute("moduleDataTypeId", QString(static_cast<int>(dataType())));
	xmlWriter.writeEndElement();
	xmlWriter.writeStartElement("displayPosition");
	xmlWriter.writeAttribute("x", QString(position().x()));
	xmlWriter.writeAttribute("y", QString(position().y()));
	xmlWriter.writeEndElement();
	xmlWriter.writeEndElement();
}

/*
 *  getter
 */

QUuid FLCModuleInstance::uuid() const
{
	return m_uuid;
}

QString FLCModuleInstance::moduleFunctionalName() const
{
	return m_sModuleFunctionalName;
}

QString FLCModuleInstance::moduleTypeName() const
{
	return m_sModuleTypeName;
}

QString FLCModuleInstance::moduleName() const
{
	return m_sModuleName;
}

QString FLCModuleInstance::description() const
{
	return m_sDescription;
}

flaarlib::MODULE_TYPE FLCModuleInstance::moduleType() const
{
	return m_moduleType;
}

QPoint FLCModuleInstance::position()
{
	return m_position;
}

flaarlib::DATA_TYPE FLCModuleInstance::dataType() const
{
	return m_dataType;
}

/*
 * setter
 */

void FLCModuleInstance::setModuleTypeName(const QString &sModuleTypeName)
{
	m_sModuleTypeName = sModuleTypeName;
}

void FLCModuleInstance::setModuleName(const QString &sModuleName)
{
	m_sModuleName = sModuleName;
}

void FLCModuleInstance::setDescription(const QString &sDescription)
{
	m_sDescription = sDescription;
}

void FLCModuleInstance::setModuleType(const flaarlib::MODULE_TYPE &moduleType)
{
	m_moduleType = moduleType;
}

void FLCModuleInstance::setUuid(const QUuid &uuid)
{
	m_uuid = uuid;
}

void FLCModuleInstance::setDataType(const flaarlib::DATA_TYPE &dataType)
{
	m_dataType = dataType;
}

void FLCModuleInstance::setPosition(const QPoint &position)
{
	this->m_position = position;
}

void FLCModuleInstance::setPosition(const int x, const int y)
{
	this->m_position.setX(x);
	this->m_position.setY(y);
}

void FLCModuleInstance::setModuleFunctionalName(const QString &sModuleFunctionalName)
{
	m_sModuleFunctionalName = sModuleFunctionalName;
}
