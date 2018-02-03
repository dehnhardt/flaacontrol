#include "Flaacontrol.h"

#include "osc/oscsender.h"
#include "osc/osclistener.h"

#include "handler/FLCPingHandler.h"
#include "handler/FLCRepositoryModuleHandler.h"

#include <QCoreApplication>
#include <QThread>


Flaacontrol::Flaacontrol() : QObject ()
{
}

Flaacontrol::~Flaacontrol()
{

}

void Flaacontrol::openSockets()
{
	m_pListenerThread = new QThread(this);
	m_pUdpSender = new OscSender(m_sSendHost, m_iSendPort);
	m_pUdpListener = new OscListener(m_iListenPort);

	m_pUdpListener->moveToThread(m_pListenerThread);
	registerHandler();
	connectSlots();

	m_pUdpSender->start();
	m_pListenerThread->start();
}

void Flaacontrol::closeSockets()
{
	m_pUdpListener->setBRunning(false);
	m_pUdpListener->deleteLater();
	m_pListenerThread->terminate();
	m_pListenerThread->wait();
	m_pListenerThread->deleteLater();
}

void Flaacontrol::registerHandler()
{
	m_pUdpListener->registerHandler(new FLCPingHandler());
	m_pUdpListener->registerHandler(new FLCRepositoryModuleHandler());
}

void Flaacontrol::connectSlots()
{
	connect(m_pListenerThread, &QThread::started, m_pUdpListener, &OscListener::init);
	connect(m_pListenerThread, &QThread::finished, m_pUdpListener, &OscListener::exit);
	connect(m_pUdpListener, &OscListener::started, this, &Flaacontrol::listenerThreadStarted);
	connect(m_pUdpListener, &OscListener::finished, this, &Flaacontrol::listenerThreadFinished);
	// Allow graceful termination of the thread
	connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, this, &Flaacontrol::onApplicationExit );
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
}

void Flaacontrol::onApplicationExit()
{
	closeSockets();
}

OscListener *Flaacontrol::udpListener() const
{
	return m_pUdpListener;
}

Flaacontrol *Flaacontrol::_instance = 0;
