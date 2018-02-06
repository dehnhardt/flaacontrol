#include "FLCModuleInstancesModel.h"
#include "FLCModuleInstance.h"

FLCModuleInstancesModel::FLCModuleInstancesModel(QObject *parent)
	: QObject(parent)
{

}

void FLCModuleInstancesModel::addFLCModuleInstance(FLCModuleInstance *moduleInstance)
{
	m_moduleInstancesMap[moduleInstance->m_uuid] = moduleInstance;
}

FLCModuleInstance *FLCModuleInstancesModel::getFlcModuleInstance(QUuid uuid)
{
	return m_moduleInstancesMap[uuid];
}
