/* _INCL_VTL02 interpreter
* for use, see _INCL_VTL02 on github
* ROM at 0xFC00
*    
*   Coding style
*       - One True Brace Style formatting/indenting.
*       - Linux brackets, add brackets to all conditionals.
*/

#include "Config.h"

//#ifndef _INCL_VTL02
//// eventuele functies die altijd opgeroepen worden hier
//#else
#if _INCL_VTL02 == TRUE

    #if _TARGET == PERSONAL_COMPUTER
        unsigned char vtl02ROM[1019] = {
    #else
        const unsigned char vtl02ROM[1019] PROGMEM = {
    #endif
    0xA9, 0x00, 0x85, 0xCC, 0xA9, 0x02, 0x85, 0xCD, 0xA9, 0x00, 0x85, 0xD4,
    0xA9, 0x04, 0x85, 0xD5, 0x38, 0xD8, 0xA2, 0xFF, 0x9A, 0x90, 0x10, 0x20,
    0x39, 0xFD, 0xA9, 0x4F, 0x20, 0xEB, 0xFF, 0xA9, 0x4B, 0x20, 0xEB, 0xFF,
    0x20, 0x39, 0xFD, 0x20, 0x64, 0xFF, 0xA2, 0xC6, 0x20, 0x2F, 0xFF, 0xF0,
    0x39, 0x18, 0xA5, 0xC6, 0x05, 0xC7, 0xD0, 0x41, 0x20, 0x10, 0xFD, 0xA2,
    0xD0, 0x20, 0x94, 0xFD, 0xA9, 0x20, 0x20, 0xEB, 0xFF, 0xA9, 0x00, 0x20,
    0x1A, 0xFD, 0xB0, 0xEC, 0xF0, 0x14, 0xA4, 0xD1, 0xA6, 0xD0, 0xC4, 0xC7,
    0xD0, 0x04, 0xE4, 0xC6, 0xF0, 0x0C, 0xE8, 0xD0, 0x01, 0xC8, 0x86, 0xC2,
    0x84, 0xC3, 0x2A, 0x49, 0x01, 0x6A, 0x20, 0x10, 0xFD, 0xC8, 0x08, 0x20,
    0x3E, 0xFD, 0x28, 0xA5, 0xC6, 0x05, 0xC7, 0x90, 0xD7, 0xF0, 0x9A, 0xD0,
    0xE9, 0x98, 0x48, 0x20, 0x84, 0xFF, 0xB0, 0x34, 0x45, 0xC6, 0xD0, 0x30,
    0xE4, 0xC7, 0xD0, 0x2C, 0xAA, 0xB1, 0x80, 0xA8, 0x49, 0xFF, 0x65, 0xCC,
    0x85, 0xCC, 0xB0, 0x02, 0xC6, 0xCD, 0xA5, 0x80, 0x85, 0xFC, 0xA5, 0x81,
    0x85, 0xFD, 0xA5, 0xFC, 0xC5, 0xCC, 0xA5, 0xFD, 0xE5, 0xCD, 0xB0, 0x0C,
    0xB1, 0xFC, 0x81, 0xFC, 0xE6, 0xFC, 0xD0, 0xEE, 0xE6, 0xFD, 0x90, 0xEA,
    0x68, 0xAA, 0xA5, 0xC6, 0x48, 0xA5, 0xC7, 0x48, 0xA0, 0x02, 0xE8, 0xC8,
    0xBD, 0x7F, 0x17, 0x48, 0xD0, 0xF8, 0xC0, 0x04, 0x90, 0x43, 0xAA, 0x98,
    0x18, 0x65, 0xCC, 0x85, 0xFC, 0x8A, 0x65, 0xCD, 0x85, 0xFD, 0xA5, 0xFC,
    0xC5, 0xD4, 0xA5, 0xFD, 0xE5, 0xD5, 0xB0, 0x2D, 0xA5, 0xCC, 0xD0, 0x02,
    0xC6, 0xCD, 0xC6, 0xCC, 0xA5, 0xCC, 0xC5, 0x80, 0xA5, 0xCD, 0xE5, 0x81,
    0x90, 0x06, 0xA1, 0xCC, 0x91, 0xCC, 0xB0, 0xE8, 0x98, 0xAA, 0x68, 0x88,
    0x91, 0x80, 0xD0, 0xFA, 0xA0, 0x02, 0x8A, 0x91, 0x80, 0xA5, 0xFC, 0x85,
    0xCC, 0xA5, 0xFD, 0x85, 0xCD, 0x4C, 0x11, 0xFC, 0x20, 0x84, 0xFF, 0xB0,
    0xF8, 0x85, 0xC6, 0x86, 0xC7, 0x60, 0xC8, 0xAA, 0x8A, 0xD1, 0x80, 0xF0,
    0x0A, 0xB1, 0x80, 0xF0, 0x06, 0x20, 0xEB, 0xFF, 0xC8, 0x10, 0xF1, 0xAA,
    0x20, 0xCC, 0xFF, 0x8A, 0xF0, 0x07, 0x20, 0xC2, 0xFF, 0xC9, 0x3B, 0xF0,
    0x4F, 0xA9, 0x0D, 0x4C, 0xEB, 0xFF, 0x20, 0xC3, 0xFF, 0xF0, 0x45, 0xC9,
    0x29, 0xF0, 0x41, 0xC8, 0xA2, 0xE0, 0x20, 0x31, 0xFE, 0x20, 0xC3, 0xFF,
    0x20, 0xC2, 0xFF, 0xC9, 0x22, 0xF0, 0xC3, 0xA2, 0xE2, 0x20, 0xBF, 0xFD,
    0xA5, 0xE2, 0xA0, 0x00, 0xA6, 0xE1, 0xD0, 0x12, 0xA6, 0xE0, 0xE0, 0x80,
    0xF0, 0x25, 0xE0, 0xC8, 0xF0, 0xCD, 0xE0, 0xFE, 0xF0, 0x20, 0xE0, 0xFC,
    0xF0, 0x13, 0x91, 0xE0, 0x65, 0xCF, 0x2A, 0xAA, 0xC8, 0xA5, 0xE3, 0x91,
    0xE0, 0x65, 0xCE, 0x2A, 0x85, 0xCF, 0x86, 0xCE, 0x60, 0xAA, 0xA5, 0xE3,
    0x6C, 0xC4, 0x00, 0x91, 0xF8, 0x60, 0xA2, 0xE2, 0xA5, 0xCA, 0x48, 0xA5,
    0xCB, 0x48, 0xA9, 0x00, 0x48, 0x95, 0x03, 0xA9, 0x0A, 0x95, 0x02, 0x20,
    0x04, 0xFF, 0xA5, 0xCA, 0x09, 0x30, 0x48, 0xB5, 0x00, 0x15, 0x01, 0xD0,
    0xF2, 0x68, 0x20, 0xEB, 0xFF, 0x68, 0xD0, 0xFA, 0x68, 0x85, 0xCB, 0x68,
    0x85, 0xCA, 0x60, 0xA9, 0x00, 0x95, 0x00, 0x95, 0x01, 0xA9, 0x2B, 0x48,
    0xE8, 0xE8, 0x20, 0xDE, 0xFD, 0xCA, 0xCA, 0x68, 0x20, 0x81, 0xFE, 0x20,
    0xC3, 0xFF, 0xF0, 0x05, 0xC8, 0xC9, 0x29, 0xD0, 0xEA, 0x60, 0x20, 0x2F,
    0xFF, 0xD0, 0x4D, 0x20, 0xC3, 0xFF, 0xC8, 0xC9, 0x3F, 0xD0, 0x17, 0x98,
    0x48, 0xA5, 0x80, 0x48, 0xA5, 0x81, 0x48, 0x20, 0x64, 0xFF, 0x20, 0xBF,
    0xFD, 0x68, 0x85, 0x81, 0x68, 0x85, 0x80, 0x68, 0xA8, 0x60, 0xC9, 0x24,
    0xD0, 0x05, 0x20, 0xDF, 0xFF, 0xB0, 0x23, 0xC9, 0x40, 0xD0, 0x0A, 0x84,
    0xC8, 0xA0, 0x00, 0xB1, 0xF8, 0xA4, 0xC8, 0xD0, 0x15, 0xC9, 0x28, 0xF0,
    0xA2, 0x20, 0x31, 0xFE, 0xA1, 0x00, 0x48, 0xF6, 0x00, 0xD0, 0x02, 0xF6,
    0x01, 0xA1, 0x00, 0x95, 0x01, 0x68, 0x95, 0x00, 0x60, 0xC9, 0x3A, 0xD0,
    0x11, 0x20, 0xBF, 0xFD, 0x16, 0x00, 0x36, 0x01, 0xA5, 0xCC, 0x95, 0x02,
    0xA5, 0xCD, 0x95, 0x03, 0xD0, 0x2D, 0x0A, 0x09, 0x80, 0x30, 0x72, 0xB5,
    0x00, 0x85, 0xFC, 0xB5, 0x01, 0x85, 0xFD, 0xA9, 0x00, 0x95, 0x00, 0x95,
    0x01, 0xA5, 0xFC, 0x05, 0xFD, 0xF0, 0x13, 0x46, 0xFD, 0x66, 0xFC, 0x90,
    0x03, 0x20, 0x73, 0xFE, 0x16, 0x02, 0x36, 0x03, 0xB5, 0x02, 0x15, 0x03,
    0xD0, 0xE7, 0x60, 0x18, 0xB5, 0x00, 0x75, 0x02, 0x95, 0x00, 0xB5, 0x01,
    0x75, 0x03, 0x95, 0x01, 0x60, 0xC9, 0x2B, 0xF0, 0xEE, 0xC9, 0x2A, 0xF0,
    0xC2, 0xC9, 0x2F, 0xF0, 0x77, 0xC9, 0x5B, 0xF0, 0x32, 0xC9, 0x5D, 0xF0,
    0x3C, 0xCA, 0xC9, 0x2D, 0xF0, 0x41, 0xC9, 0x7C, 0xF0, 0x52, 0xC9, 0x5E,
    0xF0, 0x58, 0xC9, 0x26, 0xF0, 0x40, 0x49, 0x3C, 0x85, 0xFC, 0x20, 0xDB,
    0xFE, 0xC6, 0xFC, 0xD0, 0x05, 0x15, 0x00, 0xF0, 0x04, 0x18, 0xA5, 0xFC,
    0x2A, 0x69, 0x00, 0x29, 0x01, 0x95, 0x00, 0xA9, 0x00, 0xF0, 0x20, 0xB5,
    0x00, 0x15, 0x01, 0xF0, 0x1C, 0xB5, 0x02, 0x95, 0x00, 0xB5, 0x03, 0xB0,
    0x12, 0xB5, 0x00, 0x15, 0x01, 0xF0, 0x9C, 0xA9, 0x00, 0xF0, 0xE2, 0x20,
    0xDF, 0xFE, 0xE8, 0xB5, 0x01, 0xF5, 0x03, 0x95, 0x01, 0x60, 0x20, 0xEA,
    0xFE, 0xE8, 0xB5, 0x01, 0x35, 0x03, 0xB0, 0xF3, 0x20, 0xF4, 0xFE, 0xE8,
    0xB5, 0x01, 0x15, 0x03, 0xB0, 0xE9, 0x20, 0xFE, 0xFE, 0xE8, 0xB5, 0x01,
    0x55, 0x03, 0xB0, 0xDF, 0xA9, 0x00, 0x85, 0xCA, 0x85, 0xCB, 0xA9, 0x10,
    0x85, 0xFC, 0x16, 0x00, 0x36, 0x01, 0x26, 0xCA, 0x26, 0xCB, 0xA5, 0xCA,
    0xD5, 0x02, 0xA5, 0xCB, 0xF5, 0x03, 0x90, 0x0A, 0x85, 0xCB, 0xA5, 0xCA,
    0xF5, 0x02, 0x85, 0xCA, 0xF6, 0x00, 0xC6, 0xFC, 0xD0, 0xE0, 0x60, 0xA9,
    0x00, 0x95, 0x00, 0x95, 0x01, 0x95, 0x03, 0x20, 0xC3, 0xFF, 0x84, 0xFE,
    0xB1, 0x80, 0x49, 0x30, 0xC9, 0x0A, 0xB0, 0x13, 0x48, 0xA9, 0x0A, 0x95,
    0x02, 0x20, 0x4B, 0xFE, 0x95, 0x03, 0x68, 0x95, 0x02, 0x20, 0x73, 0xFE,
    0xC8, 0x10, 0xE5, 0xC4, 0xFE, 0x60, 0xC9, 0x1B, 0xF0, 0x03, 0xC8, 0x10,
    0x10, 0x20, 0x39, 0xFD, 0xA0, 0x80, 0x84, 0x80, 0xA0, 0x17, 0x84, 0x81,
    0xA0, 0x01, 0x88, 0x30, 0xF0, 0x20, 0xDF, 0xFF, 0xC9, 0x08, 0xF0, 0xF6,
    0xC9, 0x0D, 0xD0, 0x02, 0xA9, 0x00, 0x91, 0x80, 0xD0, 0xD8, 0xA8, 0x60,
    0xA2, 0x02, 0xA9, 0x00, 0x90, 0x15, 0xA6, 0x81, 0xA0, 0x02, 0xA5, 0x80,
    0xC5, 0xCC, 0xA5, 0x81, 0xE5, 0xCD, 0xB0, 0x29, 0xA5, 0x80, 0x71, 0x80,
    0x90, 0x03, 0xE8, 0x86, 0x81, 0x85, 0x80, 0xA0, 0x00, 0xB1, 0x80, 0x85,
    0xD0, 0xC5, 0xC6, 0xC8, 0xB1, 0x80, 0x85, 0xD1, 0xE5, 0xC7, 0xC8, 0x90,
    0xD9, 0xA5, 0x80, 0xC5, 0xCC, 0xA5, 0x81, 0xE5, 0xCD, 0xA5, 0xD0, 0xA6,
    0xD1, 0x60, 0xC8, 0xB1, 0x80, 0xF0, 0x04, 0xC9, 0x20, 0xF0, 0xF7, 0x60,
    0xAD, 0xF5, 0xCF, 0xF0, 0x0D, 0xC9, 0x03, 0xF0, 0x13, 0xAD, 0xF5, 0xCF,
    0xF0, 0xFB, 0xC9, 0x03, 0xF0, 0x0A, 0x60, 0xAD, 0xF5, 0xCF, 0xF0, 0xFB,
    0xC9, 0x03, 0xD0, 0x03, 0x4C, 0x11, 0xFC, 0xC9, 0x0D, 0xD0, 0x07, 0xA9,
    0x0A, 0x8D, 0xF1, 0xCF, 0xA9, 0x0D, 0x8D, 0xF1, 0xCF, 0x38, 0x60

};

#endif

