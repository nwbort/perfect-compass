# Parts List

## Core Electronics

| # | Component | Description | Qty | Approx. Cost |
|---|-----------|-------------|-----|-------------|
| 1 | **ESP32 DevKit v1** | Microcontroller with WiFi/BLE. 38-pin version recommended. | 1 | $5-8 |
| 2 | **NEO-6M GPS Module** | GPS receiver with built-in antenna. Provides lat/lon position. | 1 | $5-10 |
| 3 | **HMC5883L Compass Module** | 3-axis digital magnetometer (I2C). GY-271 breakout board. Alternatively, use QMC5883L (pin-compatible). | 1 | $2-4 |
| 4 | **SSD1306 OLED Display** | 0.96" 128x64 pixel I2C OLED display. | 1 | $3-5 |
| 5 | **SG90 Micro Servo** | Standard 180-degree hobby servo. For full 360 rotation, use a **continuous rotation servo** (e.g., FS90R). | 1 | $2-4 |

## Power

| # | Component | Description | Qty | Approx. Cost |
|---|-----------|-------------|-----|-------------|
| 6 | **18650 Li-Ion Battery** | 3.7V rechargeable battery. | 1 | $3-5 |
| 7 | **TP4056 Charge Module** | Li-Ion battery charger with USB-C/micro-USB input and over-discharge protection. | 1 | $1-2 |
| 8 | **MT3608 Boost Converter** | Adjustable step-up converter. Set output to 5V to power ESP32 and servo. | 1 | $1-2 |

*Alternative: Power directly via USB from a power bank. Skip items 6-8 if using USB power.*

## Mechanical

| # | Component | Description | Qty | Approx. Cost |
|---|-----------|-------------|-----|-------------|
| 9 | **Arrow/Needle** | 3D printed or cut from thin material (acrylic, cardboard). ~60mm long. Attaches to the servo horn. | 1 | - |
| 10 | **Enclosure** | 3D printed case, project box (~100x80x40mm), or any container. | 1 | $2-5 |
| 11 | **Servo Horn** | Usually included with servo. Cross or single-arm style. | 1 | (included) |

## Wiring & Misc

| # | Component | Description | Qty | Approx. Cost |
|---|-----------|-------------|-----|-------------|
| 12 | **Breadboard** | Half-size breadboard for prototyping. | 1 | $2 |
| 13 | **Jumper Wires** | Male-to-female and male-to-male dupont wires. | ~20 | $2 |
| 14 | **Slide Switch** | SPST power switch. | 1 | $0.50 |
| 15 | **Capacitor 100uF** | Electrolytic, across servo power to reduce noise. | 1 | $0.10 |

## Optional Upgrades

| Component | Why |
|-----------|-----|
| **28BYJ-48 Stepper + ULN2003 Driver** | Replace servo for true 360-degree rotation with precise positioning. |
| **BME280 Sensor** | Add temperature/pressure/humidity to the display. |
| **Larger OLED (1.3")** | More readable display. Same I2C interface. |
| **3D Printed Dial Face** | Printed compass rose that sits around the arrow for visual reference. |

## Total Estimated Cost

**$25-45** depending on where you source parts. All components are widely available from Amazon, AliExpress, Adafruit, or SparkFun.

## Where to Buy

- **AliExpress** - Cheapest, 2-4 week shipping
- **Amazon** - Fast shipping, slightly higher prices
- **Adafruit** (adafruit.com) - High quality, great documentation
- **SparkFun** (sparkfun.com) - Quality breakout boards
- **LCSC / JLCPCB** - For PCB fabrication if you design a custom board
