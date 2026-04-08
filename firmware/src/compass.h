#pragma once

class CompassModule {
public:
    bool begin();
    void update();

    // Returns heading in degrees (0-360), where 0 = North
    float getHeading() const;
    bool isReady() const;

    // Calibration: call startCalibration(), rotate device slowly for ~30s,
    // then call endCalibration(). Offsets are saved to NVS.
    void startCalibration();
    void endCalibration();
    bool isCalibrating() const;

    void setDeclination(float degrees);

private:
    float _heading = 0;
    float _declination = 0;
    bool _ready = false;
    bool _calibrating = false;

    float _minX = 32767, _maxX = -32768;
    float _minY = 32767, _maxY = -32768;
    float _offsetX = 0, _offsetY = 0;

    void loadCalibration();
    void saveCalibration();
};
