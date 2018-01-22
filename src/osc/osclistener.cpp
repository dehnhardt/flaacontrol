#include "osclistener.h"
#include "oscpkt.hh"
#include "udp.hh"

#include <QDebug>
#include <QString>

using namespace oscpkt;

OscListener::OscListener( int iPortNum) :
	QThread( ),
	m_iPortNum( iPortNum)
{
}

void OscListener::run()
{
	runListener();
}

void OscListener::runListener()
{

	using std::cout;
	using std::cerr;

	using namespace oscpkt;

	UdpSocket sock;
	sock.bindTo(m_iPortNum);
	if (!sock.isOk())
		cerr << "Error opening port " << m_iPortNum << ": " << sock.errorMessage() << "\n";
	else
	{
		cout << "Listener started, will listen to packets on port " << m_iPortNum << std::endl;
		PacketReader pr;
		PacketWriter pw;
		while (sock.isOk())
		{
			if (sock.receiveNextPacket(0 /*30 timeout, in ms */))
			{
				pr.init(sock.packetData(), sock.packetSize());
				oscpkt::Message *msg;
				while (pr.isOk() && (msg = pr.popMessage()) != 0)
				{
					int iarg;
					if (msg->match("/ping").popInt32(iarg).isOkNoMoreArgs())
					{
						qDebug() << "Listener: received /ping " << iarg << " from " << sock.packetOrigin().asString().c_str();
						Message repl;
						repl.init("/pong").pushInt32(iarg+1);
						pw.init().addMessage(repl);
						sock.sendPacketTo(pw.packetData(), pw.packetSize(), sock.packetOrigin());
					}
					else
						qDebug() << "Listener: unhandled message: " << msg->typeTags().c_str();
				}
			}
		}
	}
}
