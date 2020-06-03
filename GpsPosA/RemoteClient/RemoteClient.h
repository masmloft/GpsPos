#pragma once

#include <stdint.h>

#include <QObject>
#include <QVariant>
#include <QUdpSocket>

class RemoteClient : public QObject
{
    Q_OBJECT
public:
	explicit RemoteClient(QObject* parent);
	~RemoteClient();
public slots:
	void gpsChanged(const QVariantMap& data);
private:
	void timerEvent(QTimerEvent* event);
private:
	QByteArray _cid{"PT0000"};
//	QHostAddress _remoteAddr{"192.168.10.225"};
	QHostAddress _remoteAddr{"sml.pp.ua"};
	uint16_t _remotePort = 1025;
	QUdpSocket* _io = nullptr;

};
