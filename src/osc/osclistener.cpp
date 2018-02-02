#include "osclistener.h"
#include "oscpkt.hh"
#include "udp.hh"

#include <QDebug>
#include <QString>


OscListener::OscListener( int iPortNum) :
	QThread( ),
	OscHandler ("/"),
	m_iPortNum( iPortNum)
{
}

OscListener::~OscListener()
{
	delete m_pUdpSocket;
	m_pUdpSocket = 0;
}

void OscListener::run()
{
	m_bRunning = true;
	init();
	runListener();
}

void OscListener::init()
{
	m_pUdpSocket = new oscpkt::UdpSocket();
}

void OscListener::runListener()
{

	using std::cout;
	using std::cerr;

	m_pUdpSocket->bindTo(m_iPortNum);
	if (!m_pUdpSocket->isOk())
		cerr << "Error opening port " << m_iPortNum << ": " << m_pUdpSocket->errorMessage() << "\n";
	else
	{
		cout << "Listener started, will listen to packets on port " << m_iPortNum << std::endl;
		PacketReader pr;
		while (m_pUdpSocket->isOk())
		{
			if (m_pUdpSocket->receiveNextPacket(30))
			{
				pr.init(m_pUdpSocket->packetData(), m_pUdpSocket->packetSize());
				oscpkt::Message *message;
				while (pr.isOk() && (message = pr.popMessage()) != 0)
				{
					OscHandler *handler = handlerFor(message);
					if( handler)
						handler->handle(m_pUdpSocket, message);
					else
						qDebug() << "Listener: unhandled message: " << message->addressPattern().c_str();
				}
			}
			if( !m_bRunning )
				break;
		}
		qDebug() << "close socket";
		m_pUdpSocket->close();
	}
}

void OscListener::setBRunning(bool bRunning)
{
	m_bRunning = bRunning;
}
