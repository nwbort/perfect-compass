#pragma once

#include <TinyGPSPlus.h>

struct GpsData {
    double latitude;
    double longitude;
    double altitude;
    double speed;
    int satellites;
    bool valid;
};

class GpsModule {
public:
    void begin();
    void update();
    GpsData getData() const;
    bool hasfix() const;

private:
    TinyGPSPlus _gps;
    GpsData _data = {};
};
