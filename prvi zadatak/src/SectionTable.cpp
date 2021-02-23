#include "SectionTable.h"
#include <sstream>


SectionTable::SectionTable(){
    first=last=0;
}


void SectionTable::addSection(Section* s){
    ElemSecTable* newElem = new ElemSecTable(s);
    if(first==0){
        first=last=newElem;
    }else{
        last=last->next=newElem;
    }
}

void SectionTable::addInSection(string name, string byte){
    for(ElemSecTable* curr=first; curr != 0; curr=curr->next){
        if(curr->sec->name==name){
            curr->sec->addByte(byte);
            if(byte.size()>2){
                curr->sec->numOfBytes += 2;
            }else{
                curr->sec->numOfBytes++;
            }
            break;
        }
    }
}


 void SectionTable::addInSectionInSpecificAddress(string name, string byte, int offset){
     for(ElemSecTable* curr=first;curr!=0;curr=curr->next){
         if(curr->sec->name==name){
             curr->sec->addByteOnSpecificAddress(offset,byte);
             if(byte.size()>2){
                curr->sec->numOfBytes += 2;
            }else{
                curr->sec->numOfBytes++;
            }
             break;
         }
     }
 }


int SectionTable::getNumOfBytesInSection(string name){
    for(ElemSecTable* curr=first;curr!=0;curr=curr->next){
        if(curr->sec->name==name){
            return curr->sec->numOfBytes;
        }
    }
    return 0;
}



string SectionTable::toString(){
    stringstream ss;
    for(ElemSecTable* curr=first;curr!=0;curr=curr->next){
        if(curr->sec->first != 0){
            ss<<curr->sec->toString();
            ss<<"\n";
        }
    }
    return ss.str();
}








