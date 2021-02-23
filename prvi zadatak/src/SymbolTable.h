#ifndef Symbol_table_h
#define Symbol_table_h

#include <iostream>
#include <string>

using namespace std;


struct Symbol{
    string name;
    string section;
    int value;
    int number;
    char visible;
    int equSimbol; //0 no 1 yes
    Symbol(string n, string sec, int val, char vis, int num, int es){
        name=n;
        section=sec;
        value=val;
        visible=vis;
        number=num;
        equSimbol=es;
        
    }
};

struct ElemSim{
    Symbol sym;
    ElemSim* next;
    ElemSim(Symbol s): sym(s), next(0){}

};


class SymbolTable{

public:
    int numOfElem;
    ElemSim* first;
    ElemSim* last;

    void addToEnd(Symbol s);
    Symbol* find(string name);
    void updateVisibility(string name, char value);
    void updateSection(string name, string value);
    string toString();
    SymbolTable();



};



#endif