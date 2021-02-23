#ifndef Tabela_obracanja_unapred_h
#define Tabela_obracanja_unapred_h


#include <iostream>
#include <sstream>
#include <string>
#include "SymbolTable.h"
#include "RelocationTable.h"
#include "RelocationTableList.h"
#include "SectionTable.h"

using namespace std;

extern SymbolTable* tabelaSimbola;
extern RelocationTableList* tabelaRelokacija;
extern SectionTable* tabelaSekcija;

extern void obradiRelokaciju(string sekcija,Symbol* simbol);
extern void obradiPCRelokaciju(string sekcija,Symbol* s);
extern void dodajUSekcijuPCrel(string sekcija,Symbol* simbol);
extern void obradiLiteralOperand(string literal);
extern void dodajUSekciju(string sekcija,Symbol* simbol);
extern void dodajUSekcijuPCrel2(string sekcija,Symbol* simbol);
extern void dodajUSekcijuOnSpecificLocation(string sekcija,Symbol* simbol,int address);
extern void dodajUSekcijuPCrelOnSpecificLocation(string sekcija,Symbol* simbol,int address);
extern void dodajUSekcijuPCrel2OnSpecificLocation(string sekcija,Symbol* simbol,int address);

struct ElemO{
    int adresa;
    int velicina;
    string sekcija;
    int tipRelokacije; //0-bez, 1-abs, 2-pc, 3-pc2
    ElemO* next;
    ElemO(int adr, int vel, string s, int tip) : adresa(adr), velicina(vel), sekcija(s), tipRelokacije(tip), next(0){}

};

class TabelaObracanjaUnapred{

    public:
        ElemO* first;
        ElemO* last;
        string simbol;
        int global_flag; //0-lokalni, 1-globalni
        TabelaObracanjaUnapred(string sim, int globalFlag);
        void dodaj(int adr, int vel, string sekcija, int tipRelokacije);
        void obradiElement(ElemO* e);
        void obradiTabelu();



};


#endif