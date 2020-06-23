#include "GpsUdpClient.h"

#include <QDateTime>
#include <QTimerEvent>
#include <QStringList>
#include <QDebug>

static int64_t getTick()
{
	return QDateTime::currentDateTime().toMSecsSinceEpoch();
}

GpsUdpClient::GpsUdpClient(QObject* parent)
	: QObject(parent)
{
	{
		auto lm = [this]()
		{
			QByteArray txBuf("$BRIDGE,REG,");
			txBuf += _cid;
			txBuf += '\n';
			_io->writeDatagram(txBuf.data(), txBuf.size(), _remoteAddr, _remotePort);
		};
		_remRegNotifier.setArg(5000, lm);
	}

	{
		auto lm = [this]()
		{
			if(recvCallback)
			{
				_gpsPos.hasFix = false;
				recvCallback(_gpsPos);
			}
		};
		_rxGpsTimeoutNotifier.setArg(5000, lm);
	}
}

GpsUdpClient::~GpsUdpClient()
{
}

void GpsUdpClient::open(const QString& addrPort)
{
	close();

	{
		QStringList args = addrPort.split(':');
		if(args.size() == 2)
		{
			_remoteAddr = args.at(0);
			_remotePort = args.at(1).toInt();
		}
	}

	_io = new QUdpSocket(this);
	connect(_io, SIGNAL(readyRead()), this, SLOT(ioReadyRead()));
	_io->bind();

	_timerId = startTimer(1000);
}

void GpsUdpClient::close()
{
	if(_timerId)
	{
		killTimer(_timerId);
		_timerId = 0;
	}

	if(_io != nullptr)
	{
		delete _io;
		_io = nullptr;
	}
}

void GpsUdpClient::timerEvent(QTimerEvent*)
{
	int64_t currTick = getTick();
	_remRegNotifier.doTimeout(currTick);
	_rxGpsTimeoutNotifier.doTimeout(currTick);
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
	if(rxBuf.at(rxBuf.size() - 1) != '\n')
		return;
	rxBuf.remove(rxBuf.size() - 1, 1);

	QList<QByteArray> rxFields = rxBuf.split(',');

	if(rxFields.size() > 1)
	{
		if((rxFields.at(0) == "$CVTLLA") && (rxFields.size() == 1 + 6))
		{
			_rxGpsTimeoutNotifier.reset(getTick());

			QByteArray cid = rxFields.at(1);
			bool valid = (rxFields.at(2) == "Y") ? true : false;
			int64_t datetime = rxFields.at(3).toLongLong();
			double lat = rxFields.at(4).toDouble();
			double lon = rxFields.at(5).toDouble();
			double alt = rxFields.at(6).toDouble();

			if(cid != _cid)
			{
				if(cid.isEmpty() == false)
				{
					{
						QVariantMap map;
						map["cid"] = cid;
						map["valid"] = valid;
						map["time"] = datetime;
						map["lat"] = lat;
						map["lon"] = lon;
						map["alt"] = alt;
						emit(recvGps(map));
					}

					if(recvCallback)
					{
						memcpy(_gpsPos.cid, cid.data(), std::min((int)sizeof(_gpsPos.cid), cid.size() + 1));
						//val.status;
						_gpsPos.hasFix = valid;
						_gpsPos.datetime = datetime;
						_gpsPos.lat = lat;
						_gpsPos.lon = lon;
						_gpsPos.alt= alt;

						recvCallback(_gpsPos);
					}

				}
			}

		}
	}
}
