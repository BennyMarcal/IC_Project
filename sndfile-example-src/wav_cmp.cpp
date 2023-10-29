#include <iostream>
#include <vector>
#include <sndfile.hh>
#include <cmath>
#include <limits>

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536;

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cerr << "Not enough arguments\n";
        return 1;
    }

    SndfileHandle sfhIn1{ argv[argc - 2] };
    SndfileHandle sfhIn2{ argv[argc - 1] };

    if (sfhIn1.error() || sfhIn2.error()) {
        cerr << "Invalid input\n";
        return 1;
    }

    if (sfhIn1.frames() != sfhIn2.frames()) {
        cerr << "Invalid input\n";
        return 1;
    }

    vector<short> samples_f1(FRAMES_BUFFER_SIZE * sfhIn1.channels());
    vector<short> samples_f2(FRAMES_BUFFER_SIZE * sfhIn2.channels());

    size_t nFrames;
    double total_energy_signal = 0.0;
    double total_energy_noise = 0.0;
    double max_error = 0.0;
    double snr = 0.0;
    int channel_count = sfhIn1.channels(); 

    vector<double> channel_errors(channel_count, 0.0);
    vector<double> channel_max_errors(channel_count, 0.0);

    while ((nFrames = sfhIn1.readf(samples_f1.data(), FRAMES_BUFFER_SIZE))) {
        sfhIn2.readf(samples_f2.data(), FRAMES_BUFFER_SIZE);

        samples_f1.resize(nFrames * sfhIn1.channels());
        samples_f2.resize(nFrames * sfhIn2.channels());

        for (long unsigned int i = 0; i < samples_f1.size(); i++) {
            double signal_sample = static_cast<double>(samples_f1[i]);
            double noise_sample = signal_sample - static_cast<double>(samples_f2[i]);

            total_energy_signal += signal_sample * signal_sample;
            total_energy_noise += noise_sample * noise_sample;

            double abs_error = abs(noise_sample);
            max_error = max(abs_error, max_error);

            int channel_index = i % channel_count;
            channel_errors[channel_index] += noise_sample * noise_sample;
            channel_max_errors[channel_index] = max(abs_error, channel_max_errors[channel_index]);
        }
    }

    snr = 10 * log10(total_energy_signal / total_energy_noise);
    cout << "SNR: " << snr << " dB" << endl;

    for (int i = 0; i < channel_count; i++) {
        double channel_l2_norm = sqrt(channel_errors[i] / samples_f1.size());
        cout << "Channel " << i + 1 << " - L2 Norm: " << channel_l2_norm << endl;
        cout << "Channel " << i + 1 << " - Max Absolute Error (L∞ norm): " << channel_max_errors[i] << endl;
    }

    double average_l2_norm = 0.0;
    double average_max_error = 0.0;

    for (int i = 0; i < channel_count; i++) {
        average_l2_norm += sqrt(channel_errors[i] / samples_f1.size());
        average_max_error += channel_max_errors[i];
    }

    average_l2_norm /= channel_count;
    average_max_error /= channel_count;

    cout << "Average L2 Norm: " << average_l2_norm << endl;
    cout << "Average Max Absolute Error (L∞ norm): " << average_max_error << endl;
}
