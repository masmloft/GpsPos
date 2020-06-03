#pragma once

#include <stdint.h>

#include <QObject>
#include <QVariant>
#include <QUdpSocket>

class TimeNotifier
{
	int64_t _timeout = 0;
	std::function<void()> _func;
	int64_t _lastTick = 0;
public:
	TimeNotifier()
	{
	}

	TimeNotifier(int64_t timeout, std::function<void()> func)
		: _timeout(timeout)
		, _func(func)
	{
	}

	void setArg(int64_t timeout, std::function<void()> func)
	{
		_timeout = timeout;
		_func = func;
	}

	void doTimeout(int64_t currTick)
	{
		if(qAbs(currTick - _lastTick) > _timeout)
		{
			if(_func)
				_func();
			_lastTick = currTick;
		}
	}
};

class RemoteClient : public QObject
{
    Q_OBJECT
public:
	explicit RemoteClient(QObject* parent);
	~RemoteClient();
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
