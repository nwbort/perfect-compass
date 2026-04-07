#include "navigation.h"
#include <math.h>
#include <stdio.h>

static const double EARTH_RADIUS_M = 6371000.0;

float Navigation::bearingTo(double fromLat, double fromLon, double toLat, double toLon) {
    double lat1 = fromLat * DEG_TO_RAD;
    double lat2 = toLat * DEG_TO_RAD;
    double dLon = (toLon - fromLon) * DEG_TO_RAD;

    double y = sin(dLon) * cos(lat2);
    double x = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(dLon);

    double bearing = atan2(y, x) * RAD_TO_DEG;

    // Normalize to 0-360
    if (bearing < 0) bearing += 360.0;
    return (float)bearing;
}

double Navigation::distanceTo(double fromLat, double fromLon, double toLat, double toLon) {
    double lat1 = fromLat * DEG_TO_RAD;
    double lat2 = toLat * DEG_TO_RAD;
    double dLat = (toLat - fromLat) * DEG_TO_RAD;
    double dLon = (toLon - fromLon) * DEG_TO_RAD;

    double a = sin(dLat / 2) * sin(dLat / 2) +
               cos(lat1) * cos(lat2) *
               sin(dLon / 2) * sin(dLon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return EARTH_RADIUS_M * c;
}

float Navigation::relativeAngle(float bearingToTarget, float currentHeading) {
    float angle = bearingToTarget - currentHeading;
    // Normalize to 0-360
    while (angle < 0) angle += 360.0f;
    while (angle >= 360.0f) angle -= 360.0f;
    return angle;
}

void Navigation::formatDistance(double meters, char* buf, size_t bufLen) {
    if (meters >= 1000.0) {
        snprintf(buf, bufLen, "%.1f km", meters / 1000.0);
    } else {
        snprintf(buf, bufLen, "%.0f m", meters);
    }
}
