//#define EX3
#ifdef EX3
/*  This sketch is used to sample a signal with a 
    depth of 10 bit (original arduino). The sampling
    frequency can be tuned by changing the macro 
    SAMPLING_FREQUENCY to the desired frequency in Hz.
    If the macro FILTERING is defined, the signal will
    be filtered by a low pass filter (using running sum
    algorithm).
*/

#define N_OF_SAMPLES 420
#define SAMPLING_FREQUENCY 8000
//#define FILTERING 

// used for comparison with micros() function
unsigned int sampling_period_us;
unsigned long microseconds;
int samples [N_OF_SAMPLES];



#ifdef FILTERING
// used for LPF.
void calc_running_sum(double *sig_src_arr, double *sig_dest_arr, uint32_t sig_length);
#endif

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

  //maps samples from 10 bit to 4 bit
  for (int i = 0; i < N_OF_SAMPLES; i++){ 
    samples [i] = map (samples [i], 0, 1023, 0, 16); 
  }

  //prints to serial the gathered samples
  for (int i = 0; i < N_OF_SAMPLES; i++){
    Serial.println (samples [i], 1);
    delay (10);
  }


  //waits indefinitely
  while (1);
  
}

#ifdef FILTERING
void calc_running_sum(int* array_src, int N_OF_SAMPLES)
{
   int i;

    for(i = 0; i < N_OF_SAMPLES; i++){
      array_src[i] = array_src[i-1] + array_src[i];
    }
}
#endif // FILTERING
#endif // EX3