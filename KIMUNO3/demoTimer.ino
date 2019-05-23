/*
* Timer program from The First Book Of KIM-1
* 
*   Coding style
*       - One True Brace Style formatting/indenting.
*       - Linux brackets, add brackets to all conditionals.
*/

#include "Config.h"

#if _DEMO_TIMER == TRUE

    void loadDemoProgram() {  // Call this from main() if you want a program preloaded. 
    uint16_t i;
        
    // the Timer Program from The First Book of KIM-1
    uint8_t testDemo[100] = {  
/* OrgASM 6502 Assembler.      TASM 3.3 (Macintosh OS X) May 2015. */
/* 0000 */                     //  ;       
/* 0000 */                     //  ;    The First Book of KIM-1       
/* 0000 */                     //  ;    Timer program on page 106       
/* 0000 */                     //  ;       
/* 0000 */                     //  #define INH       $F9       
/* 0000 */                     //  #define POINTL    $FA       
/* 0000 */                     //  #define POINTH    $FB       
/* 0000 */                     //         
/* 0000 */                     //  #define SCANDS    $1F1F       
/* 0000 */                     //  #define GETKEY    $1F6A       
/* 0000 */                     //  ;       ROM       $1C00       
/* 0000 */                     //  #define ROM       $0003       
/* 0000 */                     //         
/* 0200 */                     //         .ORG    $0200       
/* 0200 */                     //         
/* 0200 */ 0xA9, 0x00,         //  BEGN:   LDA     #$00       
/* 0202 */ 0x85, 0xF9,         //          STA     INH             ; ZERO DISPLAY       
/* 0204 */ 0x85, 0xFA,         //          STA     POINTL       
/* 0206 */ 0x85, 0xFB,         //          STA     POINTH       
/* 0208 */ 0x20, 0x1F, 0x1F,   //  HOLD:   JSR     SCANDS          ; LIGHT DISPLAY       
/* 020B */ 0x20, 0x6A, 0x1F,   //          JSR     GETKEY       
/* 020E */ 0xC9, 0x04,         //          CMP     #$04            ; KEY 4?       
/* 0210 */ 0xD0, 0x01,         //          BNE     CONT       
/* 0212 */                     //  ;---    JMP     $1C64           ; RETURN TO KIM       
/* 0212 */ 0x00,               //          BRK                     ; RETURN TO KIM       
/* 0213 */ 0xC9, 0x02,         //  CONT:   CMP     #$02            ; KEY 2?       
/* 0215 */ 0xF0, 0xE9,         //          BEQ     BEGN            ; BACK TO ZERO       
/* 0217 */ 0xC9, 0x01,         //          CMP     #$01            ; KEY 1?       
/* 0219 */ 0xD0, 0xED,         //          BNE     HOLD       
/* 021B */ 0xA9, 0x40,         //          LDA     #$9C       ;AaW
/* 021D */ 0x8D, 0x06, 0x17,   //          STA     $1706           ; SET TIMER       
/* 0220 */ 0x20, 0x1F, 0x1F,   //  DISP:   JSR     SCANDS          ; DISPLAY VALUE       
/* 0223 */ 0xAD, 0x07, 0x17,   //  CLCK:   LDA     $1707           ; CHECK TIMER       
/* 0226 */ 0xF0, 0xFB,         //          BEQ     CLCK       
/* 0228 */ 0x85, 0x03,         //          STA     ROM             ; DELAY 4 MICROSEC.       
/* 022A */ 0xA9, 0x40,         //          LDA     #$9C       ;AaW        
/* 022C */ 0x8D, 0x06, 0x17,   //          STA     $1706           ; SET TIMER 
/* 022F */ 0x18,               //          CLC       
/* 0230 */ 0xF8,               //          SED                     ; SET FLAGS       
/* 0231 */ 0xA5, 0xF9,         //          LDA     INH       
/* 0233 */ 0x69, 0x01,         //          ADC     #$01            ; INC. 100THS       
/* 0235 */ 0x85, 0xF9,         //          STA     INH       
/* 0237 */ 0xA5, 0xFA,         //          LDA     POINTL       
/* 0239 */ 0x69, 0x00,         //          ADC     #$00            ; INC. SECONDS       
/* 023B */ 0x85, 0xFA,         //          STA     POINTL       
/* 023D */ 0xC9, 0x60,         //          CMP     #$60            ; STOP AT 60       
/* 023F */ 0xD0, 0x0B,         //          BNE     CKEY       
/* 0241 */ 0xA9, 0x00,         //          LDA     #$00       
/* 0243 */ 0x85, 0xFA,         //          STA     POINTL          ; ZERO SECONDS       
/* 0245 */ 0xA5, 0xFB,         //          LDA     POINTH       
/* 0247 */ 0x18,               //          CLC       
/* 0248 */ 0x69, 0x01,         //          ADC     #$01            ; INC. MINUTES       
/* 024A */ 0x85, 0xFB,         //          STA     POINTH       
/* 024C */ 0xD8,               //  CKEY:   CLD       
/* 024D */ 0x20, 0x6A, 0x1F,   //          JSR     GETKEY          ; READ KEYBOARD       
/* 0250 */ 0xC9, 0x00,         //          CMP     #$00            ; KEY 0?       
/* 0252 */ 0xD0, 0xCC,         //          BNE     DISP       
/* 0254 */ 0xF0, 0xB2,         //          BEQ     HOLD            ; STOP       
/* 0256 */                     //         .end       
       };
        for (i=0;i<100;i++) {
          RAM[i+0x0200]=testDemo[i];
        }     
        
}       // testProgram()

#endif
