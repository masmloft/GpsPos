#pragma once

#include <functional>

#include <QSet>
#include <QtPositioning/QGeoSatelliteInfoSource>

#include "../Classes/GpsSatInfo.h"

class GpsSatellite : public QObject
{
	Q_OBJECT
public:
	std::function<void(const std::vector<GpsSatInfo>& info)> changedCallback;
public:
	explicit GpsSatellite(QObject *parent = 0);
public:
	bool open();
	void close();
	void clear();
private slots:
	void error(QGeoSatelliteInfoSource::Error);
	void satellitesInViewUpdated(const QList<QGeoSatelliteInfo> &infos);
	void satellitesInUseUpdated(const QList<QGeoSatelliteInfo> &infos);
	void requestTimeout();
private:
	bool _running = false;
	QGeoSatelliteInfoSource* _source = nullptr;
	QList<QGeoSatelliteInfo> _knownSatellites;
	QSet<int> _knownSatelliteIds;
	QSet<int> _satellitesInUse;
private:
	void emitChanged();
};

