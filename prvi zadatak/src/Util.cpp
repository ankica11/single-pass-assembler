#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include "Global.h"
#include "SymbolTable.h"
#include "SectionTable.h"
#include "RelocationTableList.h"
#include "TabelaObracanjaUnapredLista.h"

using namespace std;

extern SymbolTable* tabelaSimbola;
extern RelocationTableList* tabelaRelokacija;
extern SectionTable* tabelaSekcija;
extern TabelaObracanjaUnapredLista* listaObracanjaUnapred;
extern int brojLinije;
extern int locationCounter;
extern string tekucaSekcija;

extern void obradiRelokaciju(string sekcija,Symbol* simbol);
extern void obradiPCRelokaciju(string sekcija,Symbol* s);
extern void dodajUSekcijuPCrel(string sekcija,Symbol* simbol);
extern void obradiLiteralOperand(string literal);
extern void dodajUSekciju(string sekcija,Symbol* simbol);

vector<string> split(string s,char delimiter){
    vector<string> tokens;
    string tokenstr;
    string token;
    istringstream tokenStream(s);
    
    while(getline(tokenStream,token,delimiter)){
        tokens.push_back(token);
    }
    return tokens;

}

/*void dodajUSekcijuPCrel2(string sekcija,Symbol* simbol){
    int offset;
    string offsetHex;
    if(simbol->visible=='g'){
        offset=-3;
    }else if(simbol->visible=='l'){
        offset=simbol->value-3;
    }
    offsetHex=convertDecToHex(offset,2);
    tabelaSekcija->addInSection(sekcija,offsetHex);
}*/

/*void dodajUSekcijuPCrel2OnSpecificLocation(string sekcija,Symbol* simbol,int address){
    int offset;
    string offsetHex;
    if(simbol->visible=='g'){
        offset=-3;
    }else if(simbol->visible=='l'){
        offset=simbol->value-3;
    }
    offsetHex=convertDecToHex(offset,2);
    tabelaSekcija->addInSectionInSpecificAddress(sekcija,offsetHex,address);
}*/

bool checkIfJumpInstruction(int opCode){
    if(opCode==3 || opCode==4 || opCode==5 || opCode==6 || opCode==7 || opCode==8){
        return true;
    }
    return false;
}




string removeSpace(string line){
    int count=0;
    for(int i=0; i<line.length(); i++){
        if(line[i]==' ' || line[i]=='\t'){
            count++;
        }else{
            break;
        }

    }
    line=line.substr(count,line.length());
    return line;
}


int odrediOpCode(string line){
    smatch m;
    int opCode;
    line=removeSpace(line);
    line=regex_replace(line,regex("\\s*"),""); //uklanja spaceove s kraja
    regex_search(line,m,regex("([a-z]{2,4}|[A-Z]{2,4})[bw]?\\s*"));
    string instruction=m[0];
    instruction=regex_replace(instruction,regex("\\s*"),"");
    //cout<<"Hello from odrediOpcode: "<<instruction<<endl;
    char sufix=instruction.back();
    if(sufix=='b' || sufix=='w'){
        if(instruction.compare("subb")==0){
            instruction="sub";
        }else{
            if(!(instruction.compare("sub")==0)){
            instruction=instruction.substr(0,instruction.length()-1);}
        }
    }
    for(int i=0; i<25; i++){
        if(instruction.compare(instructions[i].name)==0){
            return i;
        }
    }
    return -1;
}


string binaryOpCodeFormat(int opcode){
    switch(opcode){
        case 0: return "00000";
        case 1: return "00001";
        case 2: return "00010";
        case 3: return "00011";
        case 4: return "00100";
        case 5: return "00101";
        case 6: return "00110";
        case 7: return "00111";
        case 8: return "01000";
        case 9: return "01001";
        case 10: return "01010";
        case 11: return "01011";
        case 12: return "01100";
        case 13: return "01101";
        case 14: return "01110";
        case 15: return "01111";
        case 16: return "10000";
        case 17: return "10001";
        case 18: return "10010";
        case 19: return "10011";
        case 20: return "10100";
        case 21: return "10101";
        case 22: return "10110";
        case 23: return "10111";
        case 24: return "11000";

    }
}


