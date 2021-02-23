#ifndef Global_h
#define Global_h

#include <iostream>
#include <string>

using namespace std;


struct Instruction{
        string name;
        int numOfOperands;
        int bw;

};

extern string directives[];
extern Instruction instructions[];

#endif