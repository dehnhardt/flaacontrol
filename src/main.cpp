#include "mainwindow.h"
#include <QApplication>

#include "settings/XmlSettings.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QCoreApplication::setOrganizationName("punkt-k");
	QCoreApplication::setOrganizationDomain("www.punkt-k.de");
	QCoreApplication::setApplicationName("flaa-Control");
	QCoreApplication::setApplicationVersion("0.1.0-alpha");

	QSettings::setDefaultFormat(xmlFormat);

	MainWindow w;
	w.show();
	return app.exec();
}
