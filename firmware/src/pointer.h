#pragma once

class Pointer {
public:
    void begin();

    // Set the target angle (0-360 degrees, where 0 = top of compass / 12 o'clock)
    void pointTo(float angleDegrees);

    // Step toward the target angle. Call frequently from loop().
    // Returns true if the needle is still moving.
    bool update();

    // Disable stepper coils to save power when idle.
    // The 28BYJ-48 holds position via friction when coils are off.
    void sleep();
    void wake();

    float getCurrentAngle() const;

private:
    float _currentAngle = 0;
    float _targetAngle = 0;
    int _currentStep = 0;
    bool _sleeping = false;
    unsigned long _lastStepTime = 0;

    // Microseconds between steps — controls rotation speed.
    // Lower = faster. 1200us is smooth and quiet for 28BYJ-48.
    static constexpr unsigned long STEP_DELAY_US = 1200;

    // Dead zone in degrees — don't move for changes smaller than this
    // to prevent jittery oscillation from sensor noise.
    static constexpr float DEAD_ZONE = 1.5f;

    void stepCW();
    void stepCCW();
    void setCoils(int step);
    void coilsOff();
};
