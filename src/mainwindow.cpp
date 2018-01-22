#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "osc/osclistener.h"
#include "osc/oscpkt.hh"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	m_pUi(new Ui::MainWindow)
{
	m_pUi->setupUi(this);
	openSockets();
	connectSlots();
}

MainWindow::~MainWindow()
{
	delete m_pUdpSender;
	//delete m_pUdpListener;
	delete m_pUi;
}

void MainWindow::openSockets()
{
	m_pUdpSender = new OscSender(9109);
	m_pUdpListener = new OscListener(9110);
	m_pUdpSender->start();
	m_pUdpListener->start();
}

void MainWindow::connectSlots()
{
	connect(m_pUi->actionSendPing, SIGNAL(triggered()), this, SLOT(sendMessage()));
}

void MainWindow::sendMessage()
{
	Message msg("/ping");
	msg.pushInt32(m_iPing);
	m_pUdpSender->enqueuMessage(msg);
	++m_iPing;
}
