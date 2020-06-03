#include "RemoteClient.h"

#include <QTimerEvent>
#include <QDebug>

RemoteClient::RemoteClient(QObject* parent)
    : QObject(parent)
{
	_io = new QUdpSocket(this);
}

RemoteClient::~RemoteClient()
{
}

void RemoteClient::timerEvent(QTimerEvent* event)
{
}

void RemoteClient::gpsChanged(const QVariantMap& data)
{
	bool valid = data.value("valid", false).toBool();

	double lat = data.value("lat", 0).toDouble();
	double lon = data.value("lon", 0).toDouble();

	uint okCount = data.value("okCount", 0).toUInt();
	uint errCount = data.value("errCount", 0).toUInt();
	uint timeoutCount = data.value("timeoutCount", 0).toUInt();

	QByteArray txBuf;

	//txBuf += "\x02";

	txBuf += "CID:" + _cid;
	txBuf += QByteArray(",STA:") + (valid ? "1" : "0");
	txBuf += ",LAT:" + QByteArray::number(lat, 'f', 8);
	txBuf += ",LON:" + QByteArray::number(lon, 'f', 8);

	txBuf += "\n";

	qint64 ret = _io->writeDatagram(txBuf.data(), txBuf.size(), _remoteAddr, _remotePort);
//	qDebug() << "TX>" << _remoteAddr << ":" << _remotePort << ";RET:" << ret;

}

