#include "FLCModuleInstance.h"
#include "FLCRepositoryModule.h"

#include <QDebug>

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

void FLCModuleInstance::serialize( QXmlStreamWriter *xmlWriter )
{
	xmlWriter->writeStartElement("Module");
	xmlWriter->writeAttribute("functionalName", moduleFunctionalName());
	xmlWriter->writeAttribute("uuid", uuid().toString());
	xmlWriter->writeAttribute("name", moduleName());
	xmlWriter->writeStartElement("ModuleType");
	xmlWriter->writeAttribute("moduleTypeId", QString::number(static_cast<int>(moduleType())));
	xmlWriter->writeCharacters(moduleTypeName());
	xmlWriter->writeEndElement();
	xmlWriter->writeStartElement("ModuleDataType");
	xmlWriter->writeAttribute("moduleDataTypeId", QString::number(static_cast<int>(dataType())));
	xmlWriter->writeEndElement();
	xmlWriter->writeStartElement("DisplayPosition");
	xmlWriter->writeAttribute("x", QString::number(position().x()));
	xmlWriter->writeAttribute("y", QString::number(position().y()));
	xmlWriter->writeEndElement();
	xmlWriter->writeEndElement();
}

void FLCModuleInstance::deserialize(QXmlStreamReader *xmlReader)
{
	QXmlStreamReader::TokenType t = xmlReader->tokenType();
	QStringRef s = xmlReader->name();
	QString text;
	while(!xmlReader->atEnd())
	{
		switch( t )
		{
			case QXmlStreamReader::TokenType::StartElement:
				s = xmlReader->name();
				text.clear();
				qDebug() << "Model: Element Name: " << s;
				if( s == "Module")
				{
					QXmlStreamAttributes attributes = xmlReader->attributes();
					for( auto attribute : attributes )
					{
						QStringRef name = attribute.name();
						if( name == "functionalName")
							setModuleFunctionalName(attribute.value().toString());
						if( name == "uuid")
							setUuid(attribute.value().toString());
						if( name == "name")
							setModuleName(attribute.value().toString());
						qDebug() << "\tAttribute Name: " << name << ", value: " << attribute.value();
					}
				}
				if( s == "ModuleType" )
				{
					QXmlStreamAttributes attributes = xmlReader->attributes();
					for( auto attribute : attributes )
					{
						QStringRef name = attribute.name();
						qDebug() << "\tAttribute Name: " << name;
						if(name == "moduleTypeId")
							setModuleType(flaarlib::MODULE_TYPE(attribute.value().toInt()));
					}

				}
				if( s == "ModuleDataType" )
				{
					QXmlStreamAttributes attributes = xmlReader->attributes();
					for( auto attribute : attributes )
					{
						QStringRef name = attribute.name();
						qDebug() << "\tAttribute Name: " << name;
						if( name == "moduleDateTypeId")
							setDataType(flaarlib::DATA_TYPE(attribute.value().toInt()));
					}

				}
				if( s == "DisplayPosition" )
				{
					QXmlStreamAttributes attributes = xmlReader->attributes();
					QPoint p;
					for( auto attribute : attributes )
					{
						QStringRef name = attribute.name();
						qDebug() << "\tAttribute Name: " << name;
						if( name == "x")
							p.setX(attribute.value().toInt());
						if( name == "y")
							p.setY(attribute.value().toInt());
					}
					setPosition(p);
				}
				break;
			case QXmlStreamReader::TokenType::Characters:
				{
					text += xmlReader->text().toString();
					if( s == "ModuleType")
						qDebug() << text;
					break;
				}
			case QXmlStreamReader::TokenType::EndElement:
				s = xmlReader->name();
				if( s == "ModuleType")
					setModuleTypeName( text.trimmed());
				if( s == "Module")
					return;
			default:
				break;
		}
		t = xmlReader->readNext();
	}
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
