#include <string>
#include <vector>
#include <string.h>
#include <fstream>
#include <iostream>
#include <regex>
#include "SymbolTable.h"
#include "RelocationTable.h"
#include "RelocationTableList.h"
#include "SectionTable.h"
#include"Section.h"
#include "TabelaObracanjaUnapredLista.h"
#include "TabelaObracanjaUnapred.h"
#include "TabelaEquSimbola.h"
#include "Global.h"

using namespace std;

extern string removeSpace(string line);
extern int odrediTipLinije(string linija);
extern string convertDecToHex(int broj,int size);
extern int odrediOpCode(string line);
extern string binaryOpCodeFormat(int opcode);
extern int odrediVelicinuOperandaInstrukcije(string linija, int brojOperanada, int kodInstrukcije, string nazivInstrukcije);
extern int convertBinaryToDec(string binary);
extern int odrediAdresiranje2(string operand,int kodInstrukcije);
extern string odrediformatAdrese(int tipAdresiranja);
extern string kodirajRegistar(int broj);
extern bool checkIfJumpInstruction(int opCode);
extern string formatirajLiteral(string s);
extern void obradiOperandInstrukcije(string operand,int rbrOperanda,int kodInstrukcije);
extern vector<string> split(string s,char delimiter);
extern int izracunajIzraz(vector<string> izraz);
extern bool proveriSekcije(vector<string> izraz);
extern bool proveriDefinisanostiSimbolaEquIzraza(vector<string> izraz);
extern void dodajSadrzajUSekciju(string ime_sekcije, Symbol* simbol, int tip);
extern void obradiRelokacije(string ime_sekcije, Symbol* simbol, int tip);
extern void dodajSadrzajUSekcijuNaOdredjenuLokaciju(string ime_sekcije,Symbol* simbol,int addr,int tip,int size);
extern void obradiLiteral(string literal);
extern void dodajBajtUSekciju(string imeSekcije,Symbol* simbol,int tip);

SymbolTable* tabelaSimbola = new SymbolTable();
RelocationTableList* tabelaRelokacija = new RelocationTableList();
SectionTable* tabelaSekcija = new SectionTable();
TabelaObracanjaUnapredLista* listaObracanjaUnapred = new TabelaObracanjaUnapredLista();
TabelaEquSimbola* tabelaEquSimbola=new TabelaEquSimbola();

regex regexLiteralHexFormat("0x[0-9a-fA-F]{1,4}");

int locationCounter=0;
int brojLinije=0;
string tekucaSekcija;





//prvi i jedini prolaza jednoprolaznog asemblera !!!!!


