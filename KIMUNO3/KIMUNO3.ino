//version date: 201610-24
/*
*    
*   Coding style
*       - One True Brace Style formatting/indenting.
*       - Linux brackets, add brackets to all conditionals.
*/
#include "Arduino.h"
#include "Config.h"

#if _TARGET != ARDUINO_DUE
    #include <EEPROM.h>
    #include <util/delay.h>
#endif
#if _USE_KIMDOS == TRUE
    #include <SPI.h>
    #include <SD.h>
#endif
#include <stdint.h>
#include "C.h"

//#define CH2_SPEAKERPIN 9
//#define WREG_OFFSET 0x0360
// in kimuno.ino, this only relates to some code segments that are shared with PC version

#if _TARGET == PERSONAL_COMPUTER
    extern unsigned char ROM002[];
    extern unsigned char ROM003[];
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
#else       // ARDUINO
    extern const unsigned char ROM002[] PROGMEM;
    extern const unsigned char ROM003[] PROGMEM;
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

extern uint8_t RAM[1024];       // main 1KB RAM      0x000-0x3FF
extern uint16_t pc;             // 6502 pRogram cOunter
extern uint8_t SSTmode;
extern uint8_t localEcho;      // "1" is echo input, "0" don't echo
extern uint8_t useKeyboardLed;  // 0 to use Serial port or 1 for HEX digits.
uint8_t curkey = 0;
uint8_t eepromProtect=1;        // default is to write-protect EEPROM
int blitzMode=1;                // microchess status variable. 1 speeds up chess moves (and dumbs down play)
uint8_t keyboardMode=0;         // start with keyboard in KIM-1 mode. 1: calculator mode

//char threeHex[3][2];            // LED display
uint8_t segm7Mem[6];            // memorylocations F9, FA and FB
uint8_t selDigitLED;            // selected LED display (0-6) 
uint32_t doDisplay;             // millis() to display KIM LEDs
uint8_t displayLEDmode;         // 0=RAW, 1=By F9, FA and FB

#define segmLEDs 8
#if _TARGET == ARDUINO_MINI || _TARGET == ARDUINO_UNO                                                                                                                                                                                                                                                                                                                                                    
    byte aCols[segmLEDs] = { A5, 2,3,4,5,6,7,8 }; // note col A5 is the extra one linked to DP
    byte aRows[3] = { 9,10,11 };
    byte ledSelect[8] =  { 12, 13, A0, A1, A2, A3, A7, A4 }; // note that A6 and A7 are not used at present. Can delete them.
    byte ledSelect7[8] = { 12, 13, A0, A1, A4, A2, A3, A7 }; // note that A6 and A7 are not used at present. Can delete them.
#endif
#if _TARGET == ARDUINO_MEGA
    byte aCols[segmLEDs] = { A5, 2,3,4,5,6,7,8 }; // note col A5 is the extra one linked to DP
    byte aRows[3] = { 9,10,11 };
    byte ledSelect[8] =  { 12, 13, A0, A1, A2, A3, A7, A4 }; // note that A6 and A7 are not used at present. Can delete them.
    byte ledSelect7[8] = { 12, 13, A0, A1, A4, A2, A3, A7 }; // note that A6 and A7 are not used at present. Can delete them.
#endif
byte decodeHEX[19] = {
//--------------------
//         (a=64)
//           --
//    (f=2) |  | (b=32)
//           --        <-- (g=1)
//    (e=4) |  | (c=16)
//           -- 
//         (d=8)  o (DP)
//----------------------------
// bits     6543210
// digits   abcdefg
         0b01111110, //0
         0b00110000, //1
         0b01101101, //2
         0b01111001, //3
         0b00110011, //4
         0b01011011, //5
         0b01011111, //6
         0b01110000, //7 
         0b01111111, //8
         0b01111011, //9
         0b01110111, //a
         0b00011111, //b
         0b01001110, //c
         0b00111101, //d
         0b01001111, //e
         0b01000111, //f
         0b00000001, //g printed as -
         0b00001000, //h printed as _
         0b00000000  //i printed as <dot>
};

