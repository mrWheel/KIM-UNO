/*
* Hi-Lo program from The First Book Of KIM-1
* 
*   Coding style
*       - One True Brace Style formatting/indenting.
*       - Linux brackets, add brackets to all conditionals.
*/

#include "Config.h"

#if _DEMO_HILO == TRUE

    void loadDemoProgram() {  // Call this from main() if you want a program preloaded. 
    uint16_t i;
        
    // the Hi-Lo Program from The First Book of KIM-1
    uint8_t testDemo[110] = {  
/* OrgASM 6502 Assembler.      TASM 3.3 (Macintosh OS X) May 2015. */
/* 0000 */                     //  #define RND      $E0       
/* 0000 */                     //  #define NGUESS   $E1       
/* 0000 */                     //  #define LAST     $E2       
/* 0000 */                     //  #define INH      $F9       
/* 0000 */                     //  #define POINTL   $FA       
/* 0000 */                     //  #define POINTH   $FB       
/* 0000 */                     //         
/* 0000 */                     //  #define SCANDS   $1F1F       
/* 0000 */                     //  #define KEYIN    $1F40       
/* 0000 */                     //  #define GETKEY   $1F6A       
/* 0000 */                     //         
/* 0200 */                     //          .ORG    $0200       
/* 0200 */                     //         
/* 0200 */                     //         
/* 0200 */ 0xF8,               //  START:  SED       
/* 0201 */ 0xA5, 0xE0,         //  TOP:    LDA     RND             ; generate random #       
/* 0203 */ 0x38,               //          SEC                     ; 01 to 98       
/* 0204 */ 0x69, 0x00,         //          ADC     #0       
/* 0206 */ 0xA2, 0x01,         //          LDX     #1              ; overflow at 99       
/* 0208 */ 0xC9, 0x99,         //          CMP     #$99       
/* 020A */ 0xD0, 0x01,         //          BNE     OVR0       
/* 020C */ 0x8A,               //          TXA       
/* 020D */ 0x85, 0xE0,         //  OVR0:   STA     RND       
/* 020F */ 0x20, 0x40, 0x1F,   //          JSR     KEYIN       
/* 0212 */ 0xD0, 0xED,         //          BNE     TOP       
/* 0214 */ 0xD8,               //          CLD                     ; initialize:       
/* 0215 */ 0xA9, 0x99,         //          LDA     #$99            ; hi       
/* 0217 */ 0x85, 0xFB,         //          STA     POINTH       
/* 0219 */ 0xA9, 0x00,         //          LDA     #0       
/* 021B */ 0x85, 0xFA,         //          STA     POINTL          ; and lo       
/* 021D */ 0xA2, 0xA0,         //  RSET:   LDX     #$A0            ; guess counter       
/* 021F */ 0x86, 0xF9,         //  NSET:   STX     INH       
/* 0221 */ 0x86, 0xE1,         //          STX     NGUESS       
/* 0223 */ 0x20, 0x1F, 0x1F,   //  GUESS:  JSR     SCANDS          ; light display       
/* 0226 */ 0x20, 0x6A, 0x1F,   //          JSR     GETKEY          ; test key       
/* 0229 */ 0xC9, 0x13,         //          CMP     #$13            ; go key?       
/* 022B */ 0xF0, 0xD3,         //          BEQ     START       
/* 022D */ 0xC5, 0xE2,         //          CMP     LAST       
/* 022F */ 0xF0, 0xF2,         //          BEQ     GUESS           ; same key?       
/* 0231 */ 0x85, 0xE2,         //          STA     LAST       
/* 0233 */ 0xC9, 0x0A,         //          CMP     #$0A            ; 'A' key?       
/* 0235 */ 0xF0, 0x10,         //          BEQ     EVAL            ; yes, evaluate guess       
/* 0237 */ 0xB0, 0xEA,         //          BCS     GUESS           ; no key?       
/* 0239 */ 0x0A,               //          ASL     A               ; roll character       
/* 023A */ 0x0A,               //          ASL     A               ; ..into..       
/* 023B */ 0x0A,               //          ASL     A               ; position..       
/* 023C */ 0x0A,               //          ASL     A       
/* 023D */ 0xA2, 0x03,         //          LDX     #3       
/* 023F */ 0x0A,               //  LOOP:   ASL     A               ; ..then       
/* 0240 */ 0x26, 0xF9,         //          ROL     INH             ; ..into       
/* 0242 */ 0xCA,               //          DEX                     ; ..display       
/* 0243 */ 0x10, 0xFA,         //          BPL     LOOP       
/* 0245 */ 0x30, 0xDC,         //          BMI     GUESS       
/* 0247 */ 0xA5, 0xF9,         //  EVAL:   LDA     INH             ; guess lower..       
/* 0249 */ 0xC5, 0xE0,         //          CMP     RND             ; ..than number?       
/* 024B */ 0x90, 0x06,         //          BCC     OVR1            ; yes, skip       
/* 024D */ 0xC5, 0xFB,         //          CMP     POINTH          ; no, check hi       
/* 024F */ 0xB0, 0xD2,         //          BCS     GUESS           ; out of range?       
/* 0251 */ 0x85, 0xFB,         //          STA     POINTH       
/* 0253 */ 0xA6, 0xE0,         //  OVR1:   LDX     RND             ; number lower..       
/* 0255 */ 0xE4, 0xF9,         //          CPX     INH             ; ..than guess?       
/* 0257 */ 0x90, 0x08,         //          BCC     OVR2            ; yes, skip       
/* 0259 */ 0xA6, 0xFA,         //          LDX     POINTL          ; no,check lo       
/* 025B */ 0xE4, 0xF9,         //          CPX     INH       
/* 025D */ 0xB0, 0xC4,         //          BCS     GUESS           ; out of range?       
/* 025F */ 0x85, 0xFA,         //          STA     POINTL       
/* 0261 */ 0xA6, 0xE1,         //  OVR2:   LDX     NGUESS          ; 'guess' number       
/* 0263 */ 0xE8,               //          INX                     ; ..plus 1       
/* 0264 */ 0xE0, 0xAA,         //          CPX #$AA                ; past limit?       
/* 0266 */ 0xF0, 0xB5,         //          BEQ RSET                ; yes, reset       
/* 0268 */ 0xD0, 0xB5,         //          BNE NSET       
/* 026A */                     //      .END          
    };
    
        for (i=0;i<110;i++) {
          RAM[i+0x0200]=testDemo[i];
        }     
        
}       // testProgram()
#endif
