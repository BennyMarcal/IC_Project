#ifndef WAVHIST_H
#define WAVHIST_H

#include <iostream>
#include <vector>
#include <map>
#include <sndfile.hh>

class WAVHist {
private:
    std::vector<std::map<short, size_t>> counts;
    std::vector<std::map<short, size_t>> mid_counts;
    std::vector<std::map<short, size_t>> side_counts;

public:
    WAVHist(const SndfileHandle& sfh) {
        counts.resize(sfh.channels());
        mid_counts.resize(1);
        side_counts.resize(1);
    }

    short calculateCoarseBin(short value, int binSize) {
        return value / binSize * binSize;
    }

    void update(const std::vector<short>& samples) {
        size_t n = 0;
        for (auto s : samples) {
            counts[n % counts.size()][calculateCoarseBin(s, 4)]++;

            if (counts.size() == 2) {
                if (n % 2 == 0) {
                    mid_counts[0][calculateCoarseBin((samples[n] + samples[n + 1]) / 2, 4)]++;
                } else {
                    side_counts[0][calculateCoarseBin((samples[n] - samples[n + 1]) / 2, 4)]++;
                }
            }
            n++;
        }
    }

    void dump(const size_t channel) const {
        for (const auto& pair : counts[channel]) {
            std::cout << pair.first << '\t' << pair.second << '\n';
        }
    }

    void dumpMidChannel() const {
        for (const auto& pair : mid_counts[0]) {
            std::cout << pair.first << '\t' << pair.second << '\n';
        }
    }

    void dumpSideChannel() const {
        for (const auto& pair : side_counts[0]) {
            std::cout << pair.first << '\t' << pair.second << '\n';
        }
    }

    void updateMidChannel(const std::vector<short>& samples) {
        for (size_t i = 0; i < samples.size() / 2; i++) {
            mid_counts[0][calculateCoarseBin((samples[2 * i] + samples[2 * i + 1]) / 2, 4)]++;
        }
    }

    void updateSideChannel(const std::vector<short>& samples) {
        for (size_t i = 0; i < samples.size() / 2; i++) {
            side_counts[0][calculateCoarseBin((samples[2 * i] - samples[2 * i + 1]) / 2, 4)]++;
        }
    }
};

#endif