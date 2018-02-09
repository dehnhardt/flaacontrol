#ifndef FLCMODULEINSTANCESMODEL_H
#define FLCMODULEINSTANCESMODEL_H

#include <QObject>
#include <QMap>
#include <QUuid>
#include <QXmlStreamWriter>

class FLCModuleInstance;

class FLCModuleInstancesModel : public QObject
{
	Q_OBJECT
public:
	explicit FLCModuleInstancesModel(QObject *parent = nullptr);
	void serialize(QXmlStreamWriter *xmlWriter);

public: //setter
	void addFLCModuleInstance( FLCModuleInstance *moduleInstance );

public: //getter
	FLCModuleInstance *getFlcModuleInstance( QUuid uuid);

signals:
	void moduleAdded( QUuid &uuid);
	void moduleRemoved( QUuid &uuid);

public slots:

private:
	QMap<QUuid, FLCModuleInstance *> m_moduleInstancesMap;
};

#endif // FLCMODULEINSTANCESMODEL_H
