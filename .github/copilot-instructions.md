# KiCad schematic and design-rule guidance

Apply these conventions when creating, editing, reviewing, or migrating KiCad schematics and board design rules.

## Schematic architecture

- Keep the root schematic primarily a block diagram. Place only system-level items there, such as mounting holes, fiducials, or main connectors; put circuit blocks on child sheets.
- Design each child sheet as a self-contained, reusable module. Avoid requiring internal component changes when reusing a sheet in another project.

## Labels and signal scope

- Local labels are confined to a sheet. Never use them to connect signals across sheets.
- Use hierarchical labels on child-sheet signals that cross the sheet boundary, with matching hierarchical sheet pins on the parent sheet.
- Prefer hierarchical connections for ordinary signals. Reserve global labels for genuinely system-wide nets, principally power distribution.
- Keep power symbols and global power labels (such as VCC and GND) for power nets; do not use global scope as a substitute for ordinary signal routing.

## Flat-to-hierarchical migration

When helping convert a flat circuit block into a hierarchical sub-sheet, guide the user through these steps in order:

1. Create a hierarchical sheet block on the root schematic.
2. Cut the targeted circuit block from the flat schematic.
3. Enter the new child sheet.
4. Paste the circuit block into the child sheet.
5. Replace connections that cross the sheet boundary with hierarchical labels.
6. Return to the root sheet, import the child sheet's pins onto its sheet symbol, and connect those pins to the rest of the design.

Check that every boundary signal has a matching hierarchical label and sheet pin, and that internal-only signals remain local.

## Net classes and custom design rules

- Assign net classes with net-class directive labels on the relevant nets, or configure hierarchical net-class rules in **Board Setup > Design Rules** when appropriate.
- When generating or editing `.kicad_dru` files, use KiCad's valid S-expression syntax and verify rule scopes, clearances, and track widths against the intended nets and classes. Do not invent or assume syntax when the KiCad version or requirements are unclear.
