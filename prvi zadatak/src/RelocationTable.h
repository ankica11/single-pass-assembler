#ifndef Relocation_table_h
#define Relocation_table_h

#include <iostream>
#include <string>



using namespace std;

struct Record{
    string offset;
    string type;
    int value;
    Record(string offs, string t, int val):offset(offs),type(t),value(val){}
    Record(){value=0;};
};
struct ElemRel{
    Record* rec;
    ElemRel* next;
    ElemRel(Record* r):rec(r),next(0){}
};

class RelocationTable{
    public:
        ElemRel* first;
        ElemRel* last;
        string sectionName;
        RelocationTable(string section);
        void addRecord(Record* r);
        string toString();




};

#endif