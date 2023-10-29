#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "wav_hist.h"

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; 

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cerr << "Not enough arguments \n";
        return 1;
    }

    SndfileHandle sndFile{ argv[1] }; 
    if (sndFile.error()) {
        cerr << "Invalid input \n";
        return 1;
    }

    string mode{ argv[2] }; 

    size_t nFrames;
    vector<short> samples(FRAMES_BUFFER_SIZE * sndFile.channels());
    WAVHist hist{ sndFile };

    while ((nFrames = sndFile.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
        samples.resize(nFrames * sndFile.channels());

        
        hist.update(samples);
        hist.updateMidChannel(samples);
        hist.updateSideChannel(samples);
    }

    if (mode == "mid") {
        hist.dumpMidChannel();
    } else if (mode == "side") {
        hist.dumpSideChannel();
    } else {
        try {
            int channel = stoi(mode);
            if (channel < 0 || channel >= sndFile.channels()) {
                cerr << "Invalid channel \n";
                return 1;
            }
            hist.dump(channel);
        } catch (exception &err) {
            cerr << "Invalid mode \n";
            return 1;
        }
    }

    return 0;
}