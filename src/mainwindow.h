#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
	void connectSlots();

private slots:
	void openFlowControlWindow();
	void openSettingsWindow();

private:
	Ui::MainWindow *m_pUi;
};

#endif // MAINWINDOW_H
