#define TEST
#define SAMPLING_FREQUENCY 1000
#define N_OF_SAMPLES 100
#ifdef TEST
#define N_OF_SAMPLES_TEST 50
#endif

// used for comparison with micros() function
unsigned int sampling_period_us;
unsigned long microseconds;
double samples [N_OF_SAMPLES];

#ifdef TEST
extern double test_signal [180];
double RS_out_arr_test [N_OF_SAMPLES_TEST];
double FD_out_arr_test [N_OF_SAMPLES_TEST];
double real_test [N_OF_SAMPLES_TEST/2];
double imaginary_test [N_OF_SAMPLES_TEST/2];
#endif //TEST

//for mean
double Signal_mean (double *sig_src_arr, uint32_t sig_length);
double Signal_variance(double *sig_src_arr, double sig_mean, uint32_t sig_length);

double mean;
double variance;
double std_dev;
void Running_sum (double* sig_src_arr, double* sig_out_arr, int sig_length);
void First_difference (double *sig_src_arr, double *sig_dest_arr,uint32_t sig_length);
void calc_sig_dft(double *sig_src_arr, double *sig_dest_rex_arr, double *sig_dest_imx_arr, uint32_t sig_length);
void get_dft_output_mag (double* REX, double* IMX);

void setup() {
  Serial.begin (115200);
  sampling_period_us = round (1000000 * (1.0/SAMPLING_FREQUENCY));
  
}

void loop() {

#ifndef TEST 
  for (int i = 0; i < N_OF_SAMPLES_TEST; i++){
    microseconds = micros();   
    samples [i] = analogRead (A0);

    //waits for desired sampling period 
    while (micros() < (microseconds + sampling_period_us)){}; 
  }
  for (int i = 0; i < N_OF_SAMPLES; i++ ){
    Serial.println (samples [i]);
    delay (10);
  }
  
#endif 

  

  #ifdef TEST
  for (int i = 0; i < N_OF_SAMPLES_TEST; i++ ){
    Serial.println (test_signal [i]);
    delay (10);
  }

  mean = Signal_mean (test_signal, 180);
  variance = Signal_variance (test_signal, mean, 180);
  std_dev = sqrt (variance);
  delay (300);

  Serial.print ("mean: ");
  Serial.print (mean);
  Serial.print (" variance: ");
  Serial.print (variance);
  Serial.print (" standard deviation: ");
  Serial.println (std_dev);
  
  Serial.println ("TEST RUNNING SUM");
  Running_sum (test_signal, RS_out_arr_test, N_OF_SAMPLES_TEST);
  for (int i = 0; i < N_OF_SAMPLES_TEST; i++ ){
    Serial.print (test_signal [i]);
    Serial.print (",");
    Serial.println (RS_out_arr_test [i]);
    delay (10);
  }
  
  delay (300);
  Serial.println ("TEST FIRST DIFFERENCE");
  First_difference (test_signal, FD_out_arr_test, N_OF_SAMPLES_TEST);
  for (int i = 0; i < N_OF_SAMPLES_TEST; i++ ){
    Serial.print (test_signal [i]);
    Serial.print (",");
    Serial.println (FD_out_arr_test [i]);
    delay (10);
  }

  delay (300);
  Serial.println ("TEST DFT OF TEST SIGNAL");
  calc_sig_dft(test_signal, real_test, imaginary_test, N_OF_SAMPLES_TEST);
  for (int i = 0; i < N_OF_SAMPLES_TEST; i++)
    test_signal [i] = 0;
  get_dft_output_mag(test_signal, imaginary_test);

  for (int i = 0; i < N_OF_SAMPLES_TEST; i++ ){
    Serial.println (test_signal [i]);
    delay (10);
  }
  delay (300);
  Serial.println ("TEST DFT OF RUNNING SUM");
  calc_sig_dft(RS_out_arr_test, real_test, imaginary_test, N_OF_SAMPLES_TEST);
  for (int i = 0; i < N_OF_SAMPLES_TEST; i++){
    RS_out_arr_test [i] = 0;
  }
  get_dft_output_mag(RS_out_arr_test, imaginary_test);

  for (int i = 0; i < N_OF_SAMPLES_TEST; i++ ){
    Serial.println (RS_out_arr_test [i]);
    delay (10);
  }

  delay (300);
  Serial.println ("TEST DFT OF FIRST DIFFERENCE");
  calc_sig_dft(FD_out_arr_test, real_test, imaginary_test, N_OF_SAMPLES_TEST);
  for (int i = 0; i < N_OF_SAMPLES_TEST; i++){
        FD_out_arr_test [i] = 0;
  }
  get_dft_output_mag(FD_out_arr_test, imaginary_test);

  for (int i = 0; i < N_OF_SAMPLES_TEST; i++ ){
    Serial.println (FD_out_arr_test [i]);
    delay (10);
    
  }



  #else

  mean = Signal_mean (samples, N_OF_SAMPLES);
  variance = Signal_variance (samples, mean, N_OF_SAMPLES);
  std_dev = sqrt (variance);
  delay (300);

  Serial.print ("mean: ");
  Serial.println (mean);
  Serial.print ("variance: ");
  Serial.println (variance);  
  Serial.print ("standard deviation: ");
  Serial.println (std_dev);
  
  #endif 


/*
  mean = Signal_mean (test_signal, 180);
  variance = Signal_variance (test_signal, mean, 180);
  delay (300);
  Serial.print (mean);
  Serial.print (",");
  Serial.println (variance);
*/
while (1);

  
}

