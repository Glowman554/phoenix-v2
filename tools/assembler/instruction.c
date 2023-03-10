#include "cpu.h"

#include <assert.h>

instruction_t build_nop_instr(int ir0, int ir1, int iimm, int sreg) {
	return NOP();
}

instruction_t build_mov_instr(int ir0, int ir1, int iimm, int sreg) {
	return MOV(ir0, ir1);
}

instruction_t build_lod_instr(int ir0, int ir1, int iimm, int sreg) {
	return LOD(ir0, iimm);
}

instruction_t build_out_instr(int ir0, int ir1, int iimm, int sreg) {
	return OUT(sreg, ir0);
}

instruction_t build_inp_instr(int ir0, int ir1, int iimm, int sreg) {
	return INP(sreg, ir0);
}

instruction_t build_jnz_instr(int ir0, int ir1, int iimm, int sreg) {
	return JNZ(sreg);
}

instruction_t build_add_instr(int ir0, int ir1, int iimm, int sreg) {
	return ADD(ir0, ir1);
}

instruction_t build_addi_instr(int ir0, int ir1, int iimm, int sreg) {
	return ADDI(ir0, iimm);
}

instruction_t build_sub_instr(int ir0, int ir1, int iimm, int sreg) {
	return SUB(ir0, ir1);
}

instruction_t build_subi_instr(int ir0, int ir1, int iimm, int sreg) {
	return SUBI(ir0, iimm);
}

instruction_t build_nad_instr(int ir0, int ir1, int iimm, int sreg) {
	return NAD(ir0, ir1);
}

instruction_t build_nadi_instr(int ir0, int ir1, int iimm, int sreg) {
	return NADI(ir0, iimm);
}

instruction_t build_nor_instr(int ir0, int ir1, int iimm, int sreg) {
	return NOR(ir0, ir1);
}

instruction_t build_nori_instr(int ir0, int ir1, int iimm, int sreg) {
	return NORI(ir0, iimm);
}

instruction_t build_cmp_instr(int ir0, int ir1, int iimm, int sreg) {
	return CMP(ir0, ir1);
}

instruction_t build_cmpi_instr(int ir0, int ir1, int iimm, int sreg) {
	return CMPI(ir0, iimm);
}

instruction_t build_jzr_instr(int ir0, int ir1, int iimm, int sreg) {
	return JZR(sreg);
}

instruction_t build_ldr_instr(int ir0, int ir1, int iimm, int sreg) {
	return LDR(ir0, sreg);
}

instruction_t build_wtr_instr(int ir0, int ir1, int iimm, int sreg) {
	return WTR(sreg, ir0);
}

instruction_t build_jmp_instr(int ir0, int ir1, int iimm, int sreg) {
	return JMP(sreg);
}

instruction_t build_jeq_instr(int ir0, int ir1, int iimm, int sreg) {
	return JEQ(sreg);
}

instruction_t build_jnq_instr(int ir0, int ir1, int iimm, int sreg) {
	return JNQ(sreg);
}

instruction_t build_jmpi_instr(int ir0, int ir1, int iimm, int sreg) {
	return JMPI(iimm);
}

instruction_t build_jeqi_instr(int ir0, int ir1, int iimm, int sreg) {
	return JEQI(iimm);
}

instruction_t build_jnqi_instr(int ir0, int ir1, int iimm, int sreg) {
	return JNQI(iimm);
}

instruction_t build_jzri_instr(int ir0, int ir1, int iimm, int sreg) {
	return JZRI(iimm);
}

instruction_t build_jnzi_instr(int ir0, int ir1, int iimm, int sreg) {
	return JNZI(iimm);
}

instruction_t build_lad_instr(int ir0, int ir1, int iimm, int sreg) {
	return LAD(sreg, iimm);
}

instruction_t build_ire_instr(int ir0, int ir1, int iimm, int sreg) {
	return IRE();
}

instruction_t build_int_instr(int ir0, int ir1, int iimm, int sreg) {
	return INT();
}

instruction_t build_jof_instr(int ir0, int ir1, int iimm, int sreg) {
	return JOF(sreg);
}

instruction_t build_jofi_instr(int ir0, int ir1, int iimm, int sreg) {
	return JOFI(iimm);
}

instruction_t build_jno_instr(int ir0, int ir1, int iimm, int sreg) {
	return JNO(sreg);
}

instruction_t build_jnoi_instr(int ir0, int ir1, int iimm, int sreg) {
	return JNOI(iimm);
}


instruction_t build_ado_instr(int ir0, int ir1, int iimm, int sreg) {
	return ADO(ir0, ir1);
}

instruction_t build_adoi_instr(int ir0, int ir1, int iimm, int sreg) {
	return ADOI(ir0, iimm);
}

instruction_t build_sbo_instr(int ir0, int ir1, int iimm, int sreg) {
	return SBO(ir0, ir1);
}

instruction_t build_sboi_instr(int ir0, int ir1, int iimm, int sreg) {
	return SBOI(ir0, iimm);
}

instruction_t build_cfg_instr(int ir0, int ir1, int iimm, int sreg) {
	return CFG();
}

instruction_t build_lih_instr(int ir0, int ir1, int iimm, int sreg) {
	return LIH(sreg);
}

instruction_t build_lihi_instr(int ir0, int ir1, int iimm, int sreg) {
	return LIHI(iimm);
}

instruction_t build_wfg_instr(int ir0, int ir1, int iimm, int sreg) {
	return WFG(ir0);
}

instruction_t build_rfg_instr(int ir0, int ir1, int iimm, int sreg) {
	return RFG(ir0);
}

instruction_t build_lsp_instr(int ir0, int ir1, int iimm, int sreg) {
	return LSP(sreg);
}

instruction_t build_lspi_instr(int ir0, int ir1, int iimm, int sreg) {
	return LSPI(iimm);
}

instruction_t build_rsp_instr(int ir0, int ir1, int iimm, int sreg) {
	return RSP(sreg);
}

instruction_t build_put_instr(int ir0, int ir1, int iimm, int sreg) {
	return PUT(ir0);
}

instruction_t build_puti_instr(int ir0, int ir1, int iimm, int sreg) {
	return PUTI(iimm);
}

instruction_t build_pop_instr(int ir0, int ir1, int iimm, int sreg) {
	return POP(ir0);
}