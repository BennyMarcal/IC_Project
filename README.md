# IC_Project

## To build:
	make
	make all

## To test:

	Part 1:
		../sndfile-example-bin/wav_cp sample.wav copy.wav // copies "sample.wav" into "copy.wav"
		../sndfile-example-bin/wav_hist sample.wav 0 // outputs the histogram of channel 0 (left)
		../sndfile-example-bin/wav_dct sample.wav out.wav // generates a DCT "compressed" version
	
	Part 2:
		./decoder imputfile outputfile
		./encoder imputfile outputfile
