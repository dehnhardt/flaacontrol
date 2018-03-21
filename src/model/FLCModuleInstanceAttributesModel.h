#ifndef FLCMODULEINSTANCEATTRIBUTESMODEL_H
#define FLCMODULEINSTANCEATTRIBUTESMODEL_H

#include "../flaaoscsdk/FLOModuleInstanceDAO.h"
#include "../flaaoscsdk/FLOParameter.h"

#include <QObject>
#include <QAbstractTableModel>

class FLCModuleInstanceAttributesModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	FLCModuleInstanceAttributesModel(FLOModuleInstanceDAO *moduleInstanceDAO, QObject *parent);
	virtual ~FLCModuleInstanceAttributesModel();

public:
	int rowCount(const QModelIndex &parent) const;
	int columnCount(const QModelIndex &parent) const;
	QVariant data(const QModelIndex &index, int role) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role);
	Qt::ItemFlags flags(const QModelIndex &index) const;

	QVariant displayData(const QModelIndex &index) const;
	QVariant getParameterData(const FLOParameter *parameter, const QModelIndex &index) const;

signals:
	bool modifyModuleInstance(FLOModuleInstanceDAO *module);

private:
	FLOModuleInstanceDAO *m_pModuleInstanceDAO = 0;


};

#endif // FLCMODULEINSTANCEATTRIBUTESMODEL_H
