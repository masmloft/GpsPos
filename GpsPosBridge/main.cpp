#include <QCoreApplication>

#include "UdpBridge/UdpBridge.h"

int main(int argc, char *argv[])
{
	QCoreApplication qapp(argc, argv);

	UdpBridge udpBridge(nullptr);
	udpBridge.open(1025);

	int ret = qapp.exec();
	return ret;
}
