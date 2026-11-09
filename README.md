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
QMK TPS43 configuration are now implemented.

The complete key array has been lowered by 4.0 mm to clear the TPS43 mount
plate. The main PCB, top plate, and bottom plate are 62.85 mm tall. The top
key-opening bezel remains 7.0 mm while the lower bezel has been reduced to
3.5 mm. Both five-column key blocks now use uniform 19.05 mm pitch with no
inter-column gaps; their outer four columns were moved 3.0 mm inward to meet
the touchpad-adjacent columns. SW20 is adjacent to SW19, SW29 is adjacent to
SW24, and SW25 is centered at x = 148.5 mm. The two gaps around SW25 are
intentional.

The top plate's checker-pattern copper keepout is limited to the narrow left
and right edge strips; obsolete cells below the inner key area have been
removed. The solid copper fill and F.Mask frames are centered on all 33 key
cutouts. The central opening is symmetric about x = 148.5 mm and widens to
x = 121.9--175.1 mm below y = 56.1 mm, clearing the two lower touchpad-mount
screw holes and the back-side FPC connector while retaining a 2.575 mm web
above the bottom-row key openings.

The Base touchpad-mount holes match the Touchpad plate at (124.0, 21.6),
(173.0, 21.6), (124.0, 57.6), and (173.0, 57.6) mm. The widened Top opening
retains 1.0 mm radial clearance around each lower 2.2 mm mounting hole.

An 8.0 x 3.5 mm, 45-degree cable slot carries the TPS43 flat cable from the
front to the back of the main PCB. FPC1 is on the back at (132.5, 54.5),
rotated to 315 degrees (shown as -45 degrees in KiCad), so its terminal face
points lower-left. The cable-slot center is at (137.5, 49.5), 5 mm left and
5 mm below its previous position. The I2C
traces are 0.30 mm wide, the
3V3/GND controller trunks are 0.50 mm wide, and all matrix and touchpad
routes use 90-degree-or-greater junctions. The power return is kept separate
from SDA/SCL around the touchpad interface.

The enclosure, physical FPC bend/length check, noise validation on assembled
hardware, and final manufacturing review still require completion.

Open `assemble/UniversalStar33_Assemble.kicad_pro` in KiCad to inspect the
current main schematic and PCB.

Generated Gerbers, firmware binaries, KiCad local settings, and Girasole40
build photos are intentionally not included in this baseline migration.