byte decodeRAW[8] = {
        6,  // 0
        5,  // 1
        4,  // 2
        3,  // 3
        2,  // 4
        1,  // 5
        0,  // 6
        7   // 7
};

extern "C" {
    // ---------- in CPU.c ------------------------------
    uint16_t getpc();
    uint8_t getop();
    void exec6502(int32_t tickcount);
    void reset6502();
    void nmi6502();
    void initKIM(void);
    void loadDemoProgram(void);
    extern void driveLEDs();
    extern void driveRAW(uint8_t digitNo);
    //extern void driveCalcLEDs(uint8_t reg);
    //extern void scanKeys();
    void scanKeys();

    // ---------- called from CPU.c ----------------------
    void serout(uint8_t val)
    {
        Serial.write(val);
    }
    void serouthex(uint8_t val)
    {
        Serial.print(val, HEX);
    }
    uint8_t getAkey()
    {
        return(curkey);
    }
    void clearkey()
    {
        curkey = 0;
    }
    void printhex(uint16_t val)
    {
        Serial.print(val, HEX);
        Serial.println();
    }

    // getKIMkey() translates ASCII keypresses to codes the KIM ROM expects.
    // note that, inefficiently, the KIM Uno board's key codes are first translated to ASCII, then routed through
    // here just like key presses from the ASCII serial port are. That's inefficient but left like this
    // for hysterical raisins.

    uint8_t getKIMkey()
    {
        //Serial.print('#');  Serial.print(curkey);  Serial.print('#');

        if (curkey==0) {
            return (0xFF);    //0xFF: illegal keycode
        }

        if ((curkey>='0') & (curkey <='9')) {
            return (curkey-'0');
        }
        if ((curkey>='A') & (curkey <='F')) {
            return(curkey-'A'+10);
        }
        if ((curkey>='a') & (curkey <='f')) {
            return(curkey-'a'+10);
        }

        if (curkey==1) { // ctrlA
            return(0x10);    // AD address mode
        }
        if (curkey==4) { // ctrlD
            return(0x11);    // DA data mode
        }
        if (curkey=='+') { // +
            return(0x12);    // step
        }
        if (curkey==7) { // ctrlG
            return(0x13);    // GO
        }
        if (curkey==16) { // ctrlP
            return(0x14);    // PC mode
        }
        // curkey==ctrlR for hard reset (/RST) (KIM's RS key) is handled in main loop!
        // curkey==ctrlT for ST key (/NMI) is handled in main loop!
        return(curkey); // any other key, should not be hit but ignored by KIM
    }

#if _TARGET == ARDUINO_UNO || _TARGET == ARDUINO_MINI
    uint8_t eepromread(uint16_t eepromaddress)
    {
        return(EEPROM.read(eepromaddress));  // thanks to Malik Enes Safak for the bug fix 20160907
    }
    void eepromwrite(uint16_t eepromaddress, uint8_t bytevalue)
    {
        if (eepromProtect==0) {
            EEPROM.write(eepromaddress, bytevalue);
        } else {
            Serial.println(F("ERROR: EEPROM STATE IS WRITE-PROTECTED. HIT '>' TO TOGGLE WRITE PROTECT"));
            Serial.println(freeRam());
        }
    }
#endif
}

