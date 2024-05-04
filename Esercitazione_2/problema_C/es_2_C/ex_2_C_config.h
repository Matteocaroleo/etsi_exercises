#define N_OF_SAMPLES 200
#define PI 3.1415926535897932384626433832795

//#define WAVEFORM_1
#define WAVEFORM_2
//#define WAVEFORM_3

#ifdef WAVEFORM_1
  #define SAMPLING_FREQUENCY 1200 //SHOULD BE 1174, NYQUIST LIMIT
  //#pragma message ("Running waveform 1...")
#endif  //WAVEFORM_1

#ifdef WAVEFORM_2
  #define SAMPLING_FREQUENCY 1350 //SHOULD BE 1318
  //#pragma message ("Running waveform 2...") 
#endif  //WAVEFORM_2

#ifdef WAVEFORM_3
  #define SAMPLING_FREQUENCY 1350 //SHOULD BE 1318
  //#pragma message ("Running waveform 3...")
#endif  //WAVEFORM_3

////////////////////////___ERROR_CHECKING___////////////////////////////////
#if defined (WAVEFORM_1) && defined (WAVEFORM_2) || \
    defined (WAVEFORM_1) && defined (WAVEFORM_3) || \
    defined (WAVEFORM_1) && defined (WAVEFORM_3)
  #error ONLY ONE WAVEFORM MUST BE DEFINED AT THE SAME TIME
#endif

#if !defined (WAVEFORM_1) && !defined (WAVEFORM_2) && !defined (WAVEFORM_3)
  #error AT LEAST ONE WAVEFORM MUST BE DEFINED
#endif





#define SAMPLING_RATE 1/SAMPLING_FREQUENCY