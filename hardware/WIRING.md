# Wiring Guide

## Pin Reference

```
ESP32 DevKit v1 Pinout (relevant pins)
=========================================

                 ┌──────────┐
            3V3 ─┤          ├─ VIN (5V)
            GND ─┤          ├─ GND
           GP15 ─┤          ├─ GP13
           GP02 ─┤          ├─ GP12
           GP04 ─┤          ├─ GP14
      RX2  GP16 ─┤          ├─ GP27
      TX2  GP17 ─┤  ESP32   ├─ GP26
           GP05 ─┤          ├─ GP25
      SCK  GP18 ─┤          ├─ GP33
           GP19 ─┤          ├─ GP32
      SDA  GP21 ─┤          ├─ GP35
      RX0  GP03 ─┤          ├─ GP34
      TX0  GP01 ─┤          ├─ VN
      SCL  GP22 ─┤          ├─ VP
           GP23 ─┤          ├─ EN
                 └──────────┘
```

## Connection Diagram

### GPS Module (NEO-6M)

```
NEO-6M          ESP32
──────          ─────
VCC    ───────  3V3
GND    ───────  GND
TX     ───────  GPIO16 (RX2)
RX     ───────  GPIO17 (TX2)
```

The GPS module communicates via UART at 9600 baud. It uses the ESP32's hardware UART2 so it won't conflict with the USB serial (UART0).

### Magnetometer (HMC5883L / GY-271)

```
GY-271          ESP32
──────          ─────
VCC    ───────  3V3
GND    ───────  GND
SDA    ───────  GPIO21 (SDA)
SCL    ───────  GPIO22 (SCL)
DRDY   ───────  (not connected)
```

The magnetometer shares the I2C bus with the OLED display. The HMC5883L uses I2C address `0x1E`. The QMC5883L (common clone) uses `0x0D` - the library auto-detects.

### OLED Display (SSD1306 128x64)

```
SSD1306         ESP32
───────         ─────
VCC    ───────  3V3
GND    ───────  GND
SDA    ───────  GPIO21 (SDA)  ← shared with magnetometer
SCL    ───────  GPIO22 (SCL)  ← shared with magnetometer
```

The OLED uses I2C address `0x3C` (some modules use `0x3D` — check yours with an I2C scanner if it doesn't work).

### Servo Motor (SG90)

```
SG90            ESP32
────            ─────
Signal (orange/white) ─── GPIO18
VCC (red)      ──────────  5V (VIN pin or external 5V)
GND (brown/black) ───────  GND
```

**Important:** Power the servo from 5V, not 3.3V. The ESP32's VIN pin provides 5V when powered via USB. For battery operation, use the boost converter output. Place a **100uF capacitor** between the servo's VCC and GND to reduce voltage spikes.

## Full Wiring Diagram (Text)

```
                        ┌─────────────┐
                        │   NEO-6M    │
                        │   GPS       │
                        │  VCC GND TX RX
                        └──┬──┬──┬──┬─┘
                           │  │  │  │
    ┌──────────────────────┼──┼──┼──┼──────────────────┐
    │                 3V3──┘  │  │  │                   │
    │                 GND─────┘  │  │                   │
    │                 GP16───────┘  │                   │
    │                 GP17──────────┘                   │
    │                                                   │
    │    ESP32 DevKit                                   │
    │                                                   │
    │                 GP21──────┬──────────────┐        │
    │                 GP22──────┼──────┐       │        │
    │                           │      │       │        │
    │                      ┌────┴──┐ ┌─┴───────┴──┐    │
    │                      │GY-271 │ │  SSD1306    │    │
    │                      │ SDA   │ │  SDA  SCL   │    │
    │                      │ SCL   │ │  OLED       │    │
    │                      └───────┘ └─────────────┘    │
    │                                                   │
    │                 GP18─────────────┐                │
    │                 VIN──────┐       │                │
    │                 GND──┐   │  ┌────┴───┐           │
    │                      │   │  │ SG90   │           │
    │                      │   │  │ Servo  │           │
    │                      │   └──┤ VCC    │           │
    │                      └─────┤ GND    │           │
    │                            └────────┘            │
    └──────────────────────────────────────────────────┘
```

## Power Wiring (Battery Option)

```
                  ┌───────────┐
  USB-C ─────────►│  TP4056   │
                  │  Charger  │
                  └─────┬─────┘
                        │ B+/B-
                  ┌─────┴─────┐
                  │  18650    │
                  │  Battery  │
                  └─────┬─────┘
                        │ OUT+/OUT-
              ┌─────────┴─────────┐
     ┌────────┤    Slide Switch   │
     │        └───────────────────┘
     │
     ├──────────┐
     │   ┌──────┴──────┐
     │   │   MT3608    │
     │   │ Boost → 5V  │
     │   └──────┬──────┘
     │          │ 5V out
     │          ├────────── ESP32 VIN
     │          └────────── Servo VCC
     │
     └── GND ──────────── ESP32 GND, Servo GND
```

Adjust the MT3608 potentiometer to output exactly **5.0V** before connecting anything.

## I2C Bus Notes

Both the magnetometer and OLED share the same I2C bus (GPIO21/22). They work on different addresses so there's no conflict:
- HMC5883L: `0x1E`
- QMC5883L: `0x0D`
- SSD1306 OLED: `0x3C`

No external pull-up resistors are needed — the ESP32 has internal pull-ups, and most breakout boards include them.

## Troubleshooting

| Problem | Check |
|---------|-------|
| OLED blank | Verify I2C address (try `0x3D`). Check SDA/SCL not swapped. |
| GPS no fix | Move outdoors or near window. Cold start can take 1-5 minutes. |
| Compass erratic | Calibrate via web interface. Keep away from magnets/motors. |
| Servo jittering | Add 100uF cap. Ensure 5V supply can source 500mA+. |
| WiFi not visible | Check serial monitor for AP SSID. Default: `PerfectCompass`. |
