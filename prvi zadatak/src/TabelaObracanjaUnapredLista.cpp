#include "TabelaObracanjaUnapredLista.h"


using namespace std;


TabelaObracanjaUnapredLista::TabelaObracanjaUnapredLista(){
    first=last=0;
}


void TabelaObracanjaUnapredLista::dodajTabelu(string simbol,int globalFleg){
    Elem* novi=new Elem(new TabelaObracanjaUnapred(simbol,globalFleg));
    if(first==0){
        first=last=novi;
        return;
    }
    last=last->next=novi;
}

void TabelaObracanjaUnapredLista::dodaj(string simbol,int globalni, int adresa, int velicina, string sekcija, int tipRelokacije){
    Elem* novi= new Elem(new TabelaObracanjaUnapred(simbol,globalni));
    if(first==0){
        first=last=novi;
        novi->tabela->dodaj(adresa,velicina,sekcija,tipRelokacije);
        return;
    }
    int exist=0;
    for(Elem* curr=first;curr != 0; curr=curr->next){
        if(curr->tabela->simbol.compare(simbol)==0){
            exist=1;
            curr->tabela->dodaj(adresa,velicina,sekcija,tipRelokacije);
        }
    }
    if(!exist){
        last=last->next=novi;
        novi->tabela->dodaj(adresa,velicina,sekcija,tipRelokacije);

    }
    
}


void TabelaObracanjaUnapredLista::postaviGlobalni(string simbol){
    //cout<<"postavi globalni"<<endl;
    int exist=0;
    for(Elem* curr=first;curr!=0;curr=curr->next){
        if(curr->tabela->simbol.compare(simbol)==0){
            curr->tabela->global_flag=1;
            exist=1;
        }
    }
    if(exist==0){
        dodajTabelu(simbol,1);
        
    }

}


void TabelaObracanjaUnapredLista::obradiListu(){
    for(Elem* curr=first; curr!=0;curr=curr->next){
            curr->tabela->obradiTabelu();
    }
}


bool TabelaObracanjaUnapredLista::checkIfSymbolGlobal(string simbol){
    for(Elem* curr=first; curr!=0;curr=curr->next){
        if(curr->tabela->simbol.compare(simbol)==0 && curr->tabela->global_flag==1){
            return true;
        }
    }
    return false;

}