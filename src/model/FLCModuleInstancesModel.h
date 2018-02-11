#ifndef FLCMODULEINSTANCESMODEL_H
#define FLCMODULEINSTANCESMODEL_H

#include <QObject>
#include <QMap>
#include <QUuid>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

class FLCModuleInstance;

class FLCModuleInstancesModel : public QObject
{
	Q_OBJECT
public:
	explicit FLCModuleInstancesModel(QObject *parent = nullptr);
	void serialize(QXmlStreamWriter *xmlWriter);
	void deserialize(QXmlStreamReader *xmlReader);

public: //setter
	void addFLCModuleInstance( FLCModuleInstance *moduleInstance );

public: //getter
	FLCModuleInstance *getFlcModuleInstance( QUuid uuid);
	QMap<QUuid, FLCModuleInstance *> getModuleInstancesMap();

signals:
	void moduleAdded( FLCModuleInstance *moduleInstance);
	void moduleRemoved( const QUuid &uuid);

public slots:

private:
	QMap<QUuid, FLCModuleInstance *> m_moduleInstancesMap;
};

#endif // FLCMODULEINSTANCESMODEL_H
