# Build Guide

How to build a compass that looks completely analog but points wherever you want.

## The Concept

From the outside, this looks like an ordinary compass — a needle behind glass in a round brass or wooden case. But instead of pointing north, it always points toward a GPS location you secretly configured from your phone. Nobody can tell it's not a normal compass just by looking at it.

All the electronics (ESP32, GPS, magnetometer, stepper motor, battery) are hidden inside the base, below the compass face.

## Prerequisites

- Soldering iron and thin solder
- Thin hookup wire (28-30 AWG)
- Hot glue gun or epoxy
- Small screwdrivers
- Computer with USB cable for initial firmware flash
- [PlatformIO](https://platformio.org/) installed (VS Code extension or CLI)
- All parts from the [Parts List](PARTS_LIST.md)

---

## Step 1: Flash the Firmware First

Do this before assembling anything — it's much easier to debug with the ESP32 accessible.

1. Connect the ESP32 to your computer via USB.
2. Open the `firmware/` directory in VS Code with PlatformIO, or use the CLI:

```bash
cd firmware
pio run --target upload
```

3. Open the serial monitor to verify it boots:

```bash
pio device monitor
```

You should see:
```
Perfect Compass v1.0

[Config] Target: ...
[GPS] Initialized on UART2
[Compass] HMC5883L initialized
[Pointer] Stepper initialized
[Web] AP started. IP: 192.168.4.1
[Web] HTTP server started on port 80
[Main] Setup complete
```

4. On your phone, connect to `PerfectCompass` WiFi (password: `compass123`) and open `http://192.168.4.1` to verify the web interface loads.

---

## Step 2: Prepare the Stepper + Needle Assembly

This is the most important part — it's what makes it look like a real compass.

### Mount the needle on the stepper shaft

1. The 28BYJ-48 has a small flat-sided shaft sticking out the top.
2. Your needle needs a small hub that press-fits or glues onto this shaft.
3. **Option A (easiest):** Glue the needle directly to the shaft with a tiny drop of epoxy. Make sure it's centered and balanced.
4. **Option B (removable):** 3D print a small friction-fit cap that goes over the shaft, with the needle attached.

### Balance the needle

An unbalanced needle will droop and look wrong. Test by holding the stepper upright — the needle should stay horizontal regardless of which direction it points. File or add a tiny counterweight if needed.

### Needle design tips

- Use thin brass, aluminum, or copper sheet (~0.5mm thick)
- A classic diamond or arrow shape, 40-50mm long
- Paint the north-indicating half red or black (to look authentic)
- Keep it lightweight — the stepper has limited torque

```
  ◄─── painted red ───►◄── bare metal ──►

         ╱╲
        ╱  ╲
       ╱    ╲
      ╱      ╲
     ▕        ▏━━━━━━━━━━━━━━━━◇
      ╲      ╱         tail
       ╲    ╱
        ╲  ╱
         ╲╱
        tip

     [shaft hole in center]
```

---

## Step 3: Prepare the Case

You need a round case with two "levels":
- **Top level** (visible): The compass face — needle, printed dial, glass dome
- **Bottom level** (hidden): All the electronics

### Option A: Repurpose a vintage compass or pocket watch case

1. Gut the original mechanism.
2. The existing glass/crystal is your dome. Keep it.
3. Drill a small hole in the center of the face plate for the stepper shaft.

### Option B: Build from a round tin or wooden box

1. Find a round container, 70-90mm diameter, at least 35mm deep.
2. Cut or find a clear disc (watch crystal, acrylic circle) for the top.
3. Create a divider shelf inside: a disc of thin plywood, acrylic, or cardboard with a center hole for the stepper shaft. This separates the visible compass face from the hidden electronics below.

### Option C: 3D print

Design a two-part round case:
- Bottom half: holds all electronics, has a center post for the stepper
- Top half: has a recessed area for the dial and a lip for the glass dome
- Print in wood-fill PLA or paint brass/gold for authenticity

---

## Step 4: Create the Compass Rose Dial

Print a compass rose on paper or cardstock that fits inside your case. This is the fixed background behind the needle.

The dial sits **fixed** (does not rotate). Only the needle moves.

```
              N
         NW       NE
       W     ✦      E
         SW       SE
              S
```

Tips:
- Search for "vintage compass rose" images and print one sized to your case
- Laser print on aged/tea-stained paper for a vintage look
- Cut a center hole for the stepper shaft
- Glue it to the divider shelf with the N marking aligned to the front of the device

**Important:** The firmware assumes the compass rose N marking is aligned with the magnetometer's forward axis. When you assemble everything, the magnetometer's X-axis (or "forward" direction printed on the breakout board) must point the same direction as the N on your dial.

---

## Step 5: Assemble the Electronics

Wire everything per the [Wiring Guide](WIRING.md). Work from the bottom up:

### Layer 1 (bottom of case): Power

1. Attach the LiPo battery flat against the bottom.
2. Mount the TP4056 charger module so the USB port is accessible through a small hole in the case side or bottom.
3. Wire: Battery → TP4056 → slide switch → MT3608 boost converter.
4. Adjust MT3608 to output 5.0V (measure with multimeter before connecting anything else).

### Layer 2: Main electronics

1. Mount the ESP32 and ULN2003 driver board with hot glue, pads facing up.
2. Wire: MT3608 5V → ESP32 VIN + ULN2003 VCC. Share ground.
3. Wire the GPS module (VCC, GND, TX→GPIO16, RX→GPIO17). Position the GPS antenna facing upward, near the top of the case.
4. Wire the magnetometer (VCC, GND, SDA→GPIO21, SCL→GPIO22). Mount it as far from the stepper as possible — ideally at the edge of the case, oriented so its X-axis arrow points toward where N will be on your dial.
5. Wire the ULN2003 inputs (IN1→GPIO18, IN2→GPIO19, IN3→GPIO23, IN4→GPIO25).

### Layer 3 (top): Stepper and compass face

1. Mount the stepper motor pointing upward so its shaft pokes through the center hole of the divider shelf.
2. Place the compass rose dial on the shelf.
3. Attach the needle to the stepper shaft.
4. Place the glass dome on top.

---

## Step 6: Configure

1. Turn on the power switch.
2. On your phone, connect to WiFi: **PerfectCompass** / **compass123**
3. Open `http://192.168.4.1`
4. Set your target location (latitude/longitude and a name).
5. Set the magnetic declination for your area — look it up at NOAA's [declination calculator](https://www.ngdc.noaa.gov/geomag/declination.shtml).
6. Calibrate the magnetometer:
   - Tap **Start Compass Calibration**
   - Slowly rotate the entire compass in all directions for 30 seconds
   - Tap **Stop Calibration**

---

## Step 7: Test

1. Take the compass **outside** where GPS can get a fix.
2. Wait 1-5 minutes for the first GPS lock (the web status page shows satellite count).
3. Once locked, the needle should begin pointing toward your target.
4. Rotate your body — the needle should maintain its direction, always tracking the target.
5. Walk around — the needle should gradually shift as your position changes relative to the target.

---

## How It Works (For the Curious)

```
GPS: "You are at 40.71°N, 74.00°W"
                │
                ├──► Math: "Target is 73° bearing from you"
                │
Magnetometer: "Device faces 120°"
                │
                └──► 73° - 120° = needle points 313° (relative)
                                        │
                                   Stepper rotates
                                   needle to 313°
```

The system updates 10 times per second. The stepper moves in tiny half-steps (2048 per revolution = 0.18° precision) making the needle movement completely smooth and silent.

When the compass is sitting still and the needle isn't moving, the stepper coils automatically turn off after 5 seconds to save battery. The gear reduction in the 28BYJ-48 has enough friction to hold the needle in place without power.

---

## Tips for Making It Convincing

- **Aging the case:** Tea-stain paper, use brass/copper patina solution, add fake wear marks
- **The needle should look traditional:** Diamond shape, red-tipped, slightly tarnished metal
- **Add liquid?** Real compasses have fluid to dampen the needle. You *could* fill the top chamber with clear mineral oil — the stepper is strong enough to drive through it, and it makes the needle movement look incredibly authentic. Seal well with silicone.
- **Engraving:** Engrave or etch initials, a date, or coordinates on the case back
- **Hide the USB port:** Use a small rubber plug over the charging port. Charge it when nobody's looking.
- **The "power switch":** Hide it as a decorative element — a rotating bezel ring, a small crown like a pocket watch, or a magnetic reed switch activated by an external magnet so there's no visible switch at all
