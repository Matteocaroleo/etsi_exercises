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

//EX_FULL configuration defines
/////////////////////////////////////
#define EX_FULL
#ifdef EX_FULL
  #undef FILTER_RS
  #undef N_OF_SAMPLES 
  #define N_OF_SAMPLE 100
  #define QUANT_4_BIT
  #define QUANT_8_BIT
  #define FILTER_RMA
#endif //EX_FULL
//////////////////////////////////////


//USED FOR RUNNING THE TEST ROUTINE: uncomment line below for usage 
//#define TEST
#ifdef TEST
  #undef EX1_FULL
  #define N_OF_SAMPLES_TEST 80
  #define FILTER_PTS_TEST 5
#endif //TEST