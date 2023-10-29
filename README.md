# IC - Project 1

# Authors and Contribuitions
- Bernardo Marçal - 103236 - 33%
- Ricardo Machado - 102737 - 33%
- Rui Campos - 103709 - 33%

## To build

Go to IC_Project/sndfile-example-src and run :

```bash
make all
``` 
## To test

Go to IC_Project/sndfile-example-bin


### Exercise 1

**To create the file**
 
```bash
./wav_hist ../sndfile-example-src/sample.wav 0 > left
``` 
You can also use this to test other channels:

```bash
../sndfile-example-src/sample.wav 1 > right
../sndfile-example-src/sample.wav mid > mid
../sndfile-example-src/sample.wav side > side
``` 

**To open gnuplot**

```bash
gnuplot
```

**To make the graphic**

```bash
plot "left" with boxes
```

You can also use this to check the other graphics:

```bash
plot "right" with boxes
plot "mid" with boxes
plot "side" with boxes
``` 
<br>


### Exercise 2

```bash
./wav_cmp ../sndfile-example-src/sample.wav sample_cmp.wav
```

### Exercise 3

```bash
./wav_quant ../sndfile-example-src/sample.wav 2 sample_quant.wav
```

<br>

### Exercise 4

**Single Echo**
```bash
./wav_effects ../sndfile-example-src/sample.wav  single_echo.wav single_echo 4 40000
```
You can test other values

<br>

**Multiple Echos**
```bash
./wav_effects ../sndfile-example-src/sample.wav multiple_echo.wav multiple_echo 1 40000
```
You can test other values

<br>

**Amplitude Modulation**
```bash
./wav_effects ../sndfile-example-src/sample.wav amp_mod.wav amplitude_modulation 3
``` 
You can test other values

<br>

### Exercise 6

**To encode**
```bash
./encoder texto.txt texto_encoded.txt
```

**To decode**
```bash
./decoder texto_encoded.txt texto_decoded.txt
```

<br>



### Exercise 7

**To encode**
```bash
./encoder_loss ../sndfile-example-src/sample.wav encoded_sample 1024 820
```

<br>

**To decode**
```bash
./decoder_loss encoded_sample decoded_sample.wav
```