int odrediAdresiranje(string operand){
    regex neposredno("\\$?([0-9][0-9]*|[a-zA-Z_][a-zA-Z_0-9]*)");
    regex registarskoDirektno("\\*?(%r[0-7](h|l)?)");
    regex registarskoIndirektno("\\*?\\(%r[0-7]\\)");
    regex registarskoIndirektno16("\\*?(([0-9][0-9]*|0x[0-9a-fA-F][0-9a-fA-F]{3})|[a-zA-Z_][a-zA-Z_0-9]*)\\(%(r[0-7]|pc)\\)");
    regex memorijsko("");
    return 0;
}



int odrediTipLinije(string linija){
    //cout<<"tip"<<endl;
    regex prazanRed("^\\s*$"); 
    regex direktiva("^\\s*\\.[a-z]+(\\s*[_a-zA-Z0-9]+,?)*\\s*");
    regex direktiva3("^\\s*\\.[a-z]+\\s*");
    regex instrukcija("^\\s*([a-z]{2,4}|[A-Z]{2,4})[bw]?(\\s*[\\*\\$\\(\\)%_a-zA-Z0-9]*,?){0,2}\\s*");
    regex direktiva2("\\s*\\.[a-z]+\\s*([a-zA-Z\" ,0-9%\\W]*|(0|-?[1-9][0-9]*)( *, *(0|-?[1-9][0-9]*))*)");
    regex instrukcija2("");


    regex regeksi[3];
    regeksi[0]=prazanRed;
    regeksi[1]=direktiva;
    regeksi[2]=instrukcija;

    smatch m,n,p;

    if(regex_search(linija,m,prazanRed)){
        
        return 0;
    }
    if(regex_search(linija,m,direktiva3)){
        //cout<<m[0];
        return 1;
    }
    if(regex_search(linija,m,instrukcija)){
        //cout<<m[0];
        return 2;
    }

    return -1;


}


string convertDecToHex(int broj,int size) //size 1=1B , 2=2B
{
	stringstream ss;
	ss<<hex<<broj;
	string hexa ( ss.str() );
	for(unsigned int i=0;i<hexa.length();i++){
		hexa[i]=toupper(hexa[i]);
	}
	if(broj<0){
        string pola;
        if(size==2){
		pola=hexa.substr(hexa.length()/2,hexa.length());}else if(size==1){
            pola=hexa.substr(6,hexa.length());
        }
		return pola;
	}
	if(hexa.length()==1 && size==1){
		return "0"+hexa;
	}
	if(hexa.length()==1 && size==2){
		return "000"+hexa;
	}
	if(hexa.length()==2 && size==2){
		return "00"+hexa;
	}
	if(hexa.length()==3 && size==2){
		return "0"+hexa;
	}
	return hexa;

}

void obradiLiteral(string literal){
    int broj=stoi(literal);
    string bajtInstrukcije;
    if(broj<128 && broj>-128){
        bajtInstrukcije=convertDecToHex(broj,1);
        locationCounter+=1;
    }else{
        bajtInstrukcije=convertDecToHex(broj,2);
        locationCounter+=2;
    }
    tabelaSekcija->addInSection(tekucaSekcija,bajtInstrukcije);
}

int odrediVelicinuOperandaInstrukcije(){
    return 0;
}

