#include "pointer.h"
#include "pins.h"
#include <Arduino.h>

// Half-step sequence for 28BYJ-48 — smoother and quieter than full-step.
// Each entry is {pin1, pin2, pin3, pin4} HIGH/LOW state.
static const uint8_t HALF_STEP_SEQ[8][4] = {
    {1, 0, 0, 0},
    {1, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 1, 0},
    {0, 0, 1, 1},
    {0, 0, 0, 1},
    {1, 0, 0, 1},
};

static const int COIL_PINS[4] = {
    STEPPER_PIN_1, STEPPER_PIN_2, STEPPER_PIN_3, STEPPER_PIN_4
};

void Pointer::begin() {
    for (int i = 0; i < 4; i++) {
        pinMode(COIL_PINS[i], OUTPUT);
        digitalWrite(COIL_PINS[i], LOW);
    }
    _currentStep = 0;
    _currentAngle = 0;
    _targetAngle = 0;
    _sleeping = false;
    Serial.println("[Pointer] Stepper initialized");
}

void Pointer::setCoils(int step) {
    int idx = step & 7;  // mod 8
    for (int i = 0; i < 4; i++) {
        digitalWrite(COIL_PINS[i], HALF_STEP_SEQ[idx][i]);
    }
}

void Pointer::coilsOff() {
    for (int i = 0; i < 4; i++) {
        digitalWrite(COIL_PINS[i], LOW);
    }
}

void Pointer::stepCW() {
    _currentStep++;
    if (_currentStep >= STEPS_PER_REVOLUTION) _currentStep = 0;
    setCoils(_currentStep);
}

void Pointer::stepCCW() {
    _currentStep--;
    if (_currentStep < 0) _currentStep = STEPS_PER_REVOLUTION - 1;
    setCoils(_currentStep);
}

void Pointer::pointTo(float angleDegrees) {
    // Normalize to 0-360
    while (angleDegrees < 0) angleDegrees += 360.0f;
    while (angleDegrees >= 360.0f) angleDegrees -= 360.0f;
    _targetAngle = angleDegrees;
}

bool Pointer::update() {
    if (_sleeping) return false;

    // Calculate shortest rotation direction
    float diff = _targetAngle - _currentAngle;
    if (diff > 180.0f) diff -= 360.0f;
    if (diff < -180.0f) diff += 360.0f;

    // Dead zone — don't move for tiny changes
    if (fabs(diff) < DEAD_ZONE) return false;

    // Rate limit steps
    unsigned long now = micros();
    if (now - _lastStepTime < STEP_DELAY_US) return true;  // still moving, just waiting
    _lastStepTime = now;

    // Take one step in the shortest direction
    if (diff > 0) {
        stepCW();
        _currentAngle += 360.0f / STEPS_PER_REVOLUTION;
    } else {
        stepCCW();
        _currentAngle -= 360.0f / STEPS_PER_REVOLUTION;
    }

    // Normalize
    while (_currentAngle < 0) _currentAngle += 360.0f;
    while (_currentAngle >= 360.0f) _currentAngle -= 360.0f;

    return true;
}

void Pointer::sleep() {
    coilsOff();
    _sleeping = true;
}

void Pointer::wake() {
    _sleeping = false;
    setCoils(_currentStep);
}

float Pointer::getCurrentAngle() const {
    return _currentAngle;
}
