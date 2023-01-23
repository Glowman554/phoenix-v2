entry:
	lad A, 0x0
	lod r5, 0x0

loop:
	nori r5, 0
	out A, r5

	jmpi addr(loop)
