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
				QByteArray txBuf("BRIDGE:UNREGED\n");
				_io->writeDatagram(txBuf.data(), txBuf.size(), item.addr, item.port);
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

	qDebug() << "Open socket - " << (ret ? "OK" : "ERR");

	connect(_io, SIGNAL(readyRead()), this, SLOT(ioReadyRead()));

	return  ret;
}

void UdpBridge::ioReadyRead()
{
	qint64 rxSize = _io->pendingDatagramSize();
	if(rxSize > 0)
	{
		QByteArray buf;
		buf.resize(rxSize);

		QHostAddress sender;
		quint16 senderPort;
		_io->readDatagram(buf.data(), buf.size(), &sender, &senderPort);
		qDebug() << "RX>" << "ADDR:" << sender.toString() << ";DATA:" << buf;

		if(buf == "BRIDGE:REG\n")
		{
			auto predicate = [sender, senderPort](auto item)
			{
				return (item.blocked == false) && (item.addr == sender) && (item.port == senderPort);
			};

			auto it = std::find_if(_obClients.begin(), _obClients.end(), predicate);
			if(it != _obClients.end())
			{
				it->lastTick = getTick();
			}
			else
			{
				NetPoint point{false, getTick(), sender, senderPort};
				auto it = std::find_if(_obClients.begin(), _obClients.end(), [](const NetPoint& item) { return (item.blocked == true); });
				if(it != _obClients.end())
					*it = point;
				else
					_obClients.push_back(point);
			}

			QByteArray txBuf("BRIDGE:REGED\n");
			_io->writeDatagram(txBuf.data(), txBuf.size(), sender, senderPort);
		}
		else
		{
			for(const NetPoint& item : _obClients)
			{
				if(item.blocked == false)
					_io->writeDatagram(buf.data(), buf.size(), item.addr, item.port);
			}
		}
	}
}
