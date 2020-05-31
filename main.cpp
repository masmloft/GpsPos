#include <QApplication>
#include <QTextCodec>

#include "MainWindow.h"
#include "GpsSource/GpsSource.h"

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

	MainWindow w(NULL);

    QObject::connect(&gpsSource, &GpsSource::changed, &w, &MainWindow::gpsChanged);

    w.showMaximized();

    gpsSource.startLocation();

	return a.exec();
}
