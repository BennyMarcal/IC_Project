#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "wav_quant.h"

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading frames

int main(int argc, char *argv[]) {

	if(argc < 4) {
		cerr << "Usage: " << argv[0] << " <input file> <bits_to_keep> <output_file>\n";
		return 1;
	}

	SndfileHandle sfhIn { argv[argc-3] };
	if(sfhIn.error()) {
		cerr << "Error: invalid input file\n";
		return 1;
    }

	if((sfhIn.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
		cerr << "Error: file is not in WAV format\n";
		return 1;
	}

	if((sfhIn.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
		cerr << "Error: file is not in PCM_16 format\n";
		return 1;
	}

    SndfileHandle sfhOut { argv[argc-1], SFM_WRITE, sfhIn.format(),
    sfhIn.channels(), sfhIn.samplerate() };
	if(sfhOut.error()) {
		cerr << "Error: invalid output file\n";
		return 1;
    }

    int leftbits { stoi(argv[argc-2]) };

    if (leftbits < 1 || leftbits > 16) {
        cerr << "Error: invalid number of bits to keep\n";
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