#include "pointer.h"
#include "pins.h"
#include <ESP32Servo.h>
#include <math.h>

static Servo servo;

void Pointer::begin() {
    servo.setPeriodHertz(50);
    servo.attach(SERVO_PIN, 500, 2400);  // min/max pulse width in microseconds
    servo.write(90);  // Center position
    _currentAngle = 0;
    _targetAngle = 0;
    Serial.println("[Pointer] Servo initialized");
}

void Pointer::pointTo(float angleDegrees) {
    // Normalize to 0-360
    while (angleDegrees < 0) angleDegrees += 360.0f;
    while (angleDegrees >= 360.0f) angleDegrees -= 360.0f;
    _targetAngle = angleDegrees;
}

void Pointer::update() {
    // Calculate shortest rotation path (handle 359->1 wrap-around)
    float diff = _targetAngle - _currentAngle;
    if (diff > 180.0f) diff -= 360.0f;
    if (diff < -180.0f) diff += 360.0f;

    // Apply smoothing
    _currentAngle += diff * SMOOTHING;

    // Normalize
    while (_currentAngle < 0) _currentAngle += 360.0f;
    while (_currentAngle >= 360.0f) _currentAngle -= 360.0f;

    // Map 0-360 compass degrees to 0-180 servo degrees
    // Standard servo has 180 degree range, so we map the full circle.
    // For a continuous rotation servo, you'd drive speed/direction instead.
    int servoAngle = (int)((_currentAngle / 360.0f) * 180.0f);
    servoAngle = constrain(servoAngle, 0, 180);
    servo.write(servoAngle);
}

float Pointer::getCurrentAngle() const {
    return _currentAngle;
}
