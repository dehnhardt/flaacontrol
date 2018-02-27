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
					addFLOModuleInstance(i);
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

void FLCModuleInstancesModel::addFLOModuleInstance(FLOModuleInstanceDAO *moduleInstance)
{
	m_moduleInstancesMap[moduleInstance->uuid()] = moduleInstance;
	emit(moduleInstanceAdded(moduleInstance));
}

FLOModuleInstanceDAO *FLCModuleInstancesModel::getFLOModuleInstance(QUuid uuid)
{
	return m_moduleInstancesMap[uuid];
}

QMap<QUuid, FLOModuleInstanceDAO *> FLCModuleInstancesModel::getModuleInstancesMap()
{
	return m_moduleInstancesMap;
}
