#include <QCoreApplication>

#include "UdpBridge/UdpBridge.h"

int main(int argc, char *argv[])
{
	qDebug() << "Starting...";
	QCoreApplication qapp(argc, argv);

	UdpBridge udpBridge(nullptr);
	udpBridge.open(1025);

	int ret = qapp.exec();
	return ret;
}
