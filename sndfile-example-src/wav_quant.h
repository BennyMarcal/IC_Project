#ifndef WAVQUANT_H
#define WAVQUANT_H

#include <iostream>
#include <vector>
#include <map>
#include <sndfile.hh>


class WAVQuant {
    private:
        std::vector<short> quant_samples;

    public:
        WAVQuant() {
            quant_samples.resize(0);
        }

        void quant(const std::vector<short>& samples, size_t cutout) {
            for (auto sample : samples) {
                sample = sample >> cutout;
                short tmp = sample << cutout;
                quant_samples.insert(quant_samples.end(), tmp);
            }
        }

        void toFile(SndfileHandle sfhOut) const {
            sfhOut.write(quant_samples.data(), quant_samples.size());
        }
};

#endif