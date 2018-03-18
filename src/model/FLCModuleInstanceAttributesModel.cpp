#include "FLCModuleInstanceAttributesModel.h"

FLCModuleInstanceAttributesModel::FLCModuleInstanceAttributesModel(FLOModuleInstanceDAO *moduleInstanceDAO, QObject *parent) :
	QAbstractTableModel(parent),
	m_pModuleInstanceDAO(moduleInstanceDAO)
{

}

FLCModuleInstanceAttributesModel::~FLCModuleInstanceAttributesModel()
{

}

int FLCModuleInstanceAttributesModel::rowCount(const QModelIndex &parent __attribute__((unused))) const
{
	return 2;
}

int FLCModuleInstanceAttributesModel::columnCount(const QModelIndex &parent __attribute__((unused))) const
{
	return 2;
}

QVariant FLCModuleInstanceAttributesModel::data(const QModelIndex &index, int role) const
{
	switch (role)
	{
		case Qt::EditRole:
			if(index.row() > 0 && index.column() == 1 )
				return displayData(index);
			return QVariant();
			break;
		case Qt::DisplayRole:
			return displayData(index);
			break;
		default:
			break;
	}
	return QVariant();
}

bool FLCModuleInstanceAttributesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if( !index.isValid())
		return false;
	return true;
}

Qt::ItemFlags FLCModuleInstanceAttributesModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::ItemIsEnabled;
	if(index.row() > 0 && index.column() == 1)
		return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
	else
		return QAbstractTableModel::flags(index);

}

QVariant FLCModuleInstanceAttributesModel::displayData(const QModelIndex &index) const
{
	switch (index.row())
	{
		case 0:
			switch (index.column())
			{
				case 0:
					return QString("Type");
					break;
				case 1:
					return m_pModuleInstanceDAO->moduleFunctionalName();
					break;
				default:
					break;
			}
			break;
		case 1:
			switch (index.column())
			{
				case 0:
					return QString("Name");
					break;
				case 1:
					return m_pModuleInstanceDAO->moduleName();
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}
