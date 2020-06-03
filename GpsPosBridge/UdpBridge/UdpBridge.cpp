#include "UdpBridge.h"

#include <QDateTime>

static int64_t getTick()
{
    return QDateTime::currentDateTime().toMSecsSinceEpoch();
}

UdpBridge::UdpBridge(QObject* parent)
	: QObject(parent)
{
}

UdpBridge::~UdpBridge()
{

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

		if(buf == "REG\n")
		{
			//qDebug() << "REG;" << "ADDR:" << sender.toString();
			//_obClients.in
            _obClients.append({getTick(), sender, senderPort});

			QByteArray txBuf("REG:REGED\n");
			_io->writeDatagram(txBuf.data(), txBuf.size(), sender, senderPort);
		}
		else
		{
			for(const auto& i : _obClients)
			{
				_io->writeDatagram(buf.data(), buf.size(), i.addr, i.port);
			}
		}
	}
}
