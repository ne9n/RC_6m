import uuid, re, sys

def add_symbols_to_sch(filepath, components, project_name):
    try:
        with open(filepath, 'r') as f:
            content = f.read()
    except Exception as e:
        print(f"Error opening {filepath}: {e}")
        return

    # Find the sheet UUID
    match = re.search(r'\(uuid "([a-f0-9\-]+)"\)', content)
    if not match:
        print(f'Sheet UUID not found in {filepath}')
        return
    sheet_uuid = match.group(1)

    # Prepare symbols string
    symbols_str = ''
    x_pos = 200
    for lib_id, ref_prefix, val in components:
        sym_uuid = str(uuid.uuid4())
        symbols_str += f'''	(symbol
		(lib_id "{lib_id}")
		(at {x_pos} 150 0)
		(unit 1)
		(exclude_from_sim no)
		(in_bom yes)
		(on_board yes)
		(dnp no)
		(fields_autoplaced yes)
		(uuid "{sym_uuid}")
		(property "Reference" "{ref_prefix}?"
			(at {x_pos} 140 0)
			(effects (font (size 1.27 1.27)))
		)
		(property "Value" "{val}"
			(at {x_pos} 160 0)
			(effects (font (size 1.27 1.27)))
		)
		(property "Footprint" ""
			(at {x_pos} 150 0)
			(effects (font (size 1.27 1.27)) (hide yes))
		)
		(property "Datasheet" ""
			(at {x_pos} 150 0)
			(effects (font (size 1.27 1.27)) (hide yes))
		)
		(property "Description" ""
			(at {x_pos} 150 0)
			(effects (font (size 1.27 1.27)) (hide yes))
		)
		(instances
			(project "{project_name}"
				(path "/{sheet_uuid}"
					(reference "{ref_prefix}?")
					(unit 1)
				)
			)
		)
	)\n'''
        x_pos += 20

    # Insert before (sheet_instances
    insert_pos = content.find('(sheet_instances')
    if insert_pos == -1:
        print(f'sheet_instances not found in {filepath}')
        return
    
    new_content = content[:insert_pos] + symbols_str + content[insert_pos:]
    with open(filepath, 'w') as f:
        f.write(new_content)
    print(f'Successfully added components to {filepath}')

tx_comps = [
    ('Kraft6M:BGS12PL6', 'U', 'BGS12PL6'),
    ('Kraft6M:SPF5043Z', 'U', 'SPF5043Z'),
    ('Kraft6M:LT5560', 'U', 'LT5560'),
    ('Kraft6M:10.7MHz_Filter', 'FL', '10.7MHz_Filter')
]
add_symbols_to_sch('C:\\radio\\KiCad_Projects\\TX_50MHz_1W\\TX_50MHz_1W.kicad_sch', tx_comps, 'TX_50MHz_1W')

rx_comps = [
    ('Kraft6M:BGS12PL6', 'U', 'BGS12PL6'),
    ('Kraft6M:INA219', 'U', 'INA219')
]
add_symbols_to_sch('C:\\radio\\KiCad_Projects\\RX_50MHz_SDR\\rx_n\\rx_n.kicad_sch', rx_comps, 'rx_n')
