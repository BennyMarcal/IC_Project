#include <iostream>
#include "bitstream.h"

using namespace std;

int main (int argc, char *argv[]) {	

    if (argc < 3) {	
        cerr << "Not enough argument \n" ;
        return 1 ;
    } 

    ifstream inputFile (argv [1], ios::in) ;
    if (! inputFile) {
        cerr << "Invalid input file " << argv [1] << ".\n" ;
        return 1 ;
    }

    string outputFileName = argv[2];

    string line;
    getline(inputFile, line);
    inputFile.close();

    BitStream outputFile (outputFileName, "w") ;

    vector<int> bits;
    for (int i = 0; i < line.length(); i++){
        bits.push_back(line[i] - '0');
    }
    outputFile.writeBits(bits);
    outputFile.close();

    return 0;
}