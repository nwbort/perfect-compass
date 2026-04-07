#include "display.h"
#include "pins.h"
#include "navigation.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>

static Adafruit_SSD1306 oled(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);

bool Display::begin() {
    if (!oled.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDR)) {
        Serial.println("[Display] SSD1306 not found!");
        _ready = false;
        return false;
    }

    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(0, 0);
    oled.println("Perfect Compass");
    oled.println("Starting...");
    oled.display();
    _ready = true;
    Serial.println("[Display] OLED initialized");
    return true;
}

void Display::drawCompassRose(int cx, int cy, int radius, float heading, float bearingToTarget) {
    // Draw circle
    oled.drawCircle(cx, cy, radius, SSD1306_WHITE);

    // Draw N/S/E/W tick marks
    for (int i = 0; i < 4; i++) {
        float angle = (i * 90.0f - heading) * DEG_TO_RAD;
        int outerX = cx + (int)(sin(angle) * radius);
        int outerY = cy - (int)(cos(angle) * radius);
        int innerX = cx + (int)(sin(angle) * (radius - 3));
        int innerY = cy - (int)(cos(angle) * (radius - 3));
        oled.drawLine(innerX, innerY, outerX, outerY, SSD1306_WHITE);
    }

    // Draw arrow pointing to target (relative to current heading)
    float relAngle = (bearingToTarget - heading) * DEG_TO_RAD;
    int tipX = cx + (int)(sin(relAngle) * (radius - 2));
    int tipY = cy - (int)(cos(relAngle) * (radius - 2));
    oled.drawLine(cx, cy, tipX, tipY, SSD1306_WHITE);

    // Arrow head
    float leftAngle = relAngle - 0.3f;
    float rightAngle = relAngle + 0.3f;
    int headLen = radius / 3;
    int leftX = tipX - (int)(sin(leftAngle) * headLen);
    int leftY = tipY + (int)(cos(leftAngle) * headLen);
    int rightX = tipX - (int)(sin(rightAngle) * headLen);
    int rightY = tipY + (int)(cos(rightAngle) * headLen);
    oled.drawLine(tipX, tipY, leftX, leftY, SSD1306_WHITE);
    oled.drawLine(tipX, tipY, rightX, rightY, SSD1306_WHITE);
}

void Display::show(const DisplayData& data) {
    if (!_ready) return;

    oled.clearDisplay();

    // Left side: compass rose (46x46 area)
    drawCompassRose(23, 23, 20, data.heading, data.bearingToTarget);

    // Right side: info text
    oled.setTextSize(1);
    oled.setCursor(50, 0);
    if (data.targetName) {
        // Truncate long names
        char nameBuf[11];
        strncpy(nameBuf, data.targetName, 10);
        nameBuf[10] = '\0';
        oled.print(nameBuf);
    } else {
        oled.print("Target");
    }

    // Distance
    oled.setCursor(50, 12);
    char distBuf[16];
    Navigation::formatDistance(data.distance, distBuf, sizeof(distBuf));
    oled.print(distBuf);

    // Bearing
    oled.setCursor(50, 24);
    oled.printf("Brg: %.0f", data.bearingToTarget);
    oled.print((char)247);  // degree symbol

    // Heading
    oled.setCursor(50, 36);
    oled.printf("Hdg: %.0f", data.heading);
    oled.print((char)247);

    // Bottom row: GPS status
    oled.setCursor(0, 56);
    if (data.calibrating) {
        oled.print("CALIBRATING...");
    } else if (!data.gpsValid) {
        oled.printf("GPS: waiting (%d sat)", data.satellites);
    } else {
        oled.printf("GPS:%d sat  ", data.satellites);
        if (!data.compassReady) {
            oled.print("No mag!");
        }
    }

    oled.display();
}

void Display::showMessage(const char* line1, const char* line2) {
    if (!_ready) return;

    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setCursor(0, 20);
    oled.println(line1);
    if (line2) {
        oled.setCursor(0, 36);
        oled.println(line2);
    }
    oled.display();
}

void Display::showCalibrating() {
    if (!_ready) return;

    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setCursor(10, 10);
    oled.println("CALIBRATING");
    oled.setCursor(10, 28);
    oled.println("Rotate device");
    oled.setCursor(10, 40);
    oled.println("slowly in all");
    oled.setCursor(10, 52);
    oled.println("directions...");
    oled.display();
}
