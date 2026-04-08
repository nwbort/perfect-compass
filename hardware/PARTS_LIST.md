# Parts List

Everything you need to build a compass that looks analog but secretly points to your target.

## Core Electronics (Hidden Inside)

| # | Component | Description | Qty | Approx. Cost |
|---|-----------|-------------|-----|-------------|
| 1 | **ESP32 DevKit v1** | Microcontroller with WiFi for phone-based configuration. | 1 | $5-8 |
| 2 | **NEO-6M GPS Module** | GPS receiver. Get the version with a detached antenna on a wire so you can position it under the lid. | 1 | $5-10 |
| 3 | **HMC5883L Compass Module** | 3-axis magnetometer (GY-271 breakout). Detects which way the device is facing. | 1 | $2-4 |
| 4 | **28BYJ-48 Stepper Motor** | Tiny geared stepper motor. 2048 steps/revolution gives smooth, precise, silent needle movement. Full 360-degree rotation. | 1 | $2-3 |
| 5 | **ULN2003 Driver Board** | Stepper motor driver. Usually sold bundled with the 28BYJ-48. | 1 | (included with motor) |

## Power

| # | Component | Description | Qty | Approx. Cost |
|---|-----------|-------------|-----|-------------|
| 6 | **3.7V LiPo Battery** | Flat LiPo cell, 500-1000mAh. Choose the largest that fits your enclosure. | 1 | $3-5 |
| 7 | **TP4056 Charge Module** | Tiny Li-Ion charger with micro-USB/USB-C input. Get the version with over-discharge protection. | 1 | $1-2 |
| 8 | **MT3608 Boost Converter** | Step-up to 5V for the ESP32 and stepper. Adjustable — set to 5V before connecting. | 1 | $1-2 |

## The Compass Housing

| # | Component | Description | Qty | Approx. Cost |
|---|-----------|-------------|-----|-------------|
| 9 | **Round enclosure** | A brass or wooden round case, ~70-90mm diameter. Pocket watch cases, vintage compass cases, or round tins work perfectly. | 1 | $3-10 |
| 10 | **Clear dome/lens** | Glass or acrylic disc to cover the compass face. A watch crystal, magnifying lens, or clear epoxy dome. | 1 | $2-5 |
| 11 | **Compass rose dial** | Printed paper or engraved disc with N/S/E/W markings. Sits fixed inside the case around the needle. | 1 | (print yourself) |
| 12 | **Needle** | Thin, lightweight pointer. A balanced brass or aluminum arrow, ~40-50mm. Can be cut from sheet metal or 3D printed thin. | 1 | (make yourself) |
| 13 | **Shaft coupler** | Connects the needle to the stepper shaft. A small press-fit cap or drop of epoxy on the stepper's flat shaft. | 1 | - |

## Wiring & Misc

| # | Component | Description | Qty | Approx. Cost |
|---|-----------|-------------|-----|-------------|
| 14 | **Thin hookup wire** | 28-30 AWG silicone wire for compact internal wiring. | ~1m | $1 |
| 15 | **Slide switch** | Tiny SPST power switch, hidden on the side or bottom of the case. | 1 | $0.50 |
| 16 | **Hot glue / epoxy** | For securing components inside the case. | - | $2 |

## Total Estimated Cost

**$25-45** depending on the case you choose. All electronics are widely available.

## Where to Buy

- **AliExpress** — cheapest, 2-4 week shipping
- **Amazon** — fast shipping, slightly higher prices
- **Adafruit / SparkFun** — quality breakouts with good documentation
- **Etsy / antique shops** — for authentic-looking compass cases
- **Thrift stores** — old pocket watches or compass cases to gut and repurpose
