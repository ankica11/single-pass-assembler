
#include "SymbolTable.h"
#include <sstream>

extern int brojLinije;

Symbol* SymbolTable::find(string name){
    for(ElemSim* curr=first;curr != 0; curr=curr->next){
        if(name.compare(curr->sym.name)==0){
            
            return &curr->sym;
        }
    }

    return 0;
}


void SymbolTable::addToEnd(Symbol sym){
    ElemSim* newSym= new ElemSim(sym);
    if(find(sym.name) != 0){
        cout<<"Greska na liniji "<<brojLinije<<" visestruka definicija simbola"<<endl;
        return;
    }
    if(first==0){
        first=last=newSym;
    }else{
        last->next=newSym;
        last=last->next;
    }
    numOfElem++;

}

SymbolTable::SymbolTable(){
    first=last=0;
    numOfElem=0;
    Symbol UND("UND","UND",0,'l',0,0);
    addToEnd(UND);
}


void SymbolTable::updateVisibility(string name,char value){
    Symbol* foundSym = find(name);
    if(foundSym != 0){
            foundSym->visible=value;
            return;
    }else{
        cout<<"Greska na liniji "<<brojLinije<<" simbol nije definisan!"<<endl;
    }
}


void SymbolTable::updateSection(string name,string value){
    Symbol* foundSym = find(name);
    if(foundSym != 0){
            foundSym->section=value;
            return;
    }
}


string SymbolTable::toString(){
    stringstream ss;
    ss<<"#Symbol table"<<endl;
    ss<<"#ime\tsek.\tvr.\tvid.\trbr.\tequ?\n";
    for(ElemSim* curr=first; curr != 0; curr=curr->next){
        ss<<curr->sym.name<<"\t"<<curr->sym.section<<"\t"<<curr->sym.value<<"\t"<<curr->sym.visible<<"\t"<<curr->sym.number<<"\t"<<curr->sym.equSimbol<<"\n";
    }
    ss<<endl;
    return ss.str();

}