#ifndef FLCMODULEINSTANCESMODEL_H
#define FLCMODULEINSTANCESMODEL_H

#include <QObject>
#include <QMap>
#include <QUuid>

class FLCModuleInstance;

class FLCModuleInstancesModel : public QObject
{
	Q_OBJECT
public:
	explicit FLCModuleInstancesModel(QObject *parent = nullptr);

public: //setter
	void addFLCModuleInstance( FLCModuleInstance *moduleInstance );

public: //getter
	FLCModuleInstance *getFlcModuleInstance( QUuid uuid);

signals:

public slots:

private:
	QMap<QUuid, FLCModuleInstance *> m_moduleInstancesMap;
};

#endif // FLCMODULEINSTANCESMODEL_H
