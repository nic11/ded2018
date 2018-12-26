// intended to be included multiple times
// hense, there's no #pragma once

#include "state.h"

#ifndef SKYPROC_DEF_CMD
#define SKYPROC_DEF_CMD(name, opcode, width) \
    const word SKYPROC_CMD_##name = opcode; \
    const word SKYPROC_CMD_##name##_WIDTH = width;
#endif

// add, sub, mul, div, mod (no args)
// Заменяют верхний элемент стека на top [op] bot

// set r<n> <x>
// Устанавливает значение <n>-го регистра в <x>

// push/pop r<n>
// Кладет значение из r<n> в стек или достает из стека в него

// stor/load r<n>
// Достать из стека и сохранить в ram[r<n>]
// или наоборот достать из памяти в стек

// jmp/jeq/jne/jlt/jgt/jle/jge <L>
// Прыгают на метку <L> в зависимости от top [pred] bot

// call <L|offset?|addr?>
// Делает вызов на метку <L>
// Кладет в стек <ip> и прыгает на <L>

// ret
// Возврат из функции
// Достает из стека старый <ip> и возвращается туда

// arithmetic
SKYPROC_DEF_CMD(ADD,  0x1337ff0001LL, 1)
SKYPROC_DEF_CMD(SUB,  0x1337ff0002LL, 1)
SKYPROC_DEF_CMD(MUL,  0x1337ff0003LL, 1)
SKYPROC_DEF_CMD(DIV,  0x1337ff0004LL, 1)
SKYPROC_DEF_CMD(MOD,  0x1337ff0005LL, 1)

// memory:
// const --> reg
SKYPROC_DEF_CMD(SET,  0x1337ff0011LL, 3)

// stack <-> reg
SKYPROC_DEF_CMD(PUSH, 0x1337ff0012LL, 2)
SKYPROC_DEF_CMD(POP,  0x1337ff0013LL, 2)

// stack <-> ram
SKYPROC_DEF_CMD(STOR, 0x1337ff0014LL, 2)
SKYPROC_DEF_CMD(LOAD, 0x1337ff0015LL, 2)

// branch
SKYPROC_DEF_CMD(JMP,  0x1337ff0021LL, 2)
SKYPROC_DEF_CMD(JEQ,  0x1337ff0022LL, 2)
SKYPROC_DEF_CMD(JNE,  0x1337ff0023LL, 2)
SKYPROC_DEF_CMD(JLT,  0x1337ff0024LL, 2)
SKYPROC_DEF_CMD(JGT,  0x1337ff0025LL, 2)
SKYPROC_DEF_CMD(JLE,  0x1337ff0026LL, 2)
SKYPROC_DEF_CMD(JGE,  0x1337ff0027LL, 2)

// procedures
SKYPROC_DEF_CMD(CALL, 0x1337ff0031LL, 2)
SKYPROC_DEF_CMD(RET,  0x1337ff0032LL, 1)

// nop
SKYPROC_DEF_CMD(NOP,  0x1337ff0099LL, 1)
