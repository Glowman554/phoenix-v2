entry:
	lad A, 0x0
	lod r15, 0x0

loop:
	nori r15, 0
	out A, r15

	jmpi addr(loop)