int odrediAdresiranje2(string operand, int kodInstrukcije){
   
    regex neposredno_pristup("\\s*\\$(([a-zA-Z_][a-zA-Z_0-9]*)|(-?[1-9][0-9]*)|(0x[0-9a-fA-F]{1,4}))\\s*");
    regex neposredno_skok("\\s*(([a-zA-Z_][a-zA-Z_0-9]*)|([1-9][0-9]*)|(0x[0-9a-fA-F]{1,4}))\\s*");
    regex registarskoDirektno("\\*?%((r[0-7][hl]?)|sp|pcw)");
    regex registarskoIndirektno("\\*?\\(%(r[0-7]|sp)\\)");
    regex registarskoIndirektno16("\\s*\\*?((-?[1-9][0-9]*)|(0x[0-9a-fA-F]{1,4})|([a-zA-Z_][a-zA-Z_0-9]*))\\(%(r[0-7]|pc|sp)\\)\\s*");
    //regex registarskoIndirektno16("\\*?((-?[1-9][0-9]*|0x[0-9a-fA-F][0-9a-fA-F]{3})|[a-zA-Z_][a-zA-Z_0-9]*)\\(%(r[0-7]|pc)\\)");
    //regex memorijsko_pristup("([1-9][0-9]*|[a-zA-Z_][a-zA-Z_0-9]*|0x[0-9a-fA-F][0-9a-fA-F]{1,4})");
    //regex memorijsko_skok("\\*([1-9][0-9]*|[a-zA-Z_][a-zA-Z_0-9]*|0x[0-9a-fA-F][0-9a-fA-F]{1,4})");
    regex memorijsko_pristup("\\s*(([1-9][0-9]*)|([a-zA-Z_][a-zA-Z_0-9]*)|(0x[0-9a-fA-F]{1,4}))\\s*");
    regex memorijsko_skok("\\s*\\*(([1-9][0-9]*)|([a-zA-Z_][a-zA-Z_0-9]*)|(0x[0-9a-fA-F]{1,4}))\\s*");
    smatch m;
    if(regex_match(operand,neposredno_pristup) || (regex_match(operand,neposredno_skok) && checkIfJumpInstruction(kodInstrukcije))){
        return 0;
    }
    if(regex_match(operand,registarskoDirektno)){
       
        return 1;
    }
    if(regex_match(operand,registarskoIndirektno)){
        return 2;
    }
    if(regex_match(operand,registarskoIndirektno16)){
        return 3;
    }
    if(regex_match(operand,memorijsko_pristup) || (regex_match(operand,memorijsko_skok) && checkIfJumpInstruction(kodInstrukcije))){
        return 4;
    }
    return -1;

}


int odrediVelicinuOperandaInstrukcije(string linija, int brojOperanada, int kodInstrukcije, string nazivInstrukcije){
    smatch n;
    regex imenaInstrukcijeB("pushb|popb|xchgb|movb|addb|subb|mulb|divb|cmpb|notb|andb|orb|xorb|testb|shlb|shrb");
    regex imenaInstrukcijeW("pushw|popw|xchgw|movw|addw|subw|mulw|divw|cmpw|notw|andw|orw|xorw|testw|shlw|shrw");
    if(regex_search(nazivInstrukcije,n,imenaInstrukcijeB)){
        return 1;
    }
    if(regex_search(nazivInstrukcije,n,imenaInstrukcijeW)){
        return 2;
    }
    regex regexinstrukcijaNaziv("^//s*([a-z]{2,4}|[A-Z]{2,4})[bw]?");
    linija=regex_replace(linija,regex(nazivInstrukcije),"");
    string operandi=regex_replace(linija,regex("\\s*"),"");
    
    smatch m;
    switch(brojOperanada){
        case 0:{
            return 1;
            break;
        }
        case 1:{
            int adresiranje=odrediAdresiranje2(operandi,kodInstrukcije);
            switch(adresiranje){
                case 0: {
                    
                    if(regex_search(operandi,m,regex("-?[1-9][0-9]*"))){
                        int literal = stoi(m[0]);
                        if(literal<128 && literal>-128){
                            return 1;
                        
                    }

                }else if(regex_search(operandi,m,regex("0x[0-9a-fA-F]{1,4}"))){
                    
                    string literalHex=m[0];
                    if(literalHex.size()<=4){
                        return 1;
                    }
                }
                return 2;
            }
            case 1:{
                if(regex_search(operandi,m,regex("%r[0-7](l|h)"))){
                    return 1;
                }else{
                    return 2;
                }
            }
            case 2:{
                return 2;
            }
            case 3:{
                return 2;
            }
            case 4:{
                return 2;
            }
            
        }
        break;
    }
    case 2:{ //2 operanda
        string op1,op2;
        int poz;
        for(int i=0; i<operandi.length();i++){
            if(operandi[i]==','){
                poz=i;
                break;
            }
        }
        op1=operandi.substr(0,poz); //src operand
        op2=operandi.substr(poz+1,operandi.length()-poz-1); //dst operand
        int adr1=odrediAdresiranje2(op1,kodInstrukcije); 
        int adr2=odrediAdresiranje2(op2,kodInstrukcije);
        /*if(adr1==0){
            if(regex_match(operandi,m,regex("-?[1-9][0-9]*"))){
                        int literal = stoi(m[0]);
                        if(literal<128 && literal>-128){
                            return 1;
                        
                    }

        }
        return 2;


    }*/
    if(adr2==0){
        //greska ne moze dst da bude neposredna vrednost
        cout<<"Greska na liniji "<<brojLinije<<" neposredna vrednost ne moze biti odredisni operand"<<endl;
    }
    if(adr2==1){
    if(regex_search(operandi,m,regex("%r[0-7](l|h)"))){
                    return 1;
                }else{
                    return 2;
                }

    }
     if(adr1==0){
            if(regex_search(operandi,m,regex("-?[1-9][0-9]*"))){
                        int literal = stoi(m[0]);
                        if(literal<128 && literal>-128){
                            return 1;
                        
                    }

        }else if(regex_search(operandi,m,regex("0x[0-9a-fA-F]{1,4}"))){
                    
                    string literalHex=m[0];
                    if(literalHex.size()<=4){
                        return 1;
                    }
                }
        return 2;


    }
    if(adr1==1){
        if(regex_search(operandi,m,regex("%r[0-7](l|h)"))){
                    return 1;
                }else{
                    return 2;
                }

    }
    

    if((adr1==2 || adr1==3 || adr1==4)&&(adr2==2 || adr2==3 || adr2==4)){
        cout<<"Greska na liniji "<<brojLinije<<" ne mogu oba operanda biti memorijskog tipa"<<endl;
        return -1;
        
    }
    return 2;
    break;

}
    default: {return -1;}
    }
}

