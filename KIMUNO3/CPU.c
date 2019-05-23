/* KIM-I emulator for Arduino.
*  Most of this is the 6502 emulator from Mike Chambers,
*  http://forum.arduino.cc/index.php?topic=193216.0
*  http://forum.arduino.cc/index.php?PHPSESSID=po7fh9f1tkuorr32n58jgofc61&action=dlattach;topic=193216.0;attach=56567
*  KIM-I hacked in by Oscar
*  http://obsolescenceguaranteed.blogspot.ch/
*    
*   Coding style
*       - One True Brace Style formatting/indenting.
*       - Linux brackets, add brackets to all conditionals.
*/

#define TOGGLE(a,b)             ((a) ^=  _BV(b))
#define SET(a,b)                ((a) |=  _BV(b))
#define CLEAR(a,b)              ((a) &= ~_BV(b))
#define SET_OUTPUT(_port, _pin) ((_port) |=  _BV(_pin))


#include <stdio.h>
#include <stdint.h>
#include "Config.h"

#if _TARGET == PERSONAL_COMPUTER
    #include "cpu.h" //<<--------- ???????????
    #include <stdio.h>
    #pragma warning(disable : 4996) // MS VC2008 does not like unsigned char -> signed char converts.
    extern unsigned char movitROM[];
    extern unsigned char relocateROM[];
    extern unsigned char branchROM[];
    extern unsigned char disasmROM[];
    #if _INCL_CALCULATOR == TRUE
        extern unsigned char calcROM[];
    #endif
    #if _INCL_MCHESS == TRUE
        extern unsigned char mchessROM[]; 
    #endif
    #if _INCL_VTL02 == TRUE
        extern unsigned char vtl02ROM[];
    #endif

#else                   // ARDUINO
//  #define NULL (void *) 0
    #include <avr/pgmspace.h>
    #if _TARGET == ARDUINO_UNO || _TARGET == ARDUINO_MINI
        extern uint8_t eepromread(uint16_t eepromaddress);
        extern void eepromwrite(uint16_t eepromaddress, uint8_t bytevalue);
    #endif
    extern const unsigned char movitROM[] PROGMEM;
    extern const unsigned char relocateROM[] PROGMEM;
    extern const unsigned char branchROM[] PROGMEM;
    extern const unsigned char disasmROM[] PROGMEM;
    #if _INCL_CALCULATOR == TRUE
        extern const unsigned char calcROM[] PROGMEM;
    #endif
    #if _INCL_MCHESS == TRUE
        extern const unsigned char mchessROM[] PROGMEM;
    #endif
    #if _INCL_VTL02 == TRUE
        extern const unsigned char vtl02ROM[] PROGMEM;
    #endif
#endif

//#define WREG_OFFSET 0x03DF
#define WREG_OFFSET 0x0360
//#define DEBUGUNO

#ifdef DEBUGUNO
    //--------------- debug
    extern FILE *fpx;
    uint16_t debugPC;
    // --------------
#endif

//extern char threeHex[3][2];     // buffer for 3 hex digits
extern uint8_t segm7Mem[6];
extern byte decodeRAW[8];

extern uint8_t selDigitLED;     // selected 7segm display
extern uint8_t displayLEDmode;  // selected 7segm display
extern int blitzMode;           // status variable only for microchess

extern void printhex(uint16_t val);
extern void serout(uint8_t value);
extern void serouthex(uint8_t val);
extern uint8_t getAkey(void);   // for serial port get normal ASCII keys
extern uint8_t getKIMkey();     // for emulation of KIM keypad
extern void clearkey(void);
extern void driveLEDs();
extern void driveRAW(uint8_t digitNo);
extern void scanKeys();

void nmi6502(void);

uint8_t useKeyboardLed=0x01;    // set to 0 to use Serial port, to 1 to use onboard keyboard/LED display.
uint8_t iii;                    // counter for various purposes, declared here to avoid in-function delay in 6502 functions.
uint8_t nmiFlag=0;              // added by OV to aid single-stepping SST mode on KIM-I
uint8_t SSTmode = 0;            // SST switch in KIM-I: 1 = on.
uint8_t localEcho = 0;          // "1" = do echo, "0" = Don't echo
uint8_t doShowCPUflags = 0;     // well, as it say's...
uint8_t nibbleH, nibbleL;
uint8_t col, currentBit, bitOn, decodeOut;


#define FLAG_CARRY     0x01
#define FLAG_ZERO      0x02
#define FLAG_INTERRUPT 0x04
#define FLAG_DECIMAL   0x08
#define FLAG_BREAK     0x10
#define FLAG_CONSTANT  0x20
#define FLAG_OVERFLOW  0x40
#define FLAG_SIGN      0x80
#define BASE_STACK     0x100

#define saveaccum(n) a = (uint8_t)((n) & 0x00FF)

//flag modifier macros
#define setcarry() cpustatus |= FLAG_CARRY
#define clearcarry() cpustatus &= (~FLAG_CARRY)
#define setzero() cpustatus |= FLAG_ZERO
#define clearzero() cpustatus &= (~FLAG_ZERO)
#define setinterrupt() cpustatus |= FLAG_INTERRUPT
#define clearinterrupt() cpustatus &= (~FLAG_INTERRUPT)
#define setdecimal() cpustatus |= FLAG_DECIMAL
#define cleardecimal() cpustatus &= (~FLAG_DECIMAL)
#define setoverflow() cpustatus |= FLAG_OVERFLOW
#define clearoverflow() cpustatus &= (~FLAG_OVERFLOW)
#define setsign() cpustatus |= FLAG_SIGN
#define clearsign() cpustatus &= (~FLAG_SIGN)

//flag calculation macros
#define zerocalc(n) { if ((n) & 0x00FF) clearzero(); else setzero(); }
#define signcalc(n) { if ((n) & 0x0080) setsign(); else clearsign(); }
#define carrycalc(n) { if ((n) & 0xFF00) setcarry(); else clearcarry(); }
#define overflowcalc(n, m, o) { if (((n) ^ (uint16_t)(m)) & ((n) ^ (o)) & 0x0080) setoverflow(); else clearoverflow(); }


//6502 CPU registers
uint16_t pc;
uint8_t sp, a, x, y, cpustatus;
// BCD fix OV 20140915 - helper variables for adc and sbc
uint16_t lxx,hxx;
// end of BCD fix part 1

//helper variables
uint32_t instructions = 0; //keep track of total instructions executed
int32_t clockticks6502 = 0, clockgoal6502 = 0;
uint16_t oldpc, ea, reladdr, value, result;
uint8_t opcode, oldcpustatus, useaccum;

// FLTPT65 SUPPORT ------------------------------------------------------------------
void copyWreg(uint8_t a, uint8_t b);
void swapWreg(uint8_t a, uint8_t b);
extern uint8_t enterflt(uint8_t reg);
extern uint8_t showflt(uint8_t reg);
extern uint8_t enteroperation(void);

// --- OVERVIEW OF KIM-1 MEMORY MAP -------------------------------------------------
uint8_t RAM[1024];  // main 1KB RAM      0x0000-0x03FF
#if _TARGET == PERSONAL_COMPUTER
    uint8_t RAM2[_MAX_RAM2];    // on PC, ram in     0x0400-0x08FF RAM on PC

#elif _TARGET == ARDUINO_UNO || _TARGET == ARDUINO_MINI 
    uint8_t RAM2[_MAX_RAM2];    // on ARDUINO       0x0400-0x08FF EEPROM
                                // on Arduino, these are EEPROM functions
                                
#elif _TARGET == ARDUINO_MEGA || _TARGET == ARDUINO_DUE
    uint8_t RAM2[_MAX_RAM2];    // ram in     0x0400-0x1400 
    
#endif
// empty                                     0x0900-0x13FF
// I/O and timer of 6530-003, free for user  0x1700-0x173F, not used in KIM ROM
#if _TARGET == ARDUINO_MEGA || _TARGET == ARDUINO_DUE
    uint8_t M1701;                      //   0x1700 PortA, 0x1701 DDR Port A
    uint8_t M1703;                      //   0x1702 PortB, 0x1703 DDR Port B
#endif
// I/O and timer of 6530-002, used by KIM    0x1740-0x177F, used by LED/Keyboard
uint8_t RAM003[64];    // RAM from 6530-003  0x1780-0x17BF, free for user applications
uint8_t RAM002[64];    // RAM from 6530-002  0x17C0-0x17FF, free for user except 0x17E7-0x17FF
// ROM003 is                                 0x1800-0x1BFF
// ROM002 is                                 0x1C00-0x1FFF

// note that above 8K map is not replicated 8 times to fill 64K,
// but INSTEAD, emulator mirrors last 6 bytes of ROM 002 to FFFB-FFFF:
//               FFFA, FFFB - NMI Vector
//               FFFC, FFFD - RST Vector
//               FFFE, FFFF - IRQ Vector
// Application roms (mchess, calc) are above standard 8K of KIM-1
//
// --- ROM CODE SECTION ------------------------------------------------------------
// ROM1: KIM-1 ROM002 (monitor main code)                                 $17C0
// ROM2: KIM-1 ROM003 (tape and RS-232 code)                              $1780
// mchess: the updated microchess version from www.benlo.com/microchess
//                                                 recompiled to start at $C000
// calc: the 6502 floating point library from www.crbond.com/calc65.htm
//                                                 recompiled to start at $D000
// disassembler: the famous Baum & Wozniak disassember
//    6502.org/documents/publications/6502notes/6502_user_notes_14.pdf at $2000
// storage arrays that are copied to RAM are also here:
// relocate (first book of kim)                                        to $0110
// branch (first book of kim)                                          to $01A5
// movit (first book of kim)                                           to $1780
// ---------------------------------------------------------------------------------
#if _TARGET == PERSONAL_COMPUTER
    extern unsigned char ROM002[];
    extern unsigned char ROM003[];
