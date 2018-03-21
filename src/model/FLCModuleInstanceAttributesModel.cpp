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
	return 2 + m_pModuleInstanceDAO->getParameterCount();
}

int FLCModuleInstanceAttributesModel::columnCount(const QModelIndex &parent __attribute__((unused))) const
{
	return 2;
}

QVariant FLCModuleInstanceAttributesModel::data(const QModelIndex &index, int role) const
{
	if( !index.isValid())
		return QVariant();
	switch (role)
	{
		case Qt::EditRole:
			if(index.row() > 0 && index.column() == 1 )
				return displayData(index);
			return QVariant();
		case Qt::DisplayRole:
			return displayData(index);
		default:
			break;
	}
	return QVariant();
}

bool FLCModuleInstanceAttributesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if( !index.isValid() || role != Qt::EditRole || index.column() != 1 )
		return false;
	if( index.row() == 1 )
		this->m_pModuleInstanceDAO->setModuleName(value.toString());
	else if( index.row() > 1 )
	{
		FLOParameter *p = m_pModuleInstanceDAO->getParameterAt(index.row()-2);
		switch (p->parameterType())
		{
			case FLOParameter::BOOLEAN:
				p->setValue(value.toBool());
				break;
			case FLOParameter::STRING:
				p->setValue(value.toString());
				break;
			case FLOParameter::INTEGER:
				p->setValue(value.toInt());
				break;
			case FLOParameter::LONG:
				p->setValue(value.toLongLong());
				break;
			case FLOParameter::BYTES:
				p->setValue(value.toByteArray());
				break;
		}
		emit( modifyModuleInstance(this->m_pModuleInstanceDAO));
		emit( dataChanged(index, index));
		return true;
	}
	return false;
}

Qt::ItemFlags FLCModuleInstanceAttributesModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::ItemIsEnabled;
	if( index.column() == 1)
	{
		if( index.row() == 1 )
			return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
		else if(index.row() > 1 )
		{
			FLOParameter *p = m_pModuleInstanceDAO->getParameterAt(index.row()-2);
			if( p->editable() )
				return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
		}
	}
	return QAbstractTableModel::flags(index);
}

QVariant FLCModuleInstanceAttributesModel::displayData(const QModelIndex &index) const
{
	if( index.row() < 2 )
	{
		switch (index.row())
		{
			case 0:
				switch (index.column())
				{
					case 0:
						return QString("Type");
					case 1:
						return m_pModuleInstanceDAO->moduleFunctionalName();
					default:
						break;
				}
				break;
			case 1:
				switch (index.column())
				{
					case 0:
						return QString("Name");
					case 1:
						return m_pModuleInstanceDAO->moduleName();
					default:
						break;
				}
				break;
			default:
				break;
		}
	}
	else
		return getParameterData(m_pModuleInstanceDAO->getParameterAt(index.row()-2), index);
	return QVariant();
}

QVariant FLCModuleInstanceAttributesModel::getParameterData(const FLOParameter *parameter, const QModelIndex &index) const
{
	switch(index.column())
	{
		case 0:
			return parameter->parameterName();
		case 1:
			return parameter->value();
	}
	return QVariant();
}
