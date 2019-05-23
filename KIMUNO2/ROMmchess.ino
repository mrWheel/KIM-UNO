/* 
*   C:\temp27\KIM Uno\sw\KIM Uno 6502 ROM sources\microchess for KIM Uno\UCHESS7.BIN (15/09/2014 14:35:46)
*   StartOffset: 00000000, EndOffset: 00000570, Length: 00000571 
*
* ROM at 0xC000
*    
*   Coding style:
*       - One True Brace Style formatting/indenting.
*       - Linux brackets, add brackets to all conditionals.
*/

#include "Config.h"

#if _INCL_MCHESS == TRUE

// Microchess at 0xC000:
#if _TARGET == PERSONAL_COMPUTER
    unsigned char mchessROM[1393] = {
#else
    const unsigned char mchessROM[1393] PROGMEM = {
#endif
    0xA9, 0x00, 0x85, 0xB7, 0x20, 0x54, 0xC4, 0xD8, 0xA2, 0xFF, 0x9A, 0xA2,
    0xC8, 0x86, 0xB2, 0x20, 0x78, 0xC3, 0x20, 0x49, 0xC4, 0xC9, 0x43, 0xD0,
    0x12, 0xA2, 0x1F, 0xBD, 0x14, 0xC5, 0x95, 0x50, 0xCA, 0x10, 0xF8, 0xA2,
    0x1B, 0x86, 0xDC, 0xA9, 0xCC, 0xD0, 0x19, 0xC9, 0x45, 0xD0, 0x0E, 0x20,
    0xC1, 0xC1, 0x38, 0xA9, 0x01, 0xE5, 0xB7, 0x85, 0xB7, 0xA9, 0xEE, 0xD0,
    0x07, 0xC9, 0x40, 0xD0, 0x0B, 0x20, 0xB5, 0xC2, 0x85, 0xFB, 0x85, 0xFA,
    0x85, 0xF9, 0xD0, 0xBB, 0xC9, 0x0D, 0xD0, 0x06, 0x20, 0x5B, 0xC2, 0x4C,
    0xFD, 0xC0, 0xC9, 0x41, 0xF0, 0x03, 0x4C, 0xF6, 0xC0, 0x4C, 0x00, 0xFF,
    0xA6, 0xB5, 0x30, 0x5C, 0xA5, 0xB0, 0xF0, 0x08, 0xE0, 0x08, 0xD0, 0x04,
    0xC5, 0xE6, 0xF0, 0x2E, 0xF6, 0xE3, 0xC9, 0x01, 0xD0, 0x02, 0xF6, 0xE3,
    0x50, 0x1E, 0xA0, 0x0F, 0xA5, 0xB1, 0xD9, 0x60, 0x00, 0xF0, 0x03, 0x88,
    0x10, 0xF8, 0xB9, 0x45, 0xC5, 0xD5, 0xE4, 0x90, 0x04, 0x94, 0xE6, 0x95,
    0xE4, 0x18, 0x08, 0x75, 0xE5, 0x95, 0xE5, 0x28, 0xE0, 0x04, 0xF0, 0x03,
    0x30, 0x31, 0x60, 0xA5, 0xE8, 0x85, 0xDD, 0xA9, 0x00, 0x85, 0xB5, 0x20,
    0x5B, 0xC2, 0x20, 0xC1, 0xC1, 0x20, 0x0F, 0xC1, 0x20, 0xC1, 0xC1, 0xA9,
    0x08, 0x85, 0xB5, 0x20, 0x18, 0xC1, 0x20, 0x41, 0xC2, 0x4C, 0x12, 0xC3,
    0xE0, 0xF9, 0xD0, 0x0B, 0xA5, 0x60, 0xC5, 0xB1, 0xD0, 0x04, 0xA9, 0x00,
    0x85, 0xB4, 0x60, 0x50, 0xFD, 0xA0, 0x07, 0xA5, 0xB1, 0xD9, 0x60, 0x00,
    0xF0, 0x05, 0x88, 0xF0, 0xF1, 0x10, 0xF6, 0xB9, 0x45, 0xC5, 0xD5, 0xE2,
    0x90, 0x02, 0x95, 0xE2, 0xC6, 0xB5, 0xA9, 0xFB, 0xC5, 0xB5, 0xF0, 0x03,
    0x20, 0x35, 0xC2, 0xE6, 0xB5, 0x60, 0xC9, 0x08, 0xB0, 0x12, 0x20, 0x02,
    0xC3, 0xA2, 0x1F, 0xB5, 0x50, 0xC5, 0xFA, 0xF0, 0x03, 0xCA, 0x10, 0xF7,
    0x86, 0xFB, 0x86, 0xB0, 0x4C, 0x07, 0xC0, 0xA2, 0x10, 0xA9, 0x00, 0x95,
    0xDE, 0xCA, 0x10, 0xFB, 0xA9, 0x10, 0x85, 0xB0, 0xC6, 0xB0, 0x10, 0x01,
    0x60, 0x20, 0x2E, 0xC2, 0xA4, 0xB0, 0xA2, 0x08, 0x86, 0xB6, 0xC0, 0x08,
    0x10, 0x41, 0xC0, 0x06, 0x10, 0x2E, 0xC0, 0x04, 0x10, 0x1F, 0xC0, 0x01,
    0xF0, 0x09, 0x10, 0x0E, 0x20, 0x9D, 0xC1, 0xD0, 0xFB, 0xF0, 0xD9, 0x20,
    0xAB, 0xC1, 0xD0, 0xFB, 0xF0, 0xD2, 0xA2, 0x04, 0x86, 0xB6, 0x20, 0xAB,
    0xC1, 0xD0, 0xFB, 0xF0, 0xC7, 0x20, 0xAB, 0xC1, 0xA5, 0xB6, 0xC9, 0x04,
    0xD0, 0xF7, 0xF0, 0xBC, 0xA2, 0x10, 0x86, 0xB6, 0x20, 0x9D, 0xC1, 0xA5,
    0xB6, 0xC9, 0x08, 0xD0, 0xF7, 0xF0, 0xAD, 0xA2, 0x06, 0x86, 0xB6, 0x20,
    0xD9, 0xC1, 0x50, 0x05, 0x30, 0x03, 0x20, 0x60, 0xC0, 0x20, 0x2E, 0xC2,
    0xC6, 0xB6, 0xA5, 0xB6, 0xC9, 0x05, 0xF0, 0xEB, 0x20, 0xD9, 0xC1, 0x70,
    0x8F, 0x30, 0x8D, 0x20, 0x60, 0xC0, 0xA5, 0xB1, 0x29, 0xF0, 0xC9, 0x20,
    0xF0, 0xEE, 0x4C, 0x1C, 0xC1, 0x20, 0xD9, 0xC1, 0x30, 0x03, 0x20, 0x60,
    0xC0, 0x20, 0x2E, 0xC2, 0xC6, 0xB6, 0x60, 0x20, 0xD9, 0xC1, 0x90, 0x02,
    0x50, 0xF9, 0x30, 0x07, 0x08, 0x20, 0x60, 0xC0, 0x28, 0x50, 0xF0, 0x20,
    0x2E, 0xC2, 0xC6, 0xB6, 0x60, 0xA2, 0x0F, 0x38, 0xB4, 0x60, 0xA9, 0x77,
    0xF5, 0x50, 0x95, 0x60, 0x94, 0x50, 0x38, 0xA9, 0x77, 0xF5, 0x50, 0x95,
    0x50, 0xCA, 0x10, 0xEB, 0x60, 0xA5, 0xB1, 0xA6, 0xB6, 0x18, 0x7D, 0x34,
    0xC5, 0x85, 0xB1, 0x29, 0x88, 0xD0, 0x42, 0xA5, 0xB1, 0xA2, 0x20, 0xCA,
    0x30, 0x0E, 0xD5, 0x50, 0xD0, 0xF9, 0xE0, 0x10, 0x30, 0x33, 0xA9, 0x7F,
    0x69, 0x01, 0x70, 0x01, 0xB8, 0xA5, 0xB5, 0x30, 0x24, 0xC9, 0x08, 0x10,
    0x20, 0x48, 0x08, 0xA9, 0xF9, 0x85, 0xB5, 0x85, 0xB4, 0x20, 0x5B, 0xC2,
    0x20, 0xC1, 0xC1, 0x20, 0x18, 0xC1, 0x20, 0x3E, 0xC2, 0x28, 0x68, 0x85,
    0xB5, 0xA5, 0xB4, 0x30, 0x04, 0x38, 0xA9, 0xFF, 0x60, 0x18, 0xA9, 0x00,
    0x60, 0xA9, 0xFF, 0x18, 0xB8, 0x60, 0xA6, 0xB0, 0xB5, 0x50, 0x85, 0xB1,
    0x60, 0x20, 0x5B, 0xC2, 0x20, 0xC1, 0xC1, 0x20, 0x18, 0xC1, 0x20, 0xC1,
    0xC1, 0xBA, 0x86, 0xB3, 0xA6, 0xB2, 0x9A, 0x68, 0x85, 0xB6, 0x68, 0x85,
    0xB0, 0xAA, 0x68, 0x95, 0x50, 0x68, 0xAA, 0x68, 0x85, 0xB1, 0x95, 0x50,
    0x4C, 0x80, 0xC2, 0xBA, 0x86, 0xB3, 0xA6, 0xB2, 0x9A, 0xA5, 0xB1, 0x48,
    0xA8, 0xA2, 0x1F, 0xD5, 0x50, 0xF0, 0x03, 0xCA, 0x10, 0xF9, 0xA9, 0xCC,
    0x95, 0x50, 0x8A, 0x48, 0xA6, 0xB0, 0xB5, 0x50, 0x94, 0x50, 0x48, 0x8A,
    0x48, 0xA5, 0xB6, 0x48, 0xBA, 0x86, 0xB2, 0xA6, 0xB3, 0x9A, 0x60, 0xA6,
    0xE4, 0xEC, 0x45, 0xC5, 0xD0, 0x04, 0xA9, 0x00, 0xF0, 0x0A, 0xA6, 0xE3,
    0xD0, 0x06, 0xA6, 0xEE, 0xD0, 0x02, 0xA9, 0xFF, 0xA2, 0x04, 0x86, 0xB5,
    0xC5, 0xFA, 0x90, 0x0C, 0xF0, 0x0A, 0x85, 0xFA, 0xA5, 0xB0, 0x85, 0xFB,
    0xA5, 0xB1, 0x85, 0xF9, 0xA9, 0x2E, 0x4C, 0x60, 0xC4, 0xA6, 0xDC, 0x30,
    0x1C, 0xA5, 0xF9, 0xDD, 0x55, 0xC5, 0xD0, 0x11, 0xCA, 0xBD, 0x55, 0xC5,
    0x85, 0xFB, 0xCA, 0xBD, 0x55, 0xC5, 0x85, 0xF9, 0xCA, 0x86, 0xDC, 0xD0,
    0x1C, 0xA9, 0xFF, 0x85, 0xDC, 0xA2, 0x0C, 0x86, 0xB5, 0x86, 0xFA, 0xA2,
    0x14, 0x20, 0x11, 0xC1, 0xA2, 0x04, 0x86, 0xB5, 0x20, 0x0F, 0xC1, 0xA6,
    0xFA, 0xE0, 0x0F, 0x90, 0x12, 0xA6, 0xFB, 0xB5, 0x50, 0x85, 0xFA, 0x86,
    0xB0, 0xA5, 0xF9, 0x85, 0xB1, 0x20, 0x5B, 0xC2, 0x4C, 0x07, 0xC0, 0xA9,
    0xFF, 0x60, 0xA2, 0x04, 0x06, 0xF9, 0x26, 0xFA, 0xCA, 0xD0, 0xF9, 0x05,
    0xF9, 0x85, 0xF9, 0x85, 0xB1, 0x60, 0x18, 0xA9, 0x80, 0x65, 0xEB, 0x65,
    0xEC, 0x65, 0xED, 0x65, 0xE1, 0x65, 0xDF, 0x38, 0xE5, 0xF0, 0xE5, 0xF1,
    0xE5, 0xE2, 0xE5, 0xE0, 0xE5, 0xDE, 0xE5, 0xEF, 0xE5, 0xE3, 0xB0, 0x02,
    0xA9, 0x00, 0x4A, 0x18, 0x69, 0x40, 0x65, 0xEC, 0x65, 0xED, 0x38, 0xE5,
    0xE4, 0x4A, 0x18, 0x69, 0x90, 0x65, 0xDD, 0x65, 0xDD, 0x65, 0xDD, 0x65,
    0xDD, 0x65, 0xE1, 0x38, 0xE5, 0xE4, 0xE5, 0xE4, 0xE5, 0xE5, 0xE5, 0xE5,
    0xE5, 0xE0, 0xA6, 0xB1, 0xE0, 0x33, 0xF0, 0x16, 0xE0, 0x34, 0xF0, 0x12,
    0xE0, 0x22, 0xF0, 0x0E, 0xE0, 0x25, 0xF0, 0x0A, 0xA6, 0xB0, 0xF0, 0x09,
    0xB4, 0x50, 0xC0, 0x10, 0x10, 0x03, 0x18, 0x69, 0x02, 0x4C, 0x87, 0xC2,
    0x20, 0x17, 0xC4, 0x20, 0x3B, 0xC4, 0x20, 0x22, 0xC4, 0xA0, 0x00, 0x20,
    0xE9, 0xC3, 0xA9, 0x7C, 0x20, 0x60, 0xC4, 0xA2, 0x1F, 0x98, 0xD5, 0x50,
    0xF0, 0x40, 0xCA, 0x10, 0xF8, 0x98, 0x29, 0x01, 0x85, 0xFC, 0x98, 0x4A,
    0x4A, 0x4A, 0x4A, 0x29, 0x01, 0x18, 0x65, 0xFC, 0x29, 0x01, 0xF0, 0x03,
    0xA9, 0x2A, 0x2C, 0xA9, 0x20, 0x20, 0x60, 0xC4, 0x20, 0x60, 0xC4, 0xC8,
    0x98, 0x29, 0x08, 0xF0, 0xCD, 0xA9, 0x7C, 0x20, 0x60, 0xC4, 0x20, 0x34,
    0xC4, 0x20, 0x17, 0xC4, 0x20, 0xE9, 0xC3, 0x18, 0x98, 0x69, 0x08, 0xA8,
    0xC0, 0x80, 0xF0, 0x2B, 0xD0, 0xB4, 0xA5, 0xB7, 0xF0, 0x05, 0xBD, 0xD3,
    0xC4, 0xD0, 0x03, 0xBD, 0xC3, 0xC4, 0x20, 0x60, 0xC4, 0xBD, 0xF3, 0xC4,
    0x20, 0x60, 0xC4, 0xD0, 0xCA, 0x8A, 0x48, 0xA2, 0x19, 0xA9, 0x2D, 0x20,
    0x60, 0xC4, 0xCA, 0xD0, 0xFA, 0x68, 0xAA, 0x20, 0x17, 0xC4, 0x60, 0x20,
    0x22, 0xC4, 0xA5, 0xFB, 0x20, 0x64, 0xC4, 0xA9, 0x20, 0x20, 0x60, 0xC4,
    0xA5, 0xFA, 0x20, 0x64, 0xC4, 0xA9, 0x20, 0x20, 0x60, 0xC4, 0xA5, 0xF9,
    0x20, 0x64, 0xC4, 0xA9, 0x0D, 0x20, 0x60, 0xC4, 0xA9, 0x0A, 0x20, 0x60,
    0xC4, 0x60, 0xA2, 0x00, 0xA9, 0x20, 0x20, 0x60, 0xC4, 0x8A, 0x20, 0x64,
    0xC4, 0xE8, 0xE0, 0x08, 0xD0, 0xF2, 0xF0, 0xE3, 0x98, 0x29, 0x70, 0x20,
    0x64, 0xC4, 0x60, 0xA2, 0x00, 0xBD, 0x8A, 0xC4, 0xF0, 0x06, 0x20, 0x60,
    0xC4, 0xE8, 0xD0, 0xF5, 0x60, 0xA9, 0x3F, 0x20, 0x60, 0xC4, 0x20, 0x55,
    0xC4, 0x29, 0x4F, 0x60, 0x60, 0xAD, 0xF3, 0xCF, 0xC5, 0x01, 0xF0, 0xF9,
    0xAD, 0xF4, 0xCF, 0x60, 0x8D, 0xF1, 0xCF, 0x60, 0x48, 0x4A, 0x4A, 0x4A,
    0x4A, 0x20, 0x6D, 0xC4, 0x68, 0x29, 0x0F, 0x84, 0xFF, 0xA8, 0xB9, 0x7A,
    0xC4, 0xA4, 0xFF, 0x4C, 0x60, 0xC4, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
    0x36, 0x37, 0x38, 0x39, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x4D, 0x69,
    0x63, 0x72, 0x6F, 0x43, 0x68, 0x65, 0x73, 0x73, 0x20, 0x28, 0x63, 0x29,
    0x20, 0x31, 0x39, 0x39, 0x36, 0x2D, 0x32, 0x30, 0x30, 0x35, 0x20, 0x50,
    0x65, 0x74, 0x65, 0x72, 0x20, 0x4A, 0x65, 0x6E, 0x6E, 0x69, 0x6E, 0x67,
    0x73, 0x2C, 0x20, 0x77, 0x77, 0x77, 0x2E, 0x62, 0x65, 0x6E, 0x6C, 0x6F,
    0x2E, 0x63, 0x6F, 0x6D, 0x0D, 0x0A, 0x00, 0x57, 0x57, 0x57, 0x57, 0x57,
    0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x42,
    0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,
    0x42, 0x42, 0x42, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57,
    0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x4B, 0x51, 0x52, 0x52, 0x42,
    0x42, 0x4E, 0x4E, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x4B,
    0x51, 0x52, 0x52, 0x42, 0x42, 0x4E, 0x4E, 0x50, 0x50, 0x50, 0x50, 0x50,
    0x50, 0x50, 0x50, 0x00, 0x03, 0x04, 0x00, 0x07, 0x02, 0x05, 0x01, 0x06,
    0x10, 0x17, 0x11, 0x16, 0x12, 0x15, 0x14, 0x13, 0x73, 0x74, 0x70, 0x77,
    0x72, 0x75, 0x71, 0x76, 0x60, 0x67, 0x61, 0x66, 0x62, 0x65, 0x64, 0x63,
    0x00, 0xF0, 0xFF, 0x01, 0x10, 0x11, 0x0F, 0xEF, 0xF1, 0xDF, 0xE1, 0xEE,
    0xF2, 0x12, 0x0E, 0x1F, 0x21, 0x0B, 0x0A, 0x06, 0x06, 0x04, 0x04, 0x04,
    0x04, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x99, 0x25, 0x0B,
    0x25, 0x01, 0x00, 0x33, 0x25, 0x07, 0x36, 0x34, 0x0D, 0x34, 0x34, 0x0E,
    0x52, 0x25, 0x0D, 0x45, 0x35, 0x04, 0x55, 0x22, 0x06, 0x43, 0x33, 0x0F,
    0xCC
};

#endif