#else
    extern const unsigned char ROM002[] PROGMEM;
    extern const unsigned char ROM003[] PROGMEM;
#endif

/******************(AaW)*************
 * ===============>>>>> WHAT IS THIS???? | IT IS NEVER USED <<<<<===================
// --- CRBond calculator RAM workspace: --------------------------------------------
// - NOTE THAT UPPER 128 BYTES OF KIM RAM IS USED BY _INCL_CALCULATOR AS WORKSPACE!
// - W1 starts at 3Df
// - W2 starts at 3E7
// - W3 starts at 3EF
//uint8_t getRegister(uint8_t reg, uint8_t *inVal);  // returns place of decimal point

uint8_t getRegister(uint8_t reg, uint8_t *inVal)
{
    uint16_t offset;
//  uint8_t i;
//  uint8_t *eptr;

    offset =  WREG_OFFSET + reg*8; // place in RAM for W1/2/3/4 registers

//  sprintf(inVal, "%c%c.%c%c%c%c%c%c%c%c%c%c%ce%c%c%c%c",
// -----mantissa sign--
//      (RAM[0+offset]&(uint8_t)128)==0?'+':'-',
// -----mantissa--
//      ((RAM[2+offset]&0xF0)>>4) + (uint8_t)48, (RAM[2+offset]&0x0F) + (uint8_t)48,
//      ((RAM[3+offset]&0xF0)>>4) + (uint8_t)48, (RAM[3+offset]&0x0F) + (uint8_t)48,
//      ((RAM[4+offset]&0xF0)>>4) + (uint8_t)48, (RAM[4+offset]&0x0F) + (uint8_t)48,
//      ((RAM[5+offset]&0xF0)>>4) + (uint8_t)48, (RAM[5+offset]&0x0F) + (uint8_t)48,
//      ((RAM[6+offset]&0xF0)>>4) + (uint8_t)48, (RAM[6+offset]&0x0F) + (uint8_t)48,
//      ((RAM[7+offset]&0xF0)>>4) + (uint8_t)48, (RAM[7+offset]&0x0F) + (uint8_t)48,
// -----exponent sign--
//      (RAM[0+offset]&(uint8_t)64)==0?'+':'-',
// -----exponent--
//      (RAM[0+offset]&0x0F) + (uint8_t)48,
//      ((RAM[1+offset]&0xF0)>>4) + (uint8_t)48, (RAM[1+offset]&0x0F) + (uint8_t)48 );


    //  print in most compact form
    // 2. do we print a leading '-' cos mantissa negative?
    if ((RAM[0+offset]&(uint8_t)128)==0) { // mantissa positive, skip the + to save a digit
        sprintf(inVal, "%c%c%c%c%c%c%c",
                // mantissa sign
                //(RAM[0+offset]&(uint8_t)128)==0?'+':'-',
                // mantissa
                ((RAM[2+offset]&0xF0)>>4) + (uint8_t)48, (RAM[2+offset]&0x0F) + (uint8_t)48,
                ((RAM[3+offset]&0xF0)>>4) + (uint8_t)48, (RAM[3+offset]&0x0F) + (uint8_t)48,
                //((RAM[4+offset]&0xF0)>>4) + (uint8_t)48, (RAM[4+offset]&0x0F) + (uint8_t)48,
                //((RAM[5+offset]&0xF0)>>4) + (uint8_t)48, (RAM[5+offset]&0x0F) + (uint8_t)48,
                //((RAM[6+offset]&0xF0)>>4) + (uint8_t)48, (RAM[6+offset]&0x0F) + (uint8_t)48,
                //((RAM[7+offset]&0xF0)>>4) + (uint8_t)48, (RAM[7+offset]&0x0F) + (uint8_t)48,
                // exponent sign
                (RAM[0+offset]&(uint8_t)64)==0?(uint8_t)62:(uint8_t)64, // E, or G is printed as -
                // exponent
//!!!//         //(RAM[0+offset]&0x0F) + (uint8_t)48,
                ((RAM[1+offset]&0xF0)>>4) + (uint8_t)48, (RAM[1+offset]&0x0F) + (uint8_t)48 );
        return 0; // light dec point on first digit
    } else { // mantissa negative, add a minus
        sprintf(inVal, "%c%c%c%c%c%c%c",  //G printed as -
                // mantissa sign
                64, // G printed as -
                //(RAM[0+offset]&(uint8_t)128)==0?'+':'-',
                // mantissa
                ((RAM[2+offset]&0xF0)>>4) + (uint8_t)48, (RAM[2+offset]&0x0F) + (uint8_t)48,
                ((RAM[3+offset]&0xF0)>>4) + (uint8_t)48, //(RAM[3+offset]&0x0F) + (uint8_t)48,
                //((RAM[4+offset]&0xF0)>>4) + (uint8_t)48, (RAM[4+offset]&0x0F) + (uint8_t)48,
                //((RAM[5+offset]&0xF0)>>4) + (uint8_t)48, (RAM[5+offset]&0x0F) + (uint8_t)48,
                //((RAM[6+offset]&0xF0)>>4) + (uint8_t)48, (RAM[6+offset]&0x0F) + (uint8_t)48,
                //((RAM[7+offset]&0xF0)>>4) + (uint8_t)48, (RAM[7+offset]&0x0F) + (uint8_t)48,
                // exponent sign
                (RAM[0+offset]&(uint8_t)64)==0?(uint8_t)62:(uint8_t)64,  //E or G printed as -
                // exponent
//!!!//         //(RAM[0+offset]&0x0F) + (uint8_t)48,
                ((RAM[1+offset]&0xF0)>>4) + (uint8_t)48, (RAM[1+offset]&0x0F) + (uint8_t)48 );

        return 1; // light dec point on second digit (given 1st digit is a -)
    }
}   // getRegister()
************(AaW)************/
void Dprint(char *T)
{
    for (int P=0; P<strlen(T); P++) {
        serout(T[P]);    
    }
}   // Dprint()

void printError(uint16_t address, char Error)
{
    uint8_t hexError;    
    hexError += (uint8_t) Error;
    segm7Mem[0]= (address  & 0xF000) >> 12;
    segm7Mem[1]= (address  & 0x0F00) >> 8;
    segm7Mem[2]= (address  & 0x00F0) >> 4;
    segm7Mem[3]= (address  & 0x000F);
    segm7Mem[4]= 0xE0 >> 4;
    segm7Mem[5]= (Error & 0x0F);

    serout(13);
    serout('*');
    serout(' ');
    for (iii=0; iii<4; iii++) {
        serouthex(segm7Mem[iii]);
    }
    serout(' ');
    for (iii=4; iii<6; iii++) {
        serouthex(segm7Mem[iii]);
    }
    serout(' ');
    serout('*');
    serout('\r');
    serout('\n');
    //--- print original address
    segm7Mem[0]= (address  & 0xF000) >> 12;
    segm7Mem[1]= (address  & 0x0F00) >> 8;
    segm7Mem[2]= (address  & 0x00F0) >> 4;
    segm7Mem[3]= (address  & 0x000F);

    serout(13);
    for (iii=0; iii<4; iii++) {
        serouthex(segm7Mem[iii]);
    }
    serout(' ');
    
}   // printError()

void showCPUflags()
{
#if _CPU_FLAGS_2_TTY == TRUE
    serout('\r');
    serout('\n');
    serout('S');
    serout('R');
    serout(':');
    serout(' ');
    //if (cpustatus & BASE_STACK)      serout('B');
    //else                             serout('b');        
    if (cpustatus & FLAG_SIGN)         serout('N');
    else                               serout('-');        
    if (cpustatus & FLAG_OVERFLOW)     serout('V');
    else                               serout('-');        
    //if (cpustatus & FLAG_CONSTANT)   serout('C');
    //else                             serout('-');        
    serout('-'); 
    if (cpustatus & FLAG_BREAK)        serout('B');
    else                               serout('-');       
    if (cpustatus & FLAG_DECIMAL)      serout('D');
    else                               serout('-');        
    if (cpustatus & FLAG_INTERRUPT)    serout('I');
    else                               serout('-');        
    if (cpustatus & FLAG_ZERO)         serout('Z');
    else                               serout('-');        
    if (cpustatus & FLAG_CARRY)        serout('C');
    else                               serout('-');      
    Dprint(" Acc["); serouthex(RAM[0xF3]);
    Dprint("], Xreg["); serouthex(RAM[0xF5]);
    Dprint("], Yreg["); serouthex(RAM[0xF4]);
    Dprint("]\r");
#endif
    doShowCPUflags = 0;

}   // showCPUflags()


