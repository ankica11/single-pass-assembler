#ifndef Section_table_h
#define Section_table_h

#include <iostream>
#include <string>
#include "Section.h"



using namespace std;


struct ElemSecTable{
    Section* sec;
    ElemSecTable* next;
    ElemSecTable(Section* s): sec(s),next(0){}

};

class SectionTable{
    public:
        ElemSecTable* first;
        ElemSecTable* last;
        SectionTable();
        void addSection(Section* s);
        void addInSection(string name, string byte);
        int getNumOfBytesInSection(string name);
        string toString();
        void addInSectionInSpecificAddress(string name, string byte, int offset);
};



#endif