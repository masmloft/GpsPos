#pragma once

#include <stdint.h>

#include <QDateTime>

struct GpsPos
{
	char cid[8 + 1];
	uint16_t txid;
	int status;
	bool hasFix;
	//uint64_t time;
	int64_t datetime;
	double lat;
	double lon;
	float alt;
	int satCount;
	float horizontalAccuracy;
	float verticalAccuracy;
	float groundSpeed;
	float direction;
	float magneticVariation;
};