string one_pass(ifstream* inputFile){
    //all code
    string linija="";
    //string tekucaSekcija;
    int brojTekuceSekcije;
    bool end=false;

    int tipLinije=-1;
    int brojDirektive=-1;
    int brojInstrukcije=-1;

    smatch m;
    smatch pogodak;
    regex regexKomentar("^\\s*#[a-zA-Z0-9.,:#$%!; @=()]*");

    while(getline(*inputFile,linija) && !end){

        brojLinije++;
        if(regex_match(linija,m,regexKomentar)){
            continue;
        }
        //cout<<linija<<endl;
       
        linija=regex_replace(linija,regexKomentar,"");
        regex regexLabela("^\\s*[a-zA-Z_][a-zA-Z_0-9]*:");
        //smatch m;
        string labela, direktiva, instrukcija;

        if(regex_search(linija, m, regexLabela)){     //LABELA

            
            labela = m[0];
            labela = removeSpace(labela);
            labela = labela.substr(0,labela.size()-1);
            //cout<<labela<<endl;
            char vis;
            if(listaObracanjaUnapred->checkIfSymbolGlobal(labela)){
                    vis='g';
            }else{
                vis='l';
            }

            Symbol simbol(labela,tekucaSekcija,locationCounter,vis,tabelaSimbola->numOfElem,0);
            tabelaSimbola->addToEnd(simbol);
            linija=regex_replace(linija, regex(labela+":"), ""); //uklanjamo labelu iz linije
            //cout<<tabelaSimbola->toString()<<endl;
            //cout<<linija<<endl;
            //int pos=linija.find_first_of(":");
            //linija=linija.substr(pos+1,linija.length()-pos-1);
            

        }

        tipLinije = odrediTipLinije(linija);

        switch(tipLinije){
            case 0: {
                //cout<<brojLinije<<" prazan red"<<endl;
                break; //prazan red
            }
            case 1:{ //direktiva
                //cout<<"direktiva "<<brojLinije<<endl;
                brojDirektive=-1;
                regex regexDirektiva("\\.[a-z]+");
                if(regex_search(linija, m, regexDirektiva)){
                direktiva=m[0];
                //cout<<"Ime direktive "<<direktiva<<endl;
                };
                for(int i=0; i<8; i++){
                    if(direktiva.compare(directives[i])==0){
                        brojDirektive=i;
                        break;
                    }
                }
                //cout<<"broj direktive: "<<brojDirektive<<endl;

                switch(brojDirektive){
                    case 0: {//global
                            //cout<<"global direktiva"<<endl;
                            string globalniSimbol;
                            linija=regex_replace(linija,regex("\\s*\\.global\\s*"),"");
                            //cout<<"linija posle: "<<linija<<endl;
                            while(regex_search(linija,m,regex("[a-zA-Z_][a-zA-Z_0-9]*"))){
                                globalniSimbol=m[0];
                                //cout<<globalniSimbol<<"\t";
                                Symbol* foundSimbol=tabelaSimbola->find(globalniSimbol);
                                if(foundSimbol != 0){
                                    tabelaSimbola->updateVisibility(globalniSimbol, 'g');
                                }else{
                                    //obracanje unapred
                                    //cout<<"obracanje unapred"<<endl;
                                    listaObracanjaUnapred->postaviGlobalni(globalniSimbol);
                                    
                                }
                                linija=regex_replace(linija,regex(globalniSimbol),"",regex_constants::format_first_only);
                                //cout<<endl;
                                //cout<<linija<<endl;
                            }
                            
                            break;
                    }
                    case 1: {//extern
                            string externSimbol;
                            linija=regex_replace(linija,regex("\\s*\\.extern\\s*"), ""); //.extern   a,b,c,ana
                            while(regex_search(linija,m,regex("[a-zA-Z_][a-zA-Z_0-9]*"))){
                                externSimbol=m[0];
                                Symbol s(externSimbol,"UND",0,'g',tabelaSimbola->numOfElem,0);
                                tabelaSimbola->addToEnd(s);
                                linija=regex_replace(linija,regex(externSimbol),"",regex_constants::format_first_only);

                            }
                            break;
                    }
                    case 2: {//equ
                          //cout<<"equ direktiva"<<endl;
                          if(!tekucaSekcija.compare("data")==0){
                                cout<<"Greska na liniji "<<brojLinije<<" equ direktiva je dozvoljena samo u data sekciji!"<<endl;
                          }
                          linija=regex_replace(linija, regex("\\s*\\.equ\\s*"),"");
                          regex_search(linija,m,regex("[a-zA-Z_][a-zA-Z_0-9]*\\s*,\\s*")); //.equ ana, nesto u izrazu svi tokeni moraju biti razdvojeni spaceom
                          string imeEquSimbola=m[0];
                          //cout<<imeEquSimbola<<endl;
                          imeEquSimbola=regex_replace(imeEquSimbola,regex("\\s*,\\s*"),"");
                          //cout<<imeEquSimbola<<endl;
                          string izraz=regex_replace(linija,regex("[a-zA-Z_][a-zA-Z_0-9]*\\s*,\\s*"),"");
                          
                          //izraz=regex_replace(izraz,regex("\\s*"),"");
                          vector<string> tokens=split(izraz,' ');
                          vector<string> simboli;
                          vector<string> brojevi;
                          
                          int num_minus=0;
                          int num_plus=0;
                          for(int i=0; i<tokens.size();i++){
                              
                              char prev;
                              if(tokens[i]=="-" || tokens[i]=="+"){
                                  continue;
                              }
                              if(regex_search(tokens[i],m,regex("[a-zA-Z_][a-zA-Z_0-9]*"))){
                                  if(i==0){
                                      num_plus++;
                                      simboli.push_back("+");
                                      simboli.push_back(tokens[i]);
                                      continue;
                                  }
                                  
                                  if(tokens[i-1]=="-" && i>0){
                                      num_minus++;
                                      simboli.push_back("-");
                                  }else if(tokens[i-1]=="+" && i>0){
                                      num_plus++;
                                      simboli.push_back("+");
                                  }
                                  simboli.push_back(tokens[i]);
                                  
                              }else if(regex_search(tokens[i],m,regex("[1-9][0-9]*"))){
                                  if(i==0){
                                  brojevi.push_back("+");
                                  brojevi.push_back(tokens[i]);
                                  continue;
                                  }
                                  if(tokens[i-1]=="-"){
                                        brojevi.push_back("-");

                                  }else if(tokens[i-1]=="+"){
                                    brojevi.push_back("+");}
                                    brojevi.push_back(tokens[i]);
                                                                 
                              }
                          }
                          
                          //cout<<num_plus<<"\t"<<num_minus<<endl;
                          
                          if(num_plus != num_minus){
                              cout<<"Greska na liniji "<<brojLinije<<" equ simbol mora biti konstantna vrednost"<<endl;
                              break;
                          }
                          int rezultat=0;
                          string prevOp;
                          for(int i=0; i< brojevi.size(); i++){
                              if(brojevi[i]=="-"){
                                  prevOp="-";
                                  continue;
                              }else if(brojevi[i]=="+"){
                                  prevOp="+";
                                  continue;
                              }
                              if(prevOp=="-"){
                                  rezultat-=stoi(brojevi[i]);
                                  continue;
                              }
                              if(prevOp=="+"){
                                  rezultat+=stoi(brojevi[i]);

                              }
                          }
                         
                          if(simboli.empty()){
                              
                            Symbol s(imeEquSimbola,"data",rezultat,'l',tabelaSimbola->numOfElem,1);
                            tabelaSimbola->addToEnd(s);
                          }else if(proveriDefinisanostiSimbolaEquIzraza(simboli)){
                              
                              
                              
                              if(!proveriSekcije(simboli)){
                                  
                                  cout<<"Greska na liniji "<<brojLinije<<" svi simboli equ izraza moraju biti iz iste sekcije"<<endl;
                                  break;
                              }
                              int vrednostEquSim=rezultat+izracunajIzraz(simboli);
                              //cout<<vrednostEquSim<<endl;
                              Symbol s(imeEquSimbola,"data",vrednostEquSim,'l',tabelaSimbola->numOfElem,1);
                              tabelaSimbola->addToEnd(s);
                          }else{
                              

                          tabelaEquSimbola->dodaj(imeEquSimbola,simboli,rezultat);
                          
                          }                      
                           
                           break;
                        }//equ
                    case 3: {//section
                            //cout<<"section"<<endl;
                            linija=regex_replace(linija, regex("\\s*\\.section\\s*"),"");
                            //cout<<"linija posle uklanjanja naziva direktive: "<<linija<<endl;
                            regex_search(linija,m,regex("[a-zA-Z_][a-zA-Z_0-9]*"));
                            string imeSekcije=m[0];
                            tekucaSekcija=imeSekcije;
                            locationCounter=0;
                            
                            
                            Symbol s(imeSekcije,tekucaSekcija,locationCounter,'l',tabelaSimbola->numOfElem,0);
                            tabelaSimbola->addToEnd(s);
                            tabelaSekcija->addSection(new Section(tekucaSekcija));
                            tabelaRelokacija->addRelocationTable(new RelocationTable(tekucaSekcija));
                            //cout<<tabelaSimbola->toString()<<endl;

                            break;
                    }
                    case 4: { //byte  
                        
                        linija=regex_replace(linija,regex("\\s*\\.byte\\s*"),""); //ovde se ja oslanjam da ce broj biti manji od 127 i veci od -127
                        regex regexLiteral("(0|-?[1-9][0-9]*)");
                        regex regexSimbol("[a-zA-Z_][a-zA-Z_0-9]*");
                        smatch n;
                        string simbol;
                        int literalDec;
                        string bajt;
                        while(regex_search(linija,m,regexLiteral) || regex_search(linija,n,regexSimbol)){
                            regex_search(linija,m,regexLiteral);
                            regex_search(linija,n,regexSimbol);
                            if(m.position()>n.position()){
                                
                                //simbol
                                simbol=n[0];
                                Symbol* s= tabelaSimbola->find(simbol);
                                if(s!=0){
                                    obradiRelokacije(tekucaSekcija,s,0);
                                    dodajBajtUSekciju(tekucaSekcija,s,0);
                                }else{
                                    //obracanje unapred
                                    int offset=tabelaSekcija->getNumOfBytesInSection(tekucaSekcija);
                                    listaObracanjaUnapred->dodaj(simbol,0,offset,1,tekucaSekcija,1);
                                    
                                    tabelaSekcija->addInSection(tekucaSekcija,"**");

                                }
                                linija=regex_replace(linija,regex(simbol),"",regex_constants::format_first_only);
                                locationCounter++;
                            }else{
                            string literal=m[0];
                            //cout<<literal<<endl;
                            linija=regex_replace(linija,regex(literal),"",regex_constants::format_first_only);
                            //cout<<"linija posle uklanjanja literala: "<<linija<<endl;
							literalDec=stoi(literal);
                            bajt=convertDecToHex(literalDec,1);
                            //cout<<"Bajt "<<bajt<<endl;
                            tabelaSekcija->addInSection(tekucaSekcija,bajt);
                            locationCounter++;

                            }
                        }
                            //cout<<tabelaSekcija->toString()<<endl;
                            //cout<<"Location counter: "<<locationCounter<<endl;
                            //exit(-1);


                    
                    break;
                    }//byte
                    
                    case 5:{//word
                        //exit(-1);
                        linija=regex_replace(linija,regex("\\s*\\.word\\s*"),"");
                        //cout<<linija<<endl;
                        smatch lit,sim;
                        string matchedVal;
                        int literal;
                        regex regexLiteral("(0|-?[1-9][0-9]*)");
                        regex regexSimbol("[a-zA-Z_][a-zA-Z_0-9]*");
                        while(regex_search(linija,lit,regexLiteral) || regex_search(linija,sim,regexSimbol)){
                            regex_search(linija,lit,regexLiteral);
                            regex_search(linija,sim,regexSimbol);
                            if(lit.position()>sim.position()){//simbol kao operand
                            //cout<<"simbol"<<endl;
                                matchedVal=sim[0];
                                //cout<<"Simbol: "<<matchedVal<<endl;
                                //relokacija
                                Symbol* s=tabelaSimbola->find(matchedVal);
                                if(s != 0){
                                    obradiRelokacije(tekucaSekcija,s,0);
                                    //obradiRelokaciju(tekucaSekcija,s);
                                    dodajSadrzajUSekciju(tekucaSekcija,s,0);
                                    //dodajUSekciju(tekucaSekcija,s);
                                }else{
                                    //obracanje unapred
                                    int offset=tabelaSekcija->getNumOfBytesInSection(tekucaSekcija);
                                    listaObracanjaUnapred->dodaj(matchedVal,0,offset,2,tekucaSekcija,1);
                                    tabelaSekcija->addInSection(tekucaSekcija,"****");
                                    
                                }
                                linija=regex_replace(linija,regex(matchedVal),"",regex_constants::format_first_only);
                                //cout<<"Linija posle: "<<endl;

                            }else{//literal kao operand
                                matchedVal=lit[0];
                                //cout<<"Broj: "<<matchedVal<<endl;
                                literal=stoi(matchedVal);
                                string literalHex=convertDecToHex(literal,2);
                                //cout<<"Bajt: "<<literalHex<<endl;
                                tabelaSekcija->addInSection(tekucaSekcija,literalHex);
                                linija=regex_replace(linija,regex(matchedVal),"",regex_constants::format_first_only);
                                //cout<<"Linija posle: "<<endl;

                            }
                            //linija=regex_replace(linija,regex(matchedVal),"",regex_constants::format_first_only);
                            locationCounter += 2;
                            //cout<<"Linija posle: "<<linija<<endl;
                           
                        }
                        //cout<<"Location counter: "<<locationCounter<<endl;
                        //cout<<tabelaSekcija->toString()<<endl;
                        //cout<<tabelaRelokacija->toString()<<endl;
                        //exit(-1);
                        break;

                    } //word
                    case 6: {//skip
                            regex_search(linija,m,regex("[1-9][0-9]*"));
                            string skipValue=m[0];
                            int toSkip=stoi(skipValue);
                            for(int i=0;i<toSkip;i++){
                                tabelaSekcija->addInSection(tekucaSekcija, "00");
                                
                            }
                            locationCounter+=toSkip;
                            break;
                    }
                    case 7:  {
                        end=true;
                        break;
                    }//end
                    default: {
                        cout<<"Greska na liniji "<<brojLinije<<" ne postoji direktiva sa takvim nazivom"<<endl;
                        break;
                    }
                    
                }
                break;


            } //direktiva

            case 2: //instrukcija

            {   smatch i;
                string instrukcijaNaziv;
                //cout<<"instrukcija "<<brojLinije<<" "<<linija<<endl;
                regex regexinstrukcijaNaziv("^\\s*([a-z]{2,4}|[A-Z]{2,4})[bw]?");
                if(regex_search(linija,m,regex("^\\s*([a-z]{2,4}|[A-Z]{2,4})[bw]?"))){
                instrukcijaNaziv=m[0];
                }
                //cout<<"Naziv instrukcije: "<<instrukcijaNaziv<<":"<<endl;
                instrukcijaNaziv=removeSpace(instrukcijaNaziv);
                //cout<<"Naziv instrukcije:"<<instrukcijaNaziv<<":"<<endl;
                int kodInstrukcije=odrediOpCode(instrukcijaNaziv);
                //cout<<"Kod instrukcije: "<<kodInstrukcije<<endl;
                if(kodInstrukcije==-1){
                    cout<<"Greska na liniji "<<brojLinije<<" nepoznata instrukcija!"<<endl;
                    exit(-1);
                    break;
                }
                int brojOperanada=instructions[kodInstrukcije].numOfOperands;
                //cout<<"Broj operanda: "<<brojOperanada<<endl;
                //linija=regex_replace(linija,regex(instrukcijaNaziv),"");
                string prviBajtInstrukcije=binaryOpCodeFormat(kodInstrukcije);
                //exit(-1);
                if(odrediVelicinuOperandaInstrukcije(linija,brojOperanada,kodInstrukcije,instrukcijaNaziv)==1){
                    prviBajtInstrukcije+="0";
                }else if(odrediVelicinuOperandaInstrukcije(linija,brojOperanada,kodInstrukcije,instrukcijaNaziv)==2){
                    prviBajtInstrukcije+="1";
                }else{
                    break;
                }
                prviBajtInstrukcije+="00";
                //cout<<"Prvi bajt instrukcije: "<<prviBajtInstrukcije<<endl;
                int bajtBinarno=convertBinaryToDec(prviBajtInstrukcije);
                prviBajtInstrukcije=convertDecToHex(bajtBinarno,1);
                //cout<<"Prvi bajt hex: "<<prviBajtInstrukcije<<endl;
                //exit(-1);
                tabelaSekcija->addInSection(tekucaSekcija,prviBajtInstrukcije);
                locationCounter++;


                switch(brojOperanada){
                    case 0: //0 operanda
                    {   
                        /*tabelaSekcija->addInSection(tekucaSekcija,prviBajtInstrukcije);
                        locationCounter++;*/
                        break;
                    }
                    case 1: // 1 operand
                    {
                        //tabelaSekcija->addInSection(tekucaSekcija,prviBajtInstrukcije);
                        //cout<<"Instrukcija sa 1 operandom"<<endl;
                        prviBajtInstrukcije="";
                        string operand;
                        linija=regex_replace(linija,regex(instrukcijaNaziv),"");
                        //cout<<"Linija posle uklanjanja naziva instrukcije: "<<linija<<endl;
                        operand=regex_replace(linija,regex("\\s*"),"");
                        //cout<<"Operand:"<<operand<<":"<<endl;
                        //exit(-1);
                        for(int i=0;i<operand.size();i++){
                            if(operand[i]==','){
                                cout<<"Greska na liniji "<<brojLinije<<" pogresan broj operanda za datu instrukciju!"<<endl;
                                break;
                            }
                        }
                        int tipAdresiranja=odrediAdresiranje2(operand,kodInstrukcije);
                        //cout<<"Tip adresiranja: "<<tipAdresiranja<<endl;
                        //exit(-1);
                        switch(tipAdresiranja){
                            case 0:{//neposredno
                                    //cout<<"neposredno"<<endl;
                                    smatch j;
                                    prviBajtInstrukcije=odrediformatAdrese(0)+"00000";
                                    prviBajtInstrukcije=convertDecToHex(convertBinaryToDec(prviBajtInstrukcije),1);
                                    //cout<<"Drugi bajt instrukcije: "<<prviBajtInstrukcije<<endl;
                                    tabelaSekcija->addInSection(tekucaSekcija,prviBajtInstrukcije);
                                    locationCounter++;
                                    prviBajtInstrukcije="";
                                    operand=regex_replace(operand,regex("\\$"),"");
                                    if(regex_match(operand,m,regex("\\s*-?[1-9][0-9]*\\s*"))){
                                        //obradiLiteralOperand(m[0]);
                                        string broj=m[0];
                                        broj=regex_replace(broj,regex("\\$"),"");
                                        broj=regex_replace(broj,regex("\\s*"),"");
                                        obradiLiteral(broj);
                                    }else if(regex_match(operand,m,regex("\\s*0x[0-9a-fA-F]{1,4}\\s*"))){
                                        string literal=m[0];
                                        literal=regex_replace(literal,regex("0x"),"");
                                        literal=regex_replace(literal,regex("\\s*"),"");
                                        if(literal.size()==1){
                                            literal="0"+literal;
                                        }
                                        tabelaSekcija->addInSection(tekucaSekcija,literal);
                                        if(literal.size()>2){
                                            locationCounter+=2;
                                        }else{
                                            locationCounter+=1;
                                        }
                                        
                                    
                                        }else if(regex_search(operand,m,regex("[a-zA-Z_][a-zA-Z_0-9]*"))){ 
                                        //labela
                                        string labela=m[0];
                                        labela=regex_replace(labela,regex("\\$"),"");
                                        Symbol* s=tabelaSimbola->find(labela);
                                        if(s!=0){
                                            obradiRelokacije(tekucaSekcija,s,0);
                                            dodajSadrzajUSekciju(tekucaSekcija,s,0);
                                            //dodajUSekciju(tekucaSekcija,s);
                                            
                                            //obradiRelokaciju(tekucaSekcija,s);
                                            locationCounter+=2;

                                        }else{
                                            //obracanje unapred
                                            int offset=tabelaSekcija->getNumOfBytesInSection(tekucaSekcija);
                                            listaObracanjaUnapred->dodaj(labela,0,offset,2,tekucaSekcija,1);
                                            tabelaSekcija->addInSection(tekucaSekcija,"****");
                                            locationCounter+=2;
                                        }

                                    }
                                    break;

                            }
                            case 1:{//registarsko direktno
                                //cout<<"registarsko direktno"<<endl;
                                int brojRegistra;
                                if(regex_search(operand,m,regex("[0-7]"))){
                                    brojRegistra=stoi(m[0]);
                                }else if(regex_search(operand,m,regex("sp"))){
                                    brojRegistra=stoi("6");
                                }else if(regex_search(operand,m,regex("pcw"))){
                                    brojRegistra=stoi("15");
                                }
                                //cout<<"Broj registra:"<<brojRegistra<<endl;
                                string bitLH;
                                if(regex_search(operand,regex("\\%r[0-7]h"))){
                                    bitLH="1";
                                }else if(regex_search(operand,regex("\\%r[0-7]l"))){
                                    bitLH="0";
                                }else{
                                    bitLH="0";
                                }
                                prviBajtInstrukcije=odrediformatAdrese(1)+kodirajRegistar(brojRegistra)+bitLH;
                                //cout<<"Drugi bajt instrukcije binarno: "<<prviBajtInstrukcije<<endl;
                                prviBajtInstrukcije=convertDecToHex(convertBinaryToDec(prviBajtInstrukcije),1);
                                //cout<<"Drugi bajt instrukcije hex: "<<prviBajtInstrukcije<<endl;
                                tabelaSekcija->addInSection(tekucaSekcija,prviBajtInstrukcije);
                                locationCounter++;
                                break;

                            }
                            case 2:{//registarsko indirektno
                                //cout<<"registarsko indirektno"<<endl;
                                int brojRegistra;
                                if(regex_search(operand,m,regex("[0-7]"))){
                                    brojRegistra=stoi(m[0]);
                                }else if(regex_search(operand,m,regex("sp"))){
                                    brojRegistra=stoi("6");
                                }
                                prviBajtInstrukcije=odrediformatAdrese(2)+kodirajRegistar(brojRegistra)+"0";
                                //cout<<"drugi bajt bin: "<<prviBajtInstrukcije<<endl;
                                prviBajtInstrukcije=convertDecToHex(convertBinaryToDec(prviBajtInstrukcije),1);
                                //cout<<"drugi bajt hexa: "<<prviBajtInstrukcije<<endl;
                                tabelaSekcija->addInSection(tekucaSekcija,prviBajtInstrukcije);
                                locationCounter++;
                                break;
                            }
                            case 3:{//reg ind 16
                                //cout<<"Reg ind 16"<<endl;
                                int brojRegistra;
                                if(regex_match(operand,m,regex("\\*?[a-zA-Z_][a-zA-Z_0-9]*\\(%(pc|r7)\\)"))){//pc relativno adresiranje
                                        //cout<<"pc relativno"<<endl;
                                        smatch n;
                                        string matchedValue=m[0];
                                        //cout<<"matched val:"<<matchedValue<<endl;
                                        regex_search(matchedValue,n,regex("[a-zA-Z_][a-zA-Z_0-9]*"));
                                        string simbol=n[0];
                                        //cout<<"Simbol:"<<simbol<<":"<<endl;
                                        prviBajtInstrukcije=odrediformatAdrese(3)+kodirajRegistar(7)+"0";
                                        prviBajtInstrukcije=convertDecToHex(convertBinaryToDec(prviBajtInstrukcije),1);
                                        //cout<<"Drugi bajt instrukcije: "<<prviBajtInstrukcije<<endl;
                                        tabelaSekcija->addInSection(tekucaSekcija,prviBajtInstrukcije);
                                        locationCounter++;
                                        Symbol* s=tabelaSimbola->find(simbol);
                                        if(s != 0){
                                            obradiRelokacije(tekucaSekcija,s,1);
                                            dodajSadrzajUSekciju(tekucaSekcija,s,1);
                                            
                                        }else{
                                            //obracanje unapred
                                            int offset=tabelaSekcija->getNumOfBytesInSection(tekucaSekcija);
                                            listaObracanjaUnapred->dodaj(simbol,0,offset,2,tekucaSekcija,2);
                                            tabelaSekcija->addInSection(tekucaSekcija,"****");
                                        }
                                        locationCounter+=2;

                                }else{//apsolutno
                                    //cout<<"apsolutno adresiranje"<<endl;
                                    if(regex_search(operand,m,regex("\\(%r[0-6]\\)"))){
                                       string brojReg = m[0];
                                       regex_search(brojReg,m,regex("[0-6]"));
                                       brojRegistra=stoi(m[0]);
                                       
                                       operand=regex_replace(operand,regex("\\(%r[0-6]\\)"),"",regex_constants::format_first_only);
                                    } else if(regex_search(operand,m,regex("\\(%sp\\)"))){
                                    brojRegistra=stoi("6");
                                     operand=regex_replace(operand,regex("\\(%sp\\)"),"",regex_constants::format_first_only);
                                 
                                }
                                     prviBajtInstrukcije=odrediformatAdrese(3)+kodirajRegistar(brojRegistra)+"0";
                                     prviBajtInstrukcije=convertDecToHex(convertBinaryToDec(prviBajtInstrukcije),1);
                                     //cout<<"Drugi bajt instrukcije hexa: "<<prviBajtInstrukcije<<endl;
                                     tabelaSekcija->addInSection(tekucaSekcija,prviBajtInstrukcije);
                                     locationCounter++;
                                     if(regex_match(operand,m,regex("\\s*-?[1-9][0-9]*\\s*"))){
                                         //literal kao displacement
                                         int literal=stoi(m[0]);
                                         prviBajtInstrukcije=convertDecToHex(literal,2);
                                         tabelaSekcija->addInSection(tekucaSekcija,prviBajtInstrukcije);
                                     }else if(regex_match(operand,m,regex("\\s*0x[0-9a-fA-F]{1,4}\\s*"))){
                                        string literal=m[0];
                                        literal=regex_replace(literal,regex("0x"),"");
                                        literal=regex_replace(literal,regex("\\s*"),"");
                                        literal=formatirajLiteral(literal);
                                        tabelaSekcija->addInSection(tekucaSekcija,literal);
                                       
                                     }else if(regex_search(operand,m,regex("[a-zA-Z_][a-zA-Z_0-9]*"))){
                                         //simbol kaodisplacement
                                         string labela=m[0];
                                         Symbol* s=tabelaSimbola->find(labela);
                                         if(s!=0){
                                             obradiRelokacije(tekucaSekcija,s,0);
                                             dodajSadrzajUSekciju(tekucaSekcija,s,0);
                                             
                                         }else{
                                             //obracanje unapred
                                             int offset=tabelaSekcija->getNumOfBytesInSection(tekucaSekcija);
                                             listaObracanjaUnapred->dodaj(labela,0,offset,2,tekucaSekcija,1);
                                             tabelaSekcija->addInSection(tekucaSekcija,"****");
                                         }
                                     }
                                     locationCounter+=2;
                                      


                                }//apsolutno adresiranje
                                break;

                            }
                            case 4:{//memorijsko
                                //cout<<"memorijsko"<<endl;
                                prviBajtInstrukcije=odrediformatAdrese(4)+"00000";
                                prviBajtInstrukcije=convertDecToHex(convertBinaryToDec(prviBajtInstrukcije),1);
                                //cout<<"Drugi bajt instrukcije: "<<prviBajtInstrukcije<<endl;
                                tabelaSekcija->addInSection(tekucaSekcija,prviBajtInstrukcije);
                                locationCounter++;
                                operand=regex_replace(operand,regex("\\*"),"");
                                if(regex_match(operand,m,regex("\\s*0x[0-9a-fA-F]{1,4}\\s*"))){
                                    //literal
                                    string literal=m[0];
                                    literal=regex_replace(literal,regex("\\s*"),"");
                                    literal=regex_replace(literal,regex("0x"),"");
                                    literal=formatirajLiteral(literal);
                                    tabelaSekcija->addInSection(tekucaSekcija,literal);
                                    

                                    
                                    
                                }else if(regex_match(operand,m,regex("\\s*[1-9][0-9]*\\s*"))){
                                    string broj=m[0];
                                    broj=regex_replace(broj,regex("\\s*"),"");
                                    string brojHex=convertDecToHex(stoi(broj),2);
                                    tabelaSekcija->addInSection(tekucaSekcija,brojHex);

                                }
                                else if(regex_match(operand,m,regex("\\s*[a-zA-z_][a-zA-Z_0-9]*\\s*"))){

                                    //simbol
                                    string labela=m[0];
                                    labela=regex_replace(labela,regex("\\s*"),"");
                                    Symbol* s=tabelaSimbola->find(labela);
                                    if(s!=0){
                                        obradiRelokacije(tekucaSekcija,s,0);
                                        dodajSadrzajUSekciju(tekucaSekcija,s,0);
                                        
                                    }else{
                                        int offset = tabelaSekcija->getNumOfBytesInSection(tekucaSekcija);
                                        listaObracanjaUnapred->dodaj(labela,0,offset,2,tekucaSekcija,1);
                                        tabelaSekcija->addInSection(tekucaSekcija,"****");
                                    }
                                }
                                locationCounter+=2;
                                break;
                            }
                            default: {
                                cout<<"Greska na liniji "<<brojLinije<<" neodgovarajuci format operanda!"<<endl;
                                break;
                            }

                        }//switch
                        break;
                    }//1 operand
                    case 2: {//2 operanda
                        //cout<<"Instrukcija sa 2 operanda"<<endl;
                        
                        string operandi,op1,op2;
                        linija=regex_replace(linija,regex(instrukcijaNaziv),"");
                        //cout<<"Linija posle uklanjanja naziva instrukcije:"<<linija<<":"<<endl;
                        operandi=regex_replace(linija,regex("\\s*"),"");
                        //cout<<"Operandi:"<<operandi<<endl;
                        
                        int pozicija;
                        for(unsigned int i=0;i<operandi.size();i++){
						if(operandi[i]==','){
							pozicija=i;
							break;
						}
					}
                        op1=operandi.substr(0,pozicija);//src
                        //cout<<"Operand 1:"<<op1<<":"<<endl;
                        op2=operandi.substr(pozicija+1,operandi.length()-pozicija-1);//dst
                        //cout<<"Operand 2:"<<op2<<":"<<endl;
                        int adr1=odrediAdresiranje2(op1,kodInstrukcije);
                        //cout<<"Adresiranje 1:"<<adr1<<endl;
                        int adr2=odrediAdresiranje2(op2,kodInstrukcije);
                        //cout<<"Adresiranje 2:"<<adr2<<endl;
                        if(adr2==0){
                            cout<<"Greska na liniji "<<brojLinije<<" neposredna vrednost ne sme biti odredisni operand"<<endl;
                            break;
                        }
                        if((adr1==2 || adr1==3 || adr1==4) && (adr2==2 || adr2==3 || adr2==4)){
                            cout<<"Greska na liniji "<<brojLinije<<" nisu dozvoljena oba memorijska operanda u instrukciji"<<endl;
                            break;
                        }
                        //exit(-1);
                        obradiOperandInstrukcije(op1,1,kodInstrukcije);
                        obradiOperandInstrukcije(op2,2,kodInstrukcije);
                      


                    break;
                    }
                    default:{
                        cout<<"Greska na liniji "<<brojLinije<<" nedozvoljen broj operanada za instrukciju"<<endl;
                        break;
                    } //greska
                }

                

                break;
            }

            default:{
                cout<<"Greska na  liniji  "<<brojLinije<<" ne prepoznajem format procitanog (nije ni labela,ni direktiva, ni instrukcija, ni komentar)"<<endl;
                break;
            } //greska
        } //odredjivanje tipa linije

        if(end){
            break;
        }



    }//while getline
    //obradi tabelu equ simbola
   
    tabelaEquSimbola->izracunajVrednostEquSimbola();
    //obradi tabelu obracanja unapred
    listaObracanjaUnapred->obradiListu();
    //ispis

    stringstream ss;
    ss<<tabelaSimbola->toString();
    ss<<endl;
    ss<<tabelaSekcija->toString();
    ss<<endl;
    ss<<tabelaRelokacija->toString();
    
    //maybe u should have deallocated here my structures in dinamic memory


    return ss.str();
}//one-pass assembler