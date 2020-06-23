#include "GpsSatellite.h"

#include <QTimer>
#include <QDebug>

inline bool operator<(const QGeoSatelliteInfo& a, const QGeoSatelliteInfo& b)
{
	return a.satelliteIdentifier() < b.satelliteIdentifier();
}

GpsSatellite::GpsSatellite(QObject *parent)
	: QObject(parent)
{
}

bool GpsSatellite::open()
{
	qDebug() << "------------" << __FUNCTION__;
	close();

	clear();

	_running = true;

	//QStringList sl = QGeoSatelliteInfoSource::availableSources();

	_source = QGeoSatelliteInfoSource::createDefaultSource(this);

	qDebug() << "------------" << __FUNCTION__ << _source;

//	QVariantMap parameters;
//	parameters["serial_port"] = QString("COM22");
//	_source = QGeoSatelliteInfoSource::createDefaultSource(parameters, this);

//	if (_source == nullptr)
//	{
//		QVariantMap parameters;
//		parameters["serialnmea.serial_port"] = QString("COM22");
//		_source = QGeoSatelliteInfoSource::createSource("serialnmea", parameters, this);
//	}
	if (_source == nullptr)
		return false;


	{
		_source->setUpdateInterval(1000);
		connect(_source, SIGNAL(error(QGeoSatelliteInfoSource::Error)), this, SLOT(error(QGeoSatelliteInfoSource::Error)));
		connect(_source, SIGNAL(satellitesInViewUpdated(QList<QGeoSatelliteInfo>)), this, SLOT(satellitesInViewUpdated(QList<QGeoSatelliteInfo>)));
		connect(_source, SIGNAL(satellitesInUseUpdated(QList<QGeoSatelliteInfo>)), this, SLOT(satellitesInUseUpdated(QList<QGeoSatelliteInfo>)));
		connect(_source, SIGNAL(requestTimeout()), this, SLOT(requestTimeout()));
		_source->startUpdates();
	}

	return true;
}

void GpsSatellite::close()
{
	_running = false;
	if(_source == nullptr)
		return;
	_source->stopUpdates();
	delete _source;
	_source = nullptr;
}

void GpsSatellite::clear()
{
	_knownSatelliteIds.clear();
	_knownSatellites.clear();
	_satellitesInUse.clear();
}

void GpsSatellite::error(QGeoSatelliteInfoSource::Error error)
{
	qDebug() << "------------" << __FUNCTION__ << error;
//	emit errorFound((int)error);
}

void GpsSatellite::satellitesInViewUpdated(const QList<QGeoSatelliteInfo> &infos)
{
	qDebug() << "------------" << __FUNCTION__;
	if (_running == false)
		return;

	int oldEntryCount = _knownSatellites.count();

	QSet<int> satelliteIdsInUpdate;
	foreach (const QGeoSatelliteInfo &info, infos)
		satelliteIdsInUpdate.insert(info.satelliteIdentifier());

	QSet<int> toBeRemoved = _knownSatelliteIds - satelliteIdsInUpdate;

	//We reset the model as in reality just about all entry values change
	//and there are generally a lot of inserts and removals each time
	//Hence we don't bother with complex model update logic beyond resetModel()

	_knownSatellites = infos;

	//sort them for presentation purposes
	std::sort(_knownSatellites.begin(), _knownSatellites.end());

	//remove old "InUse" data
	//new satellites are by default not in "InUse"
	//existing satellites keep their "inUse" state
	_satellitesInUse -= toBeRemoved;

	_knownSatelliteIds = satelliteIdsInUpdate;

//	if (oldEntryCount != _knownSatellites.count())
//		emit entryCountChanged();

	emitChanged();
}

void GpsSatellite::satellitesInUseUpdated(const QList<QGeoSatelliteInfo> &infos)
{
	qDebug() << "------------" << __FUNCTION__;
	if (_running == false)
		return;

	_satellitesInUse.clear();
	foreach (const QGeoSatelliteInfo &info, infos)
		_satellitesInUse.insert(info.satelliteIdentifier());
}

void GpsSatellite::requestTimeout()
{
	qDebug() << "------------" << __FUNCTION__;
}

void GpsSatellite::emitChanged()
{
	if(changedCallback == nullptr)
		return;
	std::vector<GpsSatInfo> infos;

	for(const QGeoSatelliteInfo& item : _knownSatellites)
	{
		GpsSatInfo info;
		info.id = item.satelliteIdentifier();
		info.signalStrength = item.signalStrength();
		info.elev = item.attribute(QGeoSatelliteInfo::Elevation);
		info.azim = item.attribute(QGeoSatelliteInfo::Azimuth);
		infos.push_back(info);
	}

	changedCallback(infos);
}