void setup ()
{
    Serial.begin (9600);
    Serial.println ();

    setupUno();

    reset6502();
    initKIM(); // Enters 1c00 in KIM vectors 17FA and 17FE. Might consider doing 17FC as well????????
    loadDemoProgram();

    Serial.print(F("v24oct16 freeMemory()=")); // just a little check, to avoid running out of RAM!
    Serial.println(freeRam());
    #if _TARGET == ARDUINO_MINI
        Serial.println("KIMUNO on ARDUINO MINI\r");
    #endif
    #if _TARGET == ARDUINO_UNO
        Serial.println("KIMUNO on ARDUINO UNO\r");
    #endif
    #if _TARGET == ARDUINO_MEGA
        Serial.println("KIMUNO on ARDUINO MEGA\r");
    #endif
    #if _TARGET == ARDUINO_DUE
        Serial.println("KIMUNO on ARDUINO DUE\r");
    #endif
    #if _TARGET == PERSONAL_COMPUTER
        Serial.println("KIMUNO on Personal Computer\r");
    #endif
    Serial.println(F("press [Tab] to toggle TTY <-> KIM-1 pannel"));
    Serial.println(F("press [ctrl+E] to toggle local echo"));
}   // setup()

void loop ()
{
    if (Serial.available()) {
        curkey = Serial.read() & 0x7F;
        if (curkey == 5) localEcho = !localEcho;
        if (localEcho)  Serial.print(char(curkey));
        interpretkeys();
    }

    scanKeys();
    if (xkeyPressed()!=0) { //KIM Uno board input?
        interpretkeys();
    }
    //driveLEDs();

    exec6502(100); //do 100 6502 instructions

}   // loop()


