#pragma once

// ---- GPS Module (NEO-6M) ----
// Connects via UART2
#define GPS_RX_PIN 16  // ESP32 RX2 <- GPS TX
#define GPS_TX_PIN 17  // ESP32 TX2 -> GPS RX
#define GPS_BAUD 9600

// ---- Magnetometer (HMC5883L / QMC5883L) ----
// Connects via I2C (shared bus with OLED)
#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22

// ---- Servo Motor ----
#define SERVO_PIN 18

// ---- OLED Display (SSD1306 128x64) ----
// Uses same I2C bus as magnetometer
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_I2C_ADDR 0x3C

// ---- WiFi AP defaults ----
#define DEFAULT_AP_SSID "PerfectCompass"
#define DEFAULT_AP_PASS "compass123"
