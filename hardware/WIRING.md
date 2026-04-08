# Wiring Guide

All electronics are hidden inside the compass case. Keep wires short and tidy.

## Pin Reference

```
ESP32 DevKit v1 (relevant pins)
================================

        3V3  ─┤          ├─ VIN (5V input)
        GND  ─┤          ├─ GND
       GP16  ─┤ (RX2)    ├─ GP25  ── Stepper IN4
       GP17  ─┤ (TX2)    ├─ GP23  ── Stepper IN3
  Stp1 GP18  ─┤          ├─ GP22  ── I2C SCL
  Stp2 GP19  ─┤  ESP32   ├─ GP21  ── I2C SDA
              ─┤          ├─
              ─┤          ├─
              ─┤          ├─
               └──────────┘
```

## Connections

### GPS Module (NEO-6M)

```
NEO-6M          ESP32
──────          ─────
VCC    ───────  3V3
GND    ───────  GND
TX     ───────  GPIO16 (RX2)
RX     ───────  GPIO17 (TX2)
```

Use UART2 so it doesn't conflict with USB serial. Position the GPS antenna flat under the top of the case lid, facing up.

### Magnetometer (HMC5883L / GY-271)

```
GY-271          ESP32
──────          ─────
VCC    ───────  3V3
GND    ───────  GND
SDA    ───────  GPIO21
SCL    ───────  GPIO22
DRDY   ───────  (not connected)
```

**Critical placement:** Mount the magnetometer as far as possible from the stepper motor — at least 3-4cm. The stepper has permanent magnets that will distort readings. Ideally, place the magnetometer near the top of the case and the stepper below it with some vertical separation.

### Stepper Motor (28BYJ-48 + ULN2003)

```
ULN2003 Driver     ESP32
──────────────     ─────
IN1   ───────────  GPIO18
IN2   ───────────  GPIO19
IN3   ───────────  GPIO23
IN4   ───────────  GPIO25
VCC   ───────────  5V (from boost converter)
GND   ───────────  GND
```

The ULN2003 board has a white JST connector for the stepper motor cable — it just plugs in. The driver board needs 5V to power the motor coils (3.3V is not enough torque).

## Complete Internal Wiring

```
                     ┌────────────┐
                     │  NEO-6M    │   (positioned under lid,
                     │  GPS       │    antenna facing up)
                     └──┬──┬──┬──┘
                        │  │  │
    ┌───────────────────┼──┼──┼─────────────────────┐
    │              3V3──┘  │  │                      │
    │              GND─────┘  │                      │
    │              GP16───────┘                      │
    │              GP17───(GPS RX, optional)         │
    │                                                │
    │    ESP32 DevKit (hidden in base)               │
    │                                                │
    │              GP21───────┐                      │
    │              GP22───────┼──┐                   │
    │                         │  │                   │
    │                    ┌────┴──┴────┐              │
    │                    │  GY-271    │ (mounted far │
    │                    │  Compass   │  from motor) │
    │                    └────────────┘              │
    │                                                │
    │  GP18──┐  GP19──┐  GP23──┐  GP25──┐           │
    └────────┼────────┼────────┼────────┼───────────┘
             │        │        │        │
        ┌────┴────────┴────────┴────────┴────┐
        │          ULN2003 Driver             │
        │  IN1    IN2    IN3    IN4    VCC GND│
        │                              │   │  │
        │         [JST connector]      5V  G  │
        │              │                      │
        │         ┌────┴────┐                 │
        │         │ 28BYJ-48│                 │
        │         │ Stepper │                 │
        │         └─────────┘                 │
        └─────────────────────────────────────┘

              ▲ stepper shaft goes UP through
                the compass face, needle
                attaches on top
```

## Power Wiring

```
     ┌──────────────┐
     │   TP4056      │◄── USB-C (hidden charging port
     │   Charger     │    on bottom or side of case)
     └──────┬───────┘
            │ B+/B-
     ┌──────┴───────┐
     │   LiPo       │   (flat cell tucked in base)
     │   Battery     │
     └──────┬───────┘
            │
     ┌──────┴───────┐
     │  Slide Switch │   (hidden on side/bottom)
     └──────┬───────┘
            │
     ┌──────┴───────┐
     │   MT3608      │   (set to 5V output)
     │   Boost       │
     └──────┬───────┘
            │ 5V
            ├──────────── ESP32 VIN pin
            └──────────── ULN2003 VCC
```

Set the MT3608 output to exactly **5.0V** with a multimeter before connecting anything.

## Physical Layout Inside the Case

```
        ┌──── clear dome/glass ────┐
        │                          │
    ════╪══════════════════════════╪════  ← compass face (printed dial)
        │    ┌──needle──┐          │
        │    └────┬─────┘          │
    ────┼─────────┼────────────────┼───  ← case divider / shelf
        │    ┌────┴─────┐  ┌──────┤
        │    │  28BYJ-48│  │GY-271│     ← magnetometer as far
        │    │  stepper  │  │      │       from motor as possible
        │    └──────────┘  └──────┤
        │  ┌────────┐ ┌────────┐  │
        │  │ ESP32  │ │ULN2003 │  │
        │  └────────┘ └────────┘  │
        │  ┌──────┐ ┌──────────┐  │
        │  │LiPo  │ │TP4056+   │  │
        │  │      │ │MT3608    │  │
        │  └──────┘ └──────────┘  │
    ════╪══════════════════════════╪════  ← bottom of case
        │  [switch]    [USB port] │
        └──────────────────────────┘
```

The stepper shaft pokes through the compass face. The needle sits on top, visible through the glass dome. Everything else is hidden below.

## Troubleshooting

| Problem | Check |
|---------|-------|
| Needle doesn't move | Check ULN2003 has 5V. Check stepper JST cable is seated. |
| Needle points wrong direction | Calibrate magnetometer via web UI. Check motor wiring order. |
| GPS no fix | GPS antenna must face up, not blocked by metal. Move outdoors. |
| Compass erratic near motor | Increase separation between magnetometer and stepper. Use a shim/spacer. |
| WiFi not visible | Check serial monitor via USB. Default AP: `PerfectCompass` / `compass123`. |
| Short battery life | Stepper coils auto-sleep after 5s idle. If still draining, check boost converter quiescent current. |
