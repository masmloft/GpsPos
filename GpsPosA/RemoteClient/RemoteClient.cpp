#include "RemoteClient.h"

#include <QDateTime>
#include <QTimerEvent>
#include <QDebug>

static int64_t getTick()
{
	return QDateTime::currentDateTime().toMSecsSinceEpoch();
}

RemoteClient::RemoteClient(QObject* parent)
    : QObject(parent)
{
	_io = new QUdpSocket(this);
	connect(_io, SIGNAL(readyRead()), this, SLOT(ioReadyRead()));

	{
		auto lm = [io = _io, ra = _remoteAddr, rp = _remotePort]()
		{
			QByteArray txBuf("BRIDGE:REG\n");
			io->writeDatagram(txBuf.data(), txBuf.size(), ra, rp);
		};
		_remRegNotifier.setArg(5000, lm);
	}

	startTimer(1000);
}

RemoteClient::~RemoteClient()
{
}

void RemoteClient::timerEvent(QTimerEvent*)
{
	int64_t currTick = getTick();
	_remRegNotifier.doTimeout(currTick);
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
	txBuf += QByteArray(";STA:") + (valid ? "1" : "0");
	txBuf += ";LAT:" + QByteArray::number(lat, 'f', 8);
	txBuf += ";LON:" + QByteArray::number(lon, 'f', 8);

	txBuf += "\n";

	qint64 ret = _io->writeDatagram(txBuf.data(), txBuf.size(), _remoteAddr, _remotePort);
//	qDebug() << "TX>" << _remoteAddr << ":" << _remotePort << ";RET:" << ret;

}

void RemoteClient::ioReadyRead()
{
	const qint64 rxSize = _io->pendingDatagramSize();
	if(rxSize <= 0)
		return;

	QByteArray rxBuf;
	rxBuf.resize(rxSize);
	QHostAddress sender;
	quint16 senderPort;
	_io->readDatagram(rxBuf.data(), rxBuf.size(), &sender, &senderPort);
	qDebug() << "RX>" << "ADDR:" << sender.toString() << ";DATA:" << rxBuf;

	//CID:PT0000,STA:1,LAT:50.44073500,LON:30.54198333

	d

}
