#ifndef Tabela_equ_simbola_h
#define Tabela_equ_simbola_h

#include <iostream>
#include <string>
#include <vector>


using namespace std;



struct ElemEqu{
    vector<string> izrazSimboli;
    int vrednost;
    string nazivEqu;
    ElemEqu* next;
    ElemEqu(string ime,vector<string> izraz,int vr):nazivEqu(ime),izrazSimboli(izraz),vrednost(vr),next(0){}
};

class TabelaEquSimbola{
    public:
        ElemEqu* first;
        ElemEqu* last;
        TabelaEquSimbola();
        void izracunajVrednostEquSimbola();
        void dodaj(string ime,vector<string> izraz,int vr);
       
};


#endif