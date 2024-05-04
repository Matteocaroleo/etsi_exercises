/*  MATTEO CAROLEO S5216938, done in Arduino Uno R3
    
    This sketch manipulates input signal in analog pin. By changing values
    in the "ex1_config.h" file, it can:
      - Sample a signal with 10 bit depth
      - Sample a signal with 8 bit depth
      - Sample a signal with 4 bit depth
      - Filter a signal with running sum algorithm
      - Filter a signal with recursive moving average algorithm.
    
      If ex1_config.h is left untouched, the sketch will 
        - plot the input signal with depth of 10 bit
        - plot the input signal with both depth of 8 and 4 bit
        - plot the input quantized signals with RMA filtering
      the sketch will gather N_OF_SAMPLES sample.

    The extensive use of the preprocessor directives has been used
    for maximizing memory usage while keeping all in a couple of files.

    The sketch can also run a test, set by a macro in config.h
*/

#include "ex1_config.h" 

// used for comparison with micros() function
unsigned int sampling_period_us;
unsigned long microseconds;

//does not create array for input signal if in test mode
#ifndef TEST
double samples [N_OF_SAMPLES];
#endif //TEST


// creates array of different length depending if in test mode or not
#ifdef TEST
double filtered_samples [N_OF_SAMPLES_TEST];
#else
double filtered_samples [N_OF_SAMPLES];
#endif //TEST

#if defined(QUANT_8_BIT) || defined(QUANT_4_BIT)
void Quantize ();
#endif


  
// used for LPF: is an estimate of an integrator
#ifdef FILTER_RS
void Running_sum (double* sig_src_arr, double* sig_out_arr, int sig_length);
#endif // FILTER_RS

// used for LPF: acts as a low-pass-filter and smooths out quantization noise
#ifdef FILTER_RMA
void Recursive_moving_average(double *sig_src_arr, double* sig_out_arr ,uint32_t sig_length, uint32_t filter_pts);
#endif //FILTER_RMA



//functions for plotting data
void Plot_signal (double* sig_src_arr, int sig_length);
void Plot_both (double* sig_src_arr, double* sig_filtered_arr, int sig_length);
void Plot_both_quantized_signals (double* sig_src_arr, int sig_length);

// used for sampling signal in analog pin
void Sample (double* sig_src_arr, int sig_length, unsigned int sampling_period_us, int analog_pin);

// used to store quantized signals
double temp4 [N_OF_SAMPLES];
double temp8 [N_OF_SAMPLES];

void setup() {
  Serial.begin (115200);
  
  //converts frequenct in microseconds
  sampling_period_us = round (1000000 * (1.0/SAMPLING_FREQUENCY));

}

void loop() {
  
  //USED FOR TEST ROUTINE: tests LPF filtering with known signal in memory
  ////////////////////////////////////////////////////////////////////////////////////
  #ifdef TEST
    #define FILTER_RMA
    #define FILTER_RS
    //is found in waveformes.ino file
    extern double InputSignal_1kHz_15kHz[N_OF_SAMPLES_TEST];
    Serial.println ("PLOTTING SIGNAL");
    Plot_signal (&InputSignal_1kHz_15kHz[0], N_OF_SAMPLES_TEST);
    delay (800);
    Serial.println ("PLOTTING RUNNING SUM:");
    Running_sum (InputSignal_1kHz_15kHz, filtered_samples, N_OF_SAMPLES_TEST);
    Plot_both (InputSignal_1kHz_15kHz, filtered_samples, N_OF_SAMPLES_TEST);
    //Plot_signal (filtered_samples, N_OF_SAMPLES_TEST);
    delay (800);
    Serial.println ("PLOTTING FILTERED AVERAGE:");
    Recursive_moving_average (InputSignal_1kHz_15kHz, filtered_samples, N_OF_SAMPLES_TEST, FILTER_PTS_TEST);
    Plot_both (InputSignal_1kHz_15kHz, filtered_samples, N_OF_SAMPLES_TEST);
    //Plot_signal (filtered_samples, N_OF_SAMPLES_TEST);
    while (1);  
  #endif //TEST
////////////////////////////////////////////////////////////////////////////////////


//EX_FULL: shows all request in order (using RMA filtering)
////////////////////////////////////////////////////////////////////////////////////
  #ifdef EX_FULL
    // sample the input signal with 10 bit depth 
    Sample (samples, N_OF_SAMPLES, sampling_period_us, ANALOG_PIN);
    
    // plots signal with quantization depth of 10 bit
    Plot_signal (samples, N_OF_SAMPLES);
    delay (2000);

    // plot signal with quantization depth of both 8 and 4 bit
    Plot_quantized_signals (samples, N_OF_SAMPLES);
    delay (2000);

    // plot quantized and filtered signal with 8 and 4 bit of depth
    Plot_quantized_and_filtered_signals (samples, N_OF_SAMPLES);

    //waits indefinitely
    while (1);
  #endif  //EX_1
////////////////////////////////////////////////////////////////////////////////////


// this part of code is used to run specific functionalities:
// if EX_FULL OR TEST is defined in config, the part below won't run.

#ifndef TEST
  #ifdef QUANT_4_BIT
   //maps samples from 10 bit to 4 bit
  for (int i = 0; i < N_OF_SAMPLES; i++){ 
    samples [i] = map (samples [i], 0, 1023, 0, 15); 
  }
  #endif //QUANT_4_BIT

  #ifdef QUANT_8_BIT
    //maps samples from 10 bit to 8 bit
  for (int i = 0; i < N_OF_SAMPLES; i++){ 
    samples [i] = map (samples [i], 0, 1023, 0, 255); 
  }
  #endif //QUANT_8_BIT

  
  //prints to serial the gathered samples with previously chosen quantization depth
  Plot_signal (samples, N_OF_SAMPLES);


  // Runs the running sum LPF 
  #ifdef FILTER_RS
  Running_sum (samples, filtered_samples, N_OF_SAMPLES);
  #endif //FILTER_RS

  // Runs the recursive moving average LPF
  #ifdef FILTER_RMA
  Recursive_moving_average (samples, filtered_samples, N_OF_SAMPLES, FILTER_PTS);
  #endif //FILTER_RMA

  delay (500);

  // if the signal has been filtered, it plots the src and the filtered result
  #if defined(FILTER_RMA) || defined(FILTER_RS)
  Plot_both (samples, filtered_samples, N_OF_SAMPLES);
  #endif //defined(FILTER_RMA) || defined (FILTER_RS)
  
  //waits indefinitely
  while (1);
#endif // NOT TEST
}


