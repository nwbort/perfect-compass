# Perfect Compass

A compass that looks completely analog but secretly points to any GPS location you choose instead of magnetic north.

From the outside it's indistinguishable from a normal compass — a needle behind glass in a round case. But the needle always points toward your target: a friend's house, a geocache, a secret spot. Configure the target coordinates over WiFi from your phone. Nobody can tell it isn't a real compass just by looking at it.

## How It Works

A stepper motor hidden in the base silently rotates the needle. An ESP32 microcontroller reads your position from a GPS module and your facing direction from a magnetometer, calculates the bearing to your target, and drives the needle to point that way. As you move and turn, the needle tracks the target in real time.

All electronics are concealed inside the case. The only visible parts are the needle, a printed compass rose, and a glass dome — just like a real compass.

## Features

- **Looks completely analog** — no screens, LEDs, or visible electronics
- **Stepper-driven needle** — 2048 steps/revolution for smooth, silent, precise movement
- **Full 360-degree rotation** — works no matter which direction the target is
- **GPS positioning** — knows where you are anywhere on Earth
- **Magnetometer heading** — knows which way the device is facing
- **Phone configuration** — connect to the compass WiFi, set coordinates in a browser
- **Auto power saving** — stepper coils turn off when idle, gear friction holds the needle
- **Persistent memory** — target and calibration survive power cycles

## Quick Start

### 1. Get the Parts (~$30)

See the **[Parts List](hardware/PARTS_LIST.md)** — the main components:
- ESP32 DevKit
- NEO-6M GPS module
- HMC5883L magnetometer (GY-271)
- 28BYJ-48 stepper motor + ULN2003 driver
- Round case, glass dome, compass rose dial, needle

### 2. Wire It Up

Follow the **[Wiring Guide](hardware/WIRING.md)** — everything connects to the ESP32 hidden inside the case.

### 3. Flash the Firmware

```bash
cd firmware
pio run --target upload
```

### 4. Set Your Target

Connect your phone to the `PerfectCompass` WiFi (password: `compass123`), open `http://192.168.4.1`, and enter target coordinates.

### 5. Go Outside

Take it outdoors, wait for GPS lock, and watch the needle point to your target.

Full step-by-step assembly in the **[Build Guide](hardware/BUILD_GUIDE.md)**.

## Project Structure

```
perfect-compass/
├── firmware/
│   ├── platformio.ini          # PlatformIO build config
│   └── src/
│       ├── main.cpp            # Main loop
│       ├── pins.h              # Pin assignments
│       ├── gps.h / gps.cpp     # GPS module (NEO-6M via UART)
│       ├── compass.h / .cpp    # Magnetometer with calibration
│       ├── navigation.h / .cpp # Bearing and distance math
│       ├── pointer.h / .cpp    # Stepper motor control
│       ├── config.h / .cpp     # Persistent settings (NVS)
│       └── webserver.h / .cpp  # WiFi AP + phone config page
├── hardware/
│   ├── PARTS_LIST.md           # What to buy
│   ├── WIRING.md               # How to connect it
│   └── BUILD_GUIDE.md          # How to build and disguise it
└── README.md
```

## Requirements

- [PlatformIO](https://platformio.org/) (VS Code extension or CLI)
- USB cable for initial firmware flash
- Components from the parts list
- A round case to hide everything in

## License

MIT
