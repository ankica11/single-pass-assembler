#include "TabelaObracanjaUnapred.h"

using namespace std;

extern int brojLinije;
extern void obradiRelokacije(string ime_sekcije, Symbol* simbol, int tip);
extern void dodajSadrzajUSekcijuNaOdredjenuLokaciju(string ime_sekcije,Symbol* simbol,int addr,int tip,int size);
extern void dodajSadrzajUSekciju(string ime_sekcije, Symbol* simbol, int tip);
extern void obradiRelokacije2(string ime_sekcije, Symbol* simbol,int offset, int tip);

TabelaObracanjaUnapred::TabelaObracanjaUnapred(string sim, int globalFlag){
    simbol=sim;
    global_flag=globalFlag;
    first=last=0;

}

void TabelaObracanjaUnapred::dodaj(int adr, int vel, string sekcija, int tipRelokacije){
        ElemO* novi = new ElemO(adr,vel,sekcija,tipRelokacije);
        if(first==0){
            first=last=novi;
            return;
        }
        last=last->next=novi;
}


void TabelaObracanjaUnapred::obradiElement(ElemO* e){
    Symbol* s=tabelaSimbola->find(simbol);
    if(s==0){
        //greska simbol nije definisan
        cout<<"Greska na liniji "<<brojLinije<<" simbol nije definisan!"<<endl;
        return;
    }
    
    switch(e->tipRelokacije){
        case 0:{//no need for relocation
            dodajSadrzajUSekciju(e->sekcija,s,0);
            break;
        }
        case 1:{//abs
            obradiRelokacije2(e->sekcija,s,e->adresa,0);
            dodajSadrzajUSekcijuNaOdredjenuLokaciju(e->sekcija,s,e->adresa,0,e->velicina);
            //dodajUSekcijuOnSpecificLocation(e->sekcija,s,e->adresa,0);  //dodajUSekcijuNaOdredjenuLokaciju(e->sekcija,s,e->adresa,0)
            //obradiRelokaciju(e->sekcija,s); //obradiRelokacije(e->sekcija,s,0)
           

            break;
        }
        case 2:{//pc rel
            obradiRelokacije2(e->sekcija,s,e->adresa,1);
            dodajSadrzajUSekcijuNaOdredjenuLokaciju(e->sekcija,s,e->adresa,1,e->velicina);
            
            //dodajUSekcijuPCrelOnSpecificLocation(e->sekcija,s,e->adresa); //dodoajUSekcijuNaOdredjenuLokaciju(sekcija,s,adresa,1)

            //obradiPCRelokaciju(e->sekcija,s); //obradiRelokacije(sekcija,1)
            break;
        }
        case 3:{//pc rel 2
            obradiRelokacije2(e->sekcija,s,e->adresa,1);
            dodajSadrzajUSekcijuNaOdredjenuLokaciju(e->sekcija,s,e->adresa,2,e->velicina);
            //dodajUSekcijuPCrel2OnSpecificLocation(e->sekcija,s,e->adresa); //dodajUSekcijuNaOdredjenuLokaciju(sekcija,s,adresa,2)
            //obradiPCRelokaciju(e->sekcija,s); //obradiRelokacije(sekcija,1)
            
            break;
        }
        default:{
            //greska
            cout<<"Greska na liniji "<<brojLinije<<" FATAL ERROR"<<endl;
            break;
        }
    }
    



}


void TabelaObracanjaUnapred::obradiTabelu(){
    Symbol* s=tabelaSimbola->find(simbol);
    
    if(global_flag==1){
        tabelaSimbola->updateVisibility(simbol,'g');
    }
    for(ElemO* curr=first;curr!=0;curr=curr->next){
        obradiElement(curr);
        
    }
}