string odrediformatAdrese(int tipAdresiranja){
    switch(tipAdresiranja){
        case 0:{
            return "000";
            break;
        }
        case 1:{
            return "001";
            break;
        }
        case 2:{
            return "010";
            break;
        }
        case 3:{
            return "011";
            break;
        }
        case 4:{
            return "100";
            break;
        }
        default: {
            return "%%%";
            cout<<"Nepoznat tip adresiaranja"<<endl;
        }
    };
}

int convertBinaryToDec(string binary){
    int decimalnaVr=0;
    int osnova=1;
    for(int i=binary.length()-1;i>=0;i--){
        if(binary[i]=='1'){
            decimalnaVr+=osnova;
        }
        osnova*=2;
    }
    return decimalnaVr;
}

string kodirajRegistar(int broj){
        switch(broj){
            case 0: return "0000";
            case 1: return "0001";
            case 2: return "0010";
            case 3: return "0011";
            case 4: return "0100";
            case 5: return "0101";
            case 6: return "0110";
            case 7: return "0111";
            case 15: return "1111";
            default: return "";
        }

}




string formatirajLiteral(string s){
	if(s.length()==1)
		return "000"+s;
	if(s.length()==2)
		return "00"+s;
	if(s.length()==3)
		return "0"+s;
	if(s.length()==4)
		return s;
}

void dodajSadrzajUSekciju(string ime_sekcije, Symbol* simbol, int tip){ //0 dodavanje apsolutnog simbol, 1- dodavanje pc rel tip 1, 2-dodavanje pc rel tip 2
    int offset;
    string offsetHexFormat;
    if(simbol->equSimbol==1){
        offset=simbol->value;
    }else{
        if(tip==0){//apsolutno adresiranje simbola
            if(simbol->visible=='g'){
                offset=0;
            }else if(simbol->visible=='l'){
                offset=simbol->value;
            }

        }
        if(tip==1){//pc rel tip 1 adresiranje
            if(simbol->visible=='g'){
                offset=-2;
            }else if(simbol->visible=='l'){
                offset=simbol->value-2;
            }

        }
        if(tip==2){//pc rel tip 2 adresiranje
            if(simbol->visible=='g'){
                offset=-3;
            }else if(simbol->visible=='l'){
                offset=simbol->value-3;
            }

        }
    }
    offsetHexFormat=convertDecToHex(offset,2);
    tabelaSekcija->addInSection(ime_sekcije, offsetHexFormat);



}

