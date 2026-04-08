#include "compass.h"
#include "pins.h"
#include <Wire.h>
#include <Adafruit_HMC5883_U.h>
#include <Preferences.h>
#include <math.h>

static Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
static Preferences calPrefs;

bool CompassModule::begin() {
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

    if (!mag.begin()) {
        Serial.println("[Compass] HMC5883L not found! Check wiring.");
        _ready = false;
        return false;
    }

    Serial.println("[Compass] HMC5883L initialized");
    loadCalibration();
    _ready = true;
    return true;
}

void CompassModule::update() {
    if (!_ready) return;

    sensors_event_t event;
    mag.getEvent(&event);

    float x = event.magnetic.x - _offsetX;
    float y = event.magnetic.y - _offsetY;

    if (_calibrating) {
        if (event.magnetic.x < _minX) _minX = event.magnetic.x;
        if (event.magnetic.x > _maxX) _maxX = event.magnetic.x;
        if (event.magnetic.y < _minY) _minY = event.magnetic.y;
        if (event.magnetic.y > _maxY) _maxY = event.magnetic.y;
    }

    float heading = atan2(y, x);

    // Apply magnetic declination correction
    heading += _declination * DEG_TO_RAD;

    // Normalize to 0-2PI
    if (heading < 0) heading += 2 * PI;
    if (heading > 2 * PI) heading -= 2 * PI;

    _heading = heading * RAD_TO_DEG;
}

float CompassModule::getHeading() const {
    return _heading;
}

bool CompassModule::isReady() const {
    return _ready;
}

void CompassModule::startCalibration() {
    _calibrating = true;
    _minX = 32767; _maxX = -32768;
    _minY = 32767; _maxY = -32768;
    Serial.println("[Compass] Calibration started - rotate device slowly...");
}

void CompassModule::endCalibration() {
    _calibrating = false;
    _offsetX = (_minX + _maxX) / 2.0f;
    _offsetY = (_minY + _maxY) / 2.0f;
    saveCalibration();
    Serial.printf("[Compass] Calibration done. Offsets: X=%.1f Y=%.1f\n", _offsetX, _offsetY);
}

bool CompassModule::isCalibrating() const {
    return _calibrating;
}

void CompassModule::setDeclination(float degrees) {
    _declination = degrees;
}

void CompassModule::loadCalibration() {
    calPrefs.begin("compass_cal", true);
    _offsetX = calPrefs.getFloat("offX", 0);
    _offsetY = calPrefs.getFloat("offY", 0);
    calPrefs.end();
    Serial.printf("[Compass] Loaded calibration: X=%.1f Y=%.1f\n", _offsetX, _offsetY);
}

void CompassModule::saveCalibration() {
    calPrefs.begin("compass_cal", false);
    calPrefs.putFloat("offX", _offsetX);
    calPrefs.putFloat("offY", _offsetY);
    calPrefs.end();
}