uint8_t read6502(uint16_t address)
{
    uint8_t tempval = 0;

    if (address < 0x0400) {               // 0x0000-0x0400 is RAM
        return(RAM[address]);
    }
//  if (address < 0x0800) {
    if (address < _MAX_RAM2_ADDRESS) {
#if _TARGET == ARDUINO_UNO || _TARGET == ARDUINO_MINI
        return(eepromread(address-_MIN_RAM2_ADDRESS)); // 0x0400-0x0800 is EEPROM for Arduino,

#elif _TARGET == ARDUINO_MEGA || _TARGET == ARDUINO_DUE || _TARGET == PERSONAL_COMPUTER
        return(RAM2[address-_MIN_RAM2_ADDRESS]);       // ...but second RAM unit for PC or MEGA or DUE

#endif
    }                                       // 0x0800-0x1700 is empty space, should not be read
    if (address < 0x1700) {                 // read in empty space
        //serout('%'); serout('5');         // error code 5 - read in empty space
        printError(address, '5');
        return(0);
    }
    if (address < 0x1740) {                 // 0x1700-0x1740 is IO space of RIOT 003
        #if _TARGET == ARDUINO_MEGA        
            // PORTA are pins 22 - 29
            if (address == 0x1700) {            // read port A (AaW)
                return(PORTA);                  // hele poort inlezen (AaW)
            }
            if (address == 0x1701) {            // read DDR port A (AaW)
                return(DDRA);                   // hele poort inlezen (AaW)
            }
            // PORTC are pins 37 - 30
            if (address == 0x1702) {            // read port B (AaW)
                return(PORTC);                  // hele poort inlezen (AaW)
            }
            if (address == 0x1703) {            // read DDR port B (AaW)
                return(DDRC);                   // hele poort inlezen (AaW)
            }
        #else       // the whole KIMports is probably not needed (I will check this later)
            if (address == 0x1700) {            // read port A (AaW)
                return(readPORT('A'));          // hele poort inlezen (AaW)
            }
            if (address == 0x1701) {            // read DDR port A (AaW)
                return(readDDR('A'));          // hele poort inlezen (AaW)
            }
            if (address == 0x1702) {            // read port B (AaW)
                return(readPORT('B'));          // hele poort inlezen (AaW)
            }
            if (address == 0x1703) {            // read DDR port B (AaW)
                return(readDDR('B'));          // hele poort inlezen (AaW)
            }
        #endif
        if ((address >= 0x1704) && (address <= 0x1707)) {            // read timer (AaW)
            if (SSTmode == 1) {
                if (address == 0x1707) {
                    return(0X80);               // always bailout in SST mode
                }
                else {
                    value = timerRead(address);
                    return(value);
                }
            } 
            value = timerRead(address);      // status timer (AaW)
            return(value);
        }

        //serout('%');   serout('7');       // trap code 7 - read in I/O 003
        printError(address, '7');
        return(0);
    }
    if (address < 0x1780) {                 // 0x1740-0x1780 is IO space of RIOT 002
        if (address == 0x1747) {
            return (0xFF);                  // CLKRDI  =$1747,READ TIME OUT BIT,count is always complete...
        }
        if (address == 0x1740) {
            return (useKeyboardLed);        // returns 1 for Keyboard/LED or 0 for Serial terminal
        }
        //serout('%'); serout('6');         // trap code 6 - read in I/O 002
        printError(address, '6');
        return(0);
    }
    if (address < 0x17C0) {                 // 0x1780-0x17C0 is RAM from RIOT 003
        return(RAM003[address-0x1780]);
    }
    if (address < 0x1800) {                 // 0x17C0-0x1800 is RAM from RIOT 002
        return(RAM002[address-0x17C0]);
    }
    if (address < 0x1C00) {                 // 0x1800-0x1C00 is ROM 003
        #if _USE_KIMDOS == TRUE
            #if _TARGET == ARDUINO_UNO || _TARGET == ARDUINO_MINI
                #error ******* ERROR!! _TARGET UNO or MINI can't have _USE_KIMDOS ********
            #endif
            if (address == 0x1805) {
                KIMdosMenu('W');
                pc = 0x1C4F;
                address = 0x1F1F;
                // continue with SCANDS
            }
            if (address == 0x1875) {
                KIMdosMenu('R');
                pc = 0x1C4F;
                address = 0x1F1F;
                // continue with SCANDS
            }
        #endif  // _USE_KIMDOS
        
        if (address != 0x1F1F) {    // to be able to display KIMDOS info
            #if _TARGET == PERSONAL_COMPUTER
                return ROM003[address - 0x1800];
            #else
                return(pgm_read_byte_near(ROM003 + address - 0x1800));
            #endif
        } // address != 0x1F1F
    }
    if (address < 0x2000) {                 // 0x1C00-0x2000 is ROM 002. It needs some intercepting from emulator...
        if (address == 0x1EA0) {            // intercept OUTCH (send char to serial)
            serout(a);                      // print A to serial
            pc = 0x1ED3;                    // skip subroutine
            return (0xEA);                  // and return from subroutine with a fake NOP instruction
        }
        if (address == 0x1E65) {            // intercept GETCH (get char from serial). used to be 0x1E5A, 
                                            // but intercept *within* routine just before get1 test
            a=getAkey();                    // get A from main loop's curkey
            if (a==0) {
                pc=0x1E60;                  // cycle through GET1 loop for character start, let the 6502 runs 
                return (0xEA);              // through this loop in a fake way
            }
            clearkey();
            x = RAM[0x00FD];                // x is saved in TMPX by getch routine, we need to get it back in x;
            pc = 0x1E87;                    // skip subroutine
            return (0xEA);                  // and return from subroutine with a fake NOP instruction
        }
        if (address == 0x1C2A) {            // intercept DETCPS
            RAM002[0x17F3-0x17C0] = 1;      // just store some random bps delay on TTY in CNTH30
            RAM002[0x17F2-0x17C0] = 1;      // just store some random bps delay on TTY in CNTL30
            pc = 0x1C4F;                    // skip subroutine
            return (0xEA);                  // and return from subroutine with a fake NOP instruction
        }
        /*************** AAW *****/
        if (address == 0x1F1F) {            // intercept SCANDS (display F9,FA,FB)
            // light LEDs ---------------------------------------------------------
            segm7Mem[0]= (RAM[0x00FB] & 0xF0) >> 4;
            segm7Mem[1]= RAM[0x00FB] & 0xF;
            segm7Mem[2]= (RAM[0x00FA] & 0xF0) >> 4;
            segm7Mem[3]= RAM[0x00FA] & 0xF;
            segm7Mem[4]= (RAM[0x00F9] & 0xF0) >> 4;
            segm7Mem[5]= RAM[0x00F9] & 0xF;
            displayLEDmode = 1;         // display F9, FA and FB
            driveLEDs();
            //driveLEDs();
            //pc = 0x1F45;              // skip subroutine part that deals with LEDs
            //pc = 0x1F62;              // skip subroutine part that deals with LEDs
            //return (0xEA);            // and return a fake NOP instruction for this first read in 
                                        // the subroutine, it'll now go to AK

            //#if _TARGET == PERSONAL_COMPUTER                 // remove this line to get led digits on serial for AVR too
            if (useKeyboardLed == 0) {      // (AaW)
                serout(13);
                serout('>');
                serout(' ');
                for (iii=0; iii<4; iii++) {
                    serouthex(segm7Mem[iii]);
                }
                serout (' ');
                for (iii=4; iii<6; iii++) {
                    serouthex(segm7Mem[iii]);
                }
                serout (' ');
                serout('<');
                serout(13);
                pc = 0x1F45;     // AAW         // skip subroutine part that deals with LEDs
                pc = 0x1F62;     // AAW         // skip subroutine part that deals with LEDs
                return (0xEA);   // AAW         // and return a fake NOP instruction for this first read in 

            }    // (AaW)
            //#endif                        // remove this line to get led digits on serial for AVR too
#if _TARGET != PERSONAL_COMPUTER    // (AaW)
            displayLEDmode = 1;     // 0=RAW, 1=by F9, FA and FB
            driveLEDs();
#endif        //  (AaW)

            pc = 0x1F45;                    // skip subroutine part that deals with LEDs
            return (0xEA);                  // and return a fake NOP instruction for this first read in 
                                            // the subroutine, it'll now go to AK
        }   // address = 0x1F1F
        if (address == 0x1EFE) {            // intercept AK (check for any key pressed)
            a=getAkey();                    // 0 means no key pressed - the important bit - but if a key 
                                            // is pressed is curkey the right value to send back?
            //a= getKIMkey();
            if (a==0) {
                a=0xFF;                     // that's how AK wants to see 'no key'
            }
            pc = 0x1F14;                    // skip subroutine and return a fake NOP instruction for this 
            return (0xEA);                  // first read in the subroutine, it'll now RTS at its end
        }
        if (address == 0x1F6A) {            // intercept GETKEY (get key from keyboard)
            //     serout('-');serout('G');serout('K');serout('-');
            a=getKIMkey();                  // curkey = the key code in the emulator's keyboard buffer
            clearkey();                     // skip subroutine part that deals with LEDs and return a 
            pc = 0x1F90;                    // fake NOP instruction for
            return (0xEA);                  // this first read in the subroutine, it'll now RTS at its end
        }
        // if we're still here, it's normal reading from the highest ROM 002.

        #if _TARGET == PERSONAL_COMPUTER
            return ROM002[address - 0x1C00];
        #else
            return(pgm_read_byte_near(ROM002 + address - 0x1C00)); // ROM 002
        #endif
    }

    if (address < 0x21F9) {                 // 0x2000-0x21F8 is disasm
        #if _TARGET == PERSONAL_COMPUTER
            return disasmROM[address - 0x2000];
        #else
            return(pgm_read_byte_near(disasmROM + address - 0x2000));
        #endif
    }

    // 0x6FDF, plus 4 bytes for JSR manually added - Read to floating point library between $5000 and $6157
    if ((address >= 0x5000) && (address <= 0x6FE3)) {  
    #if _INCL_CALCULATOR == TRUE
        #if _TARGET == PERSONAL_COMPUTER
            return calcROM[address - 0x5000];               // <<== waar komt 0x5000 opeens vandaan (AaW)???
        #else
            return(pgm_read_byte_near(calcROM + address - 0xD000)); // calculator ROM
        #endif
    #endif
    }
#if _INCL_CALCULATOR == TRUE
    if ((address>=0x7000) && (address <=0x7200)) {      // 6502 programmable calculator functions
        switch (address) {
        // 70Ax SERIES: ASK FOR A VALUE INTO REGISTER x
        case 0x70A1:      // Load W1
            enterflt((uint8_t)0);
            return(0x60);
            break;
        case 0x70A2:      // Load W2
            enterflt(1);
            return(0x60);
            break;
        case 0x70A3:      // Load W3
            enterflt(2);
            return(0x60);
            break;
        case 0x70A4:      // Load W4
            enterflt(3);
            return(0x60);
            break;
        case 0x70AA:      // Load A
            a = enteroperation();
            serout(a);
            if (a==0) {
                setzero();
            } else {
                clearzero();
            }
            return(0x60);
            break;

        //70Dx SERIES: DISPLAY REGISTER x
        case 0x70D1:      // View W1
            showflt(0);
            return(0x60);
            break;
        case 0x70D2:      // View W2
            showflt(1);
            return(0x60);
            break;
        case 0x70D3:      // View W3
            showflt(2);
            return(0x60);
            break;
        case 0x70D4:      // View W4
            showflt(3);
            return(0x60);
            break;

        //71xy SERIES: SWAP REG x AND REG y
        case 0x7113:      // Swap W1-W3
            swapWreg(1,3);
            return(0x60);
            break;
        case 0x7123:      // Swap W2-W3
            swapWreg(2,3);
            return(0x60);
            break;
        case 0x7114:      // Swap W1-W4
            swapWreg(1,4);
            return(0x60);
            break;
        case 0x7124:      // Swap W2-W4
            swapWreg(2,4);
            return(0x60);
            break;
        case 0x7112:      // Swap W1-W2
            swapWreg(1,2);
            return(0x60);
            break;
        case 0x7134:      // Swap W3-W4
            swapWreg(3,4);
            return(0x60);
            break;

        //70xy SERIES: COPY REG x INTO REG y
        case 0x7013:      // Copy W1-W3
            copyWreg(1,3);
            return(0x60);
            break;
        case 0x7023:      // Copy W2-W3
            copyWreg(2,3);
            return(0x60);
            break;

        case 0x7031:      // Copy W3-W1
//              serout('C');
            copyWreg(3,1);
            return(0x60);
            break;
        case 0x7032:      // Copy W3-W2
            copyWreg(3,2);
            return(0x60);
            break;


        case 0x7014:      // Copy W1-W4
            copyWreg(1,4);
            return(0x60);
            break;
        case 0x7024:      // Copy W2-W4
            copyWreg(2,4);
            return(0x60);
            break;
        case 0x7034:      // Copy W3-W4
            copyWreg(3,4);
            return(0x60);
            break;

        case 0x7041:      // Copy W4-W1
            copyWreg(4,1);
            return(0x60);
            break;
        case 0x7042:      // Copy W4-W2
            copyWreg(4,2);
            return(0x60);
            break;
        case 0x7043:      // Copy W4-W3
            copyWreg(4,3);
            return(0x60);
            break;

        default:
            //serout('%'); serout('A'); // error code on serial port to warn of illegal address read
            printError(address, 'A');
            return (0x00);              // DO A brk TO HALT PROGRAM IF ANY OTHER ADDRESS IS CALLED
        }
    }   // if ((address>=0x7000) && (address <=0x7200)) // 6502 programmable calculator functions

#endif // _INCL_CALCULATOR

#if _INCL_MCHESS == TRUE
    if (address >= 0xC000 && address <=0xC571) {        // Read to Microchess ROM between $C000 and $C571
        if (address == 0xC202)                          // intercept C202: Blitz mode should return 0 instead of 8
            if (blitzMode==1) {                         // This is the Blitz mode hack from the microchess manual.
                return((uint8_t) 0x00);
            }

        #if _TARGET == PERSONAL_COMPUTER
            return mchessROM[address - 0xC000];
        #else
            return(pgm_read_byte_near(mchessROM + address - 0xC000)); // mchess ROM
        #endif
    }
    // I/O functions just for Microchess: ---------------------------------------------------
    // $CFF3: 0 = no key pressed, 1 key pressed
    // $CFF4: input from user
    // (also, in write6502: $F001: output character to display)
    if (address == 0xCFF4) {                            // simulated keyboard input
        tempval = getAkey();
        clearkey();
        // translate KIM-1 button codes into ASCII code expected by this version of Microchess
        switch (tempval) {
        case 16:
            tempval = 'P';
            break;    // PC translated to P
        case 'F':
            tempval = 13;
            break;    // F translated to Return
        case '+':
            tempval = 'W';
            break;    // + translated to W meaning Blitz mode toggle
        }
        if (tempval==0x57) { // 'W'. If user presses 'W', he wants to enable Blitz mode.
            if (blitzMode==1) {
                (blitzMode=0);
            } else {
                (blitzMode=1);
            }
            serout('>');
            serout( (blitzMode==1)?'B':'N' );
            serout('<');
        }
        return(tempval);
    }
    if (address == 0xCFF3) {                            // simulated keyboard input 0=no key press, 1 = key press
        // light LEDs ---------------------------------------------------------
        segm7Mem[0]= (RAM[0x00FB] & 0xF0) >> 4;
        segm7Mem[1]= RAM[0x00FB] & 0xF;
        segm7Mem[2]= (RAM[0x00FA] & 0xF0) >> 4;
        segm7Mem[3]= RAM[0x00FA] & 0xF;
        segm7Mem[4]= (RAM[0x00F9] & 0xF0) >> 4;
        segm7Mem[5]= RAM[0x00F9] & 0xF;
        #if _TARGET != PERSONAL_COMPUTER
            //driveLEDs();   // RAW mode
        #endif

        return(getAkey()==0?(uint8_t)0:(uint8_t)1);
    }
#endif      // _INCL_MCHESS

// inserted (hacked in) for _INCL_VTL02: CFF5 is CFF4 without Microchess key translations
    if (address == 0xCFF5) {                            // simulated keyboard input
        tempval = getAkey();
        clearkey();
        return(tempval);
    }
// --------------------------------

#if _INCL_VTL02 == TRUE
    if ((address >= 0xFC00) && (address <= 0xFFFA)) { // _INCL_VTL02
        #if _TARGET == PERSONAL_COMPUTER
            //return calcROM[vtl02ROM - 0x5000];            // <<<<========= what??? 0x5000?? (AaW)
            return vtl02ROM[address - 0xFC00];              // <<<<================= 0xFC00?? (AaW)
        #else
            return(pgm_read_byte_near(vtl02ROM + address - 0xFC00)); // _INCL_VTL02 ROM
        #endif
    }
#endif      // _INCL_VTL02


    if (address >= 0xFFFA) {                            // 6502 reset and interrupt vectors. Reroute to top of ROM002.
        #if _TARGET == PERSONAL_COMPUTER
            return ROM002[address - 0xFC00];
        #else
            return(pgm_read_byte_near(ROM002 + address - 0xFC00));
#       endif
    }
    /*
        if (address == 0x1D11) // intercept specific address for debug inspection
        {
            serout(':'); serout(x); serout(';');
        }
    */
    //serout('%'); serout('9');
    printError(address, '9');
    return (0); // This should never be reached unless some addressing bug, so return 6502 BRK

}   // read6502()


