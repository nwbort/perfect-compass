#include "config.h"
#include "pins.h"
#include <string.h>

void Config::begin() {
    _prefs.begin("pc_config", true);  // read-only

    _target.latitude = _prefs.getDouble("tgt_lat", 0);
    _target.longitude = _prefs.getDouble("tgt_lon", 0);
    _prefs.getString("tgt_name", _target.name, sizeof(_target.name));
    _hasTarget = _prefs.getBool("has_tgt", false);

    _declination = _prefs.getFloat("decl", 0);

    _prefs.getString("wifi_ssid", _wifiSSID, sizeof(_wifiSSID));
    _prefs.getString("wifi_pass", _wifiPass, sizeof(_wifiPass));

    // Use defaults if no WiFi config
    if (strlen(_wifiSSID) == 0) {
        strncpy(_wifiSSID, DEFAULT_AP_SSID, sizeof(_wifiSSID));
        strncpy(_wifiPass, DEFAULT_AP_PASS, sizeof(_wifiPass));
    }

    _prefs.end();

    Serial.printf("[Config] Target: %s (%.6f, %.6f) valid=%d\n",
                  _target.name, _target.latitude, _target.longitude, _hasTarget);
}

void Config::setTarget(double lat, double lon, const char* name) {
    _target.latitude = lat;
    _target.longitude = lon;
    strncpy(_target.name, name, sizeof(_target.name) - 1);
    _target.name[sizeof(_target.name) - 1] = '\0';
    _hasTarget = true;

    _prefs.begin("pc_config", false);
    _prefs.putDouble("tgt_lat", lat);
    _prefs.putDouble("tgt_lon", lon);
    _prefs.putString("tgt_name", name);
    _prefs.putBool("has_tgt", true);
    _prefs.end();

    Serial.printf("[Config] Target set: %s (%.6f, %.6f)\n", name, lat, lon);
}

TargetLocation Config::getTarget() const {
    return _target;
}

bool Config::hasTarget() const {
    return _hasTarget;
}

void Config::setDeclination(float degrees) {
    _declination = degrees;
    _prefs.begin("pc_config", false);
    _prefs.putFloat("decl", degrees);
    _prefs.end();
}

float Config::getDeclination() const {
    return _declination;
}

void Config::setWiFiCredentials(const char* ssid, const char* password) {
    strncpy(_wifiSSID, ssid, sizeof(_wifiSSID) - 1);
    strncpy(_wifiPass, password, sizeof(_wifiPass) - 1);

    _prefs.begin("pc_config", false);
    _prefs.putString("wifi_ssid", ssid);
    _prefs.putString("wifi_pass", password);
    _prefs.end();
}

const char* Config::getWiFiSSID() const {
    return _wifiSSID;
}

const char* Config::getWiFiPassword() const {
    return _wifiPass;
}
