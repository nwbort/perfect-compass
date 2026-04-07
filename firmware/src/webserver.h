#pragma once

#include "config.h"
#include "compass.h"
#include "gps.h"

class WebServer {
public:
    void begin(Config* config, CompassModule* compass, GpsModule* gps);
    void handleClient();

private:
    Config* _config;
    CompassModule* _compass;
    GpsModule* _gps;
};
