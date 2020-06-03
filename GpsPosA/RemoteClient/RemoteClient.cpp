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

void RemoteClient::sendGps(const QVariantMap& data)
{
	bool valid = data.value("valid", false).toBool();
	int64_t time = data.value("time", 0).toLongLong();

	double lat = data.value("lat", 0).toDouble();
	double lon = data.value("lon", 0).toDouble();

	uint okCount = data.value("okCount", 0).toUInt();
	uint errCount = data.value("errCount", 0).toUInt();
	uint timeoutCount = data.value("timeoutCount", 0).toUInt();

	QByteArray txBuf;

	//txBuf += "\x02";

	txBuf += "cid:" + _cid;
	txBuf += QByteArray(";valid:") + (valid ? "1" : "0");
	txBuf += ";time:" + QByteArray::number(time);
	txBuf += ";lat:" + QByteArray::number(lat, 'f', 8);
	txBuf += ";lon:" + QByteArray::number(lon, 'f', 8);

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
	//qDebug() << "RX>" << "ADDR:" << sender.toString() << ";DATA:" << rxBuf;

	QVariantMap map;

	QList<QByteArray> fields = rxBuf.split(';');
	for(auto field : fields)
	{
		QList<QByteArray> keyValue = field.split(':');
		if(keyValue.size() == 2)
			map[keyValue.at(0)] = keyValue.at(1);
	}

	QByteArray cid = map.value("cid", "").toByteArray();
	if(cid != _cid)
	{
		double lat = map.value("lat", 0).toDouble();
		double lon = map.value("lon", 0).toDouble();

		if(map.contains("cid") && map.contains("lat") && map.contains("lon"))
		{
			emit(recvGps(map));
		}
	}

}