void write6502(uint16_t address, uint8_t value)
{
    if (address < 0x0400) {
        RAM[address]=value;
        return;
    }
//  if (address < 0x0800) {
    if (address < _MAX_RAM2_ADDRESS) {
        #if _TARGET == ARDUINO_UNO || _TARGET == ARDUINO_MINI 
            eepromwrite(address-_MAX_RAM2_ADDRESS, value);             // 0x0500-0x0900 is EEPROM for Arduino,
        
        #elif _TARGET == ARDUINO_MEGA || _TARGET == ARDUINO_DUE
            RAM2[address-_MAX_RAM2_ADDRESS]=value;                     // ...but second RAM unit for PC
        #endif
        return;
    }
    if (address < 0x1700) {                             // illegal access
        //serout('%'); serout('1');                     // error code 1 - write in empty space
        printError(address, '1');
        return;
    }
    if (address < 0x1740) {                             // I/O 003
        #if _TARGET == ARDUINO_MEGA  
            if (address == 0x1700) {   // write value to PORTA
                PORTA = value;
                return;
            }
            if (address == 0x1701) {   // set DDR of PORTA
                DDRA = value;
                return;
            }
            if (address == 0x1702) {   // write value to PORTB
                PORTC = value;
                return;
            }
            if (address == 0x1703) {   // set DDR of PORTB
                DDRC = value;
                return;
            }
        #elif _TARGET == ARDUINO_DUE
            if (address == 0x1700) {   // write value to PORTA
                writePORT('A', value);
                return;
            }
            if (address == 0x1701) {   // set DDR of PORTA
                writeDDR('A', value);
                return;
            }
            if (address == 0x1702) {   // write value to PORTB
                writePORT('B', value);
                return;
            }
            if (address == 0x1703) {   // set DDR of PORTB
                writeDDR('B', value);
                return;
            }
        #endif
        if ((address >= 0x1704) && (address <= 0x1707)) {   // set timer
            timerWrite((address), value);                   // AaW
            return;
        }

        printError(address, '3');
        return;
    }
    if (address < 0x1780) {                             // I/O 002
        //  KIM-1 6530 ROM002   KIM-1   PCB 
        //    0x1740     PA0    aColA   D2  
        //               PA1    aColB   D3    
        //               PA2    aColC   D4    
        //               PA3    aColD   D5    
        //               PA4    aColE   D6    
        //               PA5    aColF   D7    
        //               PA6    aColG   D8    
        //    0x1742     PB0    row0    D9    
        //               PB1    row1    D10   
        //               PB2    row2    D11   
        if (address == 0x1740) {    // Set Segment 1 t/m 7
            switch(value) {
                case 0xB9:  value = 0x8D; break;    // "C"
                case 0x9E:  value = 0xB8; break;    // "J"
                case 0xF0:  value = 0x87; break;    // "K"
                case 0xA3:  value = 0xF6; break;    // "M" 
                case 0xB4:  value = 0x95; break;    // "N" 
                case 0xDC:  value = 0x9D; break;    // "O"
                case 0xC2:  value = 0xA3; break;    // "V"
                case 0xE2:  value = 0xAB; break;    // "W"
                case 0xE4:  value = 0xB6; break;    // "X"
                case 0xEE:  value = 0xBA; break;    // "Y" 
                case 0xD8:  value = 0x93; break;    // "Z"
                case 0x80:  value = 0x80; break;    // "space"
                default:    decodeOut = 0;
                            for(col=0; col<=7;col++) {
                                currentBit = (1<< col);              // isolate the current bit in loop
                                if ((currentBit & value)) decodeOut |= (1<<decodeRAW[col]);
                            }   // for col ..
                            value = decodeOut;
                            break;
            }   // switch(value);
            value &= B01111111;
            switch(selDigitLED) {
                case 0: segm7Mem[0] = value; break; 
                case 1: segm7Mem[1] = value; break; 
                case 2: segm7Mem[2] = value; break; 
                case 3: segm7Mem[3] = value; break; 
                case 4: segm7Mem[4] = value; break; 
                case 5: segm7Mem[5] = value; break; 
            }   // switch(selDigitLED)
            driveRAW(selDigitLED);
            return;
        }
        if (address == 0x1741) {    // Set DDR for segment 1 t/m 7
            //printhex(pc); Dprint(" 0x1741 ["); serouthex(value); Dprint("]\n\r");            
            return;
        }
        if (address == 0x1742) {    // Select row0 t/m row2 and led1 t/m led6
            /*if (pc < _MAX_RAM2_ADDRESS) { Dprint(" 0x1742 ["); serouthex(value); Dprint("] >> 1 "); } */
            value = (value >> 1);
            /*if (pc < _MAX_RAM2_ADDRESS) { Dprint(" 0x1742 ["); serouthex(value); Dprint("] @"); printhex((pc - 3)); } */
            switch (value) {    // toggle the DISPLAY 1 - 6
                case 4: selDigitLED = 0; break;  // D12   led1             
                case 5: selDigitLED = 1; break;  // D13   led2
                case 6: selDigitLED = 2; break;  // A0    led3
                case 7: selDigitLED = 3; break;  // A1    led4
                case 8: selDigitLED = 4; break;  // A2    led5
                case 9: selDigitLED = 5; break;  // A3    led6
                default: selDigitLED = 99;
            }
            //Dprint("0x1742 set selDigitLED to ["); serouthex(selDigitLED); Dprint("]\n\r"); 
            return;
        }
        if (address == 0x1743) {    // set DDR for row0 t/m row2 and led1 t/m led6
            return;
        }
//      //serout('%');  serout('2');                    // trap code 2 - io2 access
        //printError(address, '2');
        return;
    }       // address < 0x1780
    
    if (address < 0x17C0) {                             // RAM 003
        RAM003[address-0x1780]=value;
        return;
    }
    if (address < 0x1800) {                             // RAM002
        RAM002[address-0x17C0]=value;
        return;
    }

    if ((address >=0x5000) && (address <=0x6FDF)) {     // illegal write in fltpt65 ROM
//    printf("WARNING: WRITE TO ROM\n");
        //serout('%');  serout('F');
        //printError(address, 'F');
        return;
    }

#if _INCL_MCHESS == TRUE
    // Character out function for microchess only: write to display at $F001
    if (address == 0xCFF1) {                            // Character out for microchess only
        serout(value);
        return;
    }
#endif      // _INCL_MCHESS

    //serout('%'); serout('4');      // error code 4 - write to ROM
    //printError(address, '4');

}   // write6502()


