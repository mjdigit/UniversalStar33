# UniversalStar33

UniversalStar33 is a compact ortholinear keyboard with 33 keys and a TPS43
touchpad.

## Target layout

- 3 rows
- 5 columns on the left and right sides
- 3 center keys below the touchpad
- 33 keys total
- TPS43 touchpad
- FPC connector, resistors, and capacitor mounted on the back of the main PCB

## Current design status

The initial KiCad and QMK sources were migrated from
[Girasole40](https://github.com/mjdigit/Girasole40) as a reference baseline.
The migrated hardware is still the original 40-key/TPS65 design; the 33-key
layout and TPS43 conversion will be applied in subsequent revisions.

Open `assemble/UniversalStar33_Assemble.kicad_pro` in KiCad to inspect the
current main schematic and PCB.

Generated Gerbers, firmware binaries, KiCad local settings, and Girasole40
build photos are intentionally not included in this baseline migration.
