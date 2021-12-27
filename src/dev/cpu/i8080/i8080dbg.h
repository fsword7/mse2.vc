// i8080.h - Intel 8080/8085 Processor package - debugging/tracing
//
// Author:  Tim Stark
// Date:    12/27/2021

static ctag_t *regList8[]   = { "B", "C", "D", "E", "H", "L", "M", "A" };
static ctag_t *regList16s[] = { "B", "D", "H", "SP" };
static ctag_t *regList16p[] = { "B", "D", "H", "PSW" };


#define OP_EXEC(name, opcode, format) DO_##opcode
#define OP_FUNC(opcode, call, format) call