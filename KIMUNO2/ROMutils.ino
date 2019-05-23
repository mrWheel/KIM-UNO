/*
*   Coding style
*       - One True Brace Style formatting/indenting.
*       - Linux brackets, add brackets to all conditionals.
*/
// MOVIT utility, copied into RAM 1780-17E3. Length: decimal 102 
// for use, see http://users.telenet.be/kim1-6502/6502/fbok.html#p114 
// movit 0x1780

#if _TARGET == PERSONAL_COMPUTER
    unsigned char movitROM[95] = {
#else
    const unsigned char movitROM[100] PROGMEM = {
#endif
/* OrgASM 6502 Assembler.      TASM 3.3 (Macintosh OS X) May 2015. */
/* 0000 */                     //  #define OSAL      $D0       
/* 0000 */                     //  #define OSAH      $D1       
/* 0000 */                     //  #define OEAL      $D2       
/* 0000 */                     //  #define OEAH      $D3       
/* 0000 */                     //  #define NSAL      $D4       
/* 0000 */                     //  #define NSAH      $D5       
/* 0000 */                     //  #define NEAL      $D6       
/* 0000 */                     //  #define NEAH      $D7       
/* 0000 */                     //  #define BCL       $D8       
/* 0000 */                     //  #define BCH       $D9       
/* 0000 */                     //         
/* 1780 */                     //          .ORG    $1780       
/* 1780 */                     //         
/* 1780 */ 0xD8,               //  START:  CLD       
/* 1781 */ 0xA0, 0xFF,         //          LDY     #$FF            ; STORE TEST VALUE       
/* 1783 */ 0x38,               //          SEC       
/* 1784 */ 0xA5, 0xD2,         //          LDA     OEAL            ; HOW MANY BYTES?       
/* 1786 */ 0xE5, 0xD0,         //          SBC     OSAL            ; TO MOVE?       
/* 1788 */ 0x85, 0xD8,         //          STA     BCL       
/* 178A */ 0xA5, 0xD3,         //          LDA     OEAH       
/* 178C */ 0xE5, 0xD1,         //          SBC     OSAH       
/* 178E */ 0x85, 0xD9,         //          STA     BCH       
/* 1790 */ 0x18,               //          CLC       
/* 1791 */ 0xA5, 0xD8,         //          LDA     BCL             ; ADD THE COUNT TO       
/* 1793 */ 0x65, 0xD4,         //          ADC     NSAL            ; THE NEW START TO       
/* 1795 */ 0x85, 0xD6,         //          STA     NEAL            ; GET A NEW END       
/* 1797 */ 0xA5, 0xD9,         //          LDA     BCH       
/* 1799 */ 0x65, 0xD5,         //          ADC     NSAH       
/* 179B */ 0x85, 0xD7,         //          STA     NEAH       
/* 179D */ 0xE6, 0xD8,         //          INC     BCL             ; ADJUST THE BYTE COUNT       
/* 179F */ 0xE6, 0xD9,         //          INC     BCH             ; TO PERMIT ZERO TESTING       
/* 17A1 */ 0x38,               //          SEC       
/* 17A2 */ 0xA5, 0xD4,         //          LDA     NSAL            ; IF NEW LOCATION       
/* 17A4 */ 0xE5, 0xD0,         //          SBC     OSAL            ; HIGHER THAN OLD       
/* 17A6 */ 0xA5, 0xD5,         //          LDA     NSAH            ; CARRY FLAG IS SET       
/* 17A8 */ 0xE5, 0xD1,         //          SBC     OSAH       
/* 17AA */ 0xA2, 0x00,         //  LOOP:   LDX     #$00            ; HIGH POINTER INDEX       
/* 17AC */ 0x90, 0x02,         //          BCC     MOVE       
/* 17AE */ 0xA2, 0x02,         //          LDX     #$02            ; LOW POINTER INDEX       
/* 17B0 */ 0xA1, 0xD0,         //  MOVE:   LDA     (OSAL,X)        ; MOVE OLD. NOTE ERROR IN PRINTED LISTING       
/* 17B2 */ 0x81, 0xD4,         //          STA     (NSAL,X)        ; TO NEW. NOTE ERROR IN PRINTED LISTING       
/* 17B4 */ 0x90, 0x14,         //          BCC     DOWN       
/* 17B6 */ 0xC6, 0xD2,         //          DEC     OEAL            ; ADJUST UP POINTER, (OLD)       
/* 17B8 */ 0x98,               //          TYA                     ; BELOW ZERO?       
/* 17B9 */ 0x45, 0xD2,         //          EOR     OEAL       
/* 17BB */ 0xD0, 0x02,         //          BNE     NOT             ; NO, ENOUGH       
/* 17BD */ 0xC6, 0xD3,         //          DEC     OEAH            ; YES, ADJUST THE HIGH BYTE       
/* 17BF */ 0xC6, 0xD6,         //  NOT:    DEC     NEAL            ; ADJUST THE OTHER ONE (NEW)       
/* 17C1 */ 0x98,               //          TYA       
/* 17C2 */ 0x45, 0xD6,         //          EOR     NEAL            ; NEED HIGH BYTE ADJUSTED?       
/* 17C4 */ 0xD0, 0x02,         //          BNE     NEIN            ; NO       
/* 17C6 */ 0xC6, 0xD7,         //          DEC     NEAH            ; YES, DO IT       
/* 17C8 */ 0xB0, 0x0C,         //  NEIN:   BCS     COUNT       
/* 17CA */ 0xE6, 0xD0,         //  DOWN:   INC     OSAL            ; ADJUST "OLD" DOWN POINTER       
/* 17CC */ 0xD0, 0x02,         //          BNE     NYET       
/* 17CE */ 0xE6, 0xD1,         //          INC     OSAH            ; AND THE HIGH BYTE IF NEEDED       
/* 17D0 */ 0xE6, 0xD4,         //  NYET:   INC     NSAL            ; AND THE "NEW" ONE       
/* 17D2 */ 0xD0, 0x02,         //          BNE     COUNT       
/* 17D4 */ 0xE6, 0xD5,         //          INC     NSAH       
/* 17D6 */ 0xC6, 0xD8,         //  COUNT:  DEC     BCL             ; TICK OFF THE BYTES,       
/* 17D8 */ 0xD0, 0x02,         //          BNE     ONE             ; ENOUGH FINGERS?       
/* 17DA */ 0xC6, 0xD9,         //          DEC     BCH             ; USE THE OTHER HAND       
/* 17DC */ 0xD0, 0xCC,         //  ONE:    BNE     LOOP            ; 'TIL THEY'RE ALL DONE       
/* 17DE */ 0x00,               //  DONE:   BRK                     ; & BACK TO MONITOR       
/* 17DF */                     //         
/* 17DF */                     //          .END       

};

/* RELOCATE utility, copied into RAM 0110-01A4. Length: decimal 149 */
/* for use, see http://users.telenet.be/kim1-6502/6502/fbok.html#p114 */
#if _TARGET == PERSONAL_COMPUTER
    unsigned char relocateROM[149] = {
#else
    const unsigned char relocateROM[149] PROGMEM = {
#endif
/* OrgASM 6502 Assembler.      TASM 3.3 (Macintosh OS X) May 2015. */       
/* 0000 */                     //  ;       
/* 0000 */                     //  ; following addresses must be initialized       
/* 0000 */                     //  ; by user prior to run       
/* 0000 */                     //         
/* 0000 */                     //  #define ALOC      $E0       
/* 0000 */                     //  #define LIMIT     $E3       
/* 0000 */                     //  #define PAGLIM    $E7           ; limit above which kill relocn       
/* 0000 */                     //  #define ADJST     $E8           ; adjustment distance (signed)       
/* 0000 */                     //  #define POINT     $EA           ; start of program       
/* 0000 */                     //  #define BOUND     $EC           ; lower boundary for adjustment       
/* 0000 */                     //         
/* 0000 */                     //  ; main program starts here       
/* 0000 */                     //         
/* 0110 */                     //         .ORG    $0110       
/* 0110 */                     //         
/* 0110 */ 0xD8,               //  START:  CLD       
/* 0111 */ 0xA0, 0x00,         //          LDY     #0       
/* 0113 */ 0xB1, 0xEA,         //          LDA     (POINT),Y       ; get op code       
/* 0115 */ 0xA8,               //          TAY                     ; +cache in Y       
/* 0116 */ 0xA2, 0x07,         //          LDX     #7       
/* 0118 */ 0x98,               //  LOOP:   TYA                     ; restore op code       
/* 0119 */ 0x3D, 0x8E, 0x01,   //          AND     TAB1-1,X        ; remove unwanted bits       
/* 011C */ 0x5D, 0x95, 0x01,   //          EOR     TAB2-1,X        ; & test the rest       
/* 011F */ 0xF0, 0x03,         //          BEQ     FOUND       
/* 0121 */ 0xCA,               //          DEX                     ; on to the next test       
/* 0122 */ 0xD0, 0xF4,         //          BNE     LOOP            ; ...if any       
/* 0124 */ 0xBC, 0x9D, 0x01,   //  FOUND:  LDY     TAB3,X          ; length or flag       
/* 0127 */ 0x30, 0x0D,         //          BMI     TRIP            ; triple length?       
/* 0129 */ 0xF0, 0x22,         //          BEQ     BRAN            ; branch?       
/* 012B */ 0xE6, 0xEA,         //  SKIP:   INC     POINT           ; moving right along..       
/* 012D */ 0xD0, 0x02,         //          BNE     INEX            ; ..to next op code       
/* 012F */ 0xE6, 0xEB,         //          INC     POINT+1       
/* 0131 */ 0x88,               //  INEX:   DEY       
/* 0132 */ 0xD0, 0xF7,         //          BNE     SKIP       
/* 0134 */ 0xF0, 0xDA,         //          BEQ     START       
/* 0136 */                     //         
/* 0136 */                     //  ; length 3 or illegal       
/* 0136 */                     //         
/* 0136 */ 0xC8,               //  TRIP:   INY       
/* 0137 */ 0x30, 0xD9,         //          BMI     START+2         ; illegal/end to BRK halt       
/* 0139 */ 0xC8,               //          INY                     ; set Y to 1       
/* 013A */ 0xB1, 0xEA,         //          LDA     (POINT),Y       ; lo-order operand       
/* 013C */ 0xAA,               //          TAX                     ; ...into X reg       
/* 013D */ 0xC8,               //          INY                     ; Y=2       
/* 013E */ 0xB1, 0xEA,         //          LDA     (POINT),Y       ; hi-order operand       
/* 0140 */ 0x20, 0x79, 0x01,   //          JSR     ADJUST          ; change address, maybe       
/* 0143 */ 0x91, 0xEA,         //          STA     (POINT),Y       ; ...and put it back       
/* 0145 */ 0x88,               //          DEY                     ; Y=1       
/* 0146 */ 0x8A,               //          TXA       
/* 0147 */ 0x91, 0xEA,         //          STA     (POINT),Y       ; ...also hi-order       
/* 0149 */ 0xA0, 0x03,         //          LDY     #3              ; Y=3       
/* 014B */ 0x10, 0xDE,         //          BPL     SKIP       
/* 014D */                     //         
/* 014D */                     //  ; branch: check "to" and "from" address       
/* 014D */                     //         
/* 014D */ 0xC8,               //  BRAN:   INY                     ; Y=1       
/* 014E */ 0xA6, 0xEA,         //          LDX     POINT           ; "from" addrs lo-order       
/* 0150 */ 0xA5, 0xEB,         //          LDA     POINT+1         ; ...& hi-order       
/* 0152 */ 0x20, 0x79, 0x01,   //          JSR     ADJUST          ; change, maybe       
/* 0155 */ 0x86, 0xE0,         //          STX     ALOC            ; save lo-order only       
/* 0157 */ 0xA2, 0xFF,         //          LDX     #$FF            ; flag for "back" branches       
/* 0159 */ 0xB1, 0xEA,         //          LDA     (POINT),Y       ; get relative branch       
/* 015B */ 0x18,               //          CLC       
/* 015C */ 0x69, 0x02,         //          ADC     #2              ; adjust the offset       
/* 015E */ 0x30, 0x01,         //          BMI     OVER            ; backwards branch?       
/* 0160 */ 0xE8,               //          INX                     ; nope       
/* 0161 */ 0x86, 0xE3,         //  OVER:   STX     LIMIT       
/* 0163 */ 0x18,               //          CLC       
/* 0164 */ 0x65, 0xEA,         //          ADC     POINT           ; calculate "to" lo-order       
/* 0166 */ 0xAA,               //          TAX                     ; ...and put in X       
/* 0167 */ 0xA5, 0xE3,         //          LDA     LIMIT           ; 00 or FF       
/* 0169 */ 0x65, 0xEB,         //          ADC     POINT+1         ; "to" hi-order       
/* 016B */ 0x20, 0x79, 0x01,   //          JSR     ADJUST          ; change, maybe       
/* 016E */ 0xCA,               //          DEX                     ; readjust the offset       
/* 016F */ 0xCA,               //          DEX       
/* 0170 */ 0x8A,               //          TXA       
/* 0171 */ 0x38,               //          SEC       
/* 0172 */ 0xE5, 0xE0,         //          SBC     ALOC            ; recalculate relative branch       
/* 0174 */ 0x91, 0xEA,         //          STA     (POINT),Y       ; and re-insert       
/* 0176 */ 0xC8,               //          INY                     ; Y=2       
/* 0177 */ 0x10, 0xB2,         //          BPL     SKIP       
/* 0179 */                     //         
/* 0179 */                     //  ; examine address and adjust, maybe       
/* 0179 */                     //         
/* 0179 */ 0xC5, 0xE7,         //  ADJUST: CMP     PAGLIM       
/* 017B */ 0xB0, 0x11,         //          BCS     OUT             ; too high?       
/* 017D */ 0xC5, 0xED,         //          CMP     BOUND+1       
/* 017F */ 0xD0, 0x02,         //          BNE     TES2            ; hi-order?       
/* 0181 */ 0xE4, 0xEC,         //          CPX     BOUND           ; lo-order?       
/* 0183 */ 0x90, 0x09,         //  TES2:   BCC     OUT             ; too low?       
/* 0185 */ 0x48,               //          PHA                     ; stack hi-order       
/* 0186 */ 0x8A,               //          TXA       
/* 0187 */ 0x18,               //          CLC       
/* 0188 */ 0x65, 0xE8,         //          ADC     ADJST           ; adjust lo-order       
/* 018A */ 0xAA,               //          TAX       
/* 018B */ 0x68,               //          PLA                     ; unstack hi-order       
/* 018C */ 0x65, 0xE9,         //          ADC     ADJST+1         ; and adjust       
/* 018E */ 0x60,               //  OUT:    RTS       
/* 018F */                     //         
/* 018F */                     //  ; tables for op-code indentification       
/* 018F */                     //         
/* 018F */ 0x0C, 0x1F, 0x0D, 0x87,  // TAB1: .BYTE $0C, $1F, $0D, $87, $1F, $FF, $03
/* 0193 */ 0x1F, 0xFF, 0x03,   //      
/* 0196 */ 0x0C, 0x19, 0x08, 0x00,  // TAB2: .BYTE $0C, $19, $08, $00, $10, $20, $03
/* 019A */ 0x10, 0x20, 0x03,   //      
/* 019D */ 0x02, 0xFF, 0xFF, 0x01,  // TAB3: .BYTE $02, $FF, $FF, $01, $01, $00, $FF, $FE
/* 01A1 */ 0x01, 0x00, 0xFF, 0xFE,  // 
/* 01A5 */                     //         
/* 01A5 */                     //      .END       
};

/* BRANCH calculation utility, to be copied into RAM anywhere you want (relocatable). Length: decimal 42 */
/* for use, see http://users.telenet.be/kim1-6502/6502/fbok.html#p114 */
#if _TARGET == PERSONAL_COMPUTER
    unsigned char branchROM[42] = {
#else
    const unsigned char branchROM[42] PROGMEM = {
#endif
/* OrgASM 6502 Assembler.      TASM 3.3 (Macintosh OS X) May 2015. */       
/* 0000 */                     //  ;       
/* 0000 */                     //  ;  Branche from the First Book Of KIM-1       
/* 0000 */                     //  ;       
/* 0000 */                     //  #define LAST      $F3       
/* 0000 */                     //  #define INH       $F9       
/* 0000 */                     //  #define POINTL    $FA       
/* 0000 */                     //  #define POINTH    $FB       
/* 0000 */                     //         
/* 0000 */                     //  #define SCANDS    $1F1F       
/* 0000 */                     //  #define GETKEY    $1F6A       
/* 0000 */                     //         
/* 0000 */                     //          .ORG    $0000       
/* 0000 */                     //         
/* 0000 */ 0xD8,               //  START:  CLD       
/* 0001 */ 0x18,               //          CLC       
/* 0002 */ 0xA5, 0xFA,         //          LDA     POINTL       
/* 0004 */ 0xE5, 0xFB,         //          SBC     POINTH       
/* 0006 */ 0x85, 0xF9,         //          STA     INH       
/* 0008 */ 0xC6, 0xF9,         //          DEC     INH       
/* 000A */ 0x20, 0x1F, 0x1F,   //          JSR     SCANDS       
/* 000D */ 0x20, 0x6A, 0x1F,   //          JSR     GETKEY       
/* 0010 */ 0xC5, 0xF3,         //          CMP     LAST       
/* 0012 */ 0xF0, 0xEC,         //          BEQ     START       
/* 0014 */ 0x85, 0xF3,         //          STA     LAST       
/* 0016 */ 0xC9, 0x10,         //          CMP     #$10       
/* 0018 */ 0xB0, 0xE6,         //          BCS     START       
/* 001A */ 0x0A,               //          ASL     A       
/* 001B */ 0x0A,               //          ASL     A       
/* 001C */ 0x0A,               //          ASL     A       
/* 001D */ 0x0A,               //          ASL     A       
/* 001E */ 0xA2, 0x04,         //          LDX     #4       
/* 0020 */ 0x0A,               //  ADDR:   ASL     A       
/* 0021 */ 0x26, 0xFA,         //          ROL     POINTL       
/* 0023 */ 0x26, 0xFB,         //          ROL     POINTH       
/* 0025 */ 0xCA,               //          DEX       
/* 0026 */ 0xD0, 0xF8,         //          BNE     ADDR       
/* 0028 */ 0xF0, 0xD6,         //          BEQ     START       
/* 002A */                     //         
/* 002A */                     //      .END     
};

/* C:\temp27\KIM Uno\sw\tools\WozBaum disasm\WozBaum disasm\dis2.bin (02/09/2014 23:58:36)
   StartOffset: 00000000, EndOffset: 000001F8, Length: 000001F9 */
#if _TARGET == PERSONAL_COMPUTER
    unsigned char disasmROM[505] = {
#else
    const unsigned char disasmROM[505] PROGMEM = {
#endif
    0x20, 0x0F, 0x20, 0x20, 0x9E, 0x1E, 0x20, 0x9E, 0x1E, 0x20, 0x9E, 0x1E,
    0x4C, 0x64, 0x1C, 0xA9, 0x0D, 0x85, 0x02, 0x20, 0x21, 0x20, 0x20, 0xFC,
    0x20, 0x85, 0x00, 0x84, 0x01, 0xC6, 0x02, 0xD0, 0xF2, 0x20, 0xE2, 0x20,
    0xA1, 0x00, 0xA8, 0x4A, 0x90, 0x0B, 0x4A, 0xB0, 0x17, 0xC9, 0x22, 0xF0,
    0x13, 0x29, 0x07, 0x09, 0x80, 0x4A, 0xAA, 0xBD, 0x1B, 0x21, 0xB0, 0x04,
    0x4A, 0x4A, 0x4A, 0x4A, 0x29, 0x0F, 0xD0, 0x04, 0xA0, 0x80, 0xA9, 0x00,
    0xAA, 0xBD, 0x5F, 0x21, 0x85, 0x03, 0x29, 0x03, 0x85, 0x04, 0x98, 0x29,
    0x8F, 0xAA, 0x98, 0xA0, 0x03, 0xE0, 0x8A, 0xF0, 0x0B, 0x4A, 0x90, 0x08,
    0x4A, 0x4A, 0x09, 0x20, 0x88, 0xD0, 0xFA, 0xC8, 0x88, 0xD0, 0xF2, 0x48,
    0xB1, 0x00, 0x20, 0x13, 0x21, 0xA2, 0x01, 0x20, 0xF3, 0x20, 0xC4, 0x04,
    0xC8, 0x90, 0xF1, 0xA2, 0x03, 0xC0, 0x04, 0x90, 0xF2, 0x68, 0xA8, 0xB9,
    0x79, 0x21, 0x85, 0x05, 0xB9, 0xB9, 0x21, 0x85, 0x06, 0xA9, 0x00, 0xA0,
    0x05, 0x06, 0x06, 0x26, 0x05, 0x2A, 0x88, 0xD0, 0xF8, 0x69, 0x3F, 0x20,
    0x0B, 0x21, 0xCA, 0xD0, 0xEC, 0x20, 0xF1, 0x20, 0xA2, 0x06, 0xE0, 0x03,
    0xD0, 0x12, 0xA4, 0x04, 0xF0, 0x0E, 0xA5, 0x03, 0xC9, 0xE8, 0xB1, 0x00,
    0xB0, 0x1C, 0x20, 0x13, 0x21, 0x88, 0xD0, 0xF2, 0x06, 0x03, 0x90, 0x0E,
    0xBD, 0x6C, 0x21, 0x20, 0x0B, 0x21, 0xBD, 0x72, 0x21, 0xF0, 0x03, 0x20,
    0x0B, 0x21, 0xCA, 0xD0, 0xD5, 0x60, 0x20, 0xFF, 0x20, 0xAA, 0xE8, 0xD0,
    0x01, 0xC8, 0x98, 0x20, 0x13, 0x21, 0x8A, 0x4C, 0x13, 0x21, 0x20, 0x2F,
    0x1E, 0xA5, 0x01, 0xA6, 0x00, 0x20, 0xDB, 0x20, 0xA9, 0x2D, 0x20, 0x0B,
    0x21, 0xA2, 0x03, 0xA9, 0x20, 0x20, 0x0B, 0x21, 0xCA, 0xD0, 0xF8, 0x60,
    0xA5, 0x04, 0x38, 0xA4, 0x01, 0xAA, 0x10, 0x01, 0x88, 0x65, 0x00, 0x90,
    0x01, 0xC8, 0x60, 0x84, 0x07, 0x20, 0xA0, 0x1E, 0xA4, 0x07, 0x60, 0x84,
    0x07, 0x20, 0x3B, 0x1E, 0xA4, 0x07, 0x60, 0x40, 0x02, 0x45, 0x03, 0xD0,
    0x08, 0x40, 0x09, 0x30, 0x22, 0x45, 0x33, 0xD0, 0x08, 0x40, 0x09, 0x40,
    0x02, 0x45, 0x33, 0xD0, 0x08, 0x40, 0x09, 0x40, 0x02, 0x45, 0xB3, 0xD0,
    0x08, 0x40, 0x09, 0x00, 0x22, 0x44, 0x33, 0xD0, 0x8C, 0x44, 0x00, 0x11,
    0x22, 0x44, 0x33, 0xD0, 0x8C, 0x44, 0x9A, 0x10, 0x22, 0x44, 0x33, 0xD0,
    0x08, 0x40, 0x09, 0x10, 0x22, 0x44, 0x33, 0xD0, 0x08, 0x40, 0x09, 0x62,
    0x13, 0x78, 0xA9, 0x00, 0x21, 0x01, 0x02, 0x00, 0x80, 0x59, 0x4D, 0x11,
    0x12, 0x06, 0x4A, 0x05, 0x1D, 0x2C, 0x29, 0x2C, 0x23, 0x28, 0x41, 0x59,
    0x00, 0x58, 0x00, 0x00, 0x00, 0x1C, 0x8A, 0x1C, 0x23, 0x5D, 0x8B, 0x1B,
    0xA1, 0x9D, 0x8A, 0x1D, 0x23, 0x9D, 0x8B, 0x1D, 0xA1, 0x00, 0x29, 0x19,
    0xAE, 0x69, 0xA8, 0x19, 0x23, 0x24, 0x53, 0x1B, 0x23, 0x24, 0x53, 0x19,
    0xA1, 0x00, 0x1A, 0x5B, 0x5B, 0xA5, 0x69, 0x24, 0x24, 0xAE, 0xAE, 0xA8,
    0xAD, 0x29, 0x00, 0x7C, 0x00, 0x15, 0x9C, 0x6D, 0x9C, 0xA5, 0x69, 0x29,
    0x53, 0x84, 0x13, 0x34, 0x11, 0xA5, 0x69, 0x23, 0xA0, 0xD8, 0x62, 0x5A,
    0x48, 0x26, 0x62, 0x94, 0x88, 0x54, 0x44, 0xC8, 0x54, 0x68, 0x44, 0xE8,
    0x94, 0x00, 0xB4, 0x08, 0x84, 0x74, 0xB4, 0x28, 0x6E, 0x74, 0xF4, 0xCC,
    0x4A, 0x72, 0xF2, 0xA4, 0x8A, 0x00, 0xAA, 0xA2, 0xA2, 0x74, 0x74, 0x74,
    0x72, 0x44, 0x68, 0xB2, 0x32, 0xB2, 0x00, 0x22, 0x00, 0x1A, 0x1A, 0x26,
    0x26, 0x72, 0x72, 0x88, 0xC8, 0xC4, 0xCA, 0x26, 0x48, 0x44, 0x44, 0xA2,
    0xC8
};

