#include "FLCModuleInstancesModel.h"
#include "FLCModuleInstance.h"

#include <QDebug>

FLCModuleInstancesModel::FLCModuleInstancesModel(QObject *parent)
	: QObject(parent)
{

}

void FLCModuleInstancesModel::serialize(QXmlStreamWriter *xmlWriter)
{
	xmlWriter->writeStartElement("Modules");
	xmlWriter->writeAttribute("count", QString::number(m_moduleInstancesMap.size() ));
	for( auto it : m_moduleInstancesMap )
	{
		FLCModuleInstance *i = it;
		i->serialize(xmlWriter);
	}
	xmlWriter->writeEndElement();
}

void FLCModuleInstancesModel::deserialize(QXmlStreamReader *xmlReader)
{
	while(!xmlReader->atEnd())
	{
		QXmlStreamReader::TokenType t = xmlReader->readNext();
		QStringRef s = xmlReader->name();
		switch( t )
		{
			case QXmlStreamReader::TokenType::StartElement:
				qDebug() << "Model: Element Name: " << s;
				if( s  == "Module")
				{
					FLCModuleInstance *i = new FLCModuleInstance();
					i->deserialize(xmlReader);
					addFLCModuleInstance(i);
				}
				break;
			case QXmlStreamReader::TokenType::EndElement:
				if( s == "Modules")
					return;
			default:
				break;
		}
	}
}

void FLCModuleInstancesModel::addFLCModuleInstance(FLCModuleInstance *moduleInstance)
{
	m_moduleInstancesMap[moduleInstance->uuid()] = moduleInstance;
	emit(moduleAdded(moduleInstance));
}

FLCModuleInstance *FLCModuleInstancesModel::getFlcModuleInstance(QUuid uuid)
{
	return m_moduleInstancesMap[uuid];
}
