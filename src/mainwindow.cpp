#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Flaacontrol.h"
#include "structure/flowcontrol.h"
#include "osc/osclistener.h"
#include "osc/oscpkt.hh"
#include "settings/SettingsModel.h"

#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	m_pUi(new Ui::MainWindow)
{
	m_pUi->setupUi(this);
	setGeometry(
			QStyle::alignedRect(
					Qt::LeftToRight,
					Qt::AlignCenter,
					size(),
					qApp->desktop()->availableGeometry()
			));
	prepareCommunication();
	connectSlots();
}

MainWindow::~MainWindow()
{
	delete m_pUi;
}


void MainWindow::connectSlots()
{
	connect(m_pUi->actionSignal_Flow, SIGNAL(triggered()), this, SLOT(openFlowControlWindow()));
	connect(m_pUi->actionSession_Settings, SIGNAL(triggered()), this, SLOT(openSettingsWindow()));
}


void MainWindow::openFlowControlWindow()
{
	FlowControl *flowControlWindow = new FlowControl(this);
	flowControlWindow->show();
}

void MainWindow::openSettingsWindow()
{
	SessionSettings *sessionSettings = new SessionSettings(this);
	sessionSettings->show();
}

void MainWindow::prepareCommunication()
{
	SettingsModel *m = SettingsModel::instance();
	SettingsModel::SessionSettings *sessionSettings = m->readSessionSettings();
	Flaacontrol *f = Flaacontrol::instance();
	f->init( sessionSettings);
}
