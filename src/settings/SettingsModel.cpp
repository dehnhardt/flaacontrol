#include "SettingsModel.h"

#include <QSettings>

SettingsModel::SettingsModel()
{
	m_pSettings = new QSettings();
}

SettingsModel::~SettingsModel()
{
	delete m_pSettings;
}

QSettings *SettingsModel::getSettings() const
{
	return m_pSettings;
}

SettingsModel::SessionSettings *SettingsModel::readSessionSettings()
{
	SessionSettings *sessionSettings = new SessionSettings();
	m_pSettings->beginGroup("SessionSettings");
	sessionSettings->listenPort = m_pSettings->value("listenPort").toInt();
	sessionSettings->sendPort = m_pSettings->value("sendPort").toInt();
	sessionSettings->sendAddress = m_pSettings->value("sendAddress").toString();
	m_pSettings->endGroup();
	return sessionSettings;
}

void SettingsModel::writeSeesionSettings(SettingsModel::SessionSettings *sessionSettings)
{
	m_pSettings->beginGroup("SessionSettings");
	m_pSettings->setValue("listenPort", sessionSettings->listenPort);
	m_pSettings->setValue("sendPort", sessionSettings->sendPort );
	m_pSettings->setValue("sendAddress", sessionSettings->sendAddress );
	m_pSettings->endGroup();
}

SettingsModel *SettingsModel::_instance = 0;