void obradiRelokacije2(string ime_sekcije, Symbol* simbol,int offset, int tip){//0-ABS relokacija, 1-PC rel 

    if(simbol->equSimbol==1){//za apsolutne equ simbole se ne prave relokacije to su obicne konstante
        return;
    }
    if(tip==1 && simbol->section.compare(ime_sekcije)==0){//ne pravi se relokacija ako je simbol definisan u istoj sekciji gde se koristi kao pc rel operand

        return;
    }
    string tipRelokacije;
    if(tip==0){
        tipRelokacije="ABS_16";
    }else if(tip==1){
        tipRelokacije="PC_R_16";
    }
    int offsetInSectionToFix=offset;
    string offsetInSectionToFixHexFormat=convertDecToHex(offsetInSectionToFix,2);
    if(simbol->visible=='g'){
        int rbrSimbola=simbol->number;
        tabelaRelokacija->addToRelocationTable(ime_sekcije,new Record(offsetInSectionToFixHexFormat,tipRelokacije,rbrSimbola));
    }else if(simbol->visible=='l'){
        Symbol* sekcija=tabelaSimbola->find(simbol->section);
        int rbrSekcije=sekcija->number;
        tabelaRelokacija->addToRelocationTable(ime_sekcije,new Record(offsetInSectionToFixHexFormat,tipRelokacije,rbrSekcije));
    }

}

void obradiRelokacije(string ime_sekcije, Symbol* simbol, int tip){//0-ABS relokacija, 1-PC rel 

    if(simbol->equSimbol==1){//za apsolutne equ simbole se ne prave relokacije to su obicne konstante
        return;
    }
    if(tip==1 && simbol->section.compare(ime_sekcije)==0){//ne pravi se relokacija ako je simbol definisan u istoj sekciji gde se koristi kao pc rel operand

        return;
    }
    string tipRelokacije;
    if(tip==0){
        tipRelokacije="ABS_16";
    }else if(tip==1){
        tipRelokacije="PCR_16";
    }
    int offsetInSectionToFix=tabelaSekcija->getNumOfBytesInSection(ime_sekcije);
    string offsetInSectionToFixHexFormat=convertDecToHex(offsetInSectionToFix,2);
    if(simbol->visible=='g'){
        int rbrSimbola=simbol->number;
        tabelaRelokacija->addToRelocationTable(ime_sekcije,new Record(offsetInSectionToFixHexFormat,tipRelokacije,rbrSimbola));
    }else if(simbol->visible=='l'){
        Symbol* sekcija=tabelaSimbola->find(simbol->section);
        int rbrSekcije=sekcija->number;
        tabelaRelokacija->addToRelocationTable(ime_sekcije,new Record(offsetInSectionToFixHexFormat,tipRelokacije,rbrSekcije));
    }

}

void dodajSadrzajUSekcijuNaOdredjenuLokaciju(string ime_sekcije,Symbol* simbol,int addr,int tip,int size){
    //0-abs adresiranje, 1-pc rel tip 1, 2-pc rel tip2
    int offset;
    string offsetHexFormat;
    if(simbol->equSimbol==1){
        offset=simbol->value;
    }else{
        if(tip==0){//abs
            if(simbol->visible=='g'){
                offset=0;
            }else if(simbol->visible=='l'){
                offset=simbol->value;
            }
        }
        if(tip==1){//pc rel tip 1
            if(simbol->visible=='g'){
                offset=-2;
            }else if(simbol->visible=='l'){
                offset=simbol->value-2;
            }
        }
        if(tip==2){//pc rel tip 2
            if(simbol->visible=='g'){
                offset=-3;
            }else{
                offset=simbol->value-3;
            }
        }
    }
    if(size==1){
    offsetHexFormat=convertDecToHex(offset,1);}
    else if(size==2){
    offsetHexFormat=convertDecToHex(offset,2);
    }
    tabelaSekcija->addInSectionInSpecificAddress(ime_sekcije,offsetHexFormat,addr);
}






