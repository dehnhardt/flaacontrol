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
	explicit FLCRepositoryModuleModel( QObject *parent = nullptr);


	// Basic functionality:
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

	QModelIndex addModule( FLCRepositoryModule *module);

private:
	vector<FLCRepositoryModule *> *m_pDataVector;
};

#endif // FLCREPOSITORYMODULEMODEL_H
