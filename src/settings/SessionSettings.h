#ifndef SESSIONSETTINGS_H
#define SESSIONSETTINGS_H


#include <QDialog>

class SettingsModel;

namespace Ui
{
class SessionSettings;
}

class SessionSettings : public QDialog
{
	Q_OBJECT

public: //methods
	explicit SessionSettings(QWidget *parent = 0);
	~SessionSettings();

public slots:
	void accept() override;

private: //methods
	void createUi();
	void loadSessionData();
	void saveSessionData();

private: // members
	Ui::SessionSettings *m_pUi;
	SettingsModel *m_pSettingsModel = 0;

};

#endif // SESSIONSETTINGS_H
