#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QCoreApplication::setOrganizationName("punkt-k");
	QCoreApplication::setOrganizationDomain("www.punkt-k.de");
	QCoreApplication::setApplicationName("flaa-Control");
	QCoreApplication::setApplicationVersion("0.1.0-alpha");

	MainWindow w;
	w.show();
	return app.exec();
}
