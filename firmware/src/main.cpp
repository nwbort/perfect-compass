#include <Arduino.h>
#include "pins.h"
#include "gps.h"
#include "compass.h"
#include "navigation.h"
#include "pointer.h"
#include "display.h"
#include "config.h"
#include "webserver.h"

// ---- Module instances ----
static GpsModule gps;
static CompassModule compass;
static Pointer pointer;
static Display display;
static Config config;
static WebServer web;

// ---- Timing ----
static unsigned long lastSensorUpdate = 0;
static unsigned long lastDisplayUpdate = 0;
static const unsigned long SENSOR_INTERVAL_MS = 50;   // 20 Hz sensor reads
static const unsigned long DISPLAY_INTERVAL_MS = 200;  // 5 Hz display refresh

void setup() {
    Serial.begin(115200);
    Serial.println();
    Serial.println("=========================");
    Serial.println("  Perfect Compass v1.0");
    Serial.println("=========================");
    Serial.println();

    // Load saved configuration
    config.begin();

    // Initialize display first so we can show boot status
    display.begin();
    display.showMessage("Perfect Compass", "Initializing...");

    // Initialize sensors
    gps.begin();

    if (compass.begin()) {
        compass.setDeclination(config.getDeclination());
    }

    pointer.begin();

    // Start WiFi AP and web server
    web.begin(&config, &compass, &gps);

    display.showMessage("WiFi AP ready", config.getWiFiSSID());
    delay(1500);

    Serial.println("[Main] Setup complete");
}

void loop() {
    unsigned long now = millis();

    // Always feed the GPS parser
    gps.update();

    // Handle web requests
    web.handleClient();

    // Update sensors at fixed interval
    if (now - lastSensorUpdate >= SENSOR_INTERVAL_MS) {
        lastSensorUpdate = now;

        compass.update();

        // Calculate and update pointer direction
        if (config.hasTarget() && gps.hasfix()) {
            TargetLocation target = config.getTarget();
            GpsData gpsData = gps.getData();

            float bearing = Navigation::bearingTo(
                gpsData.latitude, gpsData.longitude,
                target.latitude, target.longitude
            );

            float relAngle = Navigation::relativeAngle(bearing, compass.getHeading());
            pointer.pointTo(relAngle);
        }

        pointer.update();
    }

    // Update display at lower rate
    if (now - lastDisplayUpdate >= DISPLAY_INTERVAL_MS) {
        lastDisplayUpdate = now;

        if (compass.isCalibrating()) {
            display.showCalibrating();
        } else if (!config.hasTarget()) {
            display.showMessage("No target set!", "Connect to WiFi AP");
        } else {
            GpsData gpsData = gps.getData();
            TargetLocation target = config.getTarget();

            DisplayData dd = {};
            dd.heading = compass.getHeading();
            dd.gpsValid = gps.hasfix();
            dd.compassReady = compass.isReady();
            dd.calibrating = compass.isCalibrating();
            dd.latitude = gpsData.latitude;
            dd.longitude = gpsData.longitude;
            dd.satellites = gpsData.satellites;
            dd.targetLat = target.latitude;
            dd.targetLon = target.longitude;
            dd.targetName = target.name;

            if (gps.hasfix()) {
                dd.bearingToTarget = Navigation::bearingTo(
                    gpsData.latitude, gpsData.longitude,
                    target.latitude, target.longitude
                );
                dd.distance = Navigation::distanceTo(
                    gpsData.latitude, gpsData.longitude,
                    target.latitude, target.longitude
                );
                dd.pointerAngle = pointer.getCurrentAngle();
            }

            display.show(dd);
        }
    }
}
