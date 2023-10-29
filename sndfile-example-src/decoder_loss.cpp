#include <iostream>
#include <vector>
#include <cmath>
#include <fftw3.h>
#include <sndfile.hh>
#include "bitstream.h"

using namespace std;

int main (int argc, char *argv[])
{

    if(argc < 3 || argc > 3 ){
		cerr << "Not enough arguments \n";
		return 1;
	}

    string inputFileName = argv[1];

    BitStream bitStream(inputFileName, "r");

    vector<int> v_bs = bitStream.readBits(16);
    vector<int> v_nBlocks= bitStream.readBits(16);
    vector<int> v_nChannels = bitStream.readBits(16);
    vector<int> v_sampleRate = bitStream.readBits(16);
    vector<int> v_nFrames = bitStream.readBits(32);

    int bs = 0;
    for(int i = 0; i < v_bs.size(); i++) {
        bs += v_bs[i] * pow(2, v_bs.size() - i - 1);
    }

    int nBlocks = 0;
    for(int i = 0; i < v_nBlocks.size(); i++) {
        nBlocks += v_nBlocks[i] * pow(2, v_nBlocks.size() - i - 1);
    }

    int nChannels = 0;
    for(int i = 0; i < v_nChannels.size(); i++) {
        nChannels += v_nChannels[i] * pow(2, v_nChannels.size() - i - 1);
    }


    int sampleRate = 0;
    for(int i = 0; i < v_sampleRate.size(); i++) {
        sampleRate += v_sampleRate[i] * pow(2, v_sampleRate.size() - i - 1);
    }

    int nFrames = 0;
    for(int i = 0; i < v_nFrames.size(); i++) {
        nFrames += v_nFrames[i] * pow(2, v_nFrames.size() - i - 1);
    }

    SndfileHandle sfhOut { argv[argc-1], SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_PCM_16, nChannels, sampleRate };
	if(sfhOut.error()) {
		cerr << "Invalid output file \n";
		return 1;
    }

    int total = bitStream.getFileSize() - 12;
    long totalBits = total*8;

    vector<int> x_dct_bits = bitStream.readBits(totalBits);
    vector<vector<double>> x_dct(nChannels, vector<double>(nBlocks * bs));
    vector<int> tmp;

    
    for(int i = 0; i < x_dct_bits.size(); i+=32) {
        int temp = 0;
        
        vector<int> reversed_temp;

        for(int j = 31; j >= 0; j--) {
            reversed_temp.push_back(x_dct_bits[i+j]);
        }
        for(int j = 0; j < reversed_temp.size(); j++) {
            temp += reversed_temp[j] * pow(2, reversed_temp.size() - j - 1);
        }
        tmp.push_back(temp);
    }

    bitStream.close();

    int count = 0;
    for(int n = 0; n < nBlocks; n++) {
        for(int c = 0; c < nChannels; c++) {
            for (int k = 0; k < bs; k++) {
                x_dct[c][n*bs + k] = tmp[count]/100.0;
                count++;
            }
        }
    }

    vector<double> x(bs);
    vector<short> samples(nChannels * nFrames);
    samples.resize(nBlocks * bs * nChannels);
    
	fftw_plan plan_i = fftw_plan_r2r_1d(bs, x.data(), x.data(), FFTW_REDFT01, FFTW_ESTIMATE);
	for(size_t n = 0 ; n < nBlocks ; n++)
		for(size_t c = 0 ; c < nChannels ; c++) {
			for(size_t k = 0 ; k < bs ; k++){
				x[k] = x_dct[c][n * bs + k];
            }

			fftw_execute(plan_i);
			for(size_t k = 0 ; k < bs ; k++)
				samples[(n * bs + k) * nChannels + c] = static_cast<short>(round(x[k]));

		}
    
    
    sfhOut.writef(samples.data(), nFrames);

}