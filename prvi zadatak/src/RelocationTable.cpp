#include "RelocationTable.h"
#include <sstream>


RelocationTable::RelocationTable(string section){
    first=last=0;
    sectionName=section;
}


void RelocationTable::addRecord(Record* r){
    ElemRel* newElem = new ElemRel(r);
    if(first==0){
        first=last=newElem;
    }else{
        last=last->next=newElem;
    }

}

string RelocationTable::toString(){
    stringstream ss;
    ss<<"#.rel."<<sectionName<<"\n";
    ss<<"#offs."<<"\t"<<"tip"<<"\t"<<"vr."<<"\n";
    for(ElemRel* curr=first;curr!=0;curr=curr->next){
        ss<<curr->rec->offset<<"\t"<<curr->rec->type<<"\t"<<curr->rec->value<<"\n";
    }
    if(first != 0){
        return ss.str();
    }else {
        return "";
    }

}