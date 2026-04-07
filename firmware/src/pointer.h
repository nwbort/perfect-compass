#pragma once

class Pointer {
public:
    void begin();

    // Set the pointer to a specific angle (0-360 degrees).
    // The servo maps 0-180 degrees of physical range to 0-360 degrees of compass.
    // For full 360 coverage, use a continuous rotation servo or stepper instead.
    void pointTo(float angleDegrees);

    // Smoothly animate toward the target angle to avoid jerky movement
    void update();

    float getCurrentAngle() const;

private:
    float _currentAngle = 0;
    float _targetAngle = 0;
    static constexpr float SMOOTHING = 0.15f;  // Lower = smoother but slower
};
