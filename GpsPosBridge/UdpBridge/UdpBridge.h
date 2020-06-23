#include <QUdpSocket>

#include <list>

class UdpBridge : public QObject
{
	Q_OBJECT
public:
	UdpBridge(QObject* parent);
	~UdpBridge();
public:
	bool open(uint16_t port);
private:
	struct NetPoint
	{
		bool blocked = false;
        int64_t lastTick = 0;
		QHostAddress addr;
		uint16_t port = 0;
		QByteArray cid;
	};
private:
	QUdpSocket* _io = nullptr;
	std::list<NetPoint> _obClients;
private:
	void timerEvent(QTimerEvent*) override;
	void send(const QByteArray &datagram, const QHostAddress &host, quint16 port);
private slots:
	void ioReadyRead();
};
