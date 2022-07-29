#include "cpu.h"
#include <stdio.h>

int main() {
	instruction_t instructions[] = {
		LOD(R0, 0xff),
		MOV(R1, R0),
		LDR(R2, A),

		LOD(R0, 0),
		LOD(R1, 0x80),
		WTR(A, R2),

		LOD(R1, 0),
		OUT(A, R2),

		LOD(R0, 0xff),
		MOV(R1, R0),
		LOD(R2, 0x10),
		CMPI(R2, 0x10),
		JEQ(A)
	};

	FILE* f = fopen("rom.bin", "wb");
	fwrite(instructions, sizeof(instructions), 1, f);
	fclose(f);
}