double Signal_mean(double *sig_src_arr, uint32_t sig_length)
{
  double _mean =0.0;
  uint32_t i;  
  for(i=0;i<sig_length;i++)
  {
  _mean  = _mean + sig_src_arr[i];
  }

  _mean = _mean /(double)sig_length;
  
 return _mean;
}

double Signal_variance(double *sig_src_arr, double sig_mean, uint32_t sig_length){
  double _variance = 0.0;
  uint32_t i;
  for (i = 0; i < sig_length; i++){
    _variance += pow (sig_src_arr[i] - sig_mean, 2) / (sig_length - 1);
  }
  //_variance = _variance / (sig_length - 1);
  return _variance;
}


void Running_sum (double* sig_src_arr, double* sig_out_arr, int sig_length){
  int i;
  sig_out_arr[0] = sig_src_arr[0];
  for(i=1; i<sig_length; i++){
      sig_out_arr[i] = (sig_out_arr[i-1] + sig_src_arr[i]);
    }
}

void First_difference (double *sig_src_arr, double *sig_dest_arr,uint32_t sig_length){
    uint32_t i;
    for(i=0;i<sig_length;i++)
    {
      sig_dest_arr[i] = sig_src_arr[i] - sig_src_arr[i-1];
    }
}

void calc_sig_dft(double *sig_src_arr, double *sig_dest_rex_arr, double *sig_dest_imx_arr, uint32_t sig_length)
{
  int i,k,j;
  for(j=0;j<(sig_length/2);j++)
  {
     sig_dest_rex_arr[j] =0;
      sig_dest_imx_arr[j] =0;
  }
  
   for(k=0;k<(sig_length/2);k++)
  {
    for(i=0;i<sig_length;i++)
    {
               sig_dest_rex_arr[k] =  sig_dest_rex_arr[k] + sig_src_arr[i]*cos(2*PI*k*i/sig_length);
               sig_dest_imx_arr[k] =  sig_dest_imx_arr[k] - sig_src_arr[i]*sin(2*PI*k*i/sig_length);
    }
  }
}

void get_dft_output_mag(double* REX, double* IMX)
{
  uint32_t k;
  for(k=0;k<N_OF_SAMPLES_TEST/2;k++)
  {
    REX[k] = sqrt(pow(REX[k],2)+pow(IMX[k],2));
  }
}
