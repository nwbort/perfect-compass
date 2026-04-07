#pragma once

#include <Preferences.h>

struct TargetLocation {
    double latitude;
    double longitude;
    char name[32];
};

class Config {
public:
    void begin();

    void setTarget(double lat, double lon, const char* name);
    TargetLocation getTarget() const;
    bool hasTarget() const;

    void setDeclination(float degrees);
    float getDeclination() const;

    void setWiFiCredentials(const char* ssid, const char* password);
    const char* getWiFiSSID() const;
    const char* getWiFiPassword() const;

private:
    Preferences _prefs;
    TargetLocation _target = {};
    float _declination = 0;
    char _wifiSSID[33];
    char _wifiPass[65];
    bool _hasTarget = false;
};
