entry:
	lod r15, 0xff
	addi r15, 5

	jofi addr(overflow)
	jmpi 0xffff

overflow:
	jmpi addr(overflow)