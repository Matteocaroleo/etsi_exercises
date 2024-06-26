//#define EX3
#ifdef EX3
/*  This sketch is used to sample a signal with a 
    depth of 4 bit. The sampling
    frequency can be tuned by changing the macro 
    SAMPLING_FREQUENCY to the desired frequency in Hz
*/

#define N_OF_SAMPLES 200
#define SAMPLING_FREQUENCY 8000
#define FILTER_RS
#undef FILTER_RS
#define FILTER_RMA

// used for comparison with micros() function
unsigned int sampling_period_us;
unsigned long microseconds;
double samples [N_OF_SAMPLES];
double filtered_samples [N_OF_SAMPLES];


void Plot_signal (double* sig_src_arr, int sig_length);
#ifdef FILTER_RS
// used for LPF
void Running_sum (double* sig_src_arr, double* sig_out_arr, int sig_length);
#endif // FILTER_RS

#ifdef FILTER_RMA
//other lpf
void Recursive_moving_average(double *sig_src_arr, double* sig_out_arr ,uint32_t sig_length, uint32_t filter_pts);
#endif //FILTER_RMA

void setup() {
  Serial.begin (115200);
  sampling_period_us = round (1000000 * (1.0/SAMPLING_FREQUENCY));
}

void loop() {
  for (int i = 0; i < N_OF_SAMPLES; i++){
    microseconds = micros();   
    samples [i] = analogRead (A0);

    //waits for desired sampling period 
    while (micros() < (microseconds + sampling_period_us)){}; 
  }

   //maps samples from 10 bit to 4 bit
  for (int i = 0; i < N_OF_SAMPLES; i++){ 
    samples [i] = map (samples [i], 0, 1023, 0, 16); 
  }


  //prints to serial the gathered samples
  for (int i = 0; i < N_OF_SAMPLES; i++){
    Serial.println (samples [i], 1);
    delay (10);
  }
  #ifdef FILTER_RS
  Running_sum (samples, filtered_samples N_OF_SAMPLES);
  #endif //FILTER_RS

  #ifdef FILTER_RMA
  Recursive_moving_average (&samples[0],&filtered_samples[0],N_OF_SAMPLES, 50);
  #endif

  delay (500);
  for (int i = 0; i < N_OF_SAMPLES; i++){
    Serial.println (samples [i], 1);
    delay (10);
  }
  delay (500);
  for (int i = 0; i < N_OF_SAMPLES; i++){
    Serial.print (samples [i]);
    Serial.print (",");
    Serial.println (filtered_samples[i]);
    delay (10);
  }
  //waits indefinitely
  while (1);
  
}


void Plot_signal (double* sig_src_arr, int sig_length){
  
}

void Running_sum (double* sig_src_arr, double* sig_out_arr, int sig_length){
  int i;
  for(i=0; i<sig_length; i++){
      sig_out_arr[i] = (sig_src_arr[i-1] + sig_src_arr[i]);
    }
}

void Recursive_moving_average(double *sig_src_arr, double *sig_out_arr, uint32_t sig_length, uint32_t filter_pts)
{
  uint32_t i,j;
  double ACC;
  ACC=0;
    for(i=0;i<filter_pts;i++)
    {
      ACC = ACC + sig_src_arr[i];
    }

   sig_out_arr[(filter_pts-1/2)]  = ACC/filter_pts;

      for(i= ceil(filter_pts/2);i<sig_length-ceil(filter_pts/2);i++)
      {
         ACC =ACC + sig_src_arr[i+(filter_pts-1)/2]- sig_src_arr[i-(uint32_t)ceil(filter_pts/2)];
         sig_out_arr[i]  = ACC/filter_pts;
      }
  
  
}
#endif // EX3