entry:
	lod r5, 0xff
	lad A, 0xff01
	out A, r5
	lad A, 0xff02
	out A, r5
	lad A, 0xff03
	out A, r5
	jmpi 0xffff