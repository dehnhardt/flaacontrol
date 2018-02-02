#include "Flaacontrol.h"

#include "osc/oscsender.h"
#include "osc/osclistener.h"

#include "handler/FLCPingHandler.h"
#include "handler/FLCRepositoryModuleHandler.h"


Flaacontrol::Flaacontrol() : QObject ()
{
	openSockets();
}

Flaacontrol::~Flaacontrol()
{
}

void Flaacontrol::openSockets()
{
	m_pUdpSender = new OscSender(9109);
	m_pUdpListener = new OscListener(9110);
	registerHandler();
	connectSlots();

	m_pUdpSender->start();
	m_pUdpListener->start();
}

void Flaacontrol::registerHandler()
{
	m_pUdpListener->registerHandler(new FLCPingHandler());
	m_pUdpListener->registerHandler(new FLCRepositoryModuleHandler());
}

void Flaacontrol::connectSlots()
{
	connect(m_pUdpListener, &OscListener::finished, m_pUdpListener, &QObject::deleteLater);
	connect(m_pUdpListener, &OscListener::finished, this, &Flaacontrol::listenerThreadFinished);
	connect(m_pUdpListener, &OscListener::started, this, &Flaacontrol::listenerThreadStarted);
}

OscSender *Flaacontrol::udpSender() const
{
	return m_pUdpSender;
}

void Flaacontrol::listenerThreadStarted()
{
	qDebug( "listener thread has started" );
}

void Flaacontrol::listenerThreadFinished()
{
	qDebug( "listener thread has stopped" );
	m_pUdpListener->deleteLater();
}

OscListener *Flaacontrol::udpListener() const
{
	return m_pUdpListener;
}

Flaacontrol *Flaacontrol::_instance = 0;
