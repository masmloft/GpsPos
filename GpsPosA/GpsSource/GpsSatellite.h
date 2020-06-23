#pragma once

#include <QSet>
#include <QtPositioning/QGeoSatelliteInfoSource>

class GpsSatellite : public QObject
{
	Q_OBJECT
public:
	explicit GpsSatellite(QObject *parent = 0);
public:
	bool open();
	void close();
	void clear();
signals:
	void entryCountChanged();
	void errorFound(int code);
private slots:
	void error(QGeoSatelliteInfoSource::Error);
	void satellitesInViewUpdated(const QList<QGeoSatelliteInfo> &infos);
	void satellitesInUseUpdated(const QList<QGeoSatelliteInfo> &infos);
private:
	bool _running = false;
	QGeoSatelliteInfoSource* _source = nullptr;
	QList <QGeoSatelliteInfo> _knownSatellites;
	QSet<int> _knownSatelliteIds;
	QSet<int> _satellitesInUse;
};

