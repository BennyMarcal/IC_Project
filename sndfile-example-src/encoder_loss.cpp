#include <iostream>
#include <vector>
#include <cmath>
#include <fftw3.h>
#include <sndfile.hh>
#include "bitstream.h"

using namespace std;

int main (int argc, char *argv[])
{


    size_t bs { 1024 };
    size_t discarded_units_per_block { 8 };

    if(argc < 5) {
		cerr << "Not enough arguments\n";
		return 1;
	}

    SndfileHandle sfhIn { argv[argc-4] };
    if(sfhIn.error()) {
        cerr << "Invalid input file \n";
        return 1;
    }

    string outputFileName = argv[2];

    bs = atoi(argv[3]);
    discarded_units_per_block = atoi(argv[4]);

    if(discarded_units_per_block > bs) {
        cerr << "Switch the numbers \n";
        return 1;
    }

    size_t nChannels { static_cast<size_t>(sfhIn.channels()) };
	size_t nFrames { static_cast<size_t>(sfhIn.frames()) };

    vector<short> samples(nChannels * nFrames);
	sfhIn.readf(samples.data(), nFrames);

	size_t nBlocks { static_cast<size_t>(ceil(static_cast<double>(nFrames) / bs)) };

	samples.resize(nBlocks * bs * nChannels);
    
	vector<vector<double>> x_dct(nChannels, vector<double>(nBlocks * bs));

	vector<double> x(bs);

    int tmp = 0;

	fftw_plan plan_d = fftw_plan_r2r_1d(bs, x.data(), x.data(), FFTW_REDFT10, FFTW_ESTIMATE);
	for(size_t n = 0 ; n < nBlocks ; n++)
		for(size_t c = 0 ; c < nChannels ; c++) {
			for(size_t k = 0 ; k < bs ; k++)
				x[k] = samples[(n * bs + k) * nChannels + c];

			fftw_execute(plan_d);
			
			for(size_t k = 0 ; k < bs - discarded_units_per_block ; k++){
				x_dct[c][n * bs + k] = x[k] / (bs << 1) * 100;
            }
            tmp++;

		}
    BitStream outputFile (outputFileName, "w") ;
    vector<int> bits; 

    
    for (int i = 15; i >= 0; i--){
        bits.push_back((bs >> i) & 1);
    }
    
    for (int i = 15; i >= 0; i--){
        bits.push_back((nBlocks >> i) & 1);
    }

    for (int i = 15; i >= 0; i--){
        bits.push_back((nChannels >> i) & 1);
    }

    for (int i = 15; i >= 0; i--){
        bits.push_back((sfhIn.samplerate() >> i) & 1);
    }

    for (int i = 31; i >= 0; i--){
        bits.push_back((nFrames >> i) & 1);
    }

    int value = 0;
    vector<int>values;

    for(size_t n = 0 ; n < nBlocks ; n++)
		for(size_t c = 0 ; c < nChannels ; c++) {
			for(size_t k = 0 ; k < bs ; k++){
                int tmp2 = x_dct[c][n * bs + k];
				values.push_back(tmp2);
            }
        }

    int count = 0;
    for(size_t n = 0 ; n < values.size() ; n++){
        vector<int> temp_array (32);
        for (int i = 31; i >= 0; i--){
            temp_array[i] = (values[n] >> i) & 1;
        }
        vector<int> temp_array2 (32);
        for (int i = 0; i < 32; i++){
            temp_array2[i] = temp_array[31-i];
        }
        for (int i = 31; i >=0; i--){
            bits.push_back(temp_array2[i]);
        }
    }
    
    outputFile.writeBits(bits);
    outputFile.close();
    
    return 0;
    
}