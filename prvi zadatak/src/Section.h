#ifndef Section_h
#define Section_h

#include <iostream>
#include <sstream>
#include <string>


using namespace std;

struct ElemSection{
     string byte;
     ElemSection* next;
     ElemSection(string b):byte(b), next(0) {}

};

class Section{

public:
    ElemSection* first;
    ElemSection* last;
    string name;
    int numOfBytes;
    Section(string name);
    void addByte(string byte);
    string toString();
    void addByteOnSpecificAddress(int offset,string byte);

};


#endif