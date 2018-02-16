#include "FLCPingHandler.h"
#include "../Flaacontrol.h"
#include "../flaaoscsdk/oscsender.h"

#include <QDebug>

FLCPingHandler::FLCPingHandler() : OscHandler ("/ping")
{

}

bool FLCPingHandler::handle(oscpkt::UdpSocket *socket, oscpkt::Message *message)
{
	int iarg;

	if (message->arg().popInt32(iarg).isOkNoMoreArgs())
	{
		qDebug() << "Listener: received /ping " << iarg << " from " << socket->packetOrigin().asString().c_str();
		oscpkt::Message repl;
		repl.init("/pong").pushInt32(iarg+1);
		oscpkt::PacketWriter pw;
		pw.init().addMessage(repl);
		socket->sendPacketTo(pw.packetData(), pw.packetSize(), socket->packetOrigin());
		return true;
	}
	return false;
}

void FLCPingHandler::sendPing()
{
	OscSender *sender = Flaacontrol::instance()->udpSender();
	Message msg("/ping");
	msg.pushInt32(m_iPing);
	sender->enqueuMessage(msg);
	++m_iPing;
}


