#include "gps.h"
#include "pins.h"
#include <HardwareSerial.h>

static HardwareSerial gpsSerial(2);

void GpsModule::begin() {
    gpsSerial.begin(GPS_BAUD, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
    Serial.println("[GPS] Initialized on UART2");
}

void GpsModule::update() {
    while (gpsSerial.available() > 0) {
        _gps.encode(gpsSerial.read());
    }

    if (_gps.location.isValid()) {
        _data.latitude = _gps.location.lat();
        _data.longitude = _gps.location.lng();
        _data.valid = true;
    } else {
        _data.valid = false;
    }

    if (_gps.altitude.isValid()) {
        _data.altitude = _gps.altitude.meters();
    }

    if (_gps.speed.isValid()) {
        _data.speed = _gps.speed.kmph();
    }

    _data.satellites = _gps.satellites.isValid() ? _gps.satellites.value() : 0;
}

GpsData GpsModule::getData() const {
    return _data;
}

bool GpsModule::hasfix() const {
    return _data.valid && _data.satellites >= 3;
}
