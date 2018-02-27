#include "Flaacontrol.h"

#include "flaaoscsdk/oscsender.h"
#include "flaaoscsdk/osclistener.h"

#include "handler/FLCPingHandler.h"
#include "handler/FLCRepositoryModuleHandler.h"
#include "handler/FLCModuleInstancesHandler.h"

#include "model/FLCModuleInstancesModel.h"
#include "flaaoscsdk/FLOModuleInstanceDAO.h"
#include "settings/SettingsModel.h"
#include "settings/SessionSettings.h"

#include <QCoreApplication>
#include <QThread>
#include <QFile>
#include <QIODevice>
#include <QXmlStreamReader>
#include <QDebug>


Flaacontrol::Flaacontrol() : QObject (),
	m_pModuleInstancesModel(new FLCModuleInstancesModel())
{
}

Flaacontrol::~Flaacontrol()
{
	if(m_pModuleInstancesModel)
		delete m_pModuleInstancesModel;
}

void Flaacontrol::readStructure()
{
	QFile *file = new QFile("/home/dehnhardt/structure.xml");
	if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
		return;
	QXmlStreamReader *xmlReader = new QXmlStreamReader(file);
	while(!xmlReader->atEnd())
	{
		QXmlStreamReader::TokenType t = xmlReader->readNext();
		QStringRef s = xmlReader->name();
		switch( t )
		{
			case QXmlStreamReader::TokenType::StartElement:
				qDebug() << "Widget: Element Name: " << s;
				if( s == "ModuleInstances")
					m_pModuleInstancesModel->deserialize(xmlReader);
				break;
			case QXmlStreamReader::TokenType::EndElement:
				if( s == "ModuleInstances")
					return;
				break;
			default:
				break;
		}
	}
	file->close();
}

void Flaacontrol::readServerStructure()
{
	this->m_pInstancesHandler->requestStructure();
}

void Flaacontrol::init(SettingsModel::SessionSettings *sessionSettings)
{
	setListenPort(sessionSettings->listenPort);
	setSendHost(sessionSettings->sendAddress.toStdString());
	setSendPort(sessionSettings->sendPort);
	openSockets();
	//readStructure();
	registerHandler();
	connectSlots();
	readServerStructure();
}

void Flaacontrol::openSockets()
{
	m_pListenerThread = new QThread(this);
	m_pUdpSender = new OscSender(m_sSendHost, m_iSendPort);
	m_pUdpListener = new OscListener(m_iListenPort);

	m_pUdpListener->moveToThread(m_pListenerThread);
	createGlobalHandlers();
	registerHandler();
	m_pInstancesHandler->setModel(m_pModuleInstancesModel);

	m_pUdpSender->start();
	m_pListenerThread->start();
}

void Flaacontrol::closeSockets()
{
	m_pUdpListener->setBRunning(false);
	m_pListenerThread->terminate();
	m_pListenerThread->wait();
	m_pListenerThread->deleteLater();
}

void Flaacontrol::registerHandler()
{
	m_pUdpListener->registerHandler(new FLCPingHandler());
	m_pUdpListener->registerHandler(new FLCRepositoryModuleHandler());
	m_pUdpListener->registerHandler(m_pInstancesHandler);
}

void Flaacontrol::connectSlots()
{
	connect(m_pListenerThread, &QThread::started, m_pUdpListener, &OscListener::init);
	connect(m_pListenerThread, &QThread::finished, m_pUdpListener, &OscListener::exit);
	connect(m_pUdpListener, &OscListener::started, this, &Flaacontrol::listenerThreadStarted);
	connect(m_pUdpListener, &OscListener::finished, this, &Flaacontrol::listenerThreadFinished);
	// Repository
	connect(m_pInstancesHandler, &FLCModuleInstancesHandler::moduleInstanceAdded, this->m_pModuleInstancesModel, &FLCModuleInstancesModel::addFLOModuleInstance);
	// Allow graceful termination of the thread
	connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, this, &Flaacontrol::onApplicationExit );
}

void Flaacontrol::createGlobalHandlers()
{
	this->m_pInstancesHandler = new FLCModuleInstancesHandler();
}

FLCModuleInstancesHandler *Flaacontrol::pInstancesHandler() const
{
	return m_pInstancesHandler;
}

FLCModuleInstancesModel *Flaacontrol::moduleInstancesModel() const
{
	return m_pModuleInstancesModel;
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
