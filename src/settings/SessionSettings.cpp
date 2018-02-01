#include "SessionSettings.h"
#include "ui_SessionSettings.h"
#include "SettingsModel.h"

SessionSettings::SessionSettings(QWidget *parent) :
	QDialog(parent),
	m_pUi(new Ui::SessionSettings)
{
	m_pSettingsModel = SettingsModel::instance();
	createUi();
	loadSessionData();
}

SessionSettings::~SessionSettings()
{
	delete m_pUi;
}

void SessionSettings::accept()
{
	saveSessionData();
	close();
}

void SessionSettings::createUi()
{
	m_pUi->setupUi(this);

}

void SessionSettings::loadSessionData()
{
	SettingsModel::SessionSettings *s  = m_pSettingsModel->readSessionSettings();
	this->m_pUi->editListenPort->setText(QString().setNum(s->listenPort));
	this->m_pUi->editSendPort->setText(QString().setNum(s->sendPort));
	this->m_pUi->editSendAddress->setText(s->sendAddress);
}

void SessionSettings::saveSessionData()
{
	SettingsModel::SessionSettings *s  = new SettingsModel::SessionSettings();
	s->listenPort = m_pUi->editListenPort->text().toInt();
	s->sendPort = m_pUi->editSendPort->text().toInt();
	s->sendAddress = m_pUi->editSendAddress->text();
	m_pSettingsModel->writeSeesionSettings(s);
}
