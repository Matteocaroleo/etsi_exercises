/*Matteo Caroleo S5216938
  This sketch filters signals using either butterworth 1st order or 
  butterworth 2nd order. They are selected by uncommenting the defines
  If TEST is defined, a signal will be generated inside arduino.
*/

#define BUTTERWORTH_FIRST_ORDER
//#define BUTTERWORTH_SECOND_ORDER

#define PI 3.1415926535897932384626433832795

#define SAMPLING_FREQUENCY 1500
#define FILTER_CUTOFF_FREQUENCY 100
#define ANALOG_PIN A0
#define N_OF_SAMPLES 128
//#define TEST

#ifdef BUTTERWORTH_FIRST_ORDER
void Butterworth_first_order (float* sig_src_arr, float* sig_out_arr, float cutoff_frequency);
#endif

#ifdef BUTTERWORTH_SECOND_ORDER
void Butterworth_second_order (float* sig_src_arr, float* sig_out_arr, float cutoff_frequency);
#endif

float sig_src_arr [N_OF_SAMPLES];
float sig_out_arr [N_OF_SAMPLES];

unsigned long sampling_period_us;
unsigned long microseconds;

void Plot_two_signals_overlapped (float* first_sig, float* second_sig, int lenght_of_sigs);

void setup() {
  Serial.begin (9600);
  ///////////////////////////////////////////////////////
  sampling_period_us = round (1000000 * (1.0/SAMPLING_FREQUENCY));
  #ifdef TEST
  for (int i = 1; i < N_OF_SAMPLES; i++){
    microseconds = micros();
    sig_src_arr [i] = 0.7 * sin (2.0*PI*220.0* (microseconds /1.0e6)) + 0.3*sin(2.0*PI*620.0* (microseconds /1.0e6));
    while(micros() < (microseconds + sampling_period_us)){};

  }

  #ifdef BUTTERWORTH_FIRST_ORDER
  Butterworth_first_order (sig_src_arr, sig_out_arr, FILTER_CUTOFF_FREQUENCY);
  #endif

  #ifdef BUTTERWORTH_SECOND_ORDER
  Butterworth_second_order (sig_src_arr, sig_out_arr, FILTER_CUTOFF_FREQUENCY);
  #endif   

  Plot_two_signals_overlapped (sig_src_arr, sig_out_arr, N_OF_SAMPLES);

///////////////////////////////////////////////////////

#else  
  //SAMPLING 
  microseconds = 0;
  // samples input signal with 10 bit depth and with given sampling frequency
  for (int i = 0; i < N_OF_SAMPLES; i++){
    microseconds = micros();   
    sig_src_arr [i] = analogRead (ANALOG_PIN);
    //waits for desired sampling period 
    while (micros() < (microseconds + sampling_period_us)){}; 
  }

  
  #ifdef BUTTERWORTH_FIRST_ORDER
  Butterworth_first_order (sig_src_arr, sig_out_arr, FILTER_CUTOFF_FREQUENCY);
  #endif

  #ifdef BUTTERWORTH_SECOND_ORDER
  Butterworth_second_order (sig_src_arr, sig_out_arr, FILTER_CUTOFF_FREQUENCY);
  #endif   

  Plot_two_signals_overlapped (sig_src_arr, sig_out_arr, N_OF_SAMPLES);

  
 #endif   
}


void loop() {
  //
}

void Plot_two_signals_overlapped (float* first_sig, float* second_sig, int lenght_of_sigs){
  for (int i = 0; i < lenght_of_sigs; i++){
    Serial.print ("SIG:");
    Serial.print (first_sig [i]);
    Serial.print ("\t");
    Serial.print ("FILTERED:");
    Serial.print (second_sig [i]);
    Serial.println();
  }

  Serial.println ();
}

void Butterworth_first_order (float* sig_src_arr, float* sig_out_arr, float cutoff_frequency){

  //for alfa
  float alfa = (2.0*PI*cutoff_frequency)/SAMPLING_FREQUENCY;
  float a1 = -((alfa - 2.0)/(alfa + 2.0));
  float b0 = (alfa)/(alfa + 2.0);
  float b1 = b0;
  sig_out_arr [0] = 1;
  sig_src_arr [0] = 1;
  for (int i = 1; i < N_OF_SAMPLES; i++){
    sig_out_arr[i] = a1*sig_out_arr[i - 1] + b0*sig_src_arr[i] + b1*sig_src_arr[i-1];
  }
}

void Butterworth_second_order (float* sig_src_arr, float* sig_out_arr, float cutoff_frequency){
    float beta0 = 1;
    float beta1 = sqrt(2);
    float beta2 = 1;
    float alfa = (2.0*PI*cutoff_frequency)/SAMPLING_FREQUENCY;
    float a1 = -((alfa - 2.0)/(alfa + 2.0));
    float b0 = (alfa)/(alfa + 2.0);
    float b1 = b0;
    float D = (alfa)*(alfa)*beta0 + 2.0*alfa*beta1 + 4*beta2;
    float a1_2 = - ((2*((alfa)*(alfa)) * beta0) - 8*beta2 )/D;
    float a2_2 = -(beta0 * ((alfa) * (alfa)) - 2*beta1*alfa + 4*beta2) / D;
    float b0_2 = (alfa * alfa) / D;
    float b1_2 = 2*(alfa*alfa)/D;
    float b2_2 = (alfa * alfa) /D;
    for (int i = 2; i < N_OF_SAMPLES; i++){
      sig_out_arr[i] = a1_2*sig_out_arr[i-1] + a2_2*sig_out_arr[i-2] + b0_2*sig_src_arr[i] + b1_2*sig_src_arr[i-1] + b2_2*sig_src_arr[i-2];
    }  
}

//////////////___ERROR_CHECKING____////////////////////
#if defined (BUTTERWORTH_FIRST_ORDER) && defined (BUTTERWORTH_SECOND_ORDER)
  #error CANNOT DEFINE BOTH FILTERS: SELECT ONLY ONE
#endif

#if !defined (BUTTERWORTH_FIRST_ORDER) && !defined (BUTTERWORTH_SECOND_ORDER)
  #error NO FILTER SELECTED
#endif