// two functions for fltpt65 support: copy/swap W registers
void copyWreg(uint8_t a, uint8_t b)
{
    uint8_t i;
    for (i=0; i<8; i++) {
        RAM[WREG_OFFSET+8*(b-1)+i] = RAM[WREG_OFFSET+8*(a-1)+i];
    }
}
void swapWreg(uint8_t a, uint8_t b)
{
    uint8_t i, buffer;
    for (i=0; i<8; i++) {
        buffer = RAM[WREG_OFFSET+8*(a-1)+i];
        RAM[WREG_OFFSET+8*(a-1)+i] = RAM[WREG_OFFSET+8*(b-1)+i];
        RAM[WREG_OFFSET+8*(b-1)+i] = buffer;
    }
}

//a few general functions used by various other functions
void push16(uint16_t pushval)
{
    write6502(BASE_STACK + sp, (pushval >> 8) & 0xFF);
    write6502(BASE_STACK + ((sp - 1) & 0xFF), pushval & 0xFF);
    sp -= 2;
}

void push8(uint8_t pushval)
{
    write6502(BASE_STACK + sp--, pushval);
}

uint16_t pull16()
{
    uint16_t temp16;
    temp16 = read6502(BASE_STACK + ((sp + 1) & 0xFF)) | ((uint16_t)read6502(BASE_STACK + ((sp + 2) & 0xFF)) << 8);
    sp += 2;
    return(temp16);
}

uint8_t pull8()
{
    return (read6502(BASE_STACK + ++sp));
}

void reset6502()
{
//printf ("test at reset: %x %x\n",0xFFFC, 0xFFFD);

    pc = (uint16_t)read6502(0xFFFC) | ((uint16_t)read6502(0xFFFD) << 8);
  //pc = 0x1C22;
  //printf ("pc: %x\n",pc);
    a = 0;
    x = 0;
    y = 0;
    sp = 0xFD;
    cpustatus |= FLAG_CONSTANT;
}

void initKIM()      // this is what user has to enter manually when powering KIM on. Why not do it here.
{

    uint16_t i;

    RAM002[(0x17FA)-(0x17C0)]=0x00;
    RAM002[(0x17FB)-(0x17C0)]=0x1C;
    RAM002[(0x17FE)-(0x17C0)]=0x00;
    RAM002[(0x17FF)-(0x17C0)]=0x1C;

    // the code below copies movit (a copy routine) to 0x1780 in RAM. It can be overwritten by 
    // users - it's an extra note that the HTML version of the book contains OR scan codes, so 
    // don't take the bytes from there!
    for (i=0; i<64; i++) {                              //64 of 102 program bytes
        #if _TARGET == PERSONAL_COMPUTER
            RAM003[i] = movitROM[i];
        #else
            RAM003[i] = pgm_read_byte_near(movitROM + i);
        #endif
    }
    for (i=0; i<(95-64); i++) {                         // movit spans into the second 64 byte memory segment...
        #if _TARGET == PERSONAL_COMPUTER
            RAM002[i] = movitROM[i+64];
        #else
            RAM002[i] = pgm_read_byte_near(movitROM + i + 64);
        #endif
    }

    // the code below copies relocate to 0x0110 in RAM. It can be overwritten by users or 
    // by the stack pointer - it's an extra
    for (i=0; i<149; i++) {
        #if _TARGET == PERSONAL_COMPUTER
            RAM[i+0x0110] = relocateROM[i];
        #else
            RAM[i+0x0110] = pgm_read_byte_near(relocateROM + i);
        #endif
    }

    // the code below copies branch to 0x01A5 (not 0x17C0 anymore) in RAM. It can be 
    // overwritten by users - it's an extra
    // note: the program can easily be damaged by the stack, because it ends at 1CF. 
    // Still, the monitor brings the stack down to no worse than 0x1FF-8.
    for (i=0; i<42; i++) {
        #if _TARGET == PERSONAL_COMPUTER
            //RAM002[i] = branchROM[i];
            RAM[i+0x01A5] = branchROM[i];
        #else
            //RAM002[i] = pgm_read_byte_near(branchROM + i);
            RAM[i+0x01A5] = pgm_read_byte_near(branchROM + i);
        #endif
    }

}


// addressing mode functions, calculates effective addresses
void imp()   //implied
{
}

void acc()   //accumulator
{
    useaccum = 1;
}

void imm()   //immediate
{
    ea = pc++;
}

void zp()   // zero-page
{
    ea = (uint16_t)read6502((uint16_t)pc++);
}

void zpx()   // zero-page,X
{
    ea = ((uint16_t)read6502((uint16_t)pc++) + (uint16_t)x) & 0xFF; // zero-page wraparound
}

void zpy()   // zero-page,Y
{
    ea = ((uint16_t)read6502((uint16_t)pc++) + (uint16_t)y) & 0xFF; // zero-page wraparound
}

void rel()   // relative for branch ops (8-bit immediate value, sign-extended)
{
    reladdr = (uint16_t)read6502(pc++);
    if (reladdr & 0x80) {
        reladdr |= 0xFF00;
    }
}

void abso()   // absolute
{
    ea = (uint16_t)read6502(pc) | ((uint16_t)read6502(pc+1) << 8);
    pc += 2;
}

