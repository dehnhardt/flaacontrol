#include "FLCModuleInstancesModel.h"
#include "../flaaoscsdk/FLOModuleInstanceDAO.h"

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
		FLOModuleInstanceDAO *i = it;
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
					FLOModuleInstanceDAO *i = new FLOModuleInstanceDAO();
					i->deserialize(xmlReader);
					addFLCModuleInstance(i);
				}
				break;
			case QXmlStreamReader::TokenType::EndElement:
				if( s == "Modules")
					return;
				break;
			default:
				break;
		}
	}
}

void FLCModuleInstancesModel::addFLCModuleInstance(FLOModuleInstanceDAO *moduleInstance)
{
	m_moduleInstancesMap[moduleInstance->uuid()] = moduleInstance;
	emit(moduleInstanceAdded(moduleInstance));
}

void FLCModuleInstancesModel::modifyFLCModuleInstance(FLOModuleInstanceDAO *moduleInstance)
{
	m_moduleInstancesMap[moduleInstance->uuid()] = moduleInstance;
	emit(moduleInstanceModified(moduleInstance));
}

void FLCModuleInstancesModel::removeFLCModuleInstance(const QUuid &uuid)
{
	m_moduleInstancesMap.erase(m_moduleInstancesMap.find(uuid));
	emit(moduleInstanceRemoved(uuid));
}


FLOModuleInstanceDAO *FLCModuleInstancesModel::getFLOModuleInstance(QUuid uuid)
{
	return m_moduleInstancesMap[uuid];
}

QMap<QUuid, FLOModuleInstanceDAO *> FLCModuleInstancesModel::getModuleInstancesMap()
{
	return m_moduleInstancesMap;
}