void obradiOperandInstrukcije(string operand,int rbrOperanda,int kodInstrukcije){
    smatch m;
    smatch j;
    string bajtInstrukcije;
    int tipAdresiranja=odrediAdresiranje2(operand,kodInstrukcije);
    switch(tipAdresiranja){
        case 0:{
            //cout<<"neposredno"<<endl;
            if(rbrOperanda==2){
                cout<<"Greska na liniji "<<brojLinije<<" neposredna vrednost ne moze biti destinacioni operand"<<endl;
                return;
            }
            
            bajtInstrukcije=odrediformatAdrese(0)+"00000";
            bajtInstrukcije=convertDecToHex(convertBinaryToDec(bajtInstrukcije),1);
            tabelaSekcija->addInSection(tekucaSekcija,bajtInstrukcije);
            locationCounter++;
            operand=regex_replace(operand,regex("\\$"),"");
            if(regex_match(operand,m,regex("\\s*-?[1-9][0-9]*\\s*"))){
                                        //obradi literal
                                    string broj=m[0];
                                    broj=regex_replace(broj,regex("\\s*"),"");
                                    //cout<<"operand:"<<broj<<endl;
                                    obradiLiteral(broj);
                                    }else if(regex_match(operand,m,regex("\\s*0x[0-9a-fA-F]{1,4}\\s*"))){
                                        string literal=m[0];
                                        literal=regex_replace(literal,regex("0x"),"");
                                        literal=regex_replace(literal,regex("\\s*"),"");
                                        //cout<<"operand:"<<literal<<endl;
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
                                        //cout<<"operand:"<<labela<<endl;
                                        Symbol* s=tabelaSimbola->find(labela);
                                        if(s!=0){
                                            obradiRelokacije(tekucaSekcija,s,0);
                                            dodajSadrzajUSekciju(tekucaSekcija,s,0);
                                            
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
        }//neposredno
        case 1:{                //cout<<"reg dir"<<endl;
                                int brojRegistra;
                                if(regex_search(operand,m,regex("[0-7]"))){
                                    brojRegistra=stoi(m[0]);
                                }else if(regex_search(operand,m,regex("sp"))){
                                    brojRegistra=stoi("6");
                                }else if(regex_search(operand,m,regex("pcw"))){
                                    brojRegistra=stoi("15");
                                }
                                string bitLH;
                                if(regex_search(operand,regex("\\%r[0-7]h"))){
                                    bitLH="1";
                                }else if(regex_search(operand,regex("\\%r[0-7]l"))){
                                    bitLH="0";
                                }else{
                                    bitLH="0";
                                }
                                bajtInstrukcije=odrediformatAdrese(1)+kodirajRegistar(brojRegistra)+bitLH;
                                bajtInstrukcije=convertDecToHex(convertBinaryToDec(bajtInstrukcije),1);
                                tabelaSekcija->addInSection(tekucaSekcija,bajtInstrukcije);
                                locationCounter++;
            break;
        }//reg dir
        case 2:{
                                //cout<<"reg ind"<<endl;
                                int brojRegistra;
                                if(regex_search(operand,m,regex("[0-7]"))){
                                    brojRegistra=stoi(m[0]);
                                }else if(regex_search(operand,m,regex("sp"))){
                                    brojRegistra=stoi("6");
                                }
                                bajtInstrukcije=odrediformatAdrese(2)+kodirajRegistar(brojRegistra)+"0";
                                bajtInstrukcije=convertDecToHex(convertBinaryToDec(bajtInstrukcije),1);
                                tabelaSekcija->addInSection(tekucaSekcija,bajtInstrukcije);
                                locationCounter++;
            break;
        }//reg ind
        case 3:{
                                //cout<<"reg ind 16"<<endl;
                                int brojRegistra;
                                if(regex_match(operand,m,regex("\\*?[a-zA-Z_][a-zA-Z_0-9]*\\(%(pc|r7)\\)"))){//pc relativno adresiranje
                                       // cout<<"pc rel"<<endl;
                                        string matchedValue=m[0];
                                        regex_search(matchedValue,m,regex("[a-zA-Z_][a-zA-Z_0-9]*"));
                                        string simbol=m[0];
                                        //cout<<"operand:"<<simbol<<endl;
                                        bajtInstrukcije=odrediformatAdrese(3)+kodirajRegistar(7)+"0";
                                        bajtInstrukcije=convertDecToHex(convertBinaryToDec(bajtInstrukcije),1);
                                        tabelaSekcija->addInSection(tekucaSekcija,bajtInstrukcije);
                                        locationCounter++;
                                        Symbol* s=tabelaSimbola->find(simbol);
                                        if(s != 0){
                                            //dodajUSekcijuPCrel(tekucaSekcija,s);
                                            if(rbrOperanda==1){
                                            obradiRelokacije(tekucaSekcija,s,1);
                                            dodajSadrzajUSekciju(tekucaSekcija,s,2);
                                            
                                            }
                                            else if(rbrOperanda==2){
                                                obradiRelokacije(tekucaSekcija,s,1);
                                                dodajSadrzajUSekciju(tekucaSekcija,s,1);
                                                
                                            }
                                        }else{
                                            //obracanje unapred
                                            int offset=tabelaSekcija->getNumOfBytesInSection(tekucaSekcija);
                                            if(rbrOperanda==1){
                                            listaObracanjaUnapred->dodaj(simbol,0,offset,2,tekucaSekcija,3);
                                            }else if(rbrOperanda==2){
                                                listaObracanjaUnapred->dodaj(simbol,0,offset,2,tekucaSekcija,2);
                                            }
                                            tabelaSekcija->addInSection(tekucaSekcija,"****");
                                        }
                                        locationCounter+=2;}
                                        else{
                                            //apsolutno
                                        //cout<<"apsolutno"<<endl;
                                       if(regex_search(operand,m,regex("\\(%r[0-6]\\)"))){
                                       string brojReg = m[0];

                                       regex_search(brojReg,m,regex("[0-6]"));
                                       
                                       brojRegistra=stoi(m[0]);
                                       //cout<<"br reg:"<<brojRegistra<<endl;
                                       
                                       operand=regex_replace(operand,regex("\\(%r[0-6]\\)"),"",regex_constants::format_first_only);
                                    }else if(regex_search(operand,m,regex("\\(%sp\\)"))){
                                    brojRegistra=stoi("6");
                                     operand=regex_replace(operand,regex("\\(%sp\\)"),"",regex_constants::format_first_only);
                                 
                                }
                                     //cout<<"operand posle:"<<operand<<endl;
                                     bajtInstrukcije=odrediformatAdrese(3)+kodirajRegistar(brojRegistra)+"0";
                                     //cout<<"drugi bajt instr: "<<bajtInstrukcije<<endl;
                                     bajtInstrukcije=convertDecToHex(convertBinaryToDec(bajtInstrukcije),1);
                                     tabelaSekcija->addInSection(tekucaSekcija,bajtInstrukcije);
                                     locationCounter++;
                                     if(regex_match(operand,m,regex("\\s*-?[1-9][0-9]*\\s*"))){
                                         //literal kao displacement
                                         int literal=stoi(m[0]);
                                         //cout<<"literal "<<literal<<endl;
                                         bajtInstrukcije=convertDecToHex(literal,2);
                                         tabelaSekcija->addInSection(tekucaSekcija,bajtInstrukcije);
                                     }else if(regex_match(operand,m,regex("\\s*0x[0-9a-fA-F]{1,4}\\s*"))){
                                        string literal=m[0];
                                        literal=regex_replace(literal,regex("0x"),"");
                                        literal=regex_replace(literal,regex("\\s*"),"");
                                        literal=formatirajLiteral(literal);
                                        //cout<<"literal "<<literal<<endl;
                                        tabelaSekcija->addInSection(tekucaSekcija,literal);
                                       
                                     
                                     
                                     
                                     }else if(regex_search(operand,m,regex("[a-zA-Z_][a-zA-Z_0-9]*"))){
                                         //simbol kao displacement
                                         string labela=m[0];
                                         //cout<<"operand "<<labela<<endl;
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
                                      
                                        }


            break;
        }//reg ind 16
        case 4:{
                                //cout<<"memorijsko"<<endl;
                                bajtInstrukcije=odrediformatAdrese(4)+"00000";
                                bajtInstrukcije=convertDecToHex(convertBinaryToDec(bajtInstrukcije),1);
                                tabelaSekcija->addInSection(tekucaSekcija,bajtInstrukcije);
                                locationCounter++;
                                operand=regex_replace(operand,regex("\\*"),"");
                                if(regex_match(operand,m,regex("\\s*0x[0-9a-fA-F]{1,4}\\s*"))){
                                    //literal
                                    string literal=m[0];
                                    literal=regex_replace(literal,regex("\\s*"),"");
                                    literal=regex_replace(literal,regex("0x"),"");
                                    //cout<<"operand:"<<literal<<endl;
                                    literal=formatirajLiteral(literal);
                                    tabelaSekcija->addInSection(tekucaSekcija,literal);
                                    

                                    
                                    
                                }else if(regex_match(operand,m,regex("\\s*[1-9][0-9]*\\s*"))){
                                    string broj=m[0];
                                    broj=regex_replace(broj,regex("\\s*"),"");
                                    //cout<<"operand:"<<broj<<endl;
                                    string brojHex=convertDecToHex(stoi(broj),2);
                                    tabelaSekcija->addInSection(tekucaSekcija,brojHex);

                                
                                
                                
                                
                                }else if(regex_match(operand,m,regex("\\s*[a-zA-z_][a-zA-Z_0-9]*\\s*"))){

                                    //simbol
                                    string labela=m[0];
                                    labela=regex_replace(labela,regex("//s*"),"");
                                    //cout<<"operand:"<<labela<<endl;
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
        }//mem
        default: {
             cout<<"Greska na liniji "<<brojLinije<<" neodgovarajuci format operanda!"<<endl;
                               
            break;
        }//error
    }


}

int izracunajIzraz(vector<string> izraz){
    int rezultat=0;
    string prevOp;
    for(int i=0; i<izraz.size();i++){
        if(izraz[i]=="+"){
            prevOp="+";
            continue;
        }else if(izraz[i]=="-"){
            prevOp="-";
            continue;
        }
        string ime=regex_replace(izraz[i],regex("\\s*"),"");
        Symbol* s=tabelaSimbola->find(ime);
        if(s==0){
            cout<<"Greska na liniji "<<brojLinije<<" simbol nije definisan"<<endl;
            break;
        }
        if(prevOp=="-"){
            rezultat-=s->value;
        }else{
            rezultat+=s->value;

        }

        
    }
    return rezultat;

}

bool proveriSekcije(vector<string> izraz){
    
    bool same=true;
    string section="";
    for(int i=0;i<izraz.size();i++){
        if(izraz[i]=="-" || izraz[i]=="+"){
            continue;
        }
        string ime=regex_replace(izraz[i],regex("\\s*"),"");
        Symbol* s1=tabelaSimbola->find(ime);
        
       
        
       if(s1==0){
            cout<<"Greska na liniji "<<brojLinije<<" simbol nije definisan"<<endl;
            return false;
        }
        if(section == ""){
            section=s1->section;
        }
        if(s1->section.compare(section)!=0){
            return false;
        }
    }
    return true;
}




bool proveriDefinisanostiSimbolaEquIzraza(vector<string> izraz){
    for(int i=0;i<izraz.size();i++){
        if(izraz[i]=="-" || izraz[i]=="+"){
            continue;
        }
        string ime=regex_replace(izraz[i],regex("\\s*"),"");
        Symbol* s=tabelaSimbola->find(ime);
        //cout<<izraz[i]<<"\t";
       if(s==0){
            return false;
        }
    }
    return true;

}

void dodajBajtUSekciju(string imeSekcije,Symbol* simbol,int tip){
    int offset;
    string offsetHexFormat;
    if(simbol->equSimbol==1){
        offset=simbol->value;
    }else{
        if(tip==0){//apsolutno adresiranje simbola
            if(simbol->visible=='g'){
                offset=0;
            }else if(simbol->visible=='l'){
                offset=simbol->value;
            }

        }
        if(tip==1){//pc rel tip 1 adresiranje
            if(simbol->visible=='g'){
                offset=-2;
            }else if(simbol->visible=='l'){
                offset=simbol->value-2;
            }

        }
        if(tip==2){//pc rel tip 2 adresiranje
            if(simbol->visible=='g'){
                offset=-3;
            }else if(simbol->visible=='l'){
                offset=simbol->value-3;
            }

        }
    }
    offsetHexFormat=convertDecToHex(offset,1);
    tabelaSekcija->addInSection(imeSekcije,offsetHexFormat);
}





