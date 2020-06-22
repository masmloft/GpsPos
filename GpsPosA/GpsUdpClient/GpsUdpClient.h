#pragma once

#include <stdint.h>

#include <QObject>
#include <QVariant>
#include <QUdpSocket>

#include "Classes/TimeNotifier.h"

class GpsUdpClient : public QObject
{
    Q_OBJECT
public:
	explicit GpsUdpClient(QObject* parent);
	~GpsUdpClient();
public:
	void setCid(const QByteArray& cid) { _cid = cid; }
signals:
	void recvGps(const QVariantMap& data);
public slots:
	void sendGps(const QVariantMap& data);
private slots:
	void ioReadyRead();
private:
	void timerEvent(QTimerEvent*) override;
private:
	QByteArray _cid{"0000"};
//	QHostAddress _remoteAddr{"192.168.10.225"};
	QHostAddress _remoteAddr{"176.36.121.108"};
	uint16_t _remotePort = 1025;
	QUdpSocket* _io = nullptr;
	TimeNotifier _remRegNotifier;

};
