#include "FLCRepositoryModuleModel.h"
#include "FLModuleDefs.h"

#include <QApplication>
#include <QWidget>
#include <QIcon>
#include <QMimeData>
#include <QCursor>
#include <QDebug>

FLCRepositoryModuleModel::FLCRepositoryModuleModel(QHash<QString, FLCRepositoryModule *> *dataVector, QObject *parent)
	: QAbstractListModel(parent),
	  m_pDataVector(dataVector)
{
}

FLCRepositoryModuleModel::FLCRepositoryModuleModel(QObject *parent)
	: QAbstractListModel(parent)
{
	m_pDataVector = new QHash<QString, FLCRepositoryModule *>();
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
	FLCRepositoryModule *m = moduleAt(index.row());
	switch( role )
	{
		case Qt::DisplayRole:
			return m->functionalName().c_str();
		case Qt::DecorationRole:
			return icon(m);
		case Qt::ToolTipRole:
			return m->description().c_str();
	}
	return QVariant();
}

QVariant FLCRepositoryModuleModel::icon(flaarlib::DATA_TYPE dataType) const
{
	switch( dataType )
	{
		case flaarlib::OSC:
			return QIcon(":/icons/etherjack");
		case flaarlib::MIDI:
			return QIcon(":/icons/midijack");
		case flaarlib::AUDIO:
			return QIcon(":/icons/audiocombojack");
		default:
			return QIcon(":/icons/usbjack");

	}
}

QVariant FLCRepositoryModuleModel::icon(const FLCRepositoryModule *m) const
{
	return icon(m->dataType());
}

QStringList FLCRepositoryModuleModel::mimeTypes() const
{
	QStringList types;
	types << "application/x-flowcontrol";
	return types;
}

QMimeData *FLCRepositoryModuleModel::mimeData(const QModelIndexList &indexes) const
{
	QMimeData *mimeData = new QMimeData();
	QByteArray encodedData;
	QDataStream stream(&encodedData, QIODevice::WriteOnly);

	foreach (QModelIndex index, indexes)
	{
		if (index.isValid())
		{
			FLCRepositoryModule *m = moduleAt(index.row());
			QPoint hotspot(0,0);
			QString text = m->functionalName().c_str();
			QIcon icon = data(index, Qt::DecorationRole).value<QIcon>();
			int moduleType = m->moduleType();
			stream << text << icon << hotspot << moduleType << index.row();
		}
	}
	mimeData->setData("application/x-flowcontrol-add", encodedData);
	return mimeData;
}

Qt::ItemFlags FLCRepositoryModuleModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

	if (index.isValid())
		return Qt::ItemIsDragEnabled | defaultFlags;
	else
		return defaultFlags;

}

QModelIndex FLCRepositoryModuleModel::addModule(FLCRepositoryModule *module)
{
	int rowIndex = rowCount();
	if( m_pDataVector->find(module->functionalName().c_str()) == m_pDataVector->end() )
	{
		beginInsertRows(QModelIndex(), rowIndex, rowIndex);
		m_pDataVector->insert(module->functionalName().c_str(), module);
		endInsertRows();
		return index(rowIndex, 0);
	}
	return index(-1, -1);
}

FLCRepositoryModule *FLCRepositoryModuleModel::moduleAt(int index) const
{
	FLCRepositoryModule *m = (m_pDataVector->begin()+index).value();
	return m;
}