// FUNCTIONS IMPLEMENTATIONS

void Plot_signal (double* sig_src_arr, int sig_length){
  for (int i = 0; i < sig_length; i++){
    Serial.println (sig_src_arr [i]);
    delay (10);
  }
}

void Plot_both (double* sig_src_arr, double* sig_filtered_arr, int sig_length){
  for (int i = 0; i < sig_length; i++){
    Serial.print ("SOURCE_SIGNAL:");
    Serial.print (sig_src_arr[i]);
    Serial.print (",");
    Serial.print ("FILTERED_SIGNAL:");
    Serial.println (sig_filtered_arr[i]);
    delay (10);
  }
}

void Plot_quantized_signals (double* sig_src_arr, int sig_length){
  for (int i = 0; i < sig_length; i++){
    Serial.print ("8_BIT_SIGNAL:");
    Serial.print (map (sig_src_arr[i], 0 ,1023, 0, 255));
    Serial.print (",");
    Serial.print ("4_BIT_SIGNAL:");
    Serial.println (map (sig_src_arr[i], 0, 1023, 0, 15)*17);
    delay (10);
  }
}

void Plot_quantized_and_filtered_signals (double* sig_src_arr, int sig_length){
 

  for (int i = 0; i < sig_length; i++){ 
    temp8 [i] = map (sig_src_arr [i], 0, 1023, 0, 255); 
  }

  for (int i = 0; i < sig_length; i++){ 
   temp4 [i] = map (sig_src_arr [i], 0, 1023, 0, 15) * 17; 
  }
  Recursive_moving_average (temp8, temp8, sig_length, FILTER_PTS);
  Recursive_moving_average (temp4, temp4, sig_length, FILTER_PTS);
  for (int i = 0; i < sig_length; i++){
    Serial.print ("8_BIT_FILTERED:");
    Serial.print (temp8[i]);
    Serial.print (",");
    Serial.print ("4_BIT_FILTERED:");
    Serial.println (temp4[i]);
    delay (10);
  }
  return;
}
void Running_sum (double* sig_src_arr, double* sig_out_arr, int sig_length){
  sig_out_arr [0] = sig_src_arr[0];
  for(int i= 1; i<sig_length; i++){
      sig_out_arr[i] = (sig_out_arr[i-1] + sig_src_arr[i]);
    }
}

void Recursive_moving_average(double *sig_src_arr, double *sig_out_arr, uint32_t sig_length, uint32_t filter_pts){
  uint32_t i,j;
  double ACC;
  ACC = 0;
  for(i=0;i<filter_pts;i++)    {
    ACC = ACC + sig_src_arr[i];
  }
  sig_out_arr[(filter_pts-1/2)]  = ACC/filter_pts;
  for(i= ceil(filter_pts/2); i<sig_length  ; i++){
      ACC =ACC + sig_src_arr[i+(filter_pts-1)/2]- sig_src_arr[i-(uint32_t)ceil(filter_pts/2)];
      sig_out_arr[i]  = ACC/filter_pts;
  }
}

void Sample (double* sig_src_arr, int sig_length, unsigned int sampling_period_us, int analog_pin){
  
  unsigned long microseconds = 0;
  // samples input signal with 10 bit depth and with given sampling frequency
  for (int i = 0; i < sig_length; i++){
    microseconds = micros();   
    sig_src_arr [i] = analogRead (analog_pin);
    //waits for desired sampling period 
    while (micros() < (microseconds + sampling_period_us)){}; 
  }
}