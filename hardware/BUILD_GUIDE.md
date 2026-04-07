# Build Guide

Step-by-step instructions to build your Perfect Compass.

## Prerequisites

- Soldering iron (for header pins if not pre-soldered)
- Computer with USB cable for programming
- [PlatformIO](https://platformio.org/) installed (VS Code extension or CLI)
- All parts from the [Parts List](PARTS_LIST.md)

## Step 1: Prepare the Components

Most modules come with header pins that may need soldering.

1. **ESP32 DevKit** - Usually comes with pins pre-soldered. If not, solder the two rows of header pins.
2. **GPS module** - Solder the 4-pin header if needed (VCC, GND, TX, RX).
3. **Magnetometer** - Solder the 5-pin header (VCC, GND, SDA, SCL, DRDY).
4. **OLED display** - Solder the 4-pin header (VCC, GND, SDA, SCL).

## Step 2: Breadboard Assembly

Place the ESP32 on the breadboard straddling the center channel. Then wire each module following the [Wiring Guide](WIRING.md).

**Recommended order:**

1. **Power rails** - Connect ESP32 3V3 to the breadboard + rail and GND to the - rail.
2. **I2C bus** - Connect GPIO21 (SDA) and GPIO22 (SCL) to two shared rows. Both the magnetometer and OLED will connect to these same rows.
3. **OLED display** - Connect VCC→3V3, GND→GND, SDA→GPIO21 row, SCL→GPIO22 row.
4. **Magnetometer** - Connect VCC→3V3, GND→GND, SDA→GPIO21 row, SCL→GPIO22 row. Leave DRDY unconnected.
5. **GPS module** - Connect VCC→3V3, GND→GND, TX→GPIO16, RX→GPIO17.
6. **Servo** - Connect signal→GPIO18, VCC→5V (ESP32 VIN pin), GND→GND. Add the 100uF capacitor across the servo's power pins.

## Step 3: Flash the Firmware

1. Connect the ESP32 to your computer via USB.
2. Open the `firmware/` directory in VS Code with PlatformIO, or use the CLI:

```bash
cd firmware
pio run --target upload
```

3. Open the serial monitor to verify boot messages:

```bash
pio device monitor
```

You should see:
```
=========================
  Perfect Compass v1.0
=========================

[Config] Target: ...
[Display] OLED initialized
[GPS] Initialized on UART2
[Compass] HMC5883L initialized
[Pointer] Servo initialized
[Web] AP started. IP: 192.168.4.1
[Web] HTTP server started on port 80
[Main] Setup complete
```

## Step 4: Configure via WiFi

1. On your phone/laptop, connect to the WiFi network:
   - **SSID:** `PerfectCompass`
   - **Password:** `compass123`
2. Open a browser and go to `http://192.168.4.1`
3. Enter your target coordinates (latitude, longitude) and a name.
4. Click **Save Target**.
5. Optionally set the magnetic declination for your area. Find yours at [NOAA Declination Calculator](https://www.ngdc.noaa.gov/geomag/declination.shtml).

## Step 5: Calibrate the Compass

The magnetometer needs calibration to account for local magnetic interference.

1. In the web interface, click **Start Compass Calibration**.
2. Slowly rotate the entire device in all three axes (pitch, roll, yaw) for about 30 seconds. Try to cover every orientation.
3. Click **Stop Calibration**. The offsets are saved automatically.

You should recalibrate if:
- You move the device to a significantly different location
- You change the enclosure or nearby components
- Readings seem consistently off

## Step 6: Build the Enclosure

### Option A: 3D Printed Case

Design a case with:
- Mounting holes for the ESP32 (hole spacing: 2.54mm grid)
- Window/cutout for the OLED display
- Top-mounted servo with the arrow visible through an opening
- GPS antenna facing up (not blocked by metal)
- Access hole for USB charging/programming

### Option B: Project Box

1. Get a plastic project box (~100x80x40mm).
2. Cut a rectangular hole for the OLED display.
3. Drill a hole in the top for the servo shaft.
4. Mount the servo with hot glue or screws so the shaft protrudes through the top.
5. Drill a small hole for the USB cable.

### The Arrow/Needle

1. Cut an arrow shape from thin acrylic, cardboard, or 3D print one.
2. The arrow should be about 50-60mm long and lightweight.
3. Attach it to the servo horn with a small screw or hot glue.
4. Mark the pointing end clearly (paint it red, add an arrowhead, etc.).

```
         ▲
        ╱ ╲
       ╱   ╲
      ╱     ╲
     ▕       ▏
     ▕       ▏
      ╲     ╱
       ╲   ╱
        ─┬─
         │
     [servo horn]
```

## Step 7: Test and Use

1. Take the compass **outside** where the GPS can get a satellite fix.
2. Wait for the GPS to acquire satellites (1-5 minutes for first fix). The OLED will show satellite count.
3. Once the GPS has a fix, the arrow will begin pointing toward your target location.
4. As you rotate your body (and the compass), the arrow maintains its direction toward the target.

## How It Works

```
Current GPS Position ──┐
                       ├──► Calculate Bearing ──┐
Target Coordinates ────┘                        │
                                                ├──► Relative Angle ──► Servo
Magnetometer Heading ───────────────────────────┘
```

1. **GPS** provides your current latitude/longitude.
2. **Navigation math** calculates the bearing (compass direction) from you to the target using the [Haversine formula](https://en.wikipedia.org/wiki/Haversine_formula).
3. **Magnetometer** reads which direction the device is currently facing (heading).
4. **Relative angle** = bearing to target minus current heading. This is the direction the arrow needs to point.
5. **Servo** rotates the physical arrow to that angle.

The system updates 20 times per second, so the arrow smoothly tracks the target as you move and rotate.

## Tips

- **Accuracy**: Consumer GPS is accurate to ~3 meters. The magnetometer is accurate to ~2 degrees. Don't expect pinpoint precision.
- **Interference**: Keep the magnetometer away from the servo motor and battery. Even 2-3cm of separation helps significantly.
- **Servo limitations**: A standard 180-degree servo only covers half the compass. For full 360 coverage, use a continuous rotation servo or a stepper motor (28BYJ-48 + ULN2003).
- **Indoor use**: GPS won't work well indoors. The compass will still show heading, but position won't update.
- **Battery life**: Expect 4-8 hours on an 18650, depending on GPS update rate and display brightness.
