#pragma once

// ---- GPS Module (NEO-6M) ----
// Connects via UART2
#define GPS_RX_PIN 16  // ESP32 RX2 <- GPS TX
#define GPS_TX_PIN 17  // ESP32 TX2 -> GPS RX
#define GPS_BAUD 9600

// ---- Magnetometer (HMC5883L / QMC5883L) ----
// Connects via I2C
#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22

// ---- Stepper Motor (28BYJ-48 via ULN2003) ----
// 4-pin control through ULN2003 driver board
#define STEPPER_PIN_1 18
#define STEPPER_PIN_2 19
#define STEPPER_PIN_3 23
#define STEPPER_PIN_4 25

// 28BYJ-48: 2048 half-steps per full revolution (with gear reduction)
#define STEPS_PER_REVOLUTION 2048

// ---- WiFi AP defaults ----
#define DEFAULT_AP_SSID "PerfectCompass"
#define DEFAULT_AP_PASS "compass123"
