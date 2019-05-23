#include "Config.h"

#if _TARGET == ARDUINO_UNO || _TARGET == ARDUINO_MINI
    #define APORTL    2
    #define APORTN    4
    #define BPORTL   10        
    #define BPORTN    5        
#elif _TARGET == ARDUINO_MEGA
    #define APORTL   22
    #define APORTN    8
    #define BPORTL   30
    #define BPORTN    8
#elif _TARGET == ARDUINO_DUE
    #define APORTL   22
    #define APORTN    8
    #define BPORTL   30
    #define BPORTN    8
#elif _TARGET == CORE_TEENSY
    #define APORTL   20
    #define APORTN    8
    #define BPORTL   28
    #define BPORTN    6
#endif 

#define DEBUG false
#if DEBUG == false 
    #define Sbegin(...)   
    #define Sprint(...)   
    #define Sprintln(...) 
    #define Sflush(...)    
#else
    #define Sbegin(...)     {Serial.begin(__VA_ARGS__); }
    #define Sprint(...)     {Serial.print(__VA_ARGS__); }
    #define Sprintln(...)   {Serial.println(__VA_ARGS__); }
    #define Sflush(...)     {Serial.flush(__VA_ARGS__); }
#endif

uint8_t DDR1701, DDR1703, DDR;
uint8_t lowPIN, maxPIN, realPIN, TMP;
uint8_t mask, P;


void writeDDR(char DDR, int Byte)
{
    if (DDR == 'A') {
        DDR1701 = Byte; 
        realPIN = APORTL+APORTN-1; 
        lowPIN = APORTL; 
        maxPIN = APORTN; 
    }
    else if (DDR == 'B') {
        DDR1703 = Byte; 
        realPIN = BPORTL+BPORTN-1; 
        lowPIN = BPORTL; 
        maxPIN = BPORTN; 
    }

    Sprintln();
    P=7;
    for (mask = B10000000; mask>0; (mask >>= 1), P--) { //iterate through bit mask
        if (P >= maxPIN) continue;
        if (Byte & mask) {
            pinMode(realPIN, OUTPUT);
            Sprint("DDR-realPIN ["); Sprint(realPIN); Sprintln("] OUTPUT");
        }
        else {
            pinMode(realPIN, INPUT);
            Sprint("DDR-realPIN ["); Sprint(realPIN); Sprintln("] INPUT");
        }
        realPIN--;
        
    }   // for bits...

    if (DDR == 'A') Sprint("1701 ");
    if (DDR == 'B') Sprint("1703 ");
     
}   // writeDDR()


uint8_t readDDR(char DDR)
{
    Sprintln();
    if (DDR == 'A') {
        Sprint("DDR1701: ["); Sprint(DDR1701, HEX); Sprintln("]");
        Sprint("1701 ");
        return DDR1701; 
    }
    else if (DDR == 'B') {
        Sprint("DDR1703: ["); Sprint(DDR1703, HEX); Sprintln("]");
        Sprint("1703 ");
        return DDR1703; 
    }

}   // readDDR()


uint8_t readPORT(char PORT)
{    
    
    uint8_t Mem = B00000000;
    uint8_t s;
    
    if (PORT == 'A') {
        DDR = DDR1701; 
        realPIN = APORTL+APORTN-1; 
        lowPIN = APORTL; 
        maxPIN = APORTN; 
    }
    else if (PORT == 'B') {
        DDR = DDR1703; 
        realPIN = BPORTL+BPORTN-1; 
        lowPIN = BPORTL; 
        maxPIN = BPORTN; 
    }

    Sprintln();
    P=7;
    for (mask = B10000000; mask>0; (mask >>= 1), P--) { //iterate through bit mask
        if (P >= maxPIN) continue;
            Sprint("digitalRead("); Sprint(realPIN);
            if (digitalRead(realPIN)) {
                Sprintln(") = HIGH");
                Mem = (Mem * 2) + 1;
            } else {
                Sprintln(") = LOW");
                Mem = Mem * 2;                
            }
        realPIN--;
    }

    if (PORT == 'A') Sprint("1700 ");
    if (PORT == 'B') Sprint("1702 ");

    return Mem;
     
}   // readPORT()


void writePORT(char PORT, uint8_t Byte)
{
    if (PORT == 'A') {
        DDR = DDR1701; 
        realPIN = APORTL+APORTN-1; 
        lowPIN = APORTL; 
        maxPIN = APORTN; 
    }
    else if (PORT == 'B') {
        DDR = DDR1703; 
        realPIN = BPORTL+BPORTN-1; 
        lowPIN = BPORTL; 
        maxPIN = BPORTN; 
    }
    Sprintln();
    P=7;
    for (mask = B10000000; mask>0; (mask >>= 1), P--) { //iterate through bit mask
        if (P >= maxPIN) continue;
        if (DDR & mask) {
            Sprint("DDR["); Sprint(P); Sprint("] output ");
            if (Byte & mask) {
                Sprint("digitalWrite("); Sprint(realPIN); Sprintln(", HIGH)");
                digitalWrite(realPIN, HIGH);
            }
            else {
                Sprint("digitalWrite("); Sprint(realPIN); Sprintln(", LOW)");
                digitalWrite(realPIN, LOW);
            }       
        }
        else   {Sprint("DDR["); Sprint(P); Sprintln("] input  ");}
        realPIN--;
    }   // for bits...
 
    if (PORT == 'A') Sprint("1700 ");
    if (PORT == 'B') Sprint("1702 ");
    
}   // writePORT()

