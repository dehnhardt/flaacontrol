#ifndef FLAACONTROL_H
#define FLAACONTROL_H

#include <string>
#include <QObject>

class OscListener;
class OscSender;

class Flaacontrol : public QObject
{
	Q_OBJECT
public:
	// singleton pattern
	static Flaacontrol *instance()
	{
		static CGuard g;   // Speicherbereinigung
		if (!_instance)
			_instance = new Flaacontrol();
		return (_instance);
	}

public: //getter
	OscListener *udpListener() const;
	OscSender *udpSender() const;

	int listenPort() const {return m_iListenPort;}
	int sendPort() const {return m_iSendPort;}
	std::string sendAddress() const {return m_sSendAddress;}

public: //setter
	void setListenPort(int iListenPort) {m_iListenPort = iListenPort;}
	void setSendPort(int iSendPort) {m_iSendPort = iSendPort;}
	void setSendAddress(const std::string &sSendAddress) { m_sSendAddress = sSendAddress;}

public slots:
	void listenerThreadStarted();
	void listenerThreadFinished();

private: // methods
	Flaacontrol();
	virtual ~Flaacontrol();

	// delete copy constructor
	Flaacontrol(const Flaacontrol &) = delete;
	void operator= (Flaacontrol const) = delete;

	void openSockets();
	void registerHandler();
	void connectSlots();

private: // members
	static Flaacontrol *_instance;
	OscListener *m_pUdpListener = 0;
	OscSender *m_pUdpSender = 0;

	int m_iListenPort = 0;
	int m_iSendPort = 0;
	std::string m_sSendAddress;


	class CGuard
	{
	public:
		~CGuard()
		{
			if ( nullptr != Flaacontrol::_instance)
			{
				delete Flaacontrol::_instance;
				Flaacontrol::_instance = nullptr;
			}
		}
	};

};

#endif // FLAACONTROL_H
