#ifndef FLCREPOSITORYMODULEMODEL_H
#define FLCREPOSITORYMODULEMODEL_H

#include "FLCRepositoryModule.h"

#include <vector>
#include <QHash>
#include <QAbstractListModel>

using std::vector;

class FLCRepositoryModuleModel : public QAbstractListModel
{
	Q_OBJECT

public:
	explicit FLCRepositoryModuleModel( QHash<QString, FLCRepositoryModule *> *dataVector, QObject *parent = nullptr);
	explicit FLCRepositoryModuleModel( QObject *parent = nullptr);

	~FLCRepositoryModuleModel();


	// Basic functionality:
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	QVariant icon(flaarlib::DATA_TYPE dataType) const;
	QVariant icon(const FLCRepositoryModule *m) const;

	// Drag functionality
	Qt::ItemFlags flags(const QModelIndex &index) const override;
	QStringList mimeTypes() const override;
	QMimeData *mimeData(const QModelIndexList &indexes) const override;

	QModelIndex addModule(FLCRepositoryModule *module);

	FLCRepositoryModule *moduleAt(int index) const;

private:
	QHash<QString, FLCRepositoryModule *> *m_pDataVector;


};
#endif // FLCREPOSITORYMODULEMODEL_H