void absx()   // absolute,X
{
    uint16_t startpage;
    ea = ((uint16_t)read6502(pc) | ((uint16_t)read6502(pc+1) << 8));
    startpage = ea & 0xFF00;
    ea += (uint16_t)x;

    pc += 2;
}

void absy()   // absolute,Y
{
    uint16_t startpage;
    ea = ((uint16_t)read6502(pc) | ((uint16_t)read6502(pc+1) << 8));
    startpage = ea & 0xFF00;
    ea += (uint16_t)y;

    pc += 2;
}

void ind()   // indirect
{
    uint16_t eahelp, eahelp2;
    eahelp = (uint16_t)read6502(pc) | (uint16_t)((uint16_t)read6502(pc+1) << 8);
    eahelp2 = (eahelp & 0xFF00) | ((eahelp + 1) & 0x00FF); //replicate 6502 page-boundary wraparound bug
    ea = (uint16_t)read6502(eahelp) | ((uint16_t)read6502(eahelp2) << 8);
    pc += 2;
}

void indx()   // (indirect,X)
{
    uint16_t eahelp;
    eahelp = (uint16_t)(((uint16_t)read6502(pc++) + (uint16_t)x) & 0xFF); //zero-page wraparound for table pointer
    ea = (uint16_t)read6502(eahelp & 0x00FF) | ((uint16_t)read6502((eahelp+1) & 0x00FF) << 8);
}

void indy()   // (indirect),Y
{
    uint16_t eahelp, eahelp2, startpage;
    eahelp = (uint16_t)read6502(pc++);
    eahelp2 = (eahelp & 0xFF00) | ((eahelp + 1) & 0x00FF); //zero-page wraparound
    ea = (uint16_t)read6502(eahelp) | ((uint16_t)read6502(eahelp2) << 8);
    startpage = ea & 0xFF00;
    ea += (uint16_t)y;

}

static uint16_t getvalue()
{
    if (useaccum) {
        return((uint16_t)a);
    } else {
        return((uint16_t)read6502(ea));
    }
}

/*static uint16_t getvalue16() {
    return((uint16_t)read6502(ea) | ((uint16_t)read6502(ea+1) << 8));
}*/

void putvalue(uint16_t saveval)
{
    if (useaccum) {
        a = (uint8_t)(saveval & 0x00FF);
    } else {
        write6502(ea, (saveval & 0x00FF));
    }
}


//instruction handler functions
void adc()
{
    value = getvalue();

    // BCD fix OV 20140915 - adc
    if ((cpustatus & FLAG_DECIMAL)==0) {
        result = (uint16_t)a + value + (uint16_t)(cpustatus & FLAG_CARRY);

        carrycalc(result);
        zerocalc(result);
        overflowcalc(result, a, value);
        signcalc(result);
    } else { // #ifndef NES_CPU
        // Decimal mode
        lxx = (a & 0x0f) + (value & 0x0f) + (uint16_t)(cpustatus & FLAG_CARRY);
        if ((lxx & 0xFF) > 0x09) {
            lxx += 0x06;
        }
        hxx = (a >> 4) + (value >> 4) + (lxx > 15 ? 1 : 0);
        if ((hxx & 0xff) > 9) {
            hxx += 6;
        }
        result = (lxx & 0x0f);
        result += (hxx << 4);
        result &= 0xff;
        // deal with carry flag:
        if (hxx>15) {
            setcarry();
        } else {
            clearcarry();
        }
        zerocalc(result);
        clearsign();        // negative flag never set for decimal mode.
        clearoverflow();    // overflow never set for decimal mode.
// end of BCD fix PART 2

        clockticks6502++;
    }
//    #endif // of NES_CPU

    saveaccum(result);
}

void op_and()
{
    value = getvalue();
    result = (uint16_t)a & value;

    zerocalc(result);
    signcalc(result);

    saveaccum(result);
}

void asl()
{
    value = getvalue();
    result = value << 1;

    carrycalc(result);
    zerocalc(result);
    signcalc(result);

    putvalue(result);
}

void bcc()
{
    if ((cpustatus & FLAG_CARRY) == 0) {
        oldpc = pc;
        pc += reladdr;
        if ((oldpc & 0xFF00) != (pc & 0xFF00)) {
            clockticks6502 += 2;                        // check if jump crossed a page boundary
        } else {
            clockticks6502++;
        }
    }
}

void bcs()
{
    if ((cpustatus & FLAG_CARRY) == FLAG_CARRY) {
        oldpc = pc;
        pc += reladdr;
        if ((oldpc & 0xFF00) != (pc & 0xFF00)) {
            clockticks6502 += 2;                        // check if jump crossed a page boundary
        } else {
            clockticks6502++;
        }
    }
}

void beq()
{
    if ((cpustatus & FLAG_ZERO) == FLAG_ZERO) {
        oldpc = pc;
        pc += reladdr;
        if ((oldpc & 0xFF00) != (pc & 0xFF00)) {
            clockticks6502 += 2;                        // check if jump crossed a page boundary
        } else {
            clockticks6502++;
        }
    }
}

void op_bit()
{
    value = getvalue();
    result = (uint16_t)a & value;

    zerocalc(result);
    cpustatus = (cpustatus & 0x3F) | (uint8_t)(value & 0xC0);
}

void bmi()
{
    if ((cpustatus & FLAG_SIGN) == FLAG_SIGN) {
        oldpc = pc;
        pc += reladdr;
        if ((oldpc & 0xFF00) != (pc & 0xFF00)) {
            clockticks6502 += 2;                        // check if jump crossed a page boundary
        } else {
            clockticks6502++;
        }
    }
}

void bne()
{
    if ((cpustatus & FLAG_ZERO) == 0) {
        oldpc = pc;
        pc += reladdr;
        if ((oldpc & 0xFF00) != (pc & 0xFF00)) {
            clockticks6502 += 2;                        // check if jump crossed a page boundary
        } else {
            clockticks6502++;
        }
    }
}

void bpl()
{
    if ((cpustatus & FLAG_SIGN) == 0) {
        oldpc = pc;
        pc += reladdr;
        if ((oldpc & 0xFF00) != (pc & 0xFF00)) {
            clockticks6502 += 2;                        // check if jump crossed a page boundary
        } else {
            clockticks6502++;
        }
    }
}

void brk()
{
    pc++;
    push16(pc);                                         // push next instruction address onto stack
    push8(cpustatus | FLAG_BREAK);                      // push CPU cpustatus to stack
    setinterrupt();                                     // set interrupt flag
    pc = (uint16_t)read6502(0xFFFE) | ((uint16_t)read6502(0xFFFF) << 8);
}

void bvc()
{
    if ((cpustatus & FLAG_OVERFLOW) == 0) {
        oldpc = pc;
        pc += reladdr;
        if ((oldpc & 0xFF00) != (pc & 0xFF00)) {
            clockticks6502 += 2;                        // check if jump crossed a page boundary
        } else {
            clockticks6502++;
        }
    }
}

void bvs()
{
    if ((cpustatus & FLAG_OVERFLOW) == FLAG_OVERFLOW) {
        oldpc = pc;
        pc += reladdr;
        if ((oldpc & 0xFF00) != (pc & 0xFF00)) {
            clockticks6502 += 2;                        // check if jump crossed a page boundary
        } else {
            clockticks6502++;
        }
    }
}

void clc()
{
    clearcarry();
}

void cld()
{
    cleardecimal();
}

void cli6502()
{
    clearinterrupt();
}

void clv()
{
    clearoverflow();
}

void cmp()
{
    value = getvalue();
    result = (uint16_t)a - value;

    if (a >= (uint8_t)(value & 0x00FF)) {
        setcarry();
    } else {
        clearcarry();
    }
    if (a == (uint8_t)(value & 0x00FF)) {
        setzero();
    } else {
        clearzero();
    }
    signcalc(result);
}

void cpx()
{
    value = getvalue();
    result = (uint16_t)x - value;

    if (x >= (uint8_t)(value & 0x00FF)) {
        setcarry();
    } else {
        clearcarry();
    }
    if (x == (uint8_t)(value & 0x00FF)) {
        setzero();
    } else {
        clearzero();
    }
    signcalc(result);
}

void cpy()
{
    value = getvalue();
    result = (uint16_t)y - value;

    if (y >= (uint8_t)(value & 0x00FF)) {
        setcarry();
    } else {
        clearcarry();
    }
    if (y == (uint8_t)(value & 0x00FF)) {
        setzero();
    } else {
        clearzero();
    }
    signcalc(result);
}

void dec()
{
    value = getvalue();
    result = value - 1;

    zerocalc(result);
    signcalc(result);

    putvalue(result);
}

void dex()
{
    x--;

    zerocalc(x);
    signcalc(x);
}

void dey()
{
    y--;

    zerocalc(y);
    signcalc(y);
}

void eor()
{
    value = getvalue();
    result = (uint16_t)a ^ value;

    zerocalc(result);
    signcalc(result);

    saveaccum(result);
}

void inc()
{
    value = getvalue();
    result = value + 1;

    zerocalc(result);
    signcalc(result);

    putvalue(result);
}

void inx()
{
    x++;

    zerocalc(x);
    signcalc(x);
}

void iny()
{
    y++;

    zerocalc(y);
    signcalc(y);
}

void jmp()
{
    pc = ea;
}

void jsr()
{
    push16(pc - 1);
    pc = ea;
}

void lda()
{
    value = getvalue();
    a = (uint8_t)(value & 0x00FF);

    zerocalc(a);
    signcalc(a);
}

