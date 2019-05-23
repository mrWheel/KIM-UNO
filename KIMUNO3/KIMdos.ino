/*
    Program :   KIMdos
    Author  :   Willem Aandewiel
    Created :   21 Okt. 2016
        
    * SD card attached to SPI bus as follows:
    ** Mega: MOSI - pin 51 
    *        MISO - pin 50 
    *        CLK  - pin 52 
    *        CS   - pin 53 (CS pin can be changed)
    *        and pin #52 (SS) must be an output
*/

#include "Config.h"

#if _USE_KIMDOS == TRUE

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


// include the SD library:
#include <SPI.h>
#include <SD.h>

#define isPrintableChar(n)  ((' ' <= n && n <= '~'))
#define isHexChar(n)        (('0' <= n && n <= '9') || ('a' <= n && n <= 'f') || ('A' <= n && n <= 'F'))
#define isNumChar(n)        (('0' <= n && n <= '9'))
#define DescriptionLen  30
#define memStartLen      6
#define RAM002BASE      0x17C0

extern unsigned char RAM002[];  // 64 bytes RAM in ROM002
extern unsigned char RAM2[];    // memory extention 
extern uint8_t useKeyboardLed;  // 0 to use Serial port or 1 for HEX digits 

enum  { ISPRINTABLE, ISNUMERIC, ISHEX };

const int chipSelect = 53;    

const String menuPrompt = "\n\r[KIMdos] C)atalog, D)elete, Q)uit : ";
char        inChar;
bool        serialChar;
uint16_t    startAddress, endAddress;
uint8_t     ID, firstFreeID, firstUsedID;


String readKeyboard(int sLen, uint8_t setMode=ISPRINTABLE) {
    String inField;
    bool   Done    = false;
    
    inField.reserve(sLen);   
    
    while (!Done) {
        while (Serial.available()) {
            inChar = (char)Serial.read();
            if (  ((setMode == ISPRINTABLE) && isPrintableChar(inChar)) 
                || (setMode == ISNUMERIC && isNumChar(inChar))
                || (setMode == ISHEX && isHexChar(inChar)) ) {
                inField += inChar;
                if (localEcho) Serial.print(inChar);
            }
            if (inChar < 32 || inField.length() > (sLen -1)) {
                Done = true;
            }
        }
    }
    return inField;
    
}   // readKeyboard()

// ------------------------------------
// convert a HEX string into a uint8_t
// ------------------------------------
uint8_t hex2Num(String inHex)
{
    uint8_t res=0;
    for (int P=0; P<inHex.length(); P++) {
        res = (res << 4);
        if(inHex[P]>=48 && inHex[P]<=57)        res += inHex[P]-48;
        else if(inHex[P]>=65 && inHex[P]<=70)   res += inHex[P] - 65 + 0x0a;
        else if(inHex[P]>=97 && inHex[P]<=102)  res += inHex[P] - 97 + 0x0a;
    }

    return res;

}   // hex2Num()

//------------------------------------------------------
// read parms from 0x17F5 .. 0x17F9
//------------------------------------------------------
bool readParms(char mode)
{
    uint8_t sAL, sAH;
    
    sAL = RAM002[0x17F5-RAM002BASE]; 
    sAH = RAM002[0x17F6-RAM002BASE];
    startAddress = (sAH << 8) + sAL;
    sAL = RAM002[0x17F7-RAM002BASE]; 
    sAH = RAM002[0x17F8-RAM002BASE];
    endAddress = (sAH << 8) + sAL;
    ID = RAM002[0x17F9-RAM002BASE];
    Sprint(F("\n\r[KIMdos] startAddress[$"));
    Sprint(startAddress, HEX);
    Sprint(F("], endAddress[$"));
    Sprint(endAddress, HEX);
    Sprint(F("], ID[$"));
    Sprint(ID, HEX);
    Sprint(F("]"));
    if ((mode == 'W') && endAddress <= startAddress) {
        Serial.println(F("\n\rAddress error!\r"));
        return false;
    }
    Sprintln(F("\r"));
    return true;
    
}   // readParms()

