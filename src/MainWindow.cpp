#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "Flaacontrol.h"
#include "structure/FLCFlowControl.h"
#include "flaaoscsdk/osclistener.h"
#include "flaaoscsdk/oscpkt.hh"
#include "settings/SessionSettings.h"

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
	FLCFlowControl *flowControlWindow = new FLCFlowControl(this);
	flowControlWindow->show();
}

void MainWindow::openSettingsWindow()
{
	SessionSettings *sessionSettings = new SessionSettings(this);
	sessionSettings->show();
}

void MainWindow::prepareCommunication()
{
	SettingsModel::SessionSettings *sessionSettings = SettingsModel::instance()->readSessionSettings();
	Flaacontrol::instance()->init( sessionSettings);
}
