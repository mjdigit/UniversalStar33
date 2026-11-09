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
3.5 mm. The Base outline is 252.0 mm wide (x = 22.5–274.5 mm), while the Top
and Bottom outlines are 262.0 mm wide (x = 17.5–279.5 mm). This removes 4.0
mm from each side while retaining approximately 1.1 mm around the outermost
switch footprints. Both five-column key blocks now use uniform 19.05 mm pitch with no
inter-column gaps. The left block was shifted 1.75 mm right and the right
block 1.7755 mm left, making SW19, SW20, SW25, SW29, and SW24 a continuous
19.05 mm-pitch row centered on SW25 at x = 148.5 mm. The horizontal clearance
between the Touchpad plate and each adjacent key opening is 4.55 mm.

The top plate's checker-pattern copper keepout is limited to the narrow left
and right edge strips; obsolete cells below the inner key area have been
removed. The solid copper fill and F.Mask frames are centered on all 33 key
cutouts. The central opening is symmetric about x = 148.5 mm, with side edges
at x = 122.0/175.0 mm and its lower edge at y = 59.6 mm. The overlapping side
ears remain 3.0 mm deep. The Touchpad plate is positioned independently above
the Top plate with spacer clearance.

The Touchpad plate is shifted 2.0 mm downward and its upper edge is shortened
by a further 2.0 mm, giving an outer y range of 23.6–61.6 mm. The Base
touchpad-mount holes match the Touchpad plate at (124.0, 25.6),
(173.0, 25.6), (124.0, 59.6), and (173.0, 59.6) mm.

Two additional case fasteners are located symmetrically in the Top plate ears
at (122.0, 48.25) and (175.0, 48.25) mm. The Top and Bottom plates use 2.2 mm
M2 screw holes, while the Base uses 4.4 mm spacer-through holes at the same
centers. The Touchpad plate intentionally has no matching through-holes because
it is separated from the Top plate by spacers.

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
