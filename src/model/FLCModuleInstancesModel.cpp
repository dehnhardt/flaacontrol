#include "FLCModuleInstancesModel.h"
#include "FLCModuleInstance.h"

FLCModuleInstancesModel::FLCModuleInstancesModel(QObject *parent)
	: QObject(parent)
{

}

void FLCModuleInstancesModel::serialize(QXmlStreamWriter *xmlWriter)
{
	xmlWriter->writeStartElement("modules");
	xmlWriter->writeAttribute("count", QString(m_moduleInstancesMap.size() ));
	for( auto it : m_moduleInstancesMap )
	{
		FLCModuleInstance *i = it;
		i->serialize(xmlWriter);
	}
	xmlWriter->writeEndElement();
}

void FLCModuleInstancesModel::addFLCModuleInstance(FLCModuleInstance *moduleInstance)
{
	m_moduleInstancesMap[moduleInstance->uuid()] = moduleInstance;
}

FLCModuleInstance *FLCModuleInstancesModel::getFlcModuleInstance(QUuid uuid)
{
	return m_moduleInstancesMap[uuid];
}
