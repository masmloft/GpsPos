#include "UdpBridge.h"

#include <QDateTime>

constexpr int REG_TIMEUOT = 10000;

static int64_t getTick()
{
	return QDateTime::currentDateTime().toMSecsSinceEpoch();
}

UdpBridge::UdpBridge(QObject* parent)
	: QObject(parent)
{
	startTimer(1000);
}

UdpBridge::~UdpBridge()
{

}

void UdpBridge::timerEvent(QTimerEvent*)
{
	int64_t currTick = getTick();

	for(NetPoint& item : _obClients)
	{
		if(item.blocked == false)
		{
			if(qAbs(currTick - item.lastTick) > REG_TIMEUOT)
			{
				item.blocked = true;
				QByteArray txBuf = QByteArray("$BRIDGE:UNREGED,") + item.cid + '\n';
				send(txBuf, item.addr, item.port);
			}
		}
	}
}

bool UdpBridge::open(uint16_t port)
{
	if(_io != nullptr)
		delete _io;

	_io = new QUdpSocket(this);
	bool ret = _io->bind(QHostAddress::Any, port);

	qDebug() << QDateTime::currentDateTime().toString(Qt::ISODate) << ": [OPEN SOCKET] " << (ret ? "OK" : "ERR");

	connect(_io, SIGNAL(readyRead()), this, SLOT(ioReadyRead()));

	return  ret;
}

void UdpBridge::send(const QByteArray& datagram, const QHostAddress &host, quint16 port)
{
	_io->writeDatagram(datagram, host, port);
	qDebug() << QDateTime::currentDateTime().toString(Qt::ISODate) << ": [TX]" << " ADDR:" << host.toString() << ":" << port << " DATA:" << datagram.mid(0, datagram.size() - 1);
}

void UdpBridge::ioReadyRead()
{
	qint64 rxSize = _io->pendingDatagramSize();
	if(rxSize <= 0)
		return;

	QByteArray rxBuf;
	rxBuf.resize(rxSize);

	QHostAddress sender;
	quint16 senderPort;
	qint64 ioRet = _io->readDatagram(rxBuf.data(), rxBuf.size(), &sender, &senderPort);

	if(ioRet == 0)
		return;
	rxBuf.resize(ioRet);
	if(rxBuf.at(rxBuf.size() - 1) != '\n')
		return;
	rxBuf.remove(rxBuf.size() - 1, 1);

	qDebug() << QDateTime::currentDateTime().toString(Qt::ISODate) << ": [RX]" << " ADDR:" << sender.toString() << " DATA:" << rxBuf;

	QList<QByteArray> rxFields = rxBuf.split(',');

	if((rxFields.size() >= 2) && (rxFields.at(0) == "$BRIDGE"))
	{
		if((rxFields.size() >= 3) && (rxFields.at(1) == "REG"))
		{
			const QByteArray cid = rxFields.at(2);

			auto predicate = [sender, senderPort, cid](auto item)
			{
				return (item.blocked == false) && (item.addr == sender) && (item.port == senderPort)  && (item.cid == cid);
			};

			auto it = std::find_if(_obClients.begin(), _obClients.end(), predicate);
			if(it != _obClients.end())
			{
				it->lastTick = getTick();
			}
			else
			{
				NetPoint point{false, getTick(), sender, senderPort, cid};
				auto it = std::find_if(_obClients.begin(), _obClients.end(), [](auto item) { return (item.blocked == true); });
				if(it != _obClients.end())
					*it = point;
				else
					_obClients.push_back(point);
			}

			QByteArray txBuf = QByteArray("$BRIDGE:REGED,") + cid + '\n';
			send(txBuf, sender, senderPort);
		}
	}

	for(const NetPoint& item : _obClients)
	{
		if(item.blocked == false)
			send(rxBuf, item.addr, item.port);
	}
}