void ldx()
{
    value = getvalue();
    x = (uint8_t)(value & 0x00FF);

    zerocalc(x);
    signcalc(x);
}

void ldy()
{
    value = getvalue();
    y = (uint8_t)(value & 0x00FF);

    zerocalc(y);
    signcalc(y);
}

void lsr()
{
    value = getvalue();
    result = value >> 1;

    if (value & 1) {
        setcarry();
    } else {
        clearcarry();
    }
    zerocalc(result);
    signcalc(result);

    putvalue(result);
}

void nop()
{
}


void ora()
{
    value = getvalue();
    result = (uint16_t)a | value;

    zerocalc(result);
    signcalc(result);

    saveaccum(result);
}

void pha()
{
    push8(a);
}

void php()
{
    push8(cpustatus | FLAG_BREAK);
}

void pla()
{
    a = pull8();

    zerocalc(a);
    signcalc(a);
}

void plp()
{
    cpustatus = pull8() | FLAG_CONSTANT;
}

void rol()
{
    value = getvalue();
    result = (value << 1) | (cpustatus & FLAG_CARRY);

    carrycalc(result);
    zerocalc(result);
    signcalc(result);

    putvalue(result);
}

void ror()
{
    value = getvalue();
    result = (value >> 1) | ((cpustatus & FLAG_CARRY) << 7);

    if (value & 1) {
        setcarry();
    } else {
        clearcarry();
    }
    zerocalc(result);
    signcalc(result);

    putvalue(result);
}

void rti()
{
    cpustatus = pull8();
    value = pull16();
    pc = value;
}

void rts()
{
    value = pull16();
    pc = value + 1;
}

void sbc()
{
// BCD fix OV 20140915 - adc
    if ((cpustatus & FLAG_DECIMAL)==0) {
        value = getvalue() ^ 0x00FF;
        result = (uint16_t)a + value + (uint16_t)(cpustatus & FLAG_CARRY);

        carrycalc(result);
        zerocalc(result);
        overflowcalc(result, a, value);
        signcalc(result);
    } else { //   #ifndef NES_CPU
        // decimal mode
        value = getvalue();
        lxx = (a & 0x0f) - (value & 0x0f) - (uint16_t)((cpustatus & FLAG_CARRY)?0:1);
        if ((lxx & 0x10) != 0) {
            lxx -= 6;
        }
        hxx = (a >> 4) - (value >> 4) - ((lxx & 0x10) != 0 ? 1 : 0);
        if ((hxx & 0x10) != 0) {
            hxx -= 6;
        }
        result = (lxx & 0x0f);
        result += (hxx << 4);
        result = (lxx & 0x0f) | (hxx << 4);
        // deal with carry
        if ((hxx & 0xff) < 15) {
            setcarry();                             // right? I think so. Intended is setCarryFlag((hxx & 0xff) < 15);
        } else {
            clearcarry();
        }
        zerocalc(result);                           // zero dec is zero hex, no problem?
        clearsign();                                // negative flag never set for decimal mode. That's a 
                                                    // simplification, see http://www.6502.org/tutorials/decimal_mode.html
        clearoverflow();                            // overflow never set for decimal mode.
        result = result & 0xff;
// end of BCD fix PART 3 (final part)

        clockticks6502++;
    }
//    #endif // of NES_CPU

    saveaccum(result);
}

void sec()
{
    setcarry();
}

void sed()
{
    setdecimal();
}

void sei6502()
{
    setinterrupt();
}

void sta()
{
    putvalue(a);
}

void stx()
{
    putvalue(x);
}

void sty()
{
    putvalue(y);
}

void tax()
{
    x = a;

    zerocalc(x);
    signcalc(x);
}

void tay()
{
    y = a;

    zerocalc(y);
    signcalc(y);
}

void tsx()
{
    x = sp;

    zerocalc(x);
    signcalc(x);
}

void txa()
{
    a = x;

    zerocalc(a);
    signcalc(a);
}

void txs()
{
    sp = x;
}

void tya()
{
    a = y;

    zerocalc(a);
    signcalc(a);
}

// undocumented instructions
#ifdef UNDOCUMENTED
void lax()
{
    lda();
    ldx();
}

void sax()
{
    sta();
    stx();
    putvalue(a & x);
}

void dcp()
{
    dec();
    cmp();
}

void isb()
{
    inc();
    sbc();
}

void slo()
{
    asl();
    ora();
}

void rla()
{
    rol();
    op_and();
}

void sre()
{
    lsr();
    eor();
}

void rra()
{
    ror();
    adc();
}
#else
#define lax nop
#define sax nop
#define dcp nop
#define isb nop
#define slo nop
#define rla nop
#define sre nop
#define rra nop
#endif


void nmi6502()
{
    push16(pc);
    push8(cpustatus);
    cpustatus |= FLAG_INTERRUPT;
    pc = (uint16_t)read6502(0xFFFA) | ((uint16_t)read6502(0xFFFB) << 8);
    pc = 0x1C1C;
}

void irq6502()
{
    push16(pc);
    push8(cpustatus);
    cpustatus |= FLAG_INTERRUPT;
    pc = (uint16_t)read6502(0xFFFE) | ((uint16_t)read6502(0xFFFF) << 8);
//  pc = 0x1C1F;
}

#ifdef USE_TIMING
prog_char ticktable[256] PROGMEM = {
    /*     |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  A  |  B  |  C  |  D  |  E  |  F  |     */
    /* 0 */   7,    6,    2,    8,    3,    3,    5,    5,    3,    2,    2,    2,    4,    4,    6,    6,  /* 0 */
    /* 1 */   2,    5,    2,    8,    4,    4,    6,    6,    2,    4,    2,    7,    4,    4,    7,    7,  /* 1 */
    /* 2 */   6,    6,    2,    8,    3,    3,    5,    5,    4,    2,    2,    2,    4,    4,    6,    6,  /* 2 */
    /* 3 */   2,    5,    2,    8,    4,    4,    6,    6,    2,    4,    2,    7,    4,    4,    7,    7,  /* 3 */
    /* 4 */   6,    6,    2,    8,    3,    3,    5,    5,    3,    2,    2,    2,    3,    4,    6,    6,  /* 4 */
    /* 5 */   2,    5,    2,    8,    4,    4,    6,    6,    2,    4,    2,    7,    4,    4,    7,    7,  /* 5 */
    /* 6 */   6,    6,    2,    8,    3,    3,    5,    5,    4,    2,    2,    2,    5,    4,    6,    6,  /* 6 */
    /* 7 */   2,    5,    2,    8,    4,    4,    6,    6,    2,    4,    2,    7,    4,    4,    7,    7,  /* 7 */
    /* 8 */   2,    6,    2,    6,    3,    3,    3,    3,    2,    2,    2,    2,    4,    4,    4,    4,  /* 8 */
    /* 9 */   2,    6,    2,    6,    4,    4,    4,    4,    2,    5,    2,    5,    5,    5,    5,    5,  /* 9 */
    /* A */   2,    6,    2,    6,    3,    3,    3,    3,    2,    2,    2,    2,    4,    4,    4,    4,  /* A */
    /* B */   2,    5,    2,    5,    4,    4,    4,    4,    2,    4,    2,    4,    4,    4,    4,    4,  /* B */
    /* C */   2,    6,    2,    8,    3,    3,    5,    5,    2,    2,    2,    2,    4,    4,    6,    6,  /* C */
    /* D */   2,    5,    2,    8,    4,    4,    6,    6,    2,    4,    2,    7,    4,    4,    7,    7,  /* D */
    /* E */   2,    6,    2,    8,    3,    3,    5,    5,    2,    2,    2,    2,    4,    4,    6,    6,  /* E */
    /* F */   2,    5,    2,    8,    4,    4,    6,    6,    2,    4,    2,    7,    4,    4,    7,    7   /* F */
};
#endif

