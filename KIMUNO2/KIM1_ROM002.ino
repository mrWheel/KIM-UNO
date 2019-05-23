// --- ROM 002 CODE SECTION ---------------------------------------------------
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
    unsigned char ROM002[1024] = {
#else
    const unsigned char ROM002[1024] PROGMEM = {
#endif
/* OrgASM 6502 Assembler.      TASM 3.3 (Macintosh OS X) May 2015. */
/* 0000 */                     //  ;       
/* 0000 */                     //  ;       ** INTERRUPTS **       
/* 0000 */                     //          .org  $1C00       
/* 1C00 */                     //  ;       
/* 1C00 */                     //  ;******************* 6530-002 I.C. *****************       
/* 1C00 */                     //  ;       ** COPYRIGHT MOS TECHNOLOGY INC.       
/* 1C00 */                     //  ;          DATE OCT 13, 1975 REV E       
/* 1C00 */                     //  ;       
/* 1C00 */                     //  ;       ** KIM **       
/* 1C00 */                     //  ;         TTY INTERFACE   6530-002       
/* 1C00 */                     //  ;         KEYBOARD INTERFACE,       
/* 1C00 */                     //  ;         7-SEGMENT 6-DIGIT DISPLAY       
/* 1C00 */                     //  ;       
/* 1C00 */                     //  ;       TTY COMANDS:       
/* 1C00 */                     //  ;         G    GOEXEC       
/* 1C00 */                     //  ;         CR   OPEN NEXT CELL       
/* 1C00 */                     //  ;         LF   OPEN PREVIOUS CELL       
/* 1C00 */                     //  ;         .    MODIFY OPEN CELL       
/* 1C00 */                     //  ;         SP   OPEN NEW CELL       
/* 1C00 */                     //  ;         L    LOAD (OBJECT FORMAT)       
/* 1C00 */                     //  ;         Q    DUMP FROM OPEN CELL ADDR TO HI LIMIT       
/* 1C00 */                     //  ;         RO   RUB OUT - RETURN TO START KIM       
/* 1C00 */                     //  ;               (ALL ILLEGAL CHARS ARE IGNORED)       
/* 1C00 */                     //  ;       
/* 1C00 */                     //  ;       KEYBOARD COMMANDS:       
/* 1C00 */                     //  ;         ADDR  SETS MODE TO MODIFY CELL ADDRESS       
/* 1C00 */                     //  ;         DATA  SETS MODE TO MODIFY DATA IN OPEN CELL       
/* 1C00 */                     //  ;         STEP  INCREMENTS TO NEXT CELL       
/* 1C00 */                     //  ;         RST   SYSTEM RESET       
/* 1C00 */                     //  ;         RUN   GOEXEC       
/* 1C00 */                     //  ;         STOP  $1C00 CAN BE LOADED INTO NMIV TO USE       
/* 1C00 */                     //  ;         PC    DISPLAY PC (PROGRAM COUNTER)       
/* 1C00 */                     //  ;                
/* 1C00 */                     //          .org  $1C00       
/* 1C00 */ 0x85, 0xF3,         //  SAVE    STA   ACC       ; KIM ENTRY VIA STOP (NMI)      1C00       
/* 1C02 */ 0x68,               //          PLA             ; OR BRK (IRQ)       
/* 1C03 */ 0x85, 0xF1,         //          STA   PREG       
/* 1C05 */ 0x68,               //          PLA             ; KIM ENTRY VIA JSR (A LOST)    1C05       
/* 1C06 */ 0x85, 0xEF,         //          STA   PCL       
/* 1C08 */ 0x85, 0xFA,         //          STA   POINTL       
/* 1C0A */ 0x68,               //          PLA                
/* 1C0B */ 0x85, 0xF0,         //          STA   PCH       
/* 1C0D */ 0x85, 0xFB,         //          STA   POINTH       
/* 1C0F */ 0x84, 0xF4,         //          STY   YREG       
/* 1C11 */ 0x86, 0xF5,         //          STX   XREG       
/* 1C13 */ 0xBA,               //          TSX          
/* 1C14 */ 0x86, 0xF2,         //          STX   SPUSER       
/* 1C16 */ 0x20, 0x88, 0x1E,   //          JSR   INITS       
/* 1C19 */ 0x4C, 0x4F, 0x1C,   //          JMP   START       
/* 1C1C */                     //  ;       
/* 1C1C */ 0x6C, 0xFA, 0x17,   //  NMIT    JMP   (NMIV)    ; NON-MASKABLE INTERRUPT TRAP   1C1C       
/* 1C1F */ 0x6C, 0xFE, 0x17,   //  IRQT    JMP   (IRQV)    ; INTERRUPT TRAP                1C1F       
/* 1C22 */ 0xA2, 0xFF,         //  RST     LDX   #$FF      ; KIM ENTRY VIA RST             1C22       
/* 1C24 */ 0x9A,               //          TXS       
/* 1C25 */ 0x86, 0xF2,         //          STX   SPUSER       
/* 1C27 */ 0x20, 0x88, 0x1E,   //          JSR   INITS       
/* 1C2A */ 0xA9, 0xFF,         //          LDA   #$FF      ; COUNT START BIT       
/* 1C2C */ 0x8D, 0xF3, 0x17,   //          STA   CNTH30    ; ZERO CNTH30       
/* 1C2F */ 0xA9, 0x01,         //          LDA   #$01      ; MASK HI ORDER BITS       
/* 1C31 */ 0x2C, 0x40, 0x17,   //  DET1    BIT   SAD       ; TEST                          1C31       
/* 1C34 */ 0xD0, 0x19,         //          BNE   START     ; KEYBD SSW TEST       
/* 1C36 */ 0x30, 0xF9,         //          BMI   DET1      ; START BIT TEST       
/* 1C38 */ 0xA9, 0xFC,         //          LDA   #$FC       
/* 1C3A */ 0x18,               //  DET3    CLC             ; THIS LOOP COUNTS              1C3A       
/* 1C3B */ 0x69, 0x01,         //          ADC   #$01      ; THE START BIT TIME       
/* 1C3D */ 0x90, 0x03,         //          BCC   DET2       
/* 1C3F */ 0xEE, 0xF3, 0x17,   //          INC   CNTH30       
/* 1C42 */ 0xAC, 0x40, 0x17,   //  DET2    LDY   SAD       ; CHECK FOR END OF START BIT    1C42       
/* 1C45 */ 0x10, 0xF3,         //          BPL   DET3       
/* 1C47 */ 0x8D, 0xF2, 0x17,   //          STA   CNTL30       
/* 1C4A */ 0xA2, 0x08,         //          LDX   #$08       
/* 1C4C */ 0x20, 0x6A, 0x1E,   //          JSR   GET5      ; GET REST OF THE CHAR, TEST CHAR       
/* 1C4F */                     //  ;       ** MAKE TTY/KB SELECTION **       
/* 1C4F */ 0x20, 0x8C, 0x1E,   //  START   JSR   INIT1     ;                               1C4F       
/* 1C52 */ 0xA9, 0x01,         //          LDA   #$01       
/* 1C54 */ 0x2C, 0x40, 0x17,   //          BIT   SAD       
/* 1C57 */ 0xD0, 0x1E,         //          BNE   TTYKB       
/* 1C59 */ 0x20, 0x2F, 0x1E,   //          JSR   CRLF      ; PRT CR LF       
/* 1C5C */ 0xA2, 0x0A,         //          LDX   #$0A      ; TYPE OUT KIM       
/* 1C5E */ 0x20, 0x31, 0x1E,   //          JSR   PRTST       
/* 1C61 */ 0x4C, 0xAF, 0x1D,   //          JMP   SHOW1       
/* 1C64 */                     //  ;       
/* 1C64 */ 0xA9, 0x00,         //  CLEAR   LDA   #$00       
/* 1C66 */ 0x85, 0xF8,         //          STA   INL       ; CLEAR INPUT BUFFER       
/* 1C68 */ 0x85, 0xF9,         //          STA   INH       
/* 1C6A */ 0x20, 0x5A, 0x1E,   //  READ    JSR   GETCH     ; GET CHAR       
/* 1C6D */ 0xC9, 0x01,         //          CMP   #$01             
/* 1C6F */ 0xF0, 0x06,         //          BEQ   TTYKB       
/* 1C71 */ 0x20, 0xAC, 0x1F,   //          JSR   PACK       
/* 1C74 */ 0x4C, 0xDB, 0x1D,   //          JMP   SCAN       
/* 1C77 */                     //  ;       ** MAIN ROUTINE FOR KEYBOARD AND DISPLAY **       
/* 1C77 */ 0x20, 0x19, 0x1F,   //  TTYKB   JSR   SCAND     ; IF A=0 NO KEY                 1C77       
/* 1C7A */ 0xD0, 0xD3,         //          BNE   START       
/* 1C7C */ 0xA9, 0x01,         //  TTYKB1  LDA   #$01       
/* 1C7E */ 0x2C, 0x40, 0x17,   //          BIT   SAD        
/* 1C81 */ 0xF0, 0xCC,         //          BEQ   START       
/* 1C83 */ 0x20, 0x19, 0x1F,   //          JSR   SCAND       
/* 1C86 */ 0xF0, 0xF4,         //          BEQ   TTYKB1       
/* 1C88 */ 0x20, 0x19, 0x1F,   //          JSR   SCAND       
/* 1C8B */ 0xF0, 0xEF,         //          BEQ   TTYKB1       
/* 1C8D */ 0x20, 0x6A, 0x1F,   //          JSR   GETKEY       
/* 1C90 */ 0xC9, 0x15,         //          CMP   #$15         
/* 1C92 */ 0x10, 0xBB,         //          BPL   START       
/* 1C94 */ 0xC9, 0x14,         //          CMP   #$14       
/* 1C96 */ 0xF0, 0x44,         //          BEQ   PCCMD     ; DISPLAY PC       
/* 1C98 */ 0xC9, 0x10,         //          CMP   #$10      ; ADDR MODE=1       
/* 1C9A */ 0xF0, 0x2C,         //          BEQ   ADDRM       
/* 1C9C */ 0xC9, 0x11,         //          CMP   #$11      ; DATA MODE=1       
/* 1C9E */ 0xF0, 0x2C,         //          BEQ   DATAM       
/* 1CA0 */ 0xC9, 0x12,         //          CMP   #$12      ; STEP       
/* 1CA2 */ 0xF0, 0x2F,         //          BEQ   STEP       
/* 1CA4 */ 0xC9, 0x13,         //          CMP   #$13      ; RUN       
/* 1CA6 */ 0xF0, 0x31,         //          BEQ   GOV       
/* 1CA8 */ 0x0A,               //          ASL   A         ; SHIFT CHAR INTO HIGH       
/* 1CA9 */ 0x0A,               //          ASL   A         ; ORDER NIBBLE       
/* 1CAA */ 0x0A,               //          ASL   A       
/* 1CAB */ 0x0A,               //          ASL   A       
/* 1CAC */ 0x85, 0xFC,         //          STA   TEMP      ; STORE IN TEMP       
/* 1CAE */ 0xA2, 0x04,         //          LDX   #$04       
/* 1CB0 */ 0xA4, 0xFF,         //  DATA1   LDY   MODE      ; TEST MODE 1=ADDR       
/* 1CB2 */ 0xD0, 0x0A,         //          BNE   ADDR      ; MODE=0 DATA       
/* 1CB4 */ 0xB1, 0xFA,         //          LDA   (POINTL),Y ; GET DATA       
/* 1CB6 */ 0x06, 0xFC,         //          ASL   TEMP      ; SHIFT CHAR       
/* 1CB8 */ 0x2A,               //          ROL   A         ; SHIFT DATA       
/* 1CB9 */ 0x91, 0xFA,         //          STA   (POINTL),Y ; STORE OUT DATA       
/* 1CBB */ 0x4C, 0xC3, 0x1C,   //          JMP   DATA2       
/* 1CBE */                     //  ;       
/* 1CBE */ 0x0A,               //  ADDR    ASL   A         ; SHIFT CHAR       
/* 1CBF */ 0x26, 0xFA,         //          ROL   POINTL    ; SHIFT ADDR       
/* 1CC1 */ 0x26, 0xFB,         //          ROL   POINTH    ; SHIFT ADDR HI       
/* 1CC3 */ 0xCA,               //  DATA2   DEX          
/* 1CC4 */ 0xD0, 0xEA,         //          BNE   DATA1     ; DO 4 TIMES       
/* 1CC6 */ 0xF0, 0x08,         //          BEQ   DATAM2    ; EXIT HERE       
/* 1CC8 */ 0xA9, 0x01,         //  ADDRM   LDA   #$01       
/* 1CCA */ 0xD0, 0x02,         //          BNE   DATAM1       
/* 1CCC */ 0xA9, 0x00,         //  DATAM   LDA   #$00        
/* 1CCE */ 0x85, 0xFF,         //  DATAM1  STA   MODE        
/* 1CD0 */ 0x4C, 0x4F, 0x1C,   //  DATAM2  JMP   START       
/* 1CD3 */                     //  ;       
/* 1CD3 */ 0x20, 0x63, 0x1F,   //  STEP    JSR   INCPT     ;                               1CD3       
/* 1CD6 */ 0x4C, 0x4F, 0x1C,   //          JMP   START       
/* 1CD9 */                     //  ;       
/* 1CD9 */ 0x4C, 0xC8, 0x1D,   //  GOV     JMP   GOEXEC    ;                               1CD9       
/* 1CDC */                     //  ;       ** DISPLAY PC BY MOVING PC TO POINT **       
/* 1CDC */ 0xA5, 0xEF,         //  PCCMD   LDA   PCL       ;                               1CDC       
/* 1CDE */ 0x85, 0xFA,         //          STA   POINTL       
/* 1CE0 */ 0xA5, 0xF0,         //          LDA   PCH       
/* 1CE2 */ 0x85, 0xFB,         //          STA   POINTH       
/* 1CE4 */ 0x4C, 0x4F, 0x1C,   //          JMP   START       
/* 1CE7 */                     //  ;       ** LOAD PAPER TAPE FROM TTY **       
/* 1CE7 */ 0x20, 0x5A, 0x1E,   //  LOAD    JSR   GETCH     ; LOOK FOR FIRST CHAR           1CE7       
/* 1CEA */ 0xC9, 0x3B,         //          CMP   #$3B      ; SEMICOLON       
/* 1CEC */ 0xD0, 0xF9,         //          BNE   LOAD       
/* 1CEE */ 0xA9, 0x00,         //          LDA   #$00       
/* 1CF0 */ 0x85, 0xF7,         //          STA   CHKSUM       
/* 1CF2 */ 0x85, 0xF6,         //          STA   CHKHI       
/* 1CF4 */ 0x20, 0x9D, 0x1F,   //          JSR   GETBYT    ; GET BYTE COUNT       
/* 1CF7 */ 0xAA,               //          TAX             ; SAVE IN X INDEX       
/* 1CF8 */ 0x20, 0x91, 0x1F,   //          JSR   CHK       ; COMPUTE CHECKSUM       
/* 1CFB */ 0x20, 0x9D, 0x1F,   //          JSR   GETBYT    ; GET ADDRESS HI       
/* 1CFE */ 0x85, 0xFB,         //          STA   POINTH       
/* 1D00 */ 0x20, 0x91, 0x1F,   //          JSR   CHK       
/* 1D03 */ 0x20, 0x9D, 0x1F,   //          JSR   GETBYT    ; GET ADDRESS LO       
/* 1D06 */ 0x85, 0xFA,         //          STA   POINTL       
/* 1D08 */ 0x20, 0x91, 0x1F,   //          JSR   CHK       
/* 1D0B */ 0x8A,               //          TXA             ; IF CNT=0 DONT       
/* 1D0C */ 0xF0, 0x0F,         //          BEQ   LOAD3     ; GET ANY DATA       
/* 1D0E */ 0x20, 0x9D, 0x1F,   //  LOAD2   JSR   GETBYT    ; GET DATA       
/* 1D11 */ 0x91, 0xFA,         //          STA   (POINTL),Y ; STORE DATA       
/* 1D13 */ 0x20, 0x91, 0x1F,   //          JSR   CHK       
/* 1D16 */ 0x20, 0x63, 0x1F,   //          JSR   INCPT     ; NEXT ADDRESS       
/* 1D19 */ 0xCA,               //          DEX           
/* 1D1A */ 0xD0, 0xF2,         //          BNE   LOAD2       
/* 1D1C */ 0xE8,               //          INX             ; X=1 DATA RCD X=0 LAST RCD       
/* 1D1D */ 0x20, 0x9D, 0x1F,   //  LOAD3   JSR   GETBYT    ; COMPARE CHKSUM       
/* 1D20 */ 0xC5, 0xF6,         //          CMP   CHKHI       
/* 1D22 */ 0xD0, 0x17,         //          BNE   LOADE1       
/* 1D24 */ 0x20, 0x9D, 0x1F,   //          JSR   GETBYT       
/* 1D27 */ 0xC5, 0xF7,         //          CMP   CHKSUM       
/* 1D29 */ 0xD0, 0x13,         //          BNE   LOADER       
/* 1D2B */ 0x8A,               //          TXA             ; X=0 LAST RECORD       
/* 1D2C */ 0xD0, 0xB9,         //          BNE   LOAD       
/* 1D2E */ 0xA2, 0x0C,         //          LDX   #$0C      ; X-OFF KIM       
/* 1D30 */ 0xA9, 0x27,         //  LOAD8   LDA   #$27       
/* 1D32 */ 0x8D, 0x42, 0x17,   //          STA   SBD       ; DISABLE DATA IN       
/* 1D35 */ 0x20, 0x31, 0x1E,   //          JSR   PRTST       
/* 1D38 */ 0x4C, 0x4F, 0x1C,   //          JMP   START       
/* 1D3B */                     //  ;       
/* 1D3B */ 0x20, 0x9D, 0x1F,   //  LOADE1  JSR   GETBYT    ; DUMMY       
/* 1D3E */ 0xA2, 0x11,         //  LOADER  LDX   #$11      ; X-OFF ERR KIM       
/* 1D40 */ 0xD0, 0xEE,         //          BNE   LOAD8       
/* 1D42 */                     //  ;       ** DUMP TO TTY FROM OPEN CELL ADDRESS TO        
/* 1D42 */                     //  ;          LIMHL, LIMHH **       
/* 1D42 */ 0xA9, 0x00,         //  DUMP    LDA   #$00      ;                               1D42       
/* 1D44 */ 0x85, 0xF8,         //          STA   INL       
/* 1D46 */ 0x85, 0xF9,         //          STA   INH       ; CLEAR RECORD COUNT       
/* 1D48 */ 0xA9, 0x00,         //  DUMP0   LDA   #$00       
/* 1D4A */ 0x85, 0xF6,         //          STA   CHKHI     ; CLEAR CHKSUM       
/* 1D4C */ 0x85, 0xF7,         //          STA   CHKSUM       
/* 1D4E */ 0x20, 0x2F, 0x1E,   //          JSR   CRLF      ; PRINT CR LF       
/* 1D51 */ 0xA9, 0x3B,         //          LDA   #$3B      ; PRINT SEMICOLON       
/* 1D53 */ 0x20, 0xA0, 0x1E,   //          JSR   OUTCH       
/* 1D56 */ 0xA5, 0xFA,         //          LDA   POINTL    ; TEST POINT GT OR ET       
/* 1D58 */ 0xCD, 0xF7, 0x17,   //          CMP   EAL       ; HI LIMIT GOTO EXIT       
/* 1D5B */ 0xA5, 0xFB,         //          LDA   POINTH       
/* 1D5D */ 0xED, 0xF8, 0x17,   //          SBC   EAH       
/* 1D60 */ 0x90, 0x18,         //          BCC   DUMP4       
/* 1D62 */ 0xA9, 0x00,         //          LDA   #$00      ; PRINT LAST RECORD       
/* 1D64 */ 0x20, 0x3B, 0x1E,   //          JSR   PRTBYT    ; 0 BYTES       
/* 1D67 */ 0x20, 0xCC, 0x1F,   //          JSR   OPEN       
/* 1D6A */ 0x20, 0x1E, 0x1E,   //          JSR   PRTPNT       
/* 1D6D */ 0xA5, 0xF6,         //          LDA   CHKHI     ; PRINT CHKSUM       
/* 1D6F */ 0x20, 0x3B, 0x1E,   //          JSR   PRTBYT    ; FOR LAST RECORD       
/* 1D72 */ 0xA5, 0xF7,         //          LDA   CHKSUM       
/* 1D74 */ 0x20, 0x3B, 0x1E,   //          JSR   PRTBYT       
/* 1D77 */ 0x4C, 0x64, 0x1C,   //          JMP   CLEAR       
/* 1D7A */                     //  ;       
/* 1D7A */ 0xA9, 0x18,         //  DUMP4   LDA   #$18      ; PRINT 24 BYTE COUNT           1D7A       
/* 1D7C */ 0xAA,               //          TAX             ; SAVE AS INDEX       
/* 1D7D */ 0x20, 0x3B, 0x1E,   //          JSR   PRTBYT       
/* 1D80 */ 0x20, 0x91, 0x1F,   //          JSR   CHK       
/* 1D83 */ 0x20, 0x1E, 0x1E,   //          JSR   PRTPNT       
/* 1D86 */ 0xA0, 0x00,         //  DUMP2   LDY   #$00      ; PRINT 24 BYTES       
/* 1D88 */ 0xB1, 0xFA,         //          LDA   (POINTL),Y ; GET DATA       
/* 1D8A */ 0x20, 0x3B, 0x1E,   //          JSR   PRTBYT    ; PRINT DATA       
/* 1D8D */ 0x20, 0x91, 0x1F,   //          JSR   CHK       ; COMPUTE CHKSUM       
/* 1D90 */ 0x20, 0x63, 0x1F,   //          JSR   INCPT     ; INCREMENT POINT       
/* 1D93 */ 0xCA,               //          DEX       
/* 1D94 */ 0xD0, 0xF0,         //          BNE   DUMP2       
/* 1D96 */ 0xA5, 0xF6,         //          LDA   CHKHI     ; PRINT CHKSUM       
/* 1D98 */ 0x20, 0x3B, 0x1E,   //          JSR   PRTBYT       
/* 1D9B */ 0xA5, 0xF7,         //          LDA   CHKSUM       
/* 1D9D */ 0x20, 0x3B, 0x1E,   //          JSR   PRTBYT       
/* 1DA0 */ 0xE6, 0xF8,         //          INC   INL       ; INCR RECORD COUNT       
/* 1DA2 */ 0xD0, 0x02,         //          BNE   DUMP3       
/* 1DA4 */ 0xE6, 0xF9,         //          INC   INH       
/* 1DA6 */ 0x4C, 0x48, 0x1D,   //  DUMP3   JMP   DUMP0       
/* 1DA9 */                     //  ;       
/* 1DA9 */ 0x20, 0xCC, 0x1F,   //  SPACE   JSR   OPEN      ; OPEN NEW CELL                 1DA9       
/* 1DAC */ 0x20, 0x2F, 0x1E,   //  SHOW    JSR   CRLF      ; PRINT CR LF       
/* 1DAF */ 0x20, 0x1E, 0x1E,   //  SHOW1   JSR   PRTPNT       
/* 1DB2 */ 0x20, 0x9E, 0x1E,   //          JSR   OUTSP     ; PRINT SPACE       
/* 1DB5 */ 0xA0, 0x00,         //          LDY   #$00      ; PRINT DATA SPECIFIED       
/* 1DB7 */ 0xB1, 0xFA,         //          LDA   (POINTL),Y ; BY POINT AD=LDA EXT       
/* 1DB9 */ 0x20, 0x3B, 0x1E,   //          JSR   PRTBYT       
/* 1DBC */ 0x20, 0x9E, 0x1E,   //          JSR   OUTSP     ; PRINT SPACE       
/* 1DBF */ 0x4C, 0x64, 0x1C,   //          JMP   CLEAR       
/* 1DC2 */                     //  ;       
/* 1DC2 */ 0x20, 0x63, 0x1F,   //  RTRN    JSR   INCPT     ; OPEN NEXT CELL                1DC2       
/* 1DC5 */ 0x4C, 0xAC, 0x1D,   //          JMP   SHOW       
/* 1DC8 */                     //  ;       
/* 1DC8 */ 0xA6, 0xF2,         //  GOEXEC  LDX   SPUSER    ;                               1DC8       
/* 1DCA */ 0x9A,               //          TXS          
/* 1DCB */ 0xA5, 0xFB,         //          LDA   POINTH    ; PROGRAM RUNS FROM       
/* 1DCD */ 0x48,               //          PHA             ; OPEN CELL ADDRESS       
/* 1DCE */ 0xA5, 0xFA,         //          LDA   POINTL       
/* 1DD0 */ 0x48,               //          PHA             
/* 1DD1 */ 0xA5, 0xF1,         //          LDA   PREG       
/* 1DD3 */ 0x48,               //          PHA              
/* 1DD4 */ 0xA6, 0xF5,         //          LDX   XREG      ; RESTORE REGS       
/* 1DD6 */ 0xA4, 0xF4,         //          LDY   YREG       
/* 1DD8 */ 0xA5, 0xF3,         //          LDA   ACC       
/* 1DDA */ 0x40,               //          RTI       
/* 1DDB */                     //  ;       
/* 1DDB */ 0xC9, 0x20,         //  SCAN    CMP   #$20      ; OPEN CELL       
/* 1DDD */ 0xF0, 0xCA,         //          BEQ   SPACE       
/* 1DDF */ 0xC9, 0x7F,         //          CMP   #$7F      ; RUB OUT (KIM)       
/* 1DE1 */ 0xF0, 0x1B,         //          BEQ   STV       
/* 1DE3 */ 0xC9, 0x0D,         //          CMP   #$0D      ; NEXT CELL       
/* 1DE5 */ 0xF0, 0xDB,         //          BEQ   RTRN       
/* 1DE7 */ 0xC9, 0x0A,         //          CMP   #$0A      ;PREV CELL       
/* 1DE9 */ 0xF0, 0x1C,         //          BEQ   FEED       
/* 1DEB */ 0xC9, 0x2E,         //          CMP   #'.'      ; MODIFY CELL       
/* 1DED */ 0xF0, 0x26,         //          BEQ   MODIFY       
/* 1DEF */ 0xC9, 0x47,         //          CMP   #'G'      ; GO EXEC       
/* 1DF1 */ 0xF0, 0xD5,         //          BEQ   GOEXEC       
/* 1DF3 */ 0xC9, 0x51,         //          CMP   #'Q'      ; DUMP FROM OPEN CELL TO HI LIMIT       
/* 1DF5 */ 0xF0, 0x0A,         //          BEQ   DUMPV       
/* 1DF7 */ 0xC9, 0x4C,         //          CMP   #'L'      ; LOAD TAPE       
/* 1DF9 */ 0xF0, 0x09,         //          BEQ   LOADV       
/* 1DFB */ 0x4C, 0x6A, 0x1C,   //          JMP   READ      ; IGNORE ILLEGAL CHAR       
/* 1DFE */                     //  ;       
/* 1DFE */ 0x4C, 0x4F, 0x1C,   //  STV     JMP   START       
/* 1E01 */ 0x4C, 0x42, 0x1D,   //  DUMPV   JMP   DUMP       
/* 1E04 */ 0x4C, 0xE7, 0x1C,   //  LOADV   JMP   LOAD        
/* 1E07 */                     //  ;       
/* 1E07 */ 0x38,               //  FEED    SEC             ;                               1E07       
/* 1E08 */ 0xA5, 0xFA,         //          LDA   POINTL    ; DEC DOUBLE BYTE       
/* 1E0A */ 0xE9, 0x01,         //          SBC   #$01      ; AT POINTL AND POINTH       
/* 1E0C */ 0x85, 0xFA,         //          STA   POINTL       
/* 1E0E */ 0xB0, 0x02,         //          BCS   FEED1       
/* 1E10 */ 0xC6, 0xFB,         //          DEC   POINTH       
/* 1E12 */ 0x4C, 0xAC, 0x1D,   //  FEED1   JMP   SHOW       
/* 1E15 */                     //  ;       
/* 1E15 */ 0xA0, 0x00,         //  MODIFY  LDY   #$00      ; GET CONTENTS OF INPUT BUFF        
/* 1E17 */ 0xA5, 0xF8,         //          LDA   INL       ; INL AND STORE IN LOC       
/* 1E19 */ 0x91, 0xFA,         //          STA   (POINTL),Y ; SPECIFIED BY POINT       
/* 1E1B */ 0x4C, 0xC2, 0x1D,   //          JMP   RTRN        
/* 1E1E */                     //  ;       
/* 1E1E */                     //  ;       ** SUBROUTINES FOLLOW **       
/* 1E1E */ 0xA5, 0xFB,         //  PRTPNT  LDA   POINTH    ; PRINT POINTL, POINTH          1E1E       
/* 1E20 */ 0x20, 0x3B, 0x1E,   //          JSR   PRTBYT       
/* 1E23 */ 0x20, 0x91, 0x1F,   //          JSR   CHK       
/* 1E26 */ 0xA5, 0xFA,         //          LDA   POINTL       
/* 1E28 */ 0x20, 0x3B, 0x1E,   //          JSR   PRTBYT       
/* 1E2B */ 0x20, 0x91, 0x1F,   //          JSR   CHK       
/* 1E2E */ 0x60,               //          RTS       
/* 1E2F */                     //  ;       **PRINT STRING OF ASCII CHARS FROM TOP+X TO TOP       
/* 1E2F */ 0xA2, 0x07,         //  CRLF    LDX   #$07           
/* 1E31 */ 0xBD, 0xD5, 0x1F,   //  PRTST   LDA   TOP,X        
/* 1E34 */ 0x20, 0xA0, 0x1E,   //          JSR   OUTCH       
/* 1E37 */ 0xCA,               //          DEX        
/* 1E38 */ 0x10, 0xF7,         //          BPL   PRTST     ; STOP ON INDEX ZERO       
/* 1E3A */ 0x60,               //          RTS       
/* 1E3B */                     //  ;       ** PRINT 1 HEX BYTE AS 2 ASCII CHARS **       
/* 1E3B */ 0x85, 0xFC,         //  PRTBYT  STA   TEMP      ;                               1E3B       
/* 1E3D */ 0x4A,               //          LSR   A         ; SHIFT CHAR RIGHT 4 BITS       
/* 1E3E */ 0x4A,               //          LSR   A       
/* 1E3F */ 0x4A,               //          LSR   A       
/* 1E40 */ 0x4A,               //          LSR   A       
/* 1E41 */ 0x20, 0x4C, 0x1E,   //          JSR   HEXTA     ; CONVERT TO HEX AND PRINT       
/* 1E44 */ 0xA5, 0xFC,         //          LDA   TEMP      ; GET OTHER HALF       
/* 1E46 */ 0x20, 0x4C, 0x1E,   //          JSR   HEXTA     ; CONVERT TO HEX AND PRINT       
/* 1E49 */ 0xA5, 0xFC,         //          LDA   TEMP      ; RESTORE BYTE IN A AND RETURN       
/* 1E4B */ 0x60,               //          RTS          
/* 1E4C */ 0x29, 0x0F,         //  HEXTA   AND   #$0F      ; MASK HI 4 BITS       
/* 1E4E */ 0xC9, 0x0A,         //          CMP   #$0A       
/* 1E50 */ 0x18,               //          CLC              
/* 1E51 */ 0x30, 0x02,         //          BMI   HEXTA1       
/* 1E53 */ 0x69, 0x07,         //          ADC   #$07      ; ALPHA HEX       
/* 1E55 */ 0x69, 0x30,         //  HEXTA1  ADC   #$30      ; DEC HEX       
/* 1E57 */ 0x4C, 0xA0, 0x1E,   //          JMP   OUTCH     ; PRINT CHAR       
/* 1E5A */                     //  ;       ** GET 1 CHAR FROM TTY, CHAR IN A       
/* 1E5A */ 0x86, 0xFD,         //  GETCH   STX   TMPX      ; SAVE X REG     1E5A       
/* 1E5C */ 0xA2, 0x08,         //          LDX   #$08      ; SET UP 8-BIT COUNT       
/* 1E5E */ 0xA9, 0x01,         //          LDA   #$01       
/* 1E60 */ 0x2C, 0x40, 0x17,   //  GET1    BIT   SAD       
/* 1E63 */ 0xD0, 0x22,         //          BNE   GET6        
/* 1E65 */ 0x30, 0xF9,         //          BMI   GET1      ; WAIT FOR START BIT       
/* 1E67 */ 0x20, 0xD4, 0x1E,   //          JSR   DELAY     ; DELAY 1 BIT       
/* 1E6A */ 0x20, 0xEB, 0x1E,   //  GET5    JSR   DEHALF    ; DELAY 1/2 BIT TIME       
/* 1E6D */ 0xAD, 0x40, 0x17,   //  GET2    LDA   SAD       ; GET 8 BITS       
/* 1E70 */ 0x29, 0x80,         //          AND   #$80      ; MASK OFF LOW ORDER BITS       
/* 1E72 */ 0x46, 0xFE,         //          LSR   CHAR      ; SHIFT RIGHT CHAR       
/* 1E74 */ 0x05, 0xFE,         //          ORA   CHAR       
/* 1E76 */ 0x85, 0xFE,         //          STA   CHAR       
/* 1E78 */ 0x20, 0xD4, 0x1E,   //          JSR   DELAY     ; DELAY 1 BIT TIME       
/* 1E7B */ 0xCA,               //          DEX         
/* 1E7C */ 0xD0, 0xEF,         //          BNE   GET2      ; GET NEXT CHAR       
/* 1E7E */ 0x20, 0xEB, 0x1E,   //          JSR   DEHALF    ; EXIT THIS ROUTINE       
/* 1E81 */ 0xA6, 0xFD,         //          LDX   TMPX       
/* 1E83 */ 0xA5, 0xFE,         //          LDA   CHAR       
/* 1E85 */ 0x2A,               //          ROL   A         ; SHIFT OFF PARITY       
/* 1E86 */ 0x4A,               //          LSR   A       
/* 1E87 */ 0x60,               //  GET6    RTS          
/* 1E88 */                     //  ;       ** INITIALIZATION FOR SIGMA **       
/* 1E88 */ 0xA2, 0x01,         //  INITS   LDX   #$01      ; SET KB MODE TO ADDR           1E88       
/* 1E8A */ 0x86, 0xFF,         //          STX   MODE       
/* 1E8C */ 0xA2, 0x00,         //  INIT1   LDX   #$00             
/* 1E8E */ 0x8E, 0x41, 0x17,   //          STX   PADD      ; FOR SIGMA USE SADD       
/* 1E91 */ 0xA2, 0x3F,         //          LDX   #$3F       
/* 1E93 */ 0x8E, 0x43, 0x17,   //          STX   PBDD      ; FOR SIGMA USE SBDD       
/* 1E96 */ 0xA2, 0x07,         //          LDX   #$07      ; ENABLE DATA IN       
/* 1E98 */ 0x8E, 0x42, 0x17,   //          STX   SBD       ; OUTPUT       
/* 1E9B */ 0xD8,               //          CLD           
/* 1E9C */ 0x78,               //          SEI           
/* 1E9D */ 0x60,               //          RTS       
/* 1E9E */                     //  ;       ** PRINT ONE CHAR IN A **       
/* 1E9E */ 0xA9, 0x20,         //  OUTSP   LDA   #$20      ; PRINT SPACE                   1E9E       
/* 1EA0 */ 0x85, 0xFE,         //  OUTCH   STA   CHAR       
/* 1EA2 */ 0x86, 0xFD,         //          STX   TMPX       
/* 1EA4 */ 0x20, 0xD4, 0x1E,   //          JSR   DELAY     ; DELAY 10/11 BIT CODE SYNC       
/* 1EA7 */ 0xAD, 0x42, 0x17,   //          LDA   SBD       ; START BIT       
/* 1EAA */ 0x29, 0xFE,         //          AND   #$FE       
/* 1EAC */ 0x8D, 0x42, 0x17,   //          STA   SBD       
/* 1EAF */ 0x20, 0xD4, 0x1E,   //          JSR   DELAY       
/* 1EB2 */ 0xA2, 0x08,         //          LDX   #$08        
/* 1EB4 */ 0xAD, 0x42, 0x17,   //  OUT1    LDA   SBD       ; DATA BIT       
/* 1EB7 */ 0x29, 0xFE,         //          AND   #$FE        
/* 1EB9 */ 0x46, 0xFE,         //          LSR   CHAR       
/* 1EBB */ 0x69, 0x00,         //          ADC   #$00       
/* 1EBD */ 0x8D, 0x42, 0x17,   //          STA   SBD         
/* 1EC0 */ 0x20, 0xD4, 0x1E,   //          JSR   DELAY       
/* 1EC3 */ 0xCA,               //          DEX         
/* 1EC4 */ 0xD0, 0xEE,         //          BNE   OUT1       
/* 1EC6 */ 0xAD, 0x42, 0x17,   //          LDA   SBD       ; STOP BIT       
/* 1EC9 */ 0x09, 0x01,         //          ORA   #$01       
/* 1ECB */ 0x8D, 0x42, 0x17,   //          STA   SBD       
/* 1ECE */ 0x20, 0xD4, 0x1E,   //          JSR   DELAY     ; STOP BIT       
/* 1ED1 */ 0xA6, 0xFD,         //          LDX   TMPX      ; RESTORE INDEX       
/* 1ED3 */ 0x60,               //          RTS       
/* 1ED4 */                     //  ;       ** DELAY 1 BIT TIME **       
/* 1ED4 */ 0xAD, 0xF3, 0x17,   //  DELAY   LDA   CNTH30    ;                               1ED4       
/* 1ED7 */ 0x8D, 0xF4, 0x17,   //          STA   TIMH       
/* 1EDA */ 0xAD, 0xF2, 0x17,   //          LDA   CNTL30       
/* 1EDD */ 0x38,               //  DE2     SEC          
/* 1EDE */ 0xE9, 0x01,         //  DE4     SBC   #$01        
/* 1EE0 */ 0xB0, 0x03,         //          BCS   DE3         
/* 1EE2 */ 0xCE, 0xF4, 0x17,   //          DEC   TIMH       
/* 1EE5 */ 0xAC, 0xF4, 0x17,   //  DE3     LDY   TIMH       
/* 1EE8 */ 0x10, 0xF3,         //          BPL   DE2       
/* 1EEA */ 0x60,               //          RTS       
/* 1EEB */                     //  ;       ** DELAY 1/2 BIT TIME **       
/* 1EEB */ 0xAD, 0xF3, 0x17,   //  DEHALF  LDA   CNTH30    ;                               1EEB         
/* 1EEE */ 0x8D, 0xF4, 0x17,   //          STA   TIMH       
/* 1EF1 */ 0xAD, 0xF2, 0x17,   //          LDA   CNTL30       
/* 1EF4 */ 0x4A,               //          LSR   A       
/* 1EF5 */ 0x4E, 0xF4, 0x17,   //          LSR   TIMH       
/* 1EF8 */ 0x90, 0xE3,         //          BCC   DE2       
/* 1EFA */ 0x09, 0x80,         //          ORA   #$80       
/* 1EFC */ 0xB0, 0xE0,         //          BCS   DE4       
/* 1EFE */                     //  ;       ** SUB TO DETERMINE IF KEY IS DEPRESSED OR        
/* 1EFE */                     //  ;          CONDITION OF SSW KEY NOT DEPRESSED OR       
/* 1EFE */                     //  ;          TTY MODE  A=0       
/* 1EFE */                     //  ;          KEY DEPRESSED OR KB MODE  A NOT ZERO       
/* 1EFE */ 0xA0, 0x03,         //  AK      LDY   #$03      ; 3 ROWS       
/* 1F00 */ 0xA2, 0x01,         //          LDX   #$01      ; DIGIT 0       
/* 1F02 */ 0xA9, 0xFF,         //  ONEKEY  LDA   #$FF       
/* 1F04 */ 0x8E, 0x42, 0x17,   //  AK1     STX   SBD       ; OUTPUT DIGIT       
/* 1F07 */ 0xE8,               //          INX             ; GET NEXT DIGIT       
/* 1F08 */ 0xE8,               //          INX        
/* 1F09 */ 0x2D, 0x40, 0x17,   //          AND   SAD       ; INPUT SEGMENTS       
/* 1F0C */ 0x88,               //          DEY        
/* 1F0D */ 0xD0, 0xF5,         //          BNE   AK1       
/* 1F0F */ 0xA0, 0x07,         //          LDY   #$07       
/* 1F11 */ 0x8C, 0x42, 0x17,   //          STY   SBD       
/* 1F14 */ 0x09, 0x80,         //          ORA   #$80       
/* 1F16 */ 0x49, 0xFF,         //          EOR   #$FF       
/* 1F18 */ 0x60,               //          RTS        
/* 1F19 */                     //  ;       ** OUTPUT TO 7-SEGMENT DISPLAY **       
/* 1F19 */ 0xA0, 0x00,         //  SCAND   LDY   #$00      ; GET DATA                      1F19        
/* 1F1B */ 0xB1, 0xFA,         //          LDA   (POINTL),Y ; SPECIFIED BY POINT       
/* 1F1D */ 0x85, 0xF9,         //          STA   INH       ; SET UP DISPLAY BUFFER       
/* 1F1F */ 0xA9, 0x7F,         //          LDA   #$7F      ; CHANGE SEG       
/* 1F21 */ 0x8D, 0x41, 0x17,   //          STA   PADD      ; TO OUTPUT       
/* 1F24 */ 0xA2, 0x09,         //          LDX   #$09      ; INIT DIGIT NUMBER       
/* 1F26 */ 0xA0, 0x03,         //          LDY   #$03      ; OUTPUT 3 BYTES       
/* 1F28 */ 0xB9, 0xF8, 0x00,   //  SCAND1  LDA   INL,Y     ; GET BYTE       
/* 1F2B */ 0x4A,               //          LSR   A         ; GET MSD       
/* 1F2C */ 0x4A,               //          LSR   A       
/* 1F2D */ 0x4A,               //          LSR   A       
/* 1F2E */ 0x4A,               //          LSR   A       
/* 1F2F */ 0x20, 0x48, 0x1F,   //          JSR   CONVD     ; OUTPUT CHAR       
/* 1F32 */ 0xB9, 0xF8, 0x00,   //          LDA   INL,Y     ; GET BYTE AGAIN       
/* 1F35 */ 0x29, 0x0F,         //          AND   #$0F      ; GET LSD       
/* 1F37 */ 0x20, 0x48, 0x1F,   //          JSR   CONVD     ; OUTPUT CHAR       
/* 1F3A */ 0x88,               //          DEY             ; SET UP FOR NEXT BYTE       
/* 1F3B */ 0xD0, 0xEB,         //          BNE   SCAND1       
/* 1F3D */ 0x8E, 0x42, 0x17,   //          STX   SBD       ; ALL DIGITS OFF       
/* 1F40 */ 0xA9, 0x00,         //          LDA   #$00      ; CHANGE SEGMENT       
/* 1F42 */ 0x8D, 0x41, 0x17,   //          STA   PADD      ; TO INPUTS       
/* 1F45 */ 0x4C, 0xFE, 0x1E,   //          JMP   AK        ; GET ANY KEY       
/* 1F48 */                     //  ;       ** CONVERT AND DISPLAY HEX (USED BY SCAND ONLY)**       
/* 1F48 */ 0x84, 0xFC,         //  CONVD   STY   TEMP       
/* 1F4A */ 0xA8,               //          TAY             ; SAVE Y       
/* 1F4B */ 0xB9, 0xE7, 0x1F,   //          LDA   TABLE,Y   ; USE CHAR AS INDEX       
/* 1F4E */ 0xA0, 0x00,         //          LDY   #$00      ; LOOKUP CONVERSION       
/* 1F50 */ 0x8C, 0x40, 0x17,   //          STY   SAD       ; TURN OFF SEGMENTS       
/* 1F53 */ 0x8E, 0x42, 0x17,   //          STX   SBD       ; OUTPUT DIGIT ENABLE       
/* 1F56 */ 0x8D, 0x40, 0x17,   //          STA   SAD       ; OUTPUT SEGMENTS       
/* 1F59 */ 0xA0, 0x7F,         //          LDY   #$7F      ; DELAY 500 CYCLES       
/* 1F5B */ 0x88,               //  CONVD1  DEY          
/* 1F5C */ 0xD0, 0xFD,         //          BNE   CONVD1       
/* 1F5E */ 0xE8,               //          INX             ; GET NEXT DIGIT NUMBER       
/* 1F5F */ 0xE8,               //          INX             ; ADD 2       
/* 1F60 */ 0xA4, 0xFC,         //          LDY   TEMP      ; RESTORE Y       
/* 1F62 */ 0x60,               //          RTS       
/* 1F63 */                     //  ;       ** INCREMENT POINT **       
/* 1F63 */ 0xE6, 0xFA,         //  INCPT   INC   POINTL       
/* 1F65 */ 0xD0, 0x02,         //          BNE   INCPT2       
/* 1F67 */ 0xE6, 0xFB,         //          INC   POINTH       
/* 1F69 */ 0x60,               //  INCPT2  RTS          
/* 1F6A */                     //  ;       ** GET KEY FROM KEYPAD A=KEYVALUE **       
/* 1F6A */ 0xA2, 0x21,         //  GETKEY  LDX   #$21      ; START AT DIGIT 0              1F6A        
/* 1F6C */ 0xA0, 0x01,         //  GETKE5  LDY   #$01      ; GET 1 ROW       
/* 1F6E */ 0x20, 0x02, 0x1F,   //          JSR   ONEKEY       
/* 1F71 */ 0xD0, 0x07,         //          BNE   KEYIN     ; A=0 NO KEY       
/* 1F73 */ 0xE0, 0x27,         //          CPX   #$27      ; TEST FOR DIGIT 2       
/* 1F75 */ 0xD0, 0xF5,         //          BNE   GETKE5       
/* 1F77 */ 0xA9, 0x15,         //          LDA   #$15      ; 15=NOKEY       
/* 1F79 */ 0x60,               //          RTS       
/* 1F7A */ 0xA0, 0xFF,         //  KEYIN   LDY   #$FF             
/* 1F7C */ 0x0A,               //  KEYIN1  ASL   A         ; SHIFT LEFT       
/* 1F7D */ 0xB0, 0x03,         //          BCS   KEYIN2    ; UNTIL Y=KEY NO       
/* 1F7F */ 0xC8,               //          INY          
/* 1F80 */ 0x10, 0xFA,         //          BPL   KEYIN1       
/* 1F82 */ 0x8A,               //  KEYIN2  TXA         
/* 1F83 */ 0x29, 0x0F,         //          AND   #$0F      ; MASK MSD       
/* 1F85 */ 0x4A,               //          LSR   A         ; DIVIDE BY 2       
/* 1F86 */ 0xAA,               //          TAX          
/* 1F87 */ 0x98,               //          TYA          
/* 1F88 */ 0x10, 0x03,         //          BPL   KEYIN4       
/* 1F8A */ 0x18,               //  KEYIN3  CLC          
/* 1F8B */ 0x69, 0x07,         //          ADC   #$07      ; MULT (X-1 TIMES A       
/* 1F8D */ 0xCA,               //  KEYIN4  DEX          
/* 1F8E */ 0xD0, 0xFA,         //          BNE   KEYIN3       
/* 1F90 */ 0x60,               //          RTS       
/* 1F91 */                     //  ;       ** COMPUTE CHECKSUM **       
/* 1F91 */ 0x18,               //  CHK     CLC          
/* 1F92 */ 0x65, 0xF7,         //          ADC   CHKSUM       
/* 1F94 */ 0x85, 0xF7,         //          STA   CHKSUM       
/* 1F96 */ 0xA5, 0xF6,         //          LDA   CHKHI       
/* 1F98 */ 0x69, 0x00,         //          ADC   #$00       
/* 1F9A */ 0x85, 0xF6,         //          STA   CHKHI       
/* 1F9C */ 0x60,               //          RTS       
/* 1F9D */                     //  ;       ** GET 2 HEX CHARS AND PACK INTO INL AND INH **       
/* 1F9D */ 0x20, 0x5A, 0x1E,   //  GETBYT  JSR   GETCH       
/* 1FA0 */ 0x20, 0xAC, 0x1F,   //          JSR   PACK       
/* 1FA3 */ 0x20, 0x5A, 0x1E,   //          JSR   GETCH       
/* 1FA6 */ 0x20, 0xAC, 0x1F,   //          JSR   PACK       
/* 1FA9 */ 0xA5, 0xF8,         //          LDA   INL       
/* 1FAB */ 0x60,               //          RTS       
/* 1FAC */                     //  ;       ** SHIFT CHAR IN A INTO INL AND INH **       
/* 1FAC */ 0xC9, 0x30,         //  PACK    CMP   #$30      ; CHECK FOR HEX  1FAC       
/* 1FAE */ 0x30, 0x1B,         //          BMI   UPDAT2       
/* 1FB0 */ 0xC9, 0x47,         //          CMP   #$47      ; NOT HEX EXIT       
/* 1FB2 */ 0x10, 0x17,         //          BPL   UPDAT2       
/* 1FB4 */ 0xC9, 0x40,         //          CMP   #$40      ; CONVERT TO HEX       
/* 1FB6 */ 0x30, 0x03,         //          BMI   UPDATE       
/* 1FB8 */ 0x18,               //          CLC          
/* 1FB9 */ 0x69, 0x09,         //          ADC   #$09       
/* 1FBB */ 0x2A,               //  UPDATE  ROL   A       
/* 1FBC */ 0x2A,               //          ROL   A       
/* 1FBD */ 0x2A,               //          ROL   A       
/* 1FBE */ 0x2A,               //          ROL   A       
/* 1FBF */ 0xA0, 0x04,         //          LDY   #$04      ; SHIFT INTO I/O BUFFER       
/* 1FC1 */ 0x2A,               //  UPDAT1  ROL   A        
/* 1FC2 */ 0x26, 0xF8,         //          ROL   INL       
/* 1FC4 */ 0x26, 0xF9,         //          ROL   INH       
/* 1FC6 */ 0x88,               //          DEY          
/* 1FC7 */ 0xD0, 0xF8,         //          BNE   UPDAT1       
/* 1FC9 */ 0xA9, 0x00,         //          LDA   #$00      ; A=0 IF HEX NUM       
/* 1FCB */ 0x60,               //  UPDAT2  RTS          
/* 1FCC */                     //  ;       
/* 1FCC */ 0xA5, 0xF8,         //  OPEN    LDA   INL       ; MOVE I/O BUFFER TO POINT       
/* 1FCE */ 0x85, 0xFA,         //          STA   POINTL       
/* 1FD0 */ 0xA5, 0xF9,         //          LDA   INH       ; TRANSFER INH- POINTH       
/* 1FD2 */ 0x85, 0xFB,         //          STA   POINTH       
/* 1FD4 */ 0x60,               //          RTS       
/* 1FD5 */                     //  ;       
/* 1FD5 */                     //  ;       ** TABLES **       
/* 1FD5 */ 0x00, 0x00, 0x00, 0x00,  // TOP .BYTE $00,$00,$00,$00,$00,$00,$0A,$0D ; 1FD5
/* 1FD9 */ 0x00, 0x00, 0x0A, 0x0D,  // 
/* 1FDD */ 0x4D, 0x49, 0x4B, 0x20,  // .BYTE 'M','I','K',$20,$13 ; KIM
/* 1FE1 */ 0x13,               // 
/* 1FE2 */ 0x52, 0x52, 0x45, 0x20,  // .BYTE 'R','R','E',$20,$13 ; ERR
/* 1FE6 */ 0x13,               // 
/* 1FE7 */ 0xBF, 0x86, 0xDB, 0xCF,  // TABLE .BYTE $BF,$86,$DB,$CF,$E6,$ED,$FD,$87 ;0-7 1FE7
/* 1FEB */ 0xE6, 0xED, 0xFD, 0x87,  // 
/* 1FEF */ 0xFF, 0xEF, 0xF7, 0xFC,  // .BYTE $FF,$EF,$F7,$FC,$B9,$DE,$F9,$F1 ;8-F HEX TO 7-SEG
/* 1FF3 */ 0xB9, 0xDE, 0xF9, 0xF1,  // 
/* 1FF7 */                     //  ;       
/* 1FF7 */                     //         
/* 1FF7 */                     //  ; Fill unused locations with $FF       
/* 1FF7 */ 0xFF, 0xFF, 0xFF,   //          .FILL 3, $FF       
/* 1FFA */                     //         
/* 1FFA */                     //  ;       ** INTERRUPT VECTORS **       
/* 1FFA */                     //          .org  $1FFA       
/* 1FFA */ 0x1C, 0x1C,         //  NMIENT  .WORD NMIT       
/* 1FFC */ 0x22, 0x1C,         //  RSTENT  .WORD RST       
/* 1FFE */ 0x1F, 0x1C          //  IRQENT  .WORD IRQT       
};

