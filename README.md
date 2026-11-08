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

The KiCad schematic, main PCB key array and outline, top/bottom plates, QMK
layout, and KLE layout have been converted to the 33-key arrangement. The
logical matrix remains 4 rows by 10 columns; its fourth matrix row is used
only by the three center keys, avoiding any GPIO reassignment. FPC1, R1, R2, and C1 are mounted on the back above the center keys.

The TPS43 mount plate, four-point attachment, back-side FPC interface, and
QMK TPS43 configuration are now implemented. The enclosure and final
manufacturing review still require completion.

Open `assemble/UniversalStar33_Assemble.kicad_pro` in KiCad to inspect the
current main schematic and PCB.

Generated Gerbers, firmware binaries, KiCad local settings, and Girasole40
build photos are intentionally not included in this baseline migration.
