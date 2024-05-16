/* MATTEO CAROLEO S5216938
  This sketch generates echo from an input signal with random delay (from OFFSET to 27)
  It then detects the value of delay using cepstrum and applies echo cancelling filter.
  It plots both the input and the input with echo AND both input and signal with echo
  AFTER cancellation. 
  At the end, it prints the calculated delay time.
*/

#include "arduinoFFT.h"

//used for skipping first values of cepstrum
#define OFFSET 3

//input signal (no echo)
double Input[128] = {
  0.0065f, -0.0078f, -0.0551f, -0.0712f, -0.1070f, -0.1192f, -0.1157f, -0.1163f,
  -0.0922f, -0.0726f, -0.0536f, -0.0456f, -0.0421f, -0.0285f, -0.0166f, 0.0172f,
  0.0251f, 0.0224f, 0.0251f, 0.0083f, -0.0037f, 0.0161f, 0.0246f, 0.0221f, 0.0243f,
  0.0213f, 0.0387f, 0.0478f, 0.0464f, 0.0445f, 0.0623f, 0.0694f, 0.0758f, 0.0962f,
  0.1144f, 0.1445f, 0.1814f, 0.1736f, 0.1770f, 0.1526f, 0.1530f, 0.1656f, 0.1854f,
  0.1794f, 0.1608f, 0.1361f, 0.0980f, 0.0564f, 0.0286f, -0.0135f, -0.0330f, -0.0475f,
  -0.0748f, -0.1083f, -0.1349f, -0.1610f, -0.1787f, -0.1870f, -0.1625f, -0.1648f,
  -0.1530f, -0.1415f, -0.1495f, -0.1509f, -0.1711f, -0.1951f, -0.2495f, -0.2257f,
  -0.3103f, -0.3021f, -0.2914f, -0.2981f, -0.2834f, -0.2428f, -0.2961f, -0.2447f,
  -0.2410f, -0.2641f, -0.2373f, -0.2452f, -0.2897f, -0.2269f, -0.2360f, -0.2227f,
  -0.2014f, -0.2290f, -0.2166f, -0.2370f, -0.2094f, -0.1896f, -0.1898f, -0.1652f,
  -0.1139f, -0.0772f, -0.0600f, -0.0103f, 0.0210f, 0.0302f, 0.0525f, 0.0705f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f
};

arduinoFFT FFT = arduinoFFT();


double InputWithEcho[128];
double vImag[128] = {0};
int randomDelay = 0;

//delay a da 1 a 27
void echoGeneration(double* Input, double* InputWithEcho, int detectedDelay, int size);
int delayDetector();
void echoCancellationFilter(int detected_delay, double* InputWithEcho, double* Output, int size);
void Plot_signal (double* sig_src_arr, int sig_length);
void Plot_two_signals_overlapped_1(double* first_sig, double* second_sig, int lenght_of_sigs);
void Plot_two_signals_overlapped_2(double* first_sig, double* second_sig, int lenght_of_sigs);
bool Two_sig_are_same(double* first_sig, double* second_sig, int size);

int detectedDelay = 0;

void setup() {
  Serial.begin(9600);
}


void loop() {
  //generate a random delay each time the program is started
  randomSeed(analogRead(A0));
  randomDelay = random(OFFSET, 27);

  //generates echo
  echoGeneration(Input, InputWithEcho, randomDelay, 128);
  Plot_two_signals_overlapped_1(Input, InputWithEcho, 128);

  //resets array
  for (int i = 0; i < 128; i++) {
    vImag[i] = 0;
  }

  //calculates cepstrum
  FFT.Compute(InputWithEcho, vImag, 128, FFT_FORWARD);
  FFT.ComplexToMagnitude(InputWithEcho, vImag, 128);
  for (int j = 0; j < 128; j++) {
    //when x = 0, log(x) = inf. This excludes this case.
    if (InputWithEcho[j] == 0) {
      InputWithEcho[j] = -60000;
    } else {
      InputWithEcho[j] = log(InputWithEcho[j]);
    }
    vImag[j] = 0;
  }

  //computes cepstrum
  FFT.Compute(InputWithEcho, vImag, 128, FFT_REVERSE);

  //detects delay
  detectedDelay = delayDetector(InputWithEcho, 128);
  
  //regenerates signal with eco
  echoGeneration(Input, InputWithEcho, detectedDelay, 128);

  //filters the signal with eco. The output is stored in vImag array
  echoCancellationFilter(detectedDelay, InputWithEcho, vImag, 128);

  //plots original signal (no echo) and output of echo cancelling filter
  Plot_two_signals_overlapped_2(Input, vImag, 128);

  //prints delays
  Serial.print("Generated delay with rand: ");
  Serial.println(randomDelay);
  Serial.print("Detected delay with function: ");
  Serial.println(detectedDelay);
  while (1);
}


void echoGeneration(double* Input, double* InputWithEcho, int randomDelay, int size) {
  //copies Input into InputEcho
  for (int i = 0; i < size; i++) {
    InputWithEcho[i] = Input[i];
  }

  //sums signal delayed to create echo
  for (int i = 0; i < 100; i++) {
    InputWithEcho[i + randomDelay] = InputWithEcho[i + randomDelay] + Input[i];
  }
}

int delayDetector(double* InputWithEcho, int size) {
  float temp = 0;
  int index = 0;
  for (int i = OFFSET; i < size / 2; i++) {
    if (temp < InputWithEcho[i]) {
      temp = InputWithEcho[i];
      index = i;
    }
  }
  return index;
}

void echoCancellationFilter(int detected_delay, double* InputWithEcho, double* Output, int size) {
  for (int i = 0; i < size; i++) {
    if (i < detected_delay) {
      Output[i] = InputWithEcho[i];
    } else {
      Output[i] = InputWithEcho[i] - Output[i - detected_delay];
    }
  }
}

void Plot_signal(double* sig_src_arr, int sig_length) {
  for (int i = 0; i < sig_length; i++) {
    Serial.print(" ");
    Serial.println(sig_src_arr[i], 5);
    delay(10);
  }
}

void Plot_two_signals_overlapped_1(double* first_sig, double* second_sig, int lenght_of_sigs) {
  for (int i = 0; i < lenght_of_sigs; i++) {
    Serial.print("SIG:");
    Serial.print(first_sig[i], 5);
    Serial.print("\t");
    Serial.print("SIG_WITH_ECHO:");
    Serial.print(second_sig[i], 5);
    Serial.println();
  }

  Serial.println();
}

void Plot_two_signals_overlapped_2(double* first_sig, double* second_sig, int lenght_of_sigs) {

  for (int i = 0; i < lenght_of_sigs; i++) {
    Serial.print("SIG:");
    Serial.print(first_sig[i], 5);
    Serial.print("\t");
    Serial.print("FILTERED_SIG:");
    Serial.print(second_sig[i], 5);
    Serial.println();
  }

  Serial.println();
}

bool Two_sig_are_same(double* first_sig, double* second_sig, int size) {
  bool result = true;
  int i = 0;
  for (i = 0; i < size; i++) {
    if (first_sig[i] != second_sig[i]) {
      result = false;
      break;
    }
  }
  Serial.print("DIFFERENT SAMPLE: ");
  Serial.println(i);
  return result;
}