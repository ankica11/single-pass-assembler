#include "TabelaEquSimbola.h"
#include "SymbolTable.h"




using namespace std;

extern int izracunajIzraz(vector<string> izraz);
extern bool proveriSekcije(vector<string> izraz);

extern SymbolTable* tabelaSimbola;
extern int brojLinije;



TabelaEquSimbola::TabelaEquSimbola(){
    first=last=0;
}

void TabelaEquSimbola::dodaj(string ime,vector<string> izraz, int vr){
    ElemEqu* e=new ElemEqu(ime,izraz,vr);
    if(first==0){
        first=last=e;
        return;
    }
    last=last->next=e;
}

void TabelaEquSimbola::izracunajVrednostEquSimbola(){
    for(ElemEqu* curr=first; curr!=0;curr=curr->next){
      if(!proveriSekcije(curr->izrazSimboli)){
          cout<<"Greska simboli u okviru .equ izraza moraju biti iz iste sekcije; broj linije "<<brojLinije<<endl;
          return;
      }
      int vrednostEquSimbola=izracunajIzraz(curr->izrazSimboli)+curr->vrednost;
      Symbol s(curr->nazivEqu,"data",vrednostEquSimbola,'l',tabelaSimbola->numOfElem,1);
      tabelaSimbola->addToEnd(s);

    }

}

