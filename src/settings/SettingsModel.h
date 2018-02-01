#ifndef SETTINGSMODEL_H
#define SETTINGSMODEL_H

#include <QString>

class QSettings;

class SettingsModel
{
public: // methods
	// singleton pattern
	static SettingsModel *instance()
	{
		static CGuard g;   // Speicherbereinigung
		if (!_instance)
			_instance = new SettingsModel();
		return (_instance);
	}

public: // object members

	QSettings *getSettings() const;
	struct SessionSettings
	{
	public:
		int listenPort = 0;
		int sendPort = 0;
		QString sendAddress;
	};

public: //modell methods
	SessionSettings *readSessionSettings();
	void writeSeesionSettings( SessionSettings *sessionSettings);


private:
	SettingsModel();
	SettingsModel( SettingsModel *model) = delete;
	void operator= (SettingsModel const) = delete;
	~SettingsModel();

private: // members
	QSettings *m_pSettings = 0;

	static SettingsModel *_instance;

	class CGuard
	{
	public:
		~CGuard()
		{
			if ( nullptr != SettingsModel::_instance)
			{
				delete SettingsModel::_instance;
				SettingsModel::_instance = nullptr;
			}
		}
	};

};

#endif // SETTINGSMODEL_H
