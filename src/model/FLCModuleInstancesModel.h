#ifndef FLCMODULEINSTANCESMODEL_H
#define FLCMODULEINSTANCESMODEL_H

#include <QObject>
#include <QMap>
#include <QUuid>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

class FLOModuleInstanceDAO;

class FLCModuleInstancesModel : public QObject
{
	Q_OBJECT
public:
	explicit FLCModuleInstancesModel(QObject *parent = nullptr);
	void serialize(QXmlStreamWriter *xmlWriter);
	void deserialize(QXmlStreamReader *xmlReader);

public: //setter
	void addFLOModuleInstance( FLOModuleInstanceDAO *moduleInstance );

public: //getter
	FLOModuleInstanceDAO *getFLOModuleInstance( QUuid uuid);
	QMap<QUuid, FLOModuleInstanceDAO *> getModuleInstancesMap();

signals:
	void moduleAdded( FLOModuleInstanceDAO *moduleInstance);
	void moduleRemoved( const QUuid &uuid);

public slots:

private:
	QMap<QUuid, FLOModuleInstanceDAO *> m_moduleInstancesMap;
};

#endif // FLCMODULEINSTANCESMODEL_H
