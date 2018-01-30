#include "osclistener.h"
#include "oscpkt.hh"
#include "udp.hh"

#include <QDebug>
#include <QString>

using namespace oscpkt;

OscListener::OscListener( int iPortNum) :
	QThread( ),
	OscHandler ("/"),
	m_iPortNum( iPortNum)
{
}

void OscListener::run()
{
	m_bRunning = true;
	runListener();
}

void OscListener::runListener()
{

	using std::cout;
	using std::cerr;

	using namespace oscpkt;

	UdpSocket *socket = new UdpSocket();
	socket->bindTo(m_iPortNum);
	if (!socket->isOk())
		cerr << "Error opening port " << m_iPortNum << ": " << socket->errorMessage() << "\n";
	else
	{
		cout << "Listener started, will listen to packets on port " << m_iPortNum << std::endl;
		PacketReader pr;
		while (m_bRunning && socket->isOk())
		{
			if (socket->receiveNextPacket(30))
			{
				pr.init(socket->packetData(), socket->packetSize());
				oscpkt::Message *message;
				while (pr.isOk() && (message = pr.popMessage()) != 0)
				{
					OscHandler *handler = handlerFor(message);
					if( handler)
						handler->handle(socket, message);
					else
						qDebug() << "Listener: unhandled message: " << message->addressPattern().c_str();
				}
			}
		}
		qDebug() << "close socket";
		socket->close();
	}
}

void OscListener::setBRunning(bool bRunning)
{
	m_bRunning = bRunning;
}
