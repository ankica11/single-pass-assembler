#include "Global.h"



string directives[] = {
                ".global",
		".extern",
		".equ",
		".section",
		".byte",
		".word",
		".skip",
		".end"
};


Instruction instructions[] = {
        {"halt",0,0},
        {"iret",0,0},
        {"ret",0,0},
        {"int",1,0},
        {"call",1,0},
        {"jmp",1,0},
        {"jeq",1,0},
        {"jne",1,0},
        {"jgt",1,0},
        {"push",1,1},
        {"pop",1,1},
        {"xchg",2,1},
        {"mov",2,1},
        {"add",2,1},
        {"sub",2,1},
        {"mul",2,1},
        {"div",2,1},
        {"cmp",2,1},
        {"not",2,1},
        {"and",2,1},
        {"or",2,1},
        {"xor",2,1},
        {"test",2,1},
        {"shl",2,1},
        {"shr",2,1}
};