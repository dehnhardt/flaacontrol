#ifndef FLAACONTROL_H
#define FLAACONTROL_H


class OscListener;
class OscSender;

class Flaacontrol
{

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
	OscListener *pUdpListener() const;
	OscSender *pUdpSender() const;

private: // methods
	Flaacontrol();
	virtual ~Flaacontrol();

	// delete copy constructor
	Flaacontrol(const Flaacontrol &) = delete;
	void operator= (Flaacontrol const) = delete;

	void openSockets();
	void registerHandler();

private: // members
	static Flaacontrol *_instance;
	OscListener *m_pUdpListener = 0;
	OscSender *m_pUdpSender = 0;


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
