/*
 * KIMUNO configuration file
*/

#include <Arduino.h>

//-----------------------------------
//----- DEFINE _TRUE AND _FALSE --
//-----------------------------------
#define FALSE          0
#define TRUE           !(FALSE)
//-----------------------------------
//----- Programs to include in ROM --
//-----------------------------------
#define _INCL_CALCULATOR  FALSE
#define _INCL_MCHESS      TRUE
#define _INCL_VTL02       FALSE
//#define vtl02RomStart 0xFC00
//#define vtl02RomEnd   0xFFFA
#define _DEMO_TIMER       TRUE
#define _DEMO_HILO        FALSE
//-----------------------------------
//------- Target to compile for -----
//-----------------------------------
#define ARDUINO_UNO         1
#define ARDUINO_MINI        2
#define ARDUINO_MEGA        3
#define ARDUINO_DUE         4
#define PERSONAL_COMPUTER   8
//-----------------------------------
//-->>> Uncomment all but one! <<<--- 
//-----------------vvvvvvvvvvvvvvvvv-
//#define _TARGET  ARDUINO_UNO
#define _TARGET  ARDUINO_MINI
//#define _TARGET  ARDUINO_MEGA
//#define _TARGET  ARDUINO_DUE
//#define _TARGET  PERSONAL_COMPUTER
//-----------------^^^^^^^^^^^^^^^^^-
//-->>> Board to compile for <<<-----
//-----------------------------------

//------- Memory Addresses
#if _TARGET == ARDUINO_UNO || _TARGET == ARDUINO_MINI 
    #define _MIN_RAM2_ADDRESS   0x0400
    #define _MAX_RAM2           64
    #define _MAX_RAM2_ADDRESS   0x0440
    #define _USE_KIMDOS         FALSE

#elif ( _TARGET == ARDUINO_MEGA || _TARGET == ARDUINO_DUE )
    #define _MIN_RAM2_ADDRESS   0x0400
    #define _MAX_RAM2           2048    // extra 2KB
    #define _MAX_RAM2_ADDRESS   0x0C00
  //#define _MAX_RAM2           4096    // extra 4KB
  //#define _MAX_RAM2_ADDRESS   0x1400
    //----- _USE_KIMDOS ('true' or 'false') -- only on DUE or MEGA
    #define _USE_KIMDOS         TRUE

#elif _TARGET == PERSONAL_COMPUTER
    #define _MIN_RAM2_ADDRESS   0x0400
    #define _MAX_RAM2           1024
    #define _MAX_RAM2_ADDRESS   0x0800
    #define _USE_KIMDOS         FALSE

#else
    #error No valid _TARGET defined
#endif

//----- SHOW CPU_STATUS (FLAGS) on TTY ('true' or 'false')
#define _CPU_FLAGS_2_TTY        TRUE


