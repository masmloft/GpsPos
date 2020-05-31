#include <QApplication>
#include <QTextCodec>

#ifdef _DEBUG
//#include "../vld/include/vld.h"
#endif

#include "MainWindow.h"


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
	MainWindow w(NULL);
    w.showMaximized();
	return a.exec();
}
