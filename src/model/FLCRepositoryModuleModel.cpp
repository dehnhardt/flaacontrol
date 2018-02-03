#include "FLCRepositoryModuleModel.h"
#include "FLModuleDefs.h"

#include <QIcon>

FLCRepositoryModuleModel::FLCRepositoryModuleModel(vector<FLCRepositoryModule *> *dataVector, QObject *parent)
	: QAbstractListModel(parent),
	  m_pDataVector(dataVector)
{
}

FLCRepositoryModuleModel::FLCRepositoryModuleModel(QObject *parent)
	: QAbstractListModel(parent)
{
	m_pDataVector = new vector<FLCRepositoryModule *>();
}

FLCRepositoryModuleModel::~FLCRepositoryModuleModel()
{
	if( m_pDataVector)
	{
		for( auto it : *m_pDataVector )
			delete it;
		m_pDataVector->clear();
		delete m_pDataVector;
	}
}

int FLCRepositoryModuleModel::rowCount(const QModelIndex &parent) const
{
	// For list models only the root node (an invalid parent) should return the list's size. For all
	// other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
	if (parent.isValid())
		return 0;
	if( ! m_pDataVector )
		return 0;
	return static_cast<int>(m_pDataVector->size());
}

QVariant FLCRepositoryModuleModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();
	FLCRepositoryModule *m = m_pDataVector->at(static_cast<unsigned long>(index.row()));
	switch( role )
	{
		case Qt::DisplayRole:
			return m->functionalName().c_str();
		case Qt::DecorationRole:
			return icon(m);
	}
	return QVariant();
}

QVariant FLCRepositoryModuleModel::icon(const FLCRepositoryModule *m) const
{
	switch( m->moduleType() )
	{
		case flaarlib::INPUT:
			return QIcon(":/icons/etherjack");
		default:
			return QVariant();

	}
}

QModelIndex FLCRepositoryModuleModel::addModule(FLCRepositoryModule *module)
{
	int rowIndex = rowCount();
	beginInsertRows(QModelIndex(), rowIndex, rowIndex);
	m_pDataVector->push_back(module);
	endInsertRows();
	return index(rowIndex, 0);
}
