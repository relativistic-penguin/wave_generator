# Signal Generator and Oscilloscope on Arduino

This repo contains an Arduino sketch file for producing periodic analogue signal.
This was originally developed for the Arduino Due, but with minimal modification should work with any Arduino board with a digital-analogue-converter output.

The key requirement is that the Arduino board must have a true analogue output port (This was not designed to work with PWM port).

Other than the signal generator itself, there is also a second sketch file for a second Arduino board to read this analogue signal and transfer this data (after digital sampling) to PC for analysis and verification purpose.
Essentially it is a (poor man's) oscilloscope with 12-bit resolution.

To ease the data analysis process with the oscilloscope sketch, some useful python functions to read the serial data, plot its time series and frequency profile are provided.
A Jupyter notebook is provided to demonstrate the use of these python functions.
The python code requires `numpy`, `matplotlib`, `pyserial` packages, but you can also just install all required packages with the `requirements.txt` file with `pip -r requirements.txt`.

## Signal Generator
The signal generator itself is located in `arduino/signal_generator/` directory.
At the top of the file, frequency and amplitude can be adjusted.
The number of voltage output value update per oscillation period can also be adjusted. (More means more accurate depiction of desired waveform).
The signal generator outputs the analogue signal on its `DAC0` port.

The waveform can also be changed.
I provided square wave, saw wave and sine wave.
These are defined in `WaveLUT.hpp` header file and it can be easily extended to define any other waveforms.

There is a `self_test` option at the top of the main sketch file.
1. `0` value means there is no data output from the Arduino board to PC via the native USB port. This should normally be used in production environment after verifying the waveform is correct.
2. `1` value means the Arduino board will output the analogue voltage value it is supposed to output at its `DAC0` port to PC.
3. `2` value should only be used when the `DAC0` port of the Arduino board is connected to the `A0` analogue input port, and then the Arduino board will read the actual analogue volage input value at `A0` and transfer that to PC via the native USB port.

These options are meant for self testing if the board is outputting the desired wave form.
The number of times values will be reported to PC per DAC value update can also be configured at the top of the sketch file.
More frequent reporting gives more accurate description of the actual wave form, and allows for more accurate frequency analysis (subject to Nyquist's theorem of course), but imposes greater pressure on the board and greater bandwidth requirement over USB. (This is why the USB port used must be the **native port** and **not the programming port**, which is slow.)

Prior to experiment, you should first calibrate the minimum and maximum output of the DAC output voltage from your arduino board (it is not 0 and 5V).
You can do this by making your arduino output 0 to `DAC0`, connect `DAC0` to `A0` and use `self_test=1` and see what voltage you get on A0.
To get maximum output voltage, you either output 1023 to `DAC0`, if your analogue write resolution is 10-bit, or output 4095 to `DAC0`, if you analogue write resolution is 12-bit. (It is 12-bit capable on Due but you must manually set the resolution to 12-bit, otherwise it is 10-bit by default)
After getting these values, you should put these measured values into the `v_out_min_mV` and `v_out_max_mV` variable (in milliVolt).
**If you don't do this then your amplitude will be wrong!**.

## Oscilloscope
The oscilloscope sketch file is in `arduino/analogue_data_reader/` directory.
At the top of the file you must ensure that you set the frequency that the board samples the analogue data to PC.
You can either do this by setting the `report_period_us` variable directly (in microsecond), or calculate an appropriate value by setting the frequency and other variables in the same way you set it for the signal generator sketch file.

Once this sketch file is loaded onto the board **via the native port**, you can then open the Jupyter notebook in `notebooks/data_processing.ipynb` to use as an oscilloscope.
The notebook automatically determines which port the board is connected to, read serial data from it, save it to the `data/` directory, plot these analogue voltage data into a time series, and perform Fourier transform on it to produce a Fourier domain plot.
**IMPORTANT NOTE**: You must ensure the `report_period_us` variable is set in the same way as in the Arduino sketch, otherwise the frequency calculation will be wrong. 
This notebook is meant as a guide for the couple of useful functions defined in `scripts/data_processing.py`. Feel free to use it as a template for further development.

## Citation
If you use this work, please cite using the `CITATION.cff` file contained at repository root.
