#include <iostream>
#include <fstream>
#include <string>
#include <string.h>

using namespace std;

extern string one_pass(ifstream* inputFile);

int main(int argc, char* argv[]){

    if(argc<3){
        cout<<"Nedovoljno argumenata!";
        exit(-1);
    }

    string inputFileName = argv[3];
    string outputFileName = argv[2];

    ifstream inputFile(inputFileName);
    ofstream outputFile(outputFileName);
    
    //inputFile.open();
    //utputFile.open();

    string output=one_pass(&inputFile);
    outputFile<<output;

    inputFile.close();
    outputFile.close();

    return 0;
}