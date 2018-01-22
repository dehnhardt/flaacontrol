#include "oscsender.h"

#include <iostream>
#include <QDebug>

using std::cout;
using std::cerr;

using namespace oscpkt;


OscSender::OscSender( int iPortNum) :
	QThread(),
	m_sHost("localhost"),
	m_iPortNum(iPortNum)
{
	init();
}

OscSender::OscSender(string sHost, int iPortNum) :
	QThread(),
	m_sHost(sHost),
	m_iPortNum(iPortNum)
{
	init();
}

OscSender::~OscSender()
{
	m_pUdpSocket->close();
	delete m_pMessageQueue;
	delete m_pUdpSocket;
	m_pMessageQueue = 0;
	m_pUdpSocket = 0;
}

void OscSender::init()
{
	m_pMessageQueue = new QQueue<Message>();
	m_pUdpSocket = new UdpSocket();
}

void OscSender::enqueuMessage(Message message)
{
	if( m_pMessageQueue )
		m_pMessageQueue->enqueue(message);
	if( ! m_bRunning )
		sendQueuedMessages();
}

void OscSender::sendQueuedMessages()
{
	if( m_pUdpSocket->isOk() )
		m_bRunning = true;
	else
	{
		cerr << "Socket not ready";
		return;
	}
	while( !m_pMessageQueue->isEmpty())
	{
		Message message = m_pMessageQueue->dequeue();
		PacketWriter pw;
		pw.startBundle().addMessage(message).endBundle();
		bool ok = m_pUdpSocket->sendPacket(pw.packetData(), pw.packetSize());
		if (ok && m_pUdpSocket->receiveNextPacket(30 /* timeout, in ms */))
		{
			PacketReader pr(m_pUdpSocket->packetData(), m_pUdpSocket->packetSize());
			Message *incoming_msg;
			while (pr.isOk() && (incoming_msg = pr.popMessage()) != 0)
				qDebug() << "Client: received " << incoming_msg->addressPattern().c_str();
		}
	}
	m_bRunning = false;
}

void OscSender::run()
{
	m_pUdpSocket->connectTo(m_sHost, m_iPortNum);
}