//------------------------------------------------------
// write the description to myFile (first 30 char if file)
//------------------------------------------------------
void writeDescription(File myFile, String Description)
{
    while (Description.length() < DescriptionLen) Description += ' ';

    myFile.seek(0);
    myFile.print(Description);
    myFile.print('\n');
    myFile.print("____________________");   // filler
        
}   // writeDescription()


//------------------------------------------------------
// read the first 30 chars of myFile into Description
//------------------------------------------------------
String readDescription(File myFile)
{
    String  Description;
    char    charF;
    
    Description.reserve(DescriptionLen);   
    charF = '\0';
    Description = "";
    myFile.seek(0);
    while (myFile.available() && (Description.length() < DescriptionLen)) {
        charF = myFile.read();
        if (isPrintableChar(charF)) Description += charF;
    }
    return Description;
    
}   // readDescription()


//------------------------------------------------------
// write the startAddress of the program to myFile
//------------------------------------------------------
void writeMemStart(File myFile, uint16_t Start)
{
    Sprint(F("startAddress[$"));
    Sprint(Start, HEX);
    Sprint("]\n\r");
    myFile.seek(DescriptionLen + 2);
    myFile.print(Start, DEC);
    myFile.print('\n');
    myFile.print("____________________");   // filler
    
}   // writeMemStart()


//------------------------------------------------------
// read the startAddress of the program from myFile
//------------------------------------------------------
uint16_t readMemStart(File myFile)
{
    String  memStart;
    char    charF;
    
    memStart.reserve(memStartLen);   
    charF = '\0';
    memStart = "";

    myFile.seek(DescriptionLen + 2);
    while (myFile.available() && ((charF != '\n') || (memStart.length() < 1))) {
        charF = myFile.read();
        if (isNumChar(charF)) memStart += charF;
    }
    return (uint16_t)memStart.toInt();
    
}   // readMemStart()


//------------------------------------------------------
// write the program data from RAM (Start to End)
//------------------------------------------------------
void writeProgram(File myFile, uint16_t Start, uint16_t End)
{
    uint8_t I=0;
    myFile.seek(DescriptionLen + 10);
    for(uint16_t M=Start; M<=End; M++) {
        I++;
        if (M < 0x0400)  
                myFile.print(RAM[M]);
        else    myFile.print(RAM2[M-_MIN_RAM2_ADDRESS]);
        if (I%16 == 0)  myFile.print("\n");
        else            myFile.print(" ");
        
    }
}   // writeProgram()


//------------------------------------------------------
// read the program into RAM (start at RAM[Start])
//------------------------------------------------------
void readProgram(File myFile, uint16_t Start)
{
    String      byteS;
    char        charF;
    uint16_t    M;
    uint8_t     I=0;
    
    byteS.reserve(4);
    M=Start;   
    if (useKeyboardLed == 0) {
        Serial.print(F(" [$"));
        Serial.print(M, HEX);
        Serial.println(F("]\r"));
    }
    myFile.seek(DescriptionLen + 10);
    charF = '\0';
    byteS = "";
    while (myFile.available()) {
        charF = myFile.read();
        //Sprint("("); Sprint(charF); Sprint(")");
        if ((charF == ' ') || (charF == '\n')) {
            if (I%16 == 0) {
                Sprint(F("\n\r[$"));
                if (M < 0x10)   {  Sprint(F("0"));}
                if (M < 0x100)  {  Sprint(F("0"));}
                if (M < 0x1000) {  Sprint(F("0"));}
                Sprint(M, HEX);
                Sprint(F("] "));
            }
            if (byteS.length() > 0) {
                Sprint(F("$"));
                if (byteS.toInt() < 0x10) Sprint(F("0")); 
                Sprint(byteS.toInt(), HEX);
                Sprint(F(" "));
                if (M < 0x0400)  
                        RAM[M]                    = byteS.toInt();
                else    RAM2[M-_MIN_RAM2_ADDRESS] = byteS.toInt();
                M++;
                I++;
                byteS = "";
            }
        }
        else if (('0' <= charF) && (charF <= '9')) byteS += charF;
    }
    
}   // readProgram()


