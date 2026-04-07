#pragma once

namespace Navigation {

// Calculate bearing from point A to point B in degrees (0-360)
// 0 = North, 90 = East, 180 = South, 270 = West
float bearingTo(double fromLat, double fromLon, double toLat, double toLon);

// Calculate distance between two points in meters (Haversine formula)
double distanceTo(double fromLat, double fromLon, double toLat, double toLon);

// Calculate the servo angle needed given the bearing to target and current heading.
// Returns angle in degrees (0-360) relative to the device's forward direction.
float relativeAngle(float bearingToTarget, float currentHeading);

// Format distance as human-readable string
// e.g., "1.2 km" or "350 m"
void formatDistance(double meters, char* buf, size_t bufLen);

}  // namespace Navigation
