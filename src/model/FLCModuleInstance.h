#ifndef FLCMODULEINSTANCE_H
#define FLCMODULEINSTANCE_H

#include "FLModuleDefs.h"

#include <QObject>
#include <QUuid>


class FLCModuleInstance : public QObject
{
	Q_OBJECT
public:
	explicit FLCModuleInstance(QObject *parent = nullptr);

	QUuid uuid() const;
	void setUuid(const QUuid &uuid);

	std::string sModuleTypeName() const;
	void setSModuleTypeName(const std::string &sModuleTypeName);

	std::string sModuleName() const;
	void setSModuleName(const std::string &sModuleName);

	std::string sDescription() const;
	void setSDescription(const std::string &sDescription);

	flaarlib::MODULE_TYPE moduleType() const;
	void setModuleType(const flaarlib::MODULE_TYPE &moduleType);

	flaarlib::DATA_TYPE dataType() const;
	void setDataType(const flaarlib::DATA_TYPE &dataType);

signals:

public slots:

private:
	QUuid m_uuid;
	std::string m_sModuleTypeName;
	std::string m_sModuleName;
	std::string m_sDescription;
	flaarlib::MODULE_TYPE m_moduleType;
	flaarlib::DATA_TYPE m_dataType;

};

#endif // FLCMODULEINSTANCE_H
