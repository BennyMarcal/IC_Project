#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "wav_quant.h"

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; 

int main(int argc, char *argv[]) {

	if(argc < 4) {
		cerr << "Not enough arguments \n";
		return 1;
	}

	SndfileHandle sfhIn { argv[argc-3] };
	if(sfhIn.error()) {
		cerr << "Invalid input file\n";
		return 1;
    }

    SndfileHandle sfhOut { argv[argc-1], SFM_WRITE, sfhIn.format(),
    sfhIn.channels(), sfhIn.samplerate() };
	if(sfhOut.error()) {
		cerr << "Invalid output file \n";
		return 1;
    }

    int leftbits { stoi(argv[argc-2]) };

    if (leftbits < 1 || leftbits > 16) {
        cerr << "Invalid number of bits \n";
        return 1;
    }

    size_t bits = 16 - leftbits;
    vector<short> samples(FRAMES_BUFFER_SIZE * sfhIn.channels());
    WAVQuant wavquant { };

    size_t framesps;
    while((framesps = sfhIn.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
        samples.resize(framesps * sfhIn.channels());
        wavquant.quant(samples, bits);
    }

    wavquant.toFile(sfhOut);
}