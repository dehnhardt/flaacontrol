#ifndef FLCREPOSITORYMODULEMODEL_H
#define FLCREPOSITORYMODULEMODEL_H

#include "FLCRepositoryModule.h"

#include <vector>
#include <QAbstractListModel>

using std::vector;

class FLCRepositoryModuleModel : public QAbstractListModel
{
	Q_OBJECT

public:
	explicit FLCRepositoryModuleModel( vector<FLCRepositoryModule *> *dataVector, QObject *parent = nullptr);
	explicit FLCRepositoryModuleModel( QObject *parent = nullptr);

	~FLCRepositoryModuleModel();


	// Basic functionality:
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

	// Drag functionality
	Qt::ItemFlags flags(const QModelIndex &index) const override;
	QStringList mimeTypes() const override;
	QMimeData *mimeData(const QModelIndexList &indexes) const override;

	QModelIndex addModule(FLCRepositoryModule *module);

	FLCRepositoryModule *moduleAt( unsigned int index)
	{
		return m_pDataVector->at(static_cast<unsigned long>(index));
	}

private:
	vector<FLCRepositoryModule *> *m_pDataVector;
	QVariant icon(const FLCRepositoryModule *m) const;


};
#endif // FLCREPOSITORYMODULEMODEL_H
