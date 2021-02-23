#ifndef Tabela_obracanja_unapred_lista_h
#define Tabela_obracanja_unapred_lista_h

#include <iostream>
#include <sstream>
#include <string>
#include "TabelaObracanjaUnapred.h"


using namespace std;

struct Elem{
    TabelaObracanjaUnapred* tabela;
    Elem* next;
    Elem(TabelaObracanjaUnapred* t): tabela(t), next(0){}
};

class TabelaObracanjaUnapredLista{
    public:
    Elem* first;
    Elem* last;
    TabelaObracanjaUnapredLista();
    void dodaj(string simbol, int globalni, int adresa, int velicina, string sekcija, int tipRelokacije);
    void postaviGlobalni(string simbol);
    void obradiListu();
    void dodajTabelu(string simbol,int globalFleg);
    bool checkIfSymbolGlobal(string simbol);
};


#endif