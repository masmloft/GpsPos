#include <QUdpSocket>

#include <QList>

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
		QHostAddress addr;
		uint16_t port = 0;
	};
private:
	QUdpSocket* _io = nullptr;
	QList<NetPoint> _obClients;
private slots:
	void ioReadyRead();
};
