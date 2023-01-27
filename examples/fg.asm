entry:
	lod r0, 0x0
	cmpi r0, 0x0
	
	cfg

	cmpi r0, 0x0

	rfg r1
	nori r1, 0b00001000
	nori r1, 0
	wfg r1