//------------------------------------------------------
// read all the file with a name "KIM<num>.PRG"
// verbose if dispMode == 'L' otherwise without any 
// output, just set firstUsedID and firstFreeID
//------------------------------------------------------
void catalogSDcard(char dispMode='L')
{
    File    myFile;
    String  fileName, ID, Description, memStart;
    char    charF;

    firstUsedID = 0x00;
    firstFreeID = 0x00;
    
    fileName.reserve(10);   
    memStart.reserve(memStartLen);   
    Description.reserve(DescriptionLen);   

    Sprint(F("FreeRam: ")); Sprintln(FreeRam());

    for (int ID=1; ID<255; ID++) {
        fileName = "KIM";
        fileName += ID;
        fileName += ".prg";
        if (SD.exists(fileName)) {
            myFile = SD.open(fileName);
            if (myFile) {
                if (firstUsedID == 0x00)    firstUsedID = ID;
                if (dispMode == 'L') Serial.print(F("$"));
                if ((dispMode == 'L') && (ID < 0x10))   Serial.print(F("0"));
                if (dispMode == 'L') Serial.print(ID, HEX);
                if (dispMode == 'L') Serial.print(F(": "));

                // read from the file until there's a newLine:
                if (dispMode == 'L') Serial.print(readDescription(myFile));
                if (dispMode == 'L') Serial.print(F("  @$"));
                if (dispMode == 'L') Serial.print(readMemStart(myFile), HEX);
            } else {
                // if the file didn't open, print an error:
                if (dispMode == 'L') Serial.print(F("[Files] Error opening "));
                if (dispMode == 'L') Serial.print(fileName);
            }
            // close the file:
            myFile.close();
            if (dispMode == 'L') Serial.println();
        }
        if (firstFreeID == 0x00) firstFreeID = ID;
    }
    
}   // catalogSDcard()


//------------------------------------------------------
// dump the contents of a file.
//------------------------------------------------------
void dumpFileFromSD()
{
    File    myFile;
    String  fileName;
    char    charF;
    uint8_t P=0;

    if (!readParms('R')) return;
    if (ID == 0) {          // read first data block that is
        catalogSDcard('X');   // in use
        ID = firstUsedID;
    }

    Serial.print(F("[Dump] file: [$"));
    Serial.print(ID, HEX);
    Serial.println(F("]\r"));

    fileName = "KIM";
    if (ID == 0xFF) {       // read first dat block that is in use and
        catalogSDcard('X');   // relocate the program to the address in
        fileName += firstUsedID;    // $1705 & $1706
    } else {
        fileName += ID;
    }
    fileName += ".prg";

    // re-open the file for reading:
    if (SD.exists(fileName)) {
        myFile = SD.open(fileName);
        if (myFile) {
            Serial.print(F("$"));
            if (ID < 0x10)        Serial.print(F("0"));
            Serial.print(ID, HEX);
            Serial.print(F(": "));

            while (myFile.available()) {
                charF = myFile.read();
                P++;
                if (isPrintableChar(charF)) Serial.print(charF);
                else {
                    if (charF == '\n') {
                        Serial.print(F("<newLine>"));
                        P=0;
                    }
                    else Serial.print(F("."));
                }
                if (P%40 == 0)  Serial.println(F("\r"));
            }

        }
        // close the file:
        myFile.close();
        Serial.println(F("\r"));
        RAM[0x00F9] = ID;
        RAM[0x00FA] = 0;
        RAM[0x00FB] = 0;
    }
    else {
        Serial.println(F("\r[Dump] file not found!"));
        RAM[0x00F9] = 0;
        RAM[0x00FA] = 0xFF;
        RAM[0x00FB] = 0xFF;
    }

}   // dumpFileFromSD()

