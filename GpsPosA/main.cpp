#include <QApplication>
#include <QTextCodec>
#include <QSysInfo>

#include "MainWindow.h"
#include "GpsSource/GpsSource.h"
#include "RemoteClient/RemoteClient.h"

int main(int argc, char *argv[])
{
	QCoreApplication::setApplicationName("GpsRemPos");
	QCoreApplication::setApplicationVersion("0.1.0");
	QCoreApplication::setOrganizationName("Smloft"); 

	{
		QTextCodec* codec = QTextCodec::codecForName("utf-8");
		QTextCodec::setCodecForLocale(codec);
#if ((QT_VERSION >= 0x040000) && (QT_VERSION < 0x050000))
		QTextCodec::setCodecForTr(codec);
		QTextCodec::setCodecForCStrings(codec);
#endif

	}

	QApplication a(argc, argv);

    GpsSource gpsSource(nullptr);

//	qDebug() << "machineHostName: " << QSysInfo::prettyProductName();
//	qDebug() << "machineUniqueId: " << QSysInfo::machineUniqueId();

	RemoteClient remoteClient(nullptr);
	remoteClient.setCid(QSysInfo::prettyProductName().mid(0, 16).toLatin1());
	QObject::connect(&gpsSource, &GpsSource::changed, &remoteClient, &RemoteClient::sendGps);
	//remoteClient.sendGps({});

	MainWindow w(NULL);

	QObject::connect(&gpsSource, &GpsSource::changed, &w, &MainWindow::gpsChanged);
	QObject::connect(&remoteClient, &RemoteClient::recvGps, &w, &MainWindow::gpsChanged);

    w.showMaximized();

    gpsSource.startLocation();

	return a.exec();
}
