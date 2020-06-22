#include "GpsUdpClient.h"

#include <QDateTime>
#include <QTimerEvent>
#include <QDebug>

static int64_t getTick()
{
	return QDateTime::currentDateTime().toMSecsSinceEpoch();
}

GpsUdpClient::GpsUdpClient(QObject* parent)
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

GpsUdpClient::~GpsUdpClient()
{
}

void GpsUdpClient::timerEvent(QTimerEvent*)
{
	int64_t currTick = getTick();
	_remRegNotifier.doTimeout(currTick);
}

void GpsUdpClient::sendGps(const QVariantMap& data)
{
	bool valid = data.value("valid", false).toBool();
	int64_t time = data.value("time", 0).toLongLong();

	double lat = data.value("lat", 0).toDouble();
	double lon = data.value("lon", 0).toDouble();
	double alt = data.value("alt", 0).toDouble();

	QByteArray txBuf;
	txBuf.reserve(256);

	txBuf = "$CVTLLA";

	txBuf += "," + _cid;
	txBuf += (valid ? ",Y" : ",N");
	txBuf += "," + QByteArray::number(time);
	txBuf += "," + QByteArray::number(lat, 'f', 8);
	txBuf += "," + QByteArray::number(lon, 'f', 8);
	txBuf += "," + QByteArray::number(alt, 'f', 1);

	txBuf += "\n";

	qint64 ret = _io->writeDatagram(txBuf.data(), txBuf.size(), _remoteAddr, _remotePort);
//	qDebug() << "TX>" << _remoteAddr << ":" << _remotePort << ";RET:" << ret;

}

void GpsUdpClient::ioReadyRead()
{
	const qint64 rxSize = _io->pendingDatagramSize();
	if(rxSize <= 0)
		return;

	QByteArray rxBuf;
	rxBuf.resize(rxSize);
	QHostAddress sender;
	quint16 senderPort;
	qint64 ioRet = _io->readDatagram(rxBuf.data(), rxBuf.size(), &sender, &senderPort);
	//qDebug() << "RX>" << "ADDR:" << sender.toString() << ";DATA:" << rxBuf;

	if(ioRet == 0)
		return;
	rxBuf.resize(ioRet);
	if(rxBuf.back() != '\n')
		return;
	rxBuf.remove(rxBuf.size() - 1, 1);

	QVariantMap map;

	QList<QByteArray> fields = rxBuf.split(',');

	if(fields.size() > 1)
	{
		if((fields.at(0) == "$CVTLLA") && (fields.size() == 1 + 6))
		{
			map["cid"] = fields.at(1);
			map["valid"] = (fields.at(2) == "Y") ? true : false;
			map["time"] = fields.at(3).toLongLong();
			map["lat"] = fields.at(4).toDouble();
			map["lon"] = fields.at(5).toDouble();
			map["alt"] = fields.at(6).toDouble();

			if(map["cid"] != _cid)
			{
				if(map.contains("cid") && map.contains("lat") && map.contains("lon"))
				{
					emit(recvGps(map));
				}
			}

		}
	}
}
