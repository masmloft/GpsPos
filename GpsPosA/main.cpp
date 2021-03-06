#include <QApplication>
#include <QTextCodec>
#include <QSysInfo>

#include "MainWindow.h"
#include "GpsSource/GpsSource.h"
#include "GpsSource/GpsSatellite.h"
#include "GpsUdpClient/GpsUdpClient.h"

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

	GpsSource gpsSource;

	QByteArray cid;
	{
		QByteArray info = QSysInfo::buildCpuArchitecture().toLatin1()
				+ QSysInfo::currentCpuArchitecture().toLatin1()
				+ QSysInfo::buildAbi().toLatin1()
				+ QSysInfo::kernelType().toLatin1()
				+ QSysInfo::kernelVersion().toLatin1()
				+ QSysInfo::productType().toLatin1()
				+ QSysInfo::productVersion().toLatin1()
				+ QSysInfo::prettyProductName().toLatin1()
				+ QSysInfo::machineHostName().toLatin1()
				+ QSysInfo::machineUniqueId()
				+ QSysInfo::bootUniqueId();

		cid = QByteArray::number(qHash(info), 16);
	}

	GpsUdpClient gpsUdpClient(nullptr);
	gpsUdpClient.setCid(cid);
	QObject::connect(&gpsSource, &GpsSource::changed, &gpsUdpClient, &GpsUdpClient::sendGps);
	gpsUdpClient.open("176.36.121.108:1025");
	//gpsUdpClient.sendGps({});

	MainWindow w(NULL);

	QObject::connect(&gpsSource, &GpsSource::changed, &w, &MainWindow::gpsChanged);
	QObject::connect(&gpsUdpClient, &GpsUdpClient::recvGps, &w, &MainWindow::gpsChanged);

    w.showMaximized();

	gpsSource.startLocation();

	GpsSatellite gpsSatellite;
	gpsSatellite.changedCallback = [&w](const std::vector<GpsSatInfo>& info) { w.setGpsSatInfo(info); };
	gpsSatellite.open();


	return a.exec();
}