void exec6502(int32_t tickcount)
{
    if (doShowCPUflags == 1)    showCPUflags();    
#ifdef USE_TIMING
    clockgoal6502 += tickcount;

    while (clockgoal6502 > 0) {
#else
    while (tickcount--) {
#endif

#ifdef DEBUGUNO
        debugPC = pc;
        int ii=0;
#endif

// part 1 of single stepping using NMI
        if ((SSTmode == 1) & (pc<0x1C00)) {             // no NMI if running ROM code (K7), that 
                                                        // would also single-step the monitor code!
            nmiFlag=1;                                  // handled after this instruction has completed.
            doShowCPUflags = 1;     // (AaW)
        }
// -------------
        opcode = read6502(pc++);
        //serouthex(opcode);  serout('@'); // **AAW**
        //printhex((pc - 1));   // **AAW**
        cpustatus |= FLAG_CONSTANT;

        useaccum = 0;

        switch (opcode) {
        case 0x0:
            imp();
            brk();
            break;
        case 0x1:
            indx();
            ora();
            break;
        case 0x5:
            zp();
            ora();
            break;
        case 0x6:
            zp();
            asl();
            break;
        case 0x8:
            imp();
            php();
            break;
        case 0x9:
            imm();
            ora();
            break;
        case 0xA:
            acc();
            asl();
            break;
        case 0xD:
            abso();
            ora();
            break;
        case 0xE:
            abso();
            asl();
            break;
        case 0x10:
            rel();
            bpl();
            break;
        case 0x11:
            indy();
            ora();
            break;
        case 0x15:
            zpx();
            ora();
            break;
        case 0x16:
            zpx();
            asl();
            break;
        case 0x18:
            imp();
            clc();
            break;
        case 0x19:
            absy();
            ora();
            break;
        case 0x1D:
            absx();
            ora();
            break;
        case 0x1E:
            absx();
            asl();
            break;
        case 0x20:
            abso();
            jsr();
            break;
        case 0x21:
            indx();
            op_and();
            break;
        case 0x24:
            zp();
            op_bit();
            break;
        case 0x25:
            zp();
            op_and();
            break;
        case 0x26:
            zp();
            rol();
            break;
        case 0x28:
            imp();
            plp();
            break;
        case 0x29:
            imm();
            op_and();
            break;
        case 0x2A:
            acc();
            rol();
            break;
        case 0x2C:
            abso();
            op_bit();
            break;
        case 0x2D:
            abso();
            op_and();
            break;
        case 0x2E:
            abso();
            rol();
            break;
        case 0x30:
            rel();
            bmi();
            break;
        case 0x31:
            indy();
            op_and();
            break;
        case 0x35:
            zpx();
            op_and();
            break;
        case 0x36:
            zpx();
            rol();
            break;
        case 0x38:
            imp();
            sec();
            break;
        case 0x39:
            absy();
            op_and();
            break;
        case 0x3D:
            absx();
            op_and();
            break;
        case 0x3E:
            absx();
            rol();
            break;
        case 0x40:
            imp();
            rti();
            break;
        case 0x41:
            indx();
            eor();
            break;
        case 0x45:
            zp();
            eor();
            break;
        case 0x46:
            zp();
            lsr();
            break;
        case 0x48:
            imp();
            pha();
            break;
        case 0x49:
            imm();
            eor();
            break;
        case 0x4A:
            acc();
            lsr();
            break;
        case 0x4C:
            abso();
            jmp();
            break;
        case 0x4D:
            abso();
            eor();
            break;
        case 0x4E:
            abso();
            lsr();
            break;
        case 0x50:
            rel();
            bvc();
            break;
        case 0x51:
            indy();
            eor();
            break;
        case 0x55:
            zpx();
            eor();
            break;
        case 0x56:
            zpx();
            lsr();
            break;
        case 0x58:
            imp();
            cli6502();
            break;
        case 0x59:
            absy();
            eor();
            break;
        case 0x5D:
            absx();
            eor();
            break;
        case 0x5E:
            absx();
            lsr();
            break;
        case 0x60:
            imp();
            rts();
            break;
        case 0x61:
            indx();
            adc();
            break;
        case 0x65:
            zp();
            adc();
            break;
        case 0x66:
            zp();
            ror();
            break;
        case 0x68:
            imp();
            pla();
            break;
        case 0x69:
            imm();
            adc();
            break;
        case 0x6A:
            acc();
            ror();
            break;
        case 0x6C:
            ind();
            jmp();
            break;
        case 0x6D:
            abso();
            adc();
            break;
        case 0x6E:
            abso();
            ror();
            break;
        case 0x70:
            rel();
            bvs();
            break;
        case 0x71:
            indy();
            adc();
            break;
        case 0x75:
            zpx();
            adc();
            break;
        case 0x76:
            zpx();
            ror();
            break;
        case 0x78:
            imp();
            sei6502();
            break;
        case 0x79:
            absy();
            adc();
            break;
        case 0x7D:
            absx();
            adc();
            break;
        case 0x7E:
            absx();
            ror();
            break;
        case 0x81:
            indx();
            sta();
            break;
        case 0x84:
            zp();
            sty();
            break;
        case 0x85:
            zp();
            sta();
            break;
        case 0x86:
            zp();
            stx();
            break;
        case 0x88:
            imp();
            dey();
            break;
        case 0x8A:
            imp();
            txa();
            break;
        case 0x8C:
            abso();
            sty();
            break;
        case 0x8D:
            abso();
            sta();
            break;
        case 0x8E:
            abso();
            stx();
            break;
        case 0x90:
            rel();
            bcc();
            break;
        case 0x91:
            indy();
            sta();
            break;
        case 0x94:
            zpx();
            sty();
            break;
        case 0x95:
            zpx();
            sta();
            break;
        case 0x96:
            zpy();
            stx();
            break;
        case 0x98:
            imp();
            tya();
            break;
        case 0x99:
            absy();
            sta();
            break;
        case 0x9A:
            imp();
            txs();
            break;
        case 0x9D:
            absx();
            sta();
            break;
        case 0xA0:
            imm();
            ldy();
            break;
        case 0xA1:
            indx();
            lda();
            break;
        case 0xA2:
            imm();
            ldx();
            break;
        case 0xA4:
            zp();
            ldy();
            break;
        case 0xA5:
            zp();
            lda();
            break;
        case 0xA6:
            zp();
            ldx();
            break;
        case 0xA8:
            imp();
            tay();
            break;
        case 0xA9:
            imm();
            lda();
            break;
        case 0xAA:
            imp();
            tax();
            break;
        case 0xAC:
            abso();
            ldy();
            break;
        case 0xAD:
            abso();
            lda();
            break;
        case 0xAE:
            abso();
            ldx();
            break;
        case 0xB0:
            rel();
            bcs();
            break;
        case 0xB1:
            indy();
            lda();
            break;
        case 0xB4:
            zpx();
            ldy();
            break;
        case 0xB5:
            zpx();
            lda();
            break;
        case 0xB6:
            zpy();
            ldx();
            break;
        case 0xB8:
            imp();
            clv();
            break;
        case 0xB9:
            absy();
            lda();
            break;
        case 0xBA:
            imp();
            tsx();
            break;
        case 0xBC:
            absx();
            ldy();
            break;
        case 0xBD:
            absx();
            lda();
            break;
        case 0xBE:
            absy();
            ldx();
            break;
        case 0xC0:
            imm();
            cpy();
            break;
        case 0xC1:
            indx();
            cmp();
            break;
        case 0xC4:
            zp();
            cpy();
            break;
        case 0xC5:
            zp();
            cmp();
            break;
        case 0xC6:
            zp();
            dec();
            break;
        case 0xC8:
            imp();
            iny();
            break;
        case 0xC9:
            imm();
            cmp();
            break;
        case 0xCA:
            imp();
            dex();
            break;
        case 0xCC:
            abso();
            cpy();
            break;
        case 0xCD:
            abso();
            cmp();
            break;
        case 0xCE:
            abso();
            dec();
            break;
        case 0xD0:
            rel();
            bne();
            break;
        case 0xD1:
            indy();
            cmp();
            break;
        case 0xD5:
            zpx();
            cmp();
            break;
        case 0xD6:
            zpx();
            dec();
            break;
        case 0xD8:
            imp();
            cld();
            break;
        case 0xD9:
            absy();
            cmp();
            break;
        case 0xDD:
            absx();
            cmp();
            break;
        case 0xDE:
            absx();
            dec();
            break;
        case 0xE0:
            imm();
            cpx();
            break;
        case 0xE1:
            indx();
            sbc();
            break;
        case 0xE4:
            zp();
            cpx();
            break;
        case 0xE5:
            zp();
            sbc();
            break;
        case 0xE6:
            zp();
            inc();
            break;
        case 0xE8:
            imp();
            inx();
            break;
        case 0xE9:
            imm();
            sbc();
            break;
        case 0xEA:          // (AaW)
            nop();
            break;
        case 0xEB:
            imm();
            sbc();
            break;
        case 0xEC:
            abso();
            cpx();
            break;
        case 0xED:
            abso();
            sbc();
            break;
        case 0xEE:
            abso();
            inc();
            break;
        case 0xF0:
            rel();
            beq();
            break;
        case 0xF1:
            indy();
            sbc();
            break;
        case 0xF5:
            zpx();
            sbc();
            break;
        case 0xF6:
            zpx();
            inc();
            break;
        case 0xF8:
            imp();
            sed();
            break;
        case 0xF9:
            absy();
            sbc();
            break;
        case 0xFD:
            absx();
            sbc();
            break;
        case 0xFE:
            absx();
            inc();
            break;
        }

#ifdef USE_TIMING
        clockgoal6502 -= (int32_t)pgm_read_byte_near(ticktable + opcode);
#endif
        instructions++;

#ifdef DEBUGUNO
        // ----------------------------- debug trace file
        if (fpx!=NULL) {
            char flagName[]="CZIDB-VN";

            fprintf(fpx, "%4x  ", debugPC);
            for (ii=0; ii<3; ii++)
                if ((int)(pc-debugPC-ii)<0) {
                    fprintf(fpx, "   ");
                } else {
                    fprintf(fpx, "%2x ", read6502(debugPC+ii));
                }
            fprintf(fpx, "               ");
            fprintf(fpx, "A:%2x X:%2x Y:%2x F:%2x S:1%2x [", a, x, y, cpustatus, sp);
            for (ii=7; ii>=6; ii--)
                if (cpustatus & 1<<ii) {
                    fprintf(fpx, "%c", flagName[ii]);
                } else {
                    fprintf(fpx, ".");
                }
            fprintf(fpx, "-");
            for (ii=4; ii>=0; ii--)
                if (cpustatus & 1<<ii) {
                    fprintf(fpx, "%c", flagName[ii]);
                } else {
                    fprintf(fpx, ".");
                }
            fprintf(fpx, "]\n");
        }
        // ----------------------------- debug
#endif


// part 2 of NMI single-step handling for KIM-I
        if (nmiFlag==1) {       // SST switch on and not in K7 area (ie, ROM002), so single step
            nmi6502();          // set up for NMI
            nmiFlag=0;
        }
// ----------------------------------
    }
}

uint16_t getpc()
{
    return(pc);
}

uint8_t getop()
{
    return(opcode);
}


