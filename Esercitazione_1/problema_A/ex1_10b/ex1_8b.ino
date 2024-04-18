//#define EX2
#ifdef EX2
/*  This sketch is used to sample a signal with a 
    depth of 10 bit (original arduino). The sampling
    frequency can be tuned by changing the macro 
    SAMPLING_FREQUENCY to the desired frequency in Hz
*/

#define N_OF_SAMPLES 429
#define SAMPLING_FREQUENCY 8000

// used for comparison with micros() function
unsigned int sampling_period_us;
unsigned long microseconds;
int samples [N_OF_SAMPLES];

void new_calc_running_sum (double* sig_src_arr, int sig_length);


// used for LPF: this is used with waveforms.ino
//void calc_running_sum(double *sig_src_arr, double *sig_dest_arr, uint32_t sig_length);

void setup() {
  Serial.begin (115200);
  sampling_period_us = round (1000000 * (1.0/SAMPLING_FREQUENCY));
}


void loop() {

  //samples the analog signal 
  for (int i = 0; i < N_OF_SAMPLES; i++){
    microseconds = micros();   
    samples [i] = analogRead (A0);

    //waits for desired sampling period 
    while (micros() < (microseconds + sampling_period_us)){}; 
  }

  //maps samples from 10 bit to 8 bit
  for (int i = 0; i < N_OF_SAMPLES; i++){ 
    samples [i] = map (samples [i], 0, 1023, 0, 256); 
  }

  //prints to serial the gathered samples
  for (int i = 0; i < N_OF_SAMPLES; i++){
    Serial.println (samples [i], 1);
    delay (10);
  }

  //waits indefinitely
  while (1);
  
}

/*
void calc_running_sum(double *sig_src_arr, double *sig_dest_arr,uint32_t sig_length)
{
   uint32_t i;

    for(i=0;i<sig_length;i++)
    {
      sig_dest_arr[i] = sig_dest_arr[i-1] + sig_src_arr[i];
    }
}
*/
#endif 