// check for out of RAM
int freeRam ()
{
#if _TARGET == ARDUINO_DUE
    return 512
#endif
    extern int __heap_start, *__brkval;
    int v;
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

// translate keyboard commands to KIM-I keycodes or emulator actions
void interpretkeys()
{
    // round 1: keys that always have the same meaning
    switch (curkey) {
    case 18:  // CtrlR = RS key = hardware reset (RST)
        reset6502();
        clearkey();
        Serial.print("RSet\n\r");
        break;
    case 20: // CtrlT = ST key = throw an NMI to stop execution of user program
        nmi6502();
        clearkey();
        Serial.print("STop\n\r");
        break;
    }

    // round 1B: keys that always have the same meaning but are disabled in _INCL_VTL02
    if(keyboardMode!=2) {
        switch(curkey) {
        case '[': // SST off
            SSTmode = 0;
            clearkey();
            Serial.print(F("                                      SST OFF         \r"));
            break;
        case ']': // SST on
            SSTmode = 1;
            clearkey();
            Serial.print(F("                                      SST ON          \r"));
            break;
        case 9: // TAB pressed, toggle between serial port and onboard keyboard/display
            if (useKeyboardLed==0) {
                useKeyboardLed=1;
                Serial.print(F("                    Keyboard/Hex Digits Mode \r"));
            } else {
                useKeyboardLed=0;
                Serial.print(F("                        Serial Terminal Mode         \r"));
            }
            reset6502();
            clearkey();
            break;
        case '>': // Toggle write protect on eeprom
#if _TARGET == ARDUINO_UNO || _TARGET == ARDUINO_MINO
            if (eepromProtect==0) {
                eepromProtect = 1;
                Serial.print(F("                                      Eeprom R/O     \r"));
            } else {
                eepromProtect = 0;
                Serial.print(F("                                      Eeprom R/W     \r"));
                delay(20);
            }
#endif
            clearkey();
            break;
        }
    }

#if _INCL_CALCULATOR == TRUE
    if (keyboardMode==1) {  // in case of Calculator Mode
        processCalcKey(curkey);
    }
#endif
}   // interpretkeys()

// =================================================================================================
// KIM Uno Board functions are bolted on from here
// =================================================================================================

void setupUno()
{
    int i;
    // --------- initialse for scanning keyboard matrix -----------------
    // set columns to input with pullups
    for (i=0; i<segmLEDs; i++) {
        pinMode(aCols[i], INPUT);           // set pin to input
        digitalWrite(aCols[i], HIGH);       // turn on pullup resistors
    }
    // set rows to output, and set them High to be in Neutral position
    for (i=0; i<3; i++) {
        pinMode(aRows[i], OUTPUT);          // set pin to output
        digitalWrite(aRows[i], HIGH);       // set to high
    }
    // --------- clear display buffer ------------------------------------
    for (i=0; i<7; i++) {
        segm7Mem[i] = 0;
    }
    Serial.println(F("Ready"));
}   // setupUno()


extern "C" {
    void driveLEDs()
    {
        uint8_t col, digitNo, currentBit, bitOn;
        
        // 1. initialse for driving the 6 (now 8) 7segment LEDs
        // ledSelect pins drive common anode for [all segments] in [one of 6 LEDs]
        for (digitNo=0; digitNo<7; digitNo++) {
            pinMode(ledSelect[digitNo], OUTPUT);  // set led pins to output
            digitalWrite(ledSelect[digitNo], LOW); // LOW = not lit
        }
        // 2. switch column pins to output mode
        // column pins are the cathode for the LED segments
        for (digitNo=0; digitNo<6; digitNo++) {
                for (col=0; col<segmLEDs; col++) {
                    pinMode(aCols[col], OUTPUT);           // set pin to output
                    currentBit = (1<<(7-col));             // isolate the current bit in loop
                    bitOn = (currentBit & decodeHEX[segm7Mem[digitNo]])==0; // Decode HEX
                    digitalWrite(aCols[col], bitOn);       // set the bit
                    //Serial.print(" Bit is ["); Serial.print(bitOn); Serial.println("]\r");
                }
                digitalWrite(ledSelect[digitNo], HIGH); // Light this LED
                delay(2);
                digitalWrite(ledSelect[digitNo], LOW); // unLight this LED
            }
    } // driveLEDs()
    
    void driveRAW(uint8_t digitNo)
    {
        uint8_t col, currentBit, bitOn;

        if (digitNo < 0 || digitNo > 6) return;

        pinMode(ledSelect[digitNo], OUTPUT);  // set led pins to output
        digitalWrite(ledSelect[digitNo], LOW); // LOW = not lit
        // 2. switch column pins to output mode
        // column pins are the cathode for the LED segments
                
                for (col=0; col<segmLEDs; col++) {
                        //Serial.print(" bit["); Serial.print(col);
                        //Serial.print("->"); Serial.print(decodeRAW[col]); Serial.print("] ");
                        pinMode(aCols[decodeRAW[col]], OUTPUT);           // set pin to output
                        currentBit = (1<<(7-col));           // isolate the current bit in loop
                        bitOn = (currentBit & segm7Mem[digitNo])==0;            // RAW
                        digitalWrite(aCols[col], bitOn);       // set the bit
                }
                digitalWrite(ledSelect[digitNo], HIGH); // Light this LED
                delay(2);
                digitalWrite(ledSelect[digitNo], LOW); // unLight this LED
            
    } // driveRAW()
    
} // end of C segment


uint8_t parseChar(uint8_t n) //  parse keycode to return its ASCII code
{
    uint8_t c;

    // KIM-I keys
    switch (n-1) {              //KIM Uno keyscan codes to ASCII codes used by emulator
    case    7   :
        c =   '0' ;
        break;   //        note: these are n-1 numbers!
    case    6   :
        c =   '1';
        break;    //
    case    5   :
        c =   '2';
        break;    //
    case    4   :
        c =   '3';
        break;    //
    case    3   :
        c =   '4';
        break;    //
    case    2   :
        c =   '5';
        break;    //
    case    1   :
        c =   '6';
        break;    //
    case    0   :
        c =   20;
        break; // ST

    case    15  :
        c =   '7' ;
        break;   //
    case    14  :
        c =   '8';
        break;    //
    case    13  :
        c =   '9';
        break;    //
    case    12  :
        c =   'A';
        break;    //
    case    11  :
        c =   'B';
        break;    //
    case    10  :
        c =   'C';
        break;    //
    case    9   :
        c =   'D';
        break;    //
    case    8   :
        c =   18;
        break; // RS

    case    23  :
        c =  'E';
        break;     //
    case    22  :
        c =  'F';
        break;     //
    case    21  :
        c =   1;
        break;     // AD
    case    20  :
        c =   4;
        break;     // DA
    case    19  :
        c =   '+';
        break;     // +
    case    18  :
        c =   7;
        break; // GO
    case    17  :
        c =   16;
        break; // PC
    case    16  :
        c = (SSTmode==0?']':'[');
        break; //  SST toggle
    }
    return c;
}

uint8_t xkeyPressed()    // just see if there's any keypress waiting
{
    return (curkey==0?0:1);
}


extern "C" {  // the extern C is to make function accessible from within CPU.c
    void scanKeys()
    {
        int led,row,col, noKeysScanned;
        static int keyCode = -1, prevKey = 0;
        static unsigned long timeFirstPressed = 0;

        // 0. disable driving the 7segment LEDs -----------------
        for (led=0; led<8; led++) {
            pinMode(ledSelect[led], INPUT);  // set led pins to input
            // not really necessary, just to stop them
            // from driving either high or low.
            digitalWrite(ledSelect[led], HIGH); // Use builtin pullup resistors
        }
        // 1. initialise: set columns to input with pullups
        for (col=0; col<segmLEDs; col++) {
            pinMode(aCols[col], INPUT);           // set pin to input
            digitalWrite(aCols[col], HIGH);       // turn on pullup resistors
        }
        // 2. perform scanning
        noKeysScanned=0;

        for (row=0; row<3; row++) {
            digitalWrite(aRows[row], LOW);       // activate this row
            for (col=0; col<segmLEDs; col++) {
                if (digitalRead(aCols[col])==LOW) { // key is pressed
                    keyCode = col+row*8+1;
                    if (keyCode!=prevKey) {
                        //Serial.println();
                        //Serial.print(" col: ");  Serial.print(col, DEC);
                        //Serial.print(" row: ");  Serial.print(row, DEC);
                        //Serial.print(" prevKey: ");  Serial.print(prevKey, DEC);
                        //Serial.print(" KeyCode: ");  Serial.println(keyCode, DEC);
                        prevKey = keyCode;
                        curkey = parseChar(keyCode);
                        //Serial.print(" curkey: ");  Serial.print(curkey, DEC);
                        timeFirstPressed=millis();  //
                    } else { // if pressed for >1sec, it's a ModeShift key
                        if ((millis()-timeFirstPressed)>1000) { // more than 1000 ms
                            if (keyCode==17) { //it was the SST button
                                keyboardMode=(keyboardMode==0?1:0); // toggle
//                              Serial.print(F("                                      Eeprom R/O     "));
                                Serial.print(F("                                keyboardMode: "));
                                Serial.print(keyboardMode, DEC);
                                SSTmode=0;
                                curkey=0;  // don't do anything else with this keypress
                            }
                            if (keyCode==9) { // it was RS button
                                curkey = '>';    // toggle eeprom write protect
                            }
// for _INCL_VTL02
                            if (keyCode==1) { // it was ST button
                                curkey = 0;  // toggle eeprom write protect
                                keyboardMode=(keyboardMode==0?2:0); // toggle
                                Serial.print(F("                                keyboardMode(VTL): "));
                                Serial.print(keyboardMode, DEC);
                            }
// ----------
                            timeFirstPressed=millis(); // because otherwise you toggle right back!

                        }
                    }
                } else {
                    noKeysScanned++;    // another row in which no keys were pressed
                }
            }
            digitalWrite(aRows[row], HIGH);       // de-activate this row
        }

        if (noKeysScanned==24) {  // no keys detected in any row, 3 rows * 8 columns = 24. used to be 28.
            prevKey=0;    // allows you to enter same key twice
        }
    } // end of function
} // end C segment
