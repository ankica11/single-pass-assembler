#ifndef Relocation_table_list_h
#define Relocation_table_list_h

#include <iostream>
#include <string>
#include "RelocationTable.h"

using namespace std;

struct ElemR{
    RelocationTable* relTab;
    ElemR* next;
    ElemR(RelocationTable* rt):relTab(rt),next(0){}

};


class RelocationTableList{
    public:
        ElemR* first;
        ElemR* last;
        RelocationTableList();
        void addRelocationTable(RelocationTable* rt);
        void addToRelocationTable(string section, Record* r);
        string toString();
};


#endif