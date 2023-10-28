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
		
	Part 3:
		./encoder_loss ../sndfile-example-src/sample.wav dec_loss_sample 1024 809
		./decoder_loss dec_loss_sample _decoded_loss_sample.wav
