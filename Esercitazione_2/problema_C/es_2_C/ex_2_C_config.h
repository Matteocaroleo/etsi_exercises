/*
  This file is used to configure the .ino file.
  Comment and uncomment #define preferrably only in the
  "SETTINGS" paragraph
*/

////////////////////////___SETTINGS___/////////////////////////

#define N_OF_SAMPLES 128
#define PI 3.1415926535897932384626433832795

#define WAVEFORM_1
//#define WAVEFORM_2
//#define WAVEFORM_3

#define PLOT_OVERLAPPED //if not definesd, will plot signal one after another

#define SAMPLE_AT_LIMIT //If not defined, it will sample higher to show all impulses



/////////////////////__SETTINGS_IMPLEMENTATION__////////////////////

#ifdef WAVEFORM_1
  #ifdef SAMPLE_AT_LIMIT
    #define SAMPLING_FREQUENCY 1114
  #else
    #define SAMPLING_FREQUENCY 1200
  #endif //SAMPLE_AT_LIMIT
#endif  //WAVEFORM_1

#ifdef WAVEFORM_2
  #ifdef SAMPLE_AT_LIMIT
    #define SAMPLING_FREQUENCY 1318 
  #else 
    #define SAMPLING_FREQUENCY 1600
  #endif //SAMPLE_AT_LIMIT
#endif  //WAVEFORM_2

#ifdef WAVEFORM_3
  #ifdef SAMPLE_AT_LIMIT
    #define SAMPLING_FREQUENCY 1318
  #else
    #define SAMPLING_FREQUENCY 1600
  #endif //SAMPLE_AT_LIMIT
#endif  //WAVEFORM_3


////////////////////////___ERROR_CHECKING___////////////////////////////////

#if defined (WAVEFORM_1) && defined (WAVEFORM_2) || \
    defined (WAVEFORM_1) && defined (WAVEFORM_3) || \
    defined (WAVEFORM_1) && defined (WAVEFORM_3)
  #error ONLY ONE WAVEFORM MUST BE DEFINED AT THE SAME TIME: comment the unused #define
#endif

#if !defined (WAVEFORM_1) && !defined (WAVEFORM_2) && !defined (WAVEFORM_3)
  #error AT LEAST ONE WAVEFORM MUST BE DEFINED
#endif