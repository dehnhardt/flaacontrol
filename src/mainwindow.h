#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "osc/oscsender.h"

#include <QMainWindow>

class OscListener;

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	void openSockets();
	void connectSlots();

private slots:
	void sendMessage();
	void openFlowControlWindow();

private:
	int m_iPing = 0;
	Ui::MainWindow *m_pUi;
	OscListener *m_pUdpListener = 0;
	OscSender *m_pUdpSender = 0;
};

#endif // MAINWINDOW_H
