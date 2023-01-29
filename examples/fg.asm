entry:
	lod r0, 0x0
	cmp r0, 0x0
	
	cfg

	cmp r0, 0x0

	rfg r1
	nor r1, 0b00001000
	nor r1, 0
	wfg r1