//------------------------------------------------------
// write a file to the SDcard or overwrite an existing
// file
// prompt for descriptions (if TTY mode). Empty 
// description leaves existing description as is.
//------------------------------------------------------
void writeFileToSD()
{
    File myFile;
    String fileName, Description;
    
    fileName.reserve(10);
//  ID.reserve(3);    
    Description.reserve(DescriptionLen);

    if (!readParms('W')) {
        RAM[0x00F9] = 0;
        RAM[0x00FA] = 0xFF;
        RAM[0x00FB] = 0xFF;
        return;
    }
    if (ID == 0) {          // use first valid data block (ID)
        
        catalogSDcard('X');   // that is not in use
        ID = firstFreeID;
    }
    
    if (useKeyboardLed == 0) {
        Serial.print(F("[Write] ID : [$"));
        Serial.print(ID, HEX);
        Serial.print(F("]\n\r[Write] enter description : "));
        Description = readKeyboard(DescriptionLen, ISPRINTABLE);
    }

    fileName = "KIM";
    fileName += ID;
    fileName += ".prg";
    if (useKeyboardLed == 0) {
        Serial.print(F("\n\r[Write] file: "));
        Serial.println(fileName);
    }
    if (SD.exists(fileName)) {
        if (Description.length() <= 0) {
            myFile = SD.open(fileName);
            Description = readDescription(myFile);
            myFile.close();
        }
        SD.remove(fileName);
    }
    myFile = SD.open(fileName, FILE_WRITE);

    // if the file opened okay, write to it:
    if (myFile) {
        if (useKeyboardLed == 0) {
            Serial.print(F("Writing ... "));
        }
        writeDescription(myFile, Description);
        writeMemStart(myFile, startAddress);
        writeProgram(myFile, startAddress, endAddress);
        myFile.print("\n");
        // close the file:
        //myFile.close();
        RAM[0x00F9] = ID;
        RAM[0x00FA] = 0;
        RAM[0x00FB] = 0;
        if (useKeyboardLed == 0) {
            Serial.println(F("done.\r"));
        }
    } else {
        if (useKeyboardLed == 0) {
            // if the file didn't open, print an error:
            Serial.println(F("[Write] Error writing file\r"));
        }
        RAM[0x00F9] = 0;
        RAM[0x00FA] = 0xFF;
        RAM[0x00FB] = 0xFF;
    }
    myFile.close();
    
}   // writeFileToSD()


