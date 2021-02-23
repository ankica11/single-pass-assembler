#include "Section.h"
#include <sstream>

Section::Section(string name){
    this->name=name;
    this->numOfBytes=0;
    first=last=0;
}

void Section::addByte(string byte){
        ElemSection* newElem = new ElemSection(byte);
        if(first == 0){
            first=last=newElem;
        }else{
            last=last->next=newElem;
        }
        //numOfBytes++;
};
void Section::addByteOnSpecificAddress(int offset,string byte){
    int numb=0;
    ElemSection* curr;
    for(curr=first;curr!=0 && numb<offset;curr=curr->next){
        if(curr->byte.size()>2){
            numb+=2;
        }else{
            numb++;
        }
    }
    curr->byte=byte;

}

string Section::toString(){
    stringstream ss;
    ss<<"#"<<name<<"\n";
    int numb=0;
    for(ElemSection* curr=first; curr != 0;curr=curr->next){
        if(numb==16 || (numb==15 && curr->byte.size()>2)){
            if(curr->byte.size()>2){
                if(numb==16){
                    ss<<"\n"<<curr->byte[2]<<curr->byte[3]<<" "<<curr->byte[0]<<curr->byte[1]<<" ";
                    numb=2;
                }
                if(numb==15){
                    ss<<curr->byte[2]<<curr->byte[3]<<"\n";
                    ss<<curr->byte[0]<<curr->byte[1]<<" ";
                    numb=1;
                }
            }else{
                ss<<"\n"<<curr->byte<<" ";
                numb=1;
            }
        }else{
            if(curr->byte.size()>2){
                ss<<curr->byte[2]<<curr->byte[3]<<" "<<curr->byte[0]<<curr->byte[1]<<" ";
                numb += 2;
            }else{
                ss<<curr->byte<<" ";
                numb++;
            }
        }
    }
    ss<<"\n";
    if(first != 0){
        return ss.str();
    }else{
        return "";
    }

}