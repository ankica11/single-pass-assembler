#include "RelocationTableList.h"
#include <sstream>


RelocationTableList::RelocationTableList(){
    first=last=0;
}


void RelocationTableList::addRelocationTable(RelocationTable* rt){
    ElemR* newElem = new ElemR(rt);
    if(first==0){
        first=last=newElem;
    }else{
        last=last->next=newElem;
    }
}

void RelocationTableList::addToRelocationTable(string section, Record* r){
    for(ElemR* curr=first; curr!=0; curr=curr->next){
        if(curr->relTab->sectionName==section){
            curr->relTab->addRecord(r);
            break;
        }
    }

}


string RelocationTableList::toString(){
    stringstream ss;
    for(ElemR* curr=first;curr!=0;curr=curr->next){
        if(curr->relTab->first != 0){
            ss<<curr->relTab->toString()<<"\n";
        }
    }
    return ss.str();
}