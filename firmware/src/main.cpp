#include <Arduino.h>
#include "pins.h"
#include "gps.h"
#include "compass.h"
#include "navigation.h"
#include "pointer.h"
#include "config.h"
#include "webserver.h"

// ---- Module instances ----
static GpsModule gps;
static CompassModule compass;
static Pointer pointer;
static Config config;
static WebServer web;

// ---- Timing ----
static unsigned long lastSensorUpdate = 0;
static unsigned long lastIdleCheck = 0;
static const unsigned long SENSOR_INTERVAL_MS = 100;   // 10 Hz sensor reads
static const unsigned long IDLE_SLEEP_MS = 5000;       // Sleep stepper coils after 5s idle

static bool needleMoving = false;
static unsigned long lastMovedTime = 0;

void setup() {
    Serial.begin(115200);
    Serial.println();
    Serial.println("Perfect Compass v1.0");
    Serial.println();

    // Load saved configuration
    config.begin();

    // Initialize sensors
    gps.begin();

    if (compass.begin()) {
        compass.setDeclination(config.getDeclination());
    }

    pointer.begin();

    // Start WiFi AP and web server for phone-based configuration
    web.begin(&config, &compass, &gps);

    Serial.println("[Main] Setup complete — connect to WiFi to configure");
}

void loop() {
    unsigned long now = millis();

    // Always feed the GPS parser
    gps.update();

    // Handle web requests (phone configuration)
    web.handleClient();

    // Step the needle toward its target (called as often as possible for smooth motion)
    bool moved = pointer.update();
    if (moved) {
        lastMovedTime = now;
        if (!needleMoving) {
            needleMoving = true;
            pointer.wake();
        }
    }

    // Sleep stepper coils when idle to save power and reduce heat.
    // The 28BYJ-48 gear train has enough friction to hold position.
    if (needleMoving && (now - lastMovedTime > IDLE_SLEEP_MS)) {
        pointer.sleep();
        needleMoving = false;
    }

    // Update sensor readings and calculate bearing at fixed interval
    if (now - lastSensorUpdate >= SENSOR_INTERVAL_MS) {
        lastSensorUpdate = now;

        compass.update();

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
    }
}
