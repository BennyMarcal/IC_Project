#include <iostream>
#include <vector>
#include <math.h>
#include <cmath>
#include <sndfile.hh>
#include "wav_hist.h"

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; 

int main(int argc, char *argv[]) {
    clock_t start = clock();
	if(argc < 4) {
		cerr << "Not enough arguments \n";
		return 1;
	}

	SndfileHandle sfhIn { argv[1] };
	if(sfhIn.error()) {
		cerr << "Invalid input \n";
		return 1;
    }

    SndfileHandle sfhOut { argv[2], SFM_WRITE, sfhIn.format(),
    sfhIn.channels(), sfhIn.samplerate() };
	if(sfhOut.error()) {
		cerr << "Invalid output \n";
		return 1;
    }

    string wanted_effect = argv[3];
    float gain = 0.8;
    int delay = 44100;
    float freq = 1.0;

    if (wanted_effect != "single_echo" && wanted_effect != "multiple_echo" && wanted_effect != "amplitude_modulation" && wanted_effect != "reverse") {
        cerr << "Invalid effects \n";
        return 1;
    } else if (wanted_effect == "single_echo" || wanted_effect == "multiple_echo") {
        if (argc != 6) {
            cerr << "Invalid number of arguments \n";
            return 1;
        }

        try {
            delay = stoi(argv[argc-1]);
            gain = stof(argv[argc-2]);
        } catch(exception &err) {
            cerr << "Invalid gain or delay \n";
            return 1;
        }
    } else if (wanted_effect == "amplitude_modulation") {
        if (argc != 5) {
            cerr << "Invalid number of arguments \n";
            return 1;
        }


        try {
            freq = stof(argv[argc-1]);
        } catch(exception &err) {
            cerr << "Invalid frequency \n";
            return 1;
        }
    } else if (wanted_effect == "reverse" && argc != 4) {
        cerr << "Invalid arguments \n";
        return 1;
    }

    vector<short> samples(FRAMES_BUFFER_SIZE * sfhIn.channels());
    vector<short> samples_out;
    samples_out.resize(0);
    size_t nFrames;
    short sample_out;

    if (wanted_effect == "single_echo" || wanted_effect == "multiple_echo") {
        while((nFrames = sfhIn.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
            samples.resize(nFrames * sfhIn.channels());
            
            for (int i = 0; i < (int)samples.size(); i++) {
                if (i >= delay) {
                    if (wanted_effect == "single_echo") {              
                        sample_out = (samples.at(i) + gain * samples.at(i - delay)) / (1 + gain);
                    } else if (wanted_effect == "multiple_echo") {
                        sample_out = (samples.at(i) + gain * samples_out.at(i - delay)) / (1 + gain);
                    }
                } else {
                    sample_out = samples.at(i);
                }

                samples_out.insert(samples_out.end(), sample_out);
            }
        }
    } else if (wanted_effect == "amplitude_modulation") {
        while((nFrames = sfhIn.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
            samples.resize(nFrames * sfhIn.channels());

            for (int i = 0; i < (int)samples.size(); i++) {
                sample_out = samples.at(i) * cos(2 * M_PI * (freq/sfhIn.samplerate()) * i);
                samples_out.insert(samples_out.end(), sample_out);
            }
        }
    } else if(wanted_effect == "reverse") {
        while((nFrames = sfhIn.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
            samples.resize(nFrames * sfhIn.channels());

            for (int i = (int)samples.size() - 1; i >= 0; i--) samples_out.insert(samples_out.end(), samples.at(i));
        }        
    }

    sfhOut.writef(samples_out.data(), samples_out.size() / sfhIn.channels());
}