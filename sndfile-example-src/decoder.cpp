#include <iostream>
#include "bitstream.h"

using namespace std;

int main (int argc, char *argv[]) {	
    clock_t start = clock();

    if (argc < 3) {	
        cerr << "Not enough arguments \n" ;
        return 1 ;
    }

    BitStream inputFile (argv [1], "r") ;
    
    ofstream outputFile (argv [2], ios::out) ;
    if (! outputFile) {
        cerr << "Invalid output file " << argv [2] << ".\n" ;
        return 1 ;
    }

    vector<int> bits;
    bits = inputFile.readBits(inputFile.getFileSize() * 8);
    inputFile.close();

    for (int i = 0; i < bits.size(); i++){
        outputFile << bits[i];
    }
    outputFile.close();

    return 0;
}
