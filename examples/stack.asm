entry:
	lsp 0xffff
	rsp A

	put 0xf0
	put r0

	pop r2
	pop r3

	jmp 0xffff
	nop ; without those nop's my assembler crashes. WTF???
	nop
	nop
	nop