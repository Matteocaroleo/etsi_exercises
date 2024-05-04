/*  This file is used to configure the ex1_full.ino file. */

#define N_OF_SAMPLES 200
#define SAMPLING_FREQUENCY 4000
#define FILTER_RMA
//#define FILTER_RS

#ifdef FILTER_RMA
  #define FILTER_PTS 10
#endif

#define QUANT_4_BIT
#define QUANT_8_BIT


//EX_FULL configuration defines requested in assignment. It uses RMA as a low-pass filter 
//////////////////////////////////////////////////////////////////
#define EX_FULL
#ifdef EX_FULL
  //the undef are used to reset macros to wanted values
  #undef FILTER_RS
  #undef N_OF_SAMPLES 
  #undef SAMPLING_FREQUENCY
  #undef FILTER_PTS

  //EX_FULL parameters:
  #define ANALOG_PIN A0
  #define SAMPLING_FREQUENCY 1000 //should not be higher than 10k (arduino limit)

  //WARNING: the number of samples should not be increased (only decreased)
  #define N_OF_SAMPLES 100

  //loads in program memory the filter RMA function
  #define FILTER_RMA
  
  //parameter given to RMA function
  #define FILTER_PTS 5
#endif //EX_FULL
////////////////////////////////////////////////////////////////////////


//USED FOR RUNNING THE TEST ROUTINE: uncomment line below for usage
////////////////////////////////////////////////////////////////////////// 
//#define TEST
#ifdef TEST
  #undef EX1_FULL
  #undef QUANT_4_BIT 
  #undef QUANT_8_BIT
  #define N_OF_SAMPLES_TEST 80
  #define FILTER_PTS_TEST 10
#endif //TEST
/////////////////////////////////////////////////////////////////////////