//------------------------------------------------------
// read file from SDcard
//------------------------------------------------------
void readFileFromSD()
{
    File    myFile;
    String  fileName, Description, memStart;
    char    charF;
    
    fileName.reserve(10);    
    Description.reserve(DescriptionLen);    
    memStart.reserve(memStartLen);    
    
    if (!readParms('R')) return;
    if (ID == 0) {          // read first data block that is
        catalogSDcard('X');   // in use
        ID = firstUsedID;
    }

    if (useKeyboardLed == 0) {
        Serial.print(F("[Read] file: [$"));
        Serial.print(ID, HEX);
        Serial.println(F("]"));
    }

    fileName = "KIM";
    if (ID == 0xFF) {       // read first dat block that is in use and
        catalogSDcard('X');   // relocate the program to the address in
        fileName += firstUsedID;    // $1705 & $1706
    } else {
        fileName += ID;
    }
    fileName += ".prg";

    // re-open the file for reading:
    if (SD.exists(fileName)) {
        myFile = SD.open(fileName);
        if (myFile) {
            if (useKeyboardLed == 0) {
                Serial.print(F("$"));
                if (ID < 0x10)        Serial.print(F("0"));
                //else if (ID < 100)  Serial.print(F("0"));
                Serial.print(ID, HEX);
                Serial.print(F(": "));

                // read Description:
                Serial.print(readDescription(myFile));
            }
            memStart = readMemStart(myFile);
            if (ID == 0xFF) {   // relocate program
                if (useKeyboardLed == 0) {
                    Serial.print(F("\n\rRelocate program to "));
                }
                readProgram(myFile, startAddress);
            } else {
                if (useKeyboardLed == 0) {
                    Serial.print(F("\n\rLoad program to "));
                }
                readProgram(myFile, memStart.toInt());
            }
            if (useKeyboardLed == 0) {
                Serial.println(F("\r"));
            }
        } else {
            if (useKeyboardLed == 0) {
                // if the file didn't open, print an error:
                Serial.print(F("\n\r[Read] Error opening "));
                Serial.print(fileName);
            }
            RAM[0x00F9] = 0;
            RAM[0x00FA] = 0xFF;
            RAM[0x00FB] = 0xFF;
        }
        // close the file:
        myFile.close();
        if (useKeyboardLed == 0) {
            Serial.println(F("\r"));
        }
        RAM[0x00F9] = ID;
        RAM[0x00FA] = 0;
        RAM[0x00FB] = 0;
    }
    else {
        if (useKeyboardLed == 0) {
            Serial.println(F("\r[Read] file not found!"));
        }
        RAM[0x00F9] = 0;
        RAM[0x00FA] = 0xFF;
        RAM[0x00FB] = 0xFF;
    }
  
}   // readFileFromSD()


//------------------------------------------------------
//------------------------------------------------------
void deleteFileFromSD()
{
    File    myFile;
    String  fileName, IDhex;
    uint8_t ID;
    
    fileName.reserve(10);    
    IDhex.reserve(4);    
    
    Serial.print(F("\n\r[Delete] enter filenumber : "));
    IDhex = readKeyboard(3, ISHEX);
    if (IDhex.length() == 0) return;
    ID = hex2Num(IDhex);

    Serial.print(F("\n\rDelete file: $"));
    if (ID < 0x10)  Serial.print("0");
    Serial.println(ID, HEX);

    fileName = "KIM";
    fileName += ID;
    fileName += ".prg";
    if (!SD.exists(fileName)) {
        Serial.println(F("\n\r[Delete] File doesn't exist."));
    } else {
        // remove the file for reading:
        SD.remove(fileName);
    }
  
}   // deleteFileFromSD()


//------------------------------------------------------
//------------------------------------------------------
void KIMdosMenu(char dosMode)
{
    bool processMenu = true;
        
    SD.begin(chipSelect);

    switch(dosMode) {
        case 'r':   // 0x1873 G
        case 'R': readFileFromSD();
                  if (useKeyboardLed == 1) return;
                  break;
        case 'w':   // 0x1800 G
        case 'W': writeFileToSD();
                  if (useKeyboardLed == 1) return;
                  break;
    }
    Serial.print(menuPrompt);
    while (processMenu) {
        while (Serial.available()) {
            inChar = (char)Serial.read();
            Serial.println();
            switch(inChar) {
                case 'c':
                case 'C': catalogSDcard('L');
                          Serial.print(menuPrompt);
                          break;
                case 'd':
                case 'D': deleteFileFromSD();
                          Serial.print(menuPrompt);
                          break;
                case 'r':
                case 'R': readFileFromSD();
                          Serial.print(menuPrompt);
                          break;
                case 'w':
                case 'W': writeFileToSD();
                          Serial.print(menuPrompt);
                          break;
                case 'x':
                case 'X': dumpFileFromSD();
                          Serial.print(menuPrompt);
                          break;
                case 'q':
                case 'Q': Serial.println(F("[KIMdos] Return"));
                          processMenu = false;
                          break;
                default:  Serial.print(menuPrompt);
            }   // switch
        }
    }

}   // KIMdosMenu()

#endif
