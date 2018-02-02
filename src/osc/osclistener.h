#ifndef OSCLISTENER_H
#define OSCLISTENER_H

#define OSCPKT_OSTREAM_OUTPUT

#include "oschandler.h"

#include <QThread>
#include <map>
#include <string>

namespace oscpkt
{
class UdpSocket;
}

class OscListener : public QThread, public OscHandler
{
	Q_OBJECT
public:
	OscListener( int iPortNum);
	~OscListener();

	void setBRunning(bool bRunning);

protected:
	void run() override;

private:
	void init();
	void runListener();

private:
	int m_iPortNum = 0;
	bool m_bRunning = false;
	oscpkt::UdpSocket *m_pUdpSocket =0;
};

#endif // OSCLISTENER_H
