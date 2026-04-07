#pragma once

struct DisplayData {
    float heading;          // Current compass heading (degrees)
    float bearingToTarget;  // Bearing to target (degrees)
    float pointerAngle;     // Current pointer angle (degrees)
    double distance;        // Distance to target (meters)
    double latitude;        // Current latitude
    double longitude;       // Current longitude
    double targetLat;       // Target latitude
    double targetLon;       // Target longitude
    int satellites;         // GPS satellite count
    bool gpsValid;          // GPS has fix
    bool compassReady;      // Compass is initialized
    bool calibrating;       // Compass is calibrating
    const char* targetName; // Name of the target location
};

class Display {
public:
    bool begin();
    void show(const DisplayData& data);
    void showMessage(const char* line1, const char* line2 = nullptr);
    void showCalibrating();

private:
    bool _ready = false;
    void drawCompassRose(int cx, int cy, int radius, float heading, float bearingToTarget);
};
