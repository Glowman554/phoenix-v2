entry:
	lspi 0xffff
	rsp A

	puti 0xf0
	put r0

	pop r2
	pop r3

	jmpi 0xffff
	nop ; without those nop's my assembler crashes. WTF???
	nop
	nop
	nop