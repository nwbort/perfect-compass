# Perfect Compass

A physical compass that points to any location you choose instead of magnetic north. Set a GPS coordinate — your home, a friend's house, a geocache — and the arrow always points that direction, no matter where you are or which way you're facing.

## How It Works

An ESP32 microcontroller reads your current position from a GPS module and your facing direction from a magnetometer. It calculates the bearing to your target and drives a servo motor to point a physical arrow in that direction. A small OLED display shows distance, heading, and satellite status. You configure the target coordinates over WiFi from your phone.

```
You are here        Target
     📍 ─────────── 🏠
     │
  Arrow points this way → 🏠
```

## Features

- **Physical pointing arrow** - servo-driven needle that tracks a target location in real time
- **GPS positioning** - knows where you are anywhere on Earth
- **Magnetometer heading** - knows which way the device is facing
- **WiFi configuration** - set target coordinates from any phone/laptop browser
- **OLED display** - shows distance, bearing, heading, and GPS status
- **Compass calibration** - built-in hard-iron calibration via the web interface
- **Magnetic declination** - adjustable correction for your region
- **Persistent storage** - target and calibration saved to flash, survives power cycles
- **Smooth animation** - pointer smoothly tracks the target without jerking

## Quick Start

### 1. Get the Parts (~$30)

See the full **[Parts List](hardware/PARTS_LIST.md)** — main components:
- ESP32 DevKit
- NEO-6M GPS module
- HMC5883L magnetometer (GY-271)
- SSD1306 0.96" OLED display
- SG90 servo motor

### 2. Wire It Up

Follow the **[Wiring Guide](hardware/WIRING.md)** to connect everything on a breadboard.

### 3. Flash the Firmware

```bash
cd firmware
pio run --target upload
```

### 4. Set Your Target

Connect to the `PerfectCompass` WiFi network (password: `compass123`), open `http://192.168.4.1`, and enter your target coordinates.

### 5. Go Outside

Take it outside, wait for GPS lock, and watch the arrow point to your target.

For detailed assembly instructions, see the **[Build Guide](hardware/BUILD_GUIDE.md)**.

## Project Structure

```
perfect-compass/
├── firmware/
│   ├── platformio.ini          # PlatformIO build configuration
│   └── src/
│       ├── main.cpp            # Application entry point and main loop
│       ├── pins.h              # Pin assignments and hardware constants
│       ├── gps.h / gps.cpp     # GPS module driver (NEO-6M via UART)
│       ├── compass.h / .cpp    # Magnetometer driver with calibration
│       ├── navigation.h / .cpp # Bearing and distance calculations
│       ├── pointer.h / .cpp    # Servo control with smooth animation
│       ├── display.h / .cpp    # OLED display with compass rose
│       ├── config.h / .cpp     # Persistent settings (NVS flash storage)
│       └── webserver.h / .cpp  # WiFi AP + web configuration interface
├── hardware/
│   ├── PARTS_LIST.md           # Components and where to buy them
│   ├── WIRING.md               # Connection diagram and pin reference
│   └── BUILD_GUIDE.md          # Step-by-step assembly instructions
└── README.md
```

## Requirements

- [PlatformIO](https://platformio.org/) (VS Code extension or CLI)
- USB cable for programming
- The components listed in the parts list

## License

MIT
