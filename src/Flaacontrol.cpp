#include "Flaacontrol.h"

#include "osc/oscsender.h"
#include "osc/osclistener.h"

#include "handler/FLCPingHandler.h"
#include "handler/FLCRepositoryModuleHandler.h"


Flaacontrol::Flaacontrol()
{
	openSockets();
}

Flaacontrol::~Flaacontrol()
{
	delete m_pUdpSender;
	delete m_pUdpListener;
}

void Flaacontrol::openSockets()
{
	m_pUdpSender = new OscSender(9109);
	m_pUdpListener = new OscListener(9110);
	registerHandler();
	m_pUdpSender->start();
	m_pUdpListener->start();
}

void Flaacontrol::registerHandler()
{
	m_pUdpListener->registerHandler(new FLCPingHandler());
	m_pUdpListener->registerHandler(new FLCRepositoryModuleHandler());
}

OscSender *Flaacontrol::pUdpSender() const
{
	return m_pUdpSender;
}

OscListener *Flaacontrol::pUdpListener() const
{
	return m_pUdpListener;
}

Flaacontrol *Flaacontrol::_instance = 0;
