/*
Matteo Caroleo S5216938
  This sketch computes and plot sdft and fft of generated signal and compares them.
  The sketch can generate 3 waveforms , which are all 5-notes chords, one can
  be selected at a time in the "SETTINGS" section in the config.h.
  
  The dft is calculated by the definition given in class, the fft
  is calculated using a library. The time took using each algorithm 
  is shown in the serial monitor at the end of operation. The graphs of
  FFT and DFT are shown in the serial plotter.
*/

#include "ex_2_C_config.h"
#include "arduinoFFT.h" /* USE VERSION 1.6.2 OR LOWER */

double coefficients [5] = {0.9, 0.5, 0.7, 0.55, 0.65};
double signal [N_OF_SAMPLES] = {0};
double real_arr [N_OF_SAMPLES];
double imaginary_arr [N_OF_SAMPLES];

//labels for the serial plotter
#ifdef WAVEFORM_1
  char label0 [3] = {'W','F','1'};
#elif defined (WAVEFORM_2)
  char label0 [3] = {'W','F','2'};
#else 
  char label0 [3] = {'W','F','3'};
#endif

char label1 [3] = {'D','F','T'};
char label2 [3] = {'F','F','T'};

// The frequencies, when played together, form a C4 Maj9 chord. 
#ifdef WAVEFORM_1
  double frequency [5] = {261.63, 329.63, 392.00, 493.88, 557.33};
#endif  //WAVEFORM_1

//It's a D4 min9 chord
#ifdef WAVEFORM_2
  double frequency [5] = {293.66, 349.23, 440.00, 523.25, 659.25};
#endif  //WAVEFORM_2

//It's a G3 dom13th chord
#ifdef WAVEFORM_3
  double frequency [5] = {196.00, 246.94 , 349.23, 493.88, 659.25};
#endif  //WAVEFORM_3


unsigned long microseconds = 0;
unsigned long elapsed_time_dft = 0;
unsigned long elapsed_time_fft = 0;
unsigned int sampling_period_us = 0;

void Generate_signal (double* sig_arr_out, unsigned int sampling_period_us) ;
void Plot_signal (double* sig_src_arr, int sig_length, char* label, int lenght_of_label);
void Plot_two_signals_overlapped (double* dft_sig, double* fft_sig, int lenght_of_sigs);
void calc_sig_dft(double* sig_src_arr, double* sig_dest_rex_arr, double* sig_dest_imx_arr, uint32_t sig_length);
void get_dft_output_mag (double* REX, double* IMX);
float Mean_square_error (double* first_arr, double* second_arr, int lenght);


void setup() {
  Serial.begin (9600);
  sampling_period_us = round(1.0e6*(1.0/SAMPLING_FREQUENCY));
  
  //creates the waveform
  Generate_signal (signal, sampling_period_us);
  Plot_signal (signal, N_OF_SAMPLES, label0, 3);

  //calculates dft by definition
  microseconds = micros();  //starts counter
  calc_sig_dft (signal, real_arr, imaginary_arr, N_OF_SAMPLES);
  get_dft_output_mag (real_arr, imaginary_arr);
  elapsed_time_dft = micros() - microseconds; //stops counter
  #ifndef PLOT_OVERLAPPED
    Plot_signal (real_arr, N_OF_SAMPLES/2, label1, 3);
    delay (1000);
  #endif

  //calculates dft with libraries 
  //setup: clears used array
  for (int i = 0; i < N_OF_SAMPLES/2; i++){
    imaginary_arr[i] = 0;
  }

  Serial.println ("FFT WITH LIBRARY: ");
  microseconds = micros();
  arduinoFFT FFT = arduinoFFT();
  FFT.Compute(signal, imaginary_arr, N_OF_SAMPLES, FFT_FORWARD); /* Compute FFT */
  FFT.ComplexToMagnitude(signal, imaginary_arr, N_OF_SAMPLES);
  elapsed_time_fft = micros() - microseconds;
  
  #ifdef PLOT_OVERLAPPED
    Plot_two_signals_overlapped (real_arr, signal, N_OF_SAMPLES/2);
  #else
    Plot_signal (signal, N_OF_SAMPLES/2, label2, 3);
  #endif //PLOT

  //timers print
  Serial.println("********DATA**********");
  Serial.print ("ELAPSED TIME DFT (s): ");
  Serial.println (elapsed_time_dft / 1.0e6);
  Serial.print ("ELAPSED TIME FFT (s): ");
  Serial.println (elapsed_time_fft/1.0e6);

  Serial.print ("MEAN SQUARE ERROR BETWEEN DFT & FFT: ");
  Serial.println (Mean_square_error (signal, real_arr, N_OF_SAMPLES/2));

}

void loop() {
//
}


void Generate_signal (double* signal, unsigned int sampling_period_us){
  for (int i = 0; i < N_OF_SAMPLES; i++){
    microseconds = micros();
    signal [i] =  coefficients [0] * sin(2.0 * PI * frequency[0] * (microseconds /1.0e6)) + 
                  coefficients [1] * sin(2.0 * PI * frequency[1] * (microseconds /1.0e6)) +
                  coefficients [2] * sin(2.0 * PI * frequency[2] * (microseconds /1.0e6)) +
                  coefficients [3] * sin(2.0 * PI * frequency[3] * (microseconds /1.0e6)) +
                  coefficients [4] * sin(2.0 * PI * frequency[4] * (microseconds /1.0e6));
    while(micros() < (microseconds + sampling_period_us)){};
  }
  return;
}

void Plot_two_signals_overlapped (double* dft_sig, double* fft_sig, int lenght_of_sigs){
  for (int i = 0; i < lenght_of_sigs; i++){
    Serial.print ("DFT:");
    Serial.print (dft_sig [i]);
    Serial.print ("\t");
    Serial.print ("FFT:");
    Serial.print (fft_sig [i]);
    Serial.println();
  }

  Serial.println ();
}

void Plot_signal (double* sig_src_arr, int sig_length, char* label, int lenght_of_label){
  for (int i = 0; i < sig_length; i++){
    for (int j = 0; j < lenght_of_label; j++){
      Serial.print (label[j]);
    }
    Serial.print (":");
    Serial.println (sig_src_arr [i]);
    delay (10);
  }
}

void calc_sig_dft(double* sig_src_arr, double* sig_dest_rex_arr, double* sig_dest_imx_arr, uint32_t sig_length){
  int i,k,j;
  for(j=0; j < (sig_length/2); j++){
    sig_dest_rex_arr[j] = 0;
    sig_dest_imx_arr[j] = 0;
  }
  
   for(k=0; k < (sig_length/2); k++) {
    for(i=0; i < sig_length; i++){
       sig_dest_rex_arr[k] = sig_dest_rex_arr[k] + sig_src_arr[i]*cos(2*PI*k*i/sig_length);
       sig_dest_imx_arr[k] = sig_dest_imx_arr[k] - sig_src_arr[i]*sin(2*PI*k*i/sig_length);
    }
  }
}

void get_dft_output_mag(double* REX, double* IMX){
  uint32_t k;
  for(k=0; k<N_OF_SAMPLES/2; k++)
  {
    REX[k] = sqrt(pow(REX[k], 2)+pow(IMX[k], 2));
  }
}

float Mean_square_error (double* first_arr, double* second_arr, int lenght){
 float acc = 0;
 for (int i = 0; i < lenght; i ++){
    //sum of squares
    acc = (first_arr[i] - second_arr[i]) * (first_arr[i] - second_arr[i]);
 } 
 //mean
 return acc / lenght;
}

