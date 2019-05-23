// --- ROM 003 CODE SECTION ---------------------------------------------------
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
    unsigned char ROM003[1024] = {
#else
    const unsigned char ROM003[1024] PROGMEM = {
#endif
/* OrgASM 6502 Assembler.      TASM 3.3 (Macintosh OS X) May 2015. */
/* 0000 */                     //  ;- - - - - - - - - - - - - - KIM.ASM - - - - - - - - - - -       
/* 0000 */                     //  ; COPYRIGHT MOS TECHNOLOGY, INC       
/* 0000 */                     //  ; DATE: OCT 18, 1975 REV-D       
/* 0000 */                     //  ;************************ 6530-003 I.C. ******************       
/* 0000 */                     //  ; 6530-003 I.C. IS AN AUDIO CASSETTE TAPE RECORDER       
/* 0000 */                     //  ; EXTENSION OF THE BASIC KIM MONITOR.  IT FEATURES       
/* 0000 */                     //  ; TWO ROUTINES:       
/* 0000 */                     //  ;   LOADT - LOAD MEMORY FROM AUDIO TAPE       
/* 0000 */                     //  ;     ID=00     IGNORE ID       
/* 0000 */                     //  ;     ID=FF     IGNORE ID, USE SA FOR START ADDR       
/* 0000 */                     //  ;     ID=01-FE  USE ADDRESS ON TAPE       
/* 0000 */                     //  ;       
/* 0000 */                     //  ;   DUMPT - STORE MEMORY ONTO AUDIO TAPE       
/* 0000 */                     //  ;     ID=00     SHOULD NOT BE USED       
/* 0000 */                     //  ;     ID=FF     SHOULD NOT BE USED       
/* 0000 */                     //  ;     ID=01-FE  NORMAL ID RANGE       
/* 0000 */                     //  ;     SAL       LSB STARTING ADDRESS OF PROGRAM       
/* 0000 */                     //  ;     SAH       MSB       
/* 0000 */                     //  ;     EAL       ENDING ADDRESS OF PROGRAM       
/* 0000 */                     //  ;     EAH       MSB       
/* 0000 */                     //  ;       
/* 1800 */                     //          .org  $1800       
/* 1800 */                     //  #define SAD      $1740     ; 6530 A DATA       
/* 1800 */                     //  #define PADD     $1741     ; 6530 A DATA DIRECTION       
/* 1800 */                     //  #define SBD      $1742     ; 6530 B DATA       
/* 1800 */                     //  #define PBDD     $1743     ; 6530 B DATA DIRECTION       
/* 1800 */                     //  #define CLK1T    $1744     ; DIV BY 1 TIME       
/* 1800 */                     //  #define CLK8T    $1745     ; DIV BY 8 TIME       
/* 1800 */                     //  #define CLK64T   $1746     ; DIV BY 64 TIME       
/* 1800 */                     //  #define CLKKT    $1747     ; DIV BY 1024 TIME       
/* 1800 */                     //  #define CLKRDI   $1747     ; READ TIME OUT BIT       
/* 1800 */                     //  #define CLKRDT   $1746     ; READ TIME       
/* 1800 */                     //  ;       ** MPU REG.  SAVX AREA IN PAGE 0 **       
/* 1800 */                     //  #define PCL      $EF       ; PROGRAM CNT LOW       
/* 1800 */                     //  #define PCH      $F0       ; PROGRAM CNT HI       
/* 1800 */                     //  #define PREG     $F1       ; CURRENT STATUS REG       
/* 1800 */                     //  #define SPUSER   $F2       ; CURRENT STACK POINTER       
/* 1800 */                     //  #define ACC      $F3       ; ACCUMULATOR       
/* 1800 */                     //  #define YREG     $F4       ; Y INDEX       
/* 1800 */                     //  #define XREG     $F5       ; X INDEX       
/* 1800 */                     //  ;       ** KIM FIXED AREA IN PAGE 0  **       
/* 1800 */                     //  #define CHKHI    $F6        
/* 1800 */                     //  #define CHKSUM   $F7        
/* 1800 */                     //  #define INL      $F8       ; INPUT BUFFER)       
/* 1800 */                     //  #define INH      $F9       ; INPUT BUFFER       
/* 1800 */                     //  #define POINTL   $FA       ; LSB OF OPEN CELL       
/* 1800 */                     //  #define POINTH   $FB       ; MSB OF OPEN CELL       
/* 1800 */                     //  #define TEMP     $FC        
/* 1800 */                     //  #define TMPX     $FD        
/* 1800 */                     //  #define CHAR     $FE        
/* 1800 */                     //  #define MODE     $FF        
/* 1800 */                     //  ;       ** KIM FIXED AREA IN PAGE 23 **       
/* 1800 */                     //  #define CHKL     $17E7        
/* 1800 */                     //  #define CHKH     $17E8     ; CHKSUM       
/* 1800 */                     //  #define SAVX     $17E9     ; (3-BYTES)       
/* 1800 */                     //  #define VEB      $17EC     ; VOLATILE EXEC BLOCK (6-B)       
/* 1800 */                     //  #define CNTL30   $17F2     ; TTY DELAY       
/* 1800 */                     //  #define CNTH30   $17F3     ; TTY DELAY       
/* 1800 */                     //  #define TIMH     $17F4        
/* 1800 */                     //  #define SAL      $17F5     ; LOW STARTING ADDRESS       
/* 1800 */                     //  #define SAH      $17F6     ; HI STARTING ADDRESS       
/* 1800 */                     //  #define EAL      $17F7     ; LOW ENDING ADDRESS       
/* 1800 */                     //  #define EAH      $17F8     ; HI ENDING ADDRESS       
/* 1800 */                     //  #define ID       $17F9     ; TAPE PROGRAM ID NUMBER       
/* 1800 */                     //  ;       ** INTERRUPT VECTORS **       
/* 1800 */                     //  #define NMIV     $17FA     ; STOP VECTOR (STOP=1C00)       
/* 1800 */                     //  #define RSTV     $17FC     ; RST VECTOR       
/* 1800 */                     //  #define IRQV     $17FE     ; IRQ VECTOR (BRK=1C00)       
/* 1800 */                     //         
/* 1800 */                     //  ;       
/* 1800 */                     //  ;       ** DUMP MEMORY TO TAPE **       
/* 1800 */ 0xA9, 0xAD,         //  DUMPT   LDA   #$AD      ; LOAD ABSOLUTE INST            1800       
/* 1802 */ 0x8D, 0xEC, 0x17,   //          STA   VEB          
/* 1805 */ 0x20, 0x32, 0x19,   //          JSR   INTVEB       
/* 1808 */ 0xA9, 0x27,         //          LDA   #$27      ; TURN OFF DATAIN PB5       
/* 180A */ 0x8D, 0x42, 0x17,   //          STA   SBD          
/* 180D */ 0xA9, 0xBF,         //          LDA   #$BF      ; CONVERT PB7 TO OUTPUT       
/* 180F */ 0x8D, 0x43, 0x17,   //          STA   PBDD         
/* 1812 */ 0xA2, 0x64,         //          LDX   #$64      ; 100 CHARS        
/* 1814 */ 0xA9, 0x16,         //  DUMPT1  LDA   #$16      ; SYNC CHARS       
/* 1816 */ 0x20, 0x7A, 0x19,   //          JSR   OUTCHT       
/* 1819 */ 0xCA,               //          DEX                
/* 181A */ 0xD0, 0xF8,         //          BNE   DUMPT1       
/* 181C */ 0xA9, 0x2A,         //          LDA   #$2A      ; START CHAR       
/* 181E */ 0x20, 0x7A, 0x19,   //          JSR   OUTCHT       
/* 1821 */ 0xAD, 0xF9, 0x17,   //          LDA   ID        ; OUTPUT ID        
/* 1824 */ 0x20, 0x61, 0x19,   //          JSR   OUTBT        
/* 1827 */ 0xAD, 0xF5, 0x17,   //          LDA   SAL       ; OUTPUT STARTING       
/* 182A */ 0x20, 0x5E, 0x19,   //          JSR   OUTBTC    ; ADDRESS        
/* 182D */ 0xAD, 0xF6, 0x17,   //          LDA   SAH          
/* 1830 */ 0x20, 0x5E, 0x19,   //          JSR   OUTBTC       
/* 1833 */ 0xAD, 0xED, 0x17,   //  DUMPT2  LDA   VEB+1     ; CHECK FOR LAST       
/* 1836 */ 0xCD, 0xF7, 0x17,   //          CMP   EAL       ; DATA BYTE       
/* 1839 */ 0xAD, 0xEE, 0x17,   //          LDA   VEB+2        
/* 183C */ 0xED, 0xF8, 0x17,   //          SBC   EAH          
/* 183F */ 0x90, 0x24,         //          BCC   DUMPT4       
/* 1841 */ 0xA9, 0x2F,         //          LDA   #'/'      ; OUTPUT END-OF-DATA CHAR       
/* 1843 */ 0x20, 0x7A, 0x19,   //          JSR   OUTCHT       
/* 1846 */ 0xAD, 0xE7, 0x17,   //          LDA   CHKL      ; LAST BYTE HAS BEEN        
/* 1849 */ 0x20, 0x61, 0x19,   //          JSR   OUTBT     ; OUTPUT  NOW OUTPUT       
/* 184C */ 0xAD, 0xE8, 0x17,   //          LDA   CHKH      ; CHKSUM       
/* 184F */ 0x20, 0x61, 0x19,   //          JSR   OUTBT        
/* 1852 */ 0xA2, 0x02,         //          LDX   #$02      ; 2 CHARS       
/* 1854 */ 0xA9, 0x04,         //  DUMPT3  LDA   #$04      ; EOT CHAR              
/* 1856 */ 0x20, 0x7A, 0x19,   //          JSR   OUTCHT        
/* 1859 */ 0xCA,               //          DEX                 
/* 185A */ 0xD0, 0xF8,         //          BNE   DUMPT3           
/* 185C */ 0xA9, 0x00,         //          LDA   #$00      ; DISPLAY 0000       
/* 185E */ 0x85, 0xFA,         //          STA   POINTL    ; FOR NORMAL EXIT       
/* 1860 */ 0x85, 0xFB,         //          STA   POINTH         
/* 1862 */ 0x4C, 0x4F, 0x1C,   //          JMP   START       
/* 1865 */ 0x20, 0xEC, 0x17,   //  DUMPT4  JSR   VEB       ; DATA BYTE OUTPUT       
/* 1868 */ 0x20, 0x5E, 0x19,   //          JSR   OUTBTC       
/* 186B */ 0x20, 0xEA, 0x19,   //          JSR   INCVEB       
/* 186E */ 0x4C, 0x33, 0x18,   //          JMP   DUMPT2       
/* 1871 */                     //  ;       
/* 1871 */                     //  ;       ** LOAD MEMORY FROM TAPE **       
/* 1871 */                     //  ;       
/* 1871 */ 0x0F, 0x19,         //  TAB     .WORD LOAD12    ; 'LOAD12' ADDRESS            1871       
/* 1873 */                     //  ;       
/* 1873 */ 0xA9, 0x8D,         //  LOADT   LDA   #$8D      ; INIT VOLATILE EXECUTION       1873       
/* 1875 */ 0x8D, 0xEC, 0x17,   //          STA   VEB       ; BLOCK WITH STA ABS.       
/* 1878 */ 0x20, 0x32, 0x19,   //          JSR   INTVEB       
/* 187B */ 0xA9, 0x4C,         //          LDA   #$4C      ; JUMP TYPE RTRN       
/* 187D */ 0x8D, 0xEF, 0x17,   //          STA   VEB+3       
/* 1880 */ 0xAD, 0x71, 0x18,   //          LDA   TAB         
/* 1883 */ 0x8D, 0xF0, 0x17,   //          STA   VEB+4       
/* 1886 */ 0xAD, 0x72, 0x18,   //          LDA   TAB+1       
/* 1889 */ 0x8D, 0xF1, 0x17,   //          STA   VEB+5       
/* 188C */ 0xA9, 0x07,         //          LDA   #$07      ; RESET PB5=0 (DATA-IN)       
/* 188E */ 0x8D, 0x42, 0x17,   //          STA   SBD       
/* 1891 */ 0xA9, 0xFF,         //  SYNC    LDA   #$FF      ; CLEAR SAVX FOR SYNC CHAR      1891       
/* 1893 */ 0x8D, 0xE9, 0x17,   //          STA   SAVX        
/* 1896 */ 0x20, 0x41, 0x1A,   //  SYNC1   JSR   RDBIT     ; GET A BIT               
/* 1899 */ 0x4E, 0xE9, 0x17,   //          LSR   SAVX      ; SHIFT BIT INTO CHAR       
/* 189C */ 0x0D, 0xE9, 0x17,   //          ORA   SAVX       
/* 189F */ 0x8D, 0xE9, 0x17,   //          STA   SAVX       
/* 18A2 */ 0xAD, 0xE9, 0x17,   //          LDA   SAVX      ; GET NEW CHAR       
/* 18A5 */ 0xC9, 0x16,         //          CMP   #$16      ; SYNC CHAR       
/* 18A7 */ 0xD0, 0xED,         //          BNE   SYNC1       
/* 18A9 */ 0xA2, 0x0A,         //          LDX   #$0A      ; TEST FOR 10 SYNC CHARS       
/* 18AB */ 0x20, 0x24, 0x1A,   //  SYNC2   JSR   RDCHT                             
/* 18AE */ 0xC9, 0x16,         //          CMP   #$16         
/* 18B0 */ 0xD0, 0xDF,         //          BNE   SYNC      ; IF NOT 10 CHAR, RE-SYNC       
/* 18B2 */ 0xCA,               //          DEX            
/* 18B3 */ 0xD0, 0xF6,         //          BNE   SYNC2       
/* 18B5 */ 0x20, 0x24, 0x1A,   //  LOADT4  JSR   RDCHT     ; LOOK FOR START OF       
/* 18B8 */ 0xC9, 0x2A,         //          CMP   #$2A      ; DATA CHAR       
/* 18BA */ 0xF0, 0x06,         //          BEQ   LOAD11       
/* 18BC */ 0xC9, 0x16,         //          CMP   #$16      ; IF NOT , SHOULD BE SYNC       
/* 18BE */ 0xD0, 0xD1,         //          BNE   SYNC       
/* 18C0 */ 0xF0, 0xF3,         //          BEQ   LOADT4       
/* 18C2 */ 0x20, 0xF3, 0x19,   //  LOAD11  JSR   RDBYT     ; READ ID FROM TAPE       
/* 18C5 */ 0xCD, 0xF9, 0x17,   //          CMP   ID        ; COMPARE WITH REQUESTED ID       
/* 18C8 */ 0xF0, 0x0D,         //          BEQ   LOADT5       
/* 18CA */ 0xAD, 0xF9, 0x17,   //          LDA   ID        ; DEFAULT 00, READ RECORD       
/* 18CD */ 0xC9, 0x00,         //          CMP   #$00      ; ANYWAY       
/* 18CF */ 0xF0, 0x06,         //          BEQ   LOADT5       
/* 18D1 */ 0xC9, 0xFF,         //          CMP   #$FF      ; DEFAULT FF, IGNORE SA ON       
/* 18D3 */ 0xF0, 0x17,         //          BEQ   LOADT6    ; TAPE       
/* 18D5 */ 0xD0, 0x9C,         //          BNE   LOADT       
/* 18D7 */ 0x20, 0xF3, 0x19,   //  LOADT5  JSR   RDBYT     ; GET SA FROM TAPE       
/* 18DA */ 0x20, 0x4C, 0x19,   //          JSR   CHKT       
/* 18DD */ 0x8D, 0xED, 0x17,   //          STA   VEB+1     ; SAVX IN VEB+1,2       
/* 18E0 */ 0x20, 0xF3, 0x19,   //          JSR   RDBYT       
/* 18E3 */ 0x20, 0x4C, 0x19,   //          JSR   CHKT       
/* 18E6 */ 0x8D, 0xEE, 0x17,   //          STA   VEB+2       
/* 18E9 */ 0x4C, 0xF8, 0x18,   //          JMP   LOADT7       
/* 18EC */                     //  ;       
/* 18EC */ 0x20, 0xF3, 0x19,   //  LOADT6  JSR   RDBYT     ; GET SA BUT IGNORE             18EC       
/* 18EF */ 0x20, 0x4C, 0x19,   //          JSR   CHKT       
/* 18F2 */ 0x20, 0xF3, 0x19,   //          JSR   RDBYT       
/* 18F5 */ 0x20, 0x4C, 0x19,   //          JSR   CHKT       
/* 18F8 */ 0xA2, 0x02,         //  LOADT7  LDX   #$02      ; GET 2 CHARS        
/* 18FA */ 0x20, 0x24, 0x1A,   //  LOAD13  JSR   RDCHT     ; GET CHAR (X)       
/* 18FD */ 0xC9, 0x2F,         //          CMP   #$2F      ; LOOK FOR LAST CHAR       
/* 18FF */ 0xF0, 0x14,         //          BEQ   LOADT8       
/* 1901 */ 0x20, 0x00, 0x1A,   //          JSR   PACKT     ; CONVERT TO HEX       
/* 1904 */ 0xD0, 0x23,         //          BNE   LOADT9    ; Y=1 NON-HEX CHAR       
/* 1906 */ 0xCA,               //          DEX                                
/* 1907 */ 0xD0, 0xF1,         //          BNE   LOAD13       
/* 1909 */ 0x20, 0x4C, 0x19,   //          JSR   CHKT      ; COMPARE CHECKSUM       
/* 190C */ 0x4C, 0xEC, 0x17,   //          JMP   VEB       ; SAVX DATA IN MEMORY       
/* 190F */ 0x20, 0xEA, 0x19,   //  LOAD12  JSR   INCVEB    ; INCR DATA POINTER       
/* 1912 */ 0x4C, 0xF8, 0x18,   //          JMP   LOADT7       
/* 1915 */                     //  ;       
/* 1915 */ 0x20, 0xF3, 0x19,   //  LOADT8  JSR   RDBYT     ; END OF DATA, COMPARE CHKSUM   1915       
/* 1918 */ 0xCD, 0xE7, 0x17,   //          CMP   CHKL       
/* 191B */ 0xD0, 0x0C,         //          BNE   LOADT9       
/* 191D */ 0x20, 0xF3, 0x19,   //          JSR   RDBYT       
/* 1920 */ 0xCD, 0xE8, 0x17,   //          CMP   CHKH       
/* 1923 */ 0xD0, 0x04,         //          BNE   LOADT9       
/* 1925 */ 0xA9, 0x00,         //          LDA   #$00      ; NORMAL EXIT       
/* 1927 */ 0xF0, 0x02,         //          BEQ   LOAD10       
/* 1929 */ 0xA9, 0xFF,         //  LOADT9  LDA   #$FF      ; ERROR EXIT        
/* 192B */ 0x85, 0xFA,         //  LOAD10  STA   POINTL                      
/* 192D */ 0x85, 0xFB,         //          STA   POINTH       
/* 192F */ 0x4C, 0x4F, 0x1C,   //          JMP   START       
/* 1932 */                     //  ;       
/* 1932 */                     //  ;       ** SUBROUTINES BELOW **       
/* 1932 */ 0xAD, 0xF5, 0x17,   //  INTVEB  LDA   SAL       ; MOVE SA TO VEB+1,2            1932       
/* 1935 */ 0x8D, 0xED, 0x17,   //          STA   VEB+1       
/* 1938 */ 0xAD, 0xF6, 0x17,   //          LDA   SAH       
/* 193B */ 0x8D, 0xEE, 0x17,   //          STA   VEB+2       
/* 193E */ 0xA9, 0x60,         //          LDA   #$60      ; RTS INST       
/* 1940 */ 0x8D, 0xEF, 0x17,   //          STA   VEB+3       
/* 1943 */ 0xA9, 0x00,         //          LDA   #$00      ; CLEAR CHKSUM AREA       
/* 1945 */ 0x8D, 0xE7, 0x17,   //          STA   CHKL       
/* 1948 */ 0x8D, 0xE8, 0x17,   //          STA   CHKH       
/* 194B */ 0x60,               //          RTS       
/* 194C */                     //  ;       ** COMPUTE CHKSUM FOR TAPE LOAD **       
/* 194C */ 0xA8,               //  CHKT    TAY             ;                               194C       
/* 194D */ 0x18,               //          CLC            
/* 194E */ 0x6D, 0xE7, 0x17,   //          ADC   CHKL       
/* 1951 */ 0x8D, 0xE7, 0x17,   //          STA   CHKL       
/* 1954 */ 0xAD, 0xE8, 0x17,   //          LDA   CHKH       
/* 1957 */ 0x69, 0x00,         //          ADC   #$00       
/* 1959 */ 0x8D, 0xE8, 0x17,   //          STA   CHKH       
/* 195C */ 0x98,               //          TYA              
/* 195D */ 0x60,               //          RTS        
/* 195E */                     //  ;       ** OUTPUT ONE BYTE **       
/* 195E */ 0x20, 0x4C, 0x19,   //  OUTBTC  JSR   CHKT      ; COMPARE CHKSUM                195E       
/* 1961 */ 0xA8,               //  OUTBT   TAY             ; SAVX DATA BYTE       
/* 1962 */ 0x4A,               //          LSR   A         ; SHIFT OFF LSD       
/* 1963 */ 0x4A,               //          LSR   A       
/* 1964 */ 0x4A,               //          LSR   A       
/* 1965 */ 0x4A,               //          LSR   A       
/* 1966 */ 0x20, 0x6F, 0x19,   //          JSR   HEXOUT    ; OUTPUT MSD       
/* 1969 */ 0x98,               //          TYA        
/* 196A */ 0x20, 0x6F, 0x19,   //          JSR   HEXOUT    ; OUTPUT LSD       
/* 196D */ 0x98,               //          TYA         
/* 196E */ 0x60,               //          RTS        
/* 196F */                     //  ;       ** CONVERT LSD OF A TO ASCII, OUTPUT TO TAPE **       
/* 196F */ 0x29, 0x0F,         //  HEXOUT  AND   #$0F      ;                               196F       
/* 1971 */ 0xC9, 0x0A,         //          CMP   #$0A             
/* 1973 */ 0x18,               //          CLC          
/* 1974 */ 0x30, 0x02,         //          BMI   HEX1       
/* 1976 */ 0x69, 0x07,         //          ADC   #$07       
/* 1978 */ 0x69, 0x30,         //  HEX1    ADC   #$30       
/* 197A */                     //  ;       ** OUTPUT TO TAPE ONE ASCII CHAR **       
/* 197A */ 0x8E, 0xE9, 0x17,   //  OUTCHT  STX   SAVX      ;                               197A       
/* 197D */ 0x8C, 0xEA, 0x17,   //          STY   SAVX+1       
/* 1980 */ 0xA0, 0x08,         //          LDY   #$08      ; START BIT       
/* 1982 */ 0x20, 0x9E, 0x19,   //  CHT1    JSR   ONE                         
/* 1985 */ 0x4A,               //          LSR   A         ; GET DATA BIT       
/* 1986 */ 0xB0, 0x06,         //          BCS   CHT2        
/* 1988 */ 0x20, 0x9E, 0x19,   //          JSR   ONE       ; DATA BIT=1       
/* 198B */ 0x4C, 0x91, 0x19,   //          JMP   CHT3       
/* 198E */ 0x20, 0xC4, 0x19,   //  CHT2    JSR   ZRO       ; DATA BIT=0        
/* 1991 */ 0x20, 0xC4, 0x19,   //  CHT3    JSR   ZRO                         
/* 1994 */ 0x88,               //          DEY       
/* 1995 */ 0xD0, 0xEB,         //          BNE   CHT1       
/* 1997 */ 0xAE, 0xE9, 0x17,   //          LDX   SAVX       
/* 199A */ 0xAC, 0xEA, 0x17,   //          LDY   SAVX+1       
/* 199D */ 0x60,               //          RTS       
/* 199E */                     //  ;       ** OUTPUT 1 TO TAPE, 9 PULSES, 138 US EACH **       
/* 199E */ 0xA2, 0x09,         //  ONE     LDX   #$09      ;                               199E       
/* 19A0 */ 0x48,               //          PHA             ; SAVX A       
/* 19A1 */ 0x2C, 0x47, 0x17,   //  ONE1    BIT   CLKRDI    ; WAIT FOR TIME OUT             19A1       
/* 19A4 */ 0x10, 0xFB,         //          BPL   ONE1       
/* 19A6 */ 0xA9, 0x7E,         //          LDA   #126       
/* 19A8 */ 0x8D, 0x44, 0x17,   //          STA   CLK1T       
/* 19AB */ 0xA9, 0xA7,         //          LDA   #$A7        
/* 19AD */ 0x8D, 0x42, 0x17,   //          STA   SBD       ; SET PB7 = 1       
/* 19B0 */ 0x2C, 0x47, 0x17,   //  ONE2    BIT   CLKRDI    ;                               19B0       
/* 19B3 */ 0x10, 0xFB,         //          BPL   ONE2        
/* 19B5 */ 0xA9, 0x7E,         //          LDA   #126       
/* 19B7 */ 0x8D, 0x44, 0x17,   //          STA   CLK1T       
/* 19BA */ 0xA9, 0x27,         //          LDA   #$27        
/* 19BC */ 0x8D, 0x42, 0x17,   //          STA   SBD       ; RESET PB7=0       
/* 19BF */ 0xCA,               //          DEX          
/* 19C0 */ 0xD0, 0xDF,         //          BNE   ONE1       
/* 19C2 */ 0x68,               //          PLA         
/* 19C3 */ 0x60,               //          RTS         
/* 19C4 */                     //  ;       ** OUTPUT 0 TO TAPE, 6 PULSES, 207 US EACH **       
/* 19C4 */ 0xA2, 0x06,         //  ZRO     LDX   #$06      ;                               19C4       
/* 19C6 */ 0x48,               //          PHA             ; SAVX A       
/* 19C7 */ 0x2C, 0x47, 0x17,   //  ZRO1    BIT   CLKRDI    ;                               19C7       
/* 19CA */ 0x10, 0xFB,         //          BPL   ZRO1        
/* 19CC */ 0xA9, 0xC3,         //          LDA   #$C3        
/* 19CE */ 0x8D, 0x44, 0x17,   //          STA   CLK1T       
/* 19D1 */ 0xA9, 0xA7,         //          LDA   #$A7        
/* 19D3 */ 0x8D, 0x42, 0x17,   //          STA   SBD       ; SET PB7=1       
/* 19D6 */ 0x2C, 0x47, 0x17,   //  ZRO2    BIT   CLKRDI       
/* 19D9 */ 0x10, 0xFB,         //          BPL   ZRO2        
/* 19DB */ 0xA9, 0xC3,         //          LDA   #195        
/* 19DD */ 0x8D, 0x44, 0x17,   //          STA   CLK1T       
/* 19E0 */ 0xA9, 0x27,         //          LDA   #$27        
/* 19E2 */ 0x8D, 0x42, 0x17,   //          STA   SBD       ; RESET PB7=0       
/* 19E5 */ 0xCA,               //          DEX               
/* 19E6 */ 0xD0, 0xDF,         //          BNE   ZRO1       
/* 19E8 */ 0x68,               //          PLA             ; RESTORE A       
/* 19E9 */ 0x60,               //          RTS       
/* 19EA */                     //  ;       ** SUB TO INC VEB+1,2 **       
/* 19EA */ 0xEE, 0xED, 0x17,   //  INCVEB  INC   VEB+1     ;                               19EA       
/* 19ED */ 0xD0, 0x03,         //          BNE   INCVE1       
/* 19EF */ 0xEE, 0xEE, 0x17,   //          INC   VEB+2       
/* 19F2 */ 0x60,               //  INCVE1  RTS            
/* 19F3 */                     //  ;       ** SUB TO READ BYTE FROM TAPE **       
/* 19F3 */ 0x20, 0x24, 0x1A,   //  RDBYT   JSR   RDCHT     ;                              19F3       
/* 19F6 */ 0x20, 0x00, 0x1A,   //          JSR   PACKT       
/* 19F9 */ 0x20, 0x24, 0x1A,   //          JSR   RDCHT       
/* 19FC */ 0x20, 0x00, 0x1A,   //          JSR   PACKT       
/* 19FF */ 0x60,               //          RTS       
/* 1A00 */                     //  ;       ** PACK A=ASCII INTO SAVX AS HEX DATA **       
/* 1A00 */ 0xC9, 0x30,         //  PACKT   CMP   #$30      ;                               1A00       
/* 1A02 */ 0x30, 0x1E,         //          BMI   PACKT3       
/* 1A04 */ 0xC9, 0x47,         //          CMP   #$47       
/* 1A06 */ 0x10, 0x1A,         //          BPL   PACKT3       
/* 1A08 */ 0xC9, 0x40,         //          CMP   #$40         
/* 1A0A */ 0x30, 0x03,         //          BMI   PACKT1       
/* 1A0C */ 0x18,               //          CLC                
/* 1A0D */ 0x69, 0x09,         //          ADC   #$09         
/* 1A0F */ 0x2A,               //  PACKT1  ROL   A       
/* 1A10 */ 0x2A,               //          ROL   A       
/* 1A11 */ 0x2A,               //          ROL   A       
/* 1A12 */ 0x2A,               //          ROL   A       
/* 1A13 */ 0xA0, 0x04,         //          LDY   #$04       
/* 1A15 */ 0x2A,               //  PACKT2  ROL   A       
/* 1A16 */ 0x2E, 0xE9, 0x17,   //          ROL   SAVX       
/* 1A19 */ 0x88,               //          DEY                 
/* 1A1A */ 0xD0, 0xF9,         //          BNE   PACKT2       
/* 1A1C */ 0xAD, 0xE9, 0x17,   //          LDA   SAVX       
/* 1A1F */ 0xA0, 0x00,         //          LDY   #$00      ; Y=0 VALID HEX CHAR       
/* 1A21 */ 0x60,               //          RTS       
/* 1A22 */ 0xC8,               //  PACKT3  INY             ; Y=1 NOT HEX       
/* 1A23 */ 0x60,               //          RTS       
/* 1A24 */                     //  ;       ** GET 1 CHAR FROM TAPE AND RETURN WITH        
/* 1A24 */                     //  ;          CHAR IN A.  USE SAVX+1 TO ASM CHAR  **       
/* 1A24 */ 0x8E, 0xEB, 0x17,   //  RDCHT   STX   SAVX+2    ;                              1A24       
/* 1A27 */ 0xA2, 0x08,         //          LDX   #$08      ; READ 8 BITS       
/* 1A29 */ 0x20, 0x41, 0x1A,   //  RDCHT1  JSR   RDBIT     ; GET NEXT DATA BIT       
/* 1A2C */ 0x4E, 0xEA, 0x17,   //          LSR   SAVX+1    ; RIGHT SHIFT CHAR       
/* 1A2F */ 0x0D, 0xEA, 0x17,   //          ORA   SAVX+1    ; OR IN SIGN BIT       
/* 1A32 */ 0x8D, 0xEA, 0x17,   //          STA   SAVX+1    ; REPLACE CHAR       
/* 1A35 */ 0xCA,               //          DEX                 
/* 1A36 */ 0xD0, 0xF1,         //          BNE   RDCHT1       
/* 1A38 */ 0xAD, 0xEA, 0x17,   //          LDA   SAVX+1    ; MOVE CHAR INTO A       
/* 1A3B */ 0x2A,               //          ROL   A         ; SHIFT OFF PARITY       
/* 1A3C */ 0x4A,               //          LSR   A       
/* 1A3D */ 0xAE, 0xEB, 0x17,   //          LDX   SAVX+2       
/* 1A40 */ 0x60,               //          RTS       
/* 1A41 */                     //  ;       ** THIS SUB GETS ONE BIT FROM TAPE AND       
/* 1A41 */                     //  ;          RETURNS IT IN SIGN OF A  **       
/* 1A41 */ 0x2C, 0x42, 0x17,   //  RDBIT   BIT   SBD       ; WAIT FOR END OF START BIT     1A41       
/* 1A44 */ 0x10, 0xFB,         //          BPL   RDBIT       
/* 1A46 */ 0xAD, 0x46, 0x17,   //          LDA   CLKRDT    ; GET START BIT TIME       
/* 1A49 */ 0xA0, 0xFF,         //          LDY   #$FF      ; A=256-T1           
/* 1A4B */ 0x8C, 0x46, 0x17,   //          STY   CLK64T    ; SET UP TIMER       
/* 1A4E */ 0xA0, 0x14,         //          LDY   #$14         
/* 1A50 */ 0x88,               //  RDBIT3  DEY             ; DELAY 100 MICRO SEC       
/* 1A51 */ 0xD0, 0xFD,         //          BNE   RDBIT3       
/* 1A53 */ 0x2C, 0x42, 0x17,   //  RDBIT2  BIT   SBD       
/* 1A56 */ 0x30, 0xFB,         //          BMI   RDBIT2    ; WAIT FOR NEXT START BIT       
/* 1A58 */ 0x38,               //          SEC          
/* 1A59 */ 0xED, 0x46, 0x17,   //          SBC   CLKRDT    ; (256-T1)-(256-T2)=T2-T1       
/* 1A5C */ 0xA0, 0xFF,         //          LDY   #$FF       
/* 1A5E */ 0x8C, 0x46, 0x17,   //          STY   CLK64T    ; SET UP TIMER FOR NEXT BIT       
/* 1A61 */ 0xA0, 0x07,         //          LDY   #$07       
/* 1A63 */ 0x88,               //  RDBIT4  DEY             ; DELAY 50 MICROSEC       
/* 1A64 */ 0xD0, 0xFD,         //          BNE   RDBIT4       
/* 1A66 */ 0x49, 0xFF,         //          EOR   #$FF      ; COMPLEMENT SIGN OF A       
/* 1A68 */ 0x29, 0x80,         //          AND   #$80      ; MASK ALL EXCEPT SIGN       
/* 1A6A */ 0x60,               //          RTS        
/* 1A6B */                     //  ;       ** PLLCAL OUTPUT 166 MICROSEC (6024 HZ)       
/* 1A6B */                     //  ;          PULSE STRING        
/* 1A6B */ 0xA9, 0x27,         //  PLLCAL  LDA   #$27      ;                              1A6B       
/* 1A6D */ 0x8D, 0x42, 0x17,   //          STA   SBD       ; TURN OFF DATIN PB5=1       
/* 1A70 */ 0xA9, 0xBF,         //          LDA   #$BF      ; CONVERT PB7 TO OUTPUT       
/* 1A72 */ 0x8D, 0x43, 0x17,   //          STA   PBDD       
/* 1A75 */ 0x2C, 0x47, 0x17,   //  PLL1    BIT   CLKRDI       
/* 1A78 */ 0x10, 0xFB,         //          BPL   PLL1         
/* 1A7A */ 0xA9, 0x9A,         //          LDA   #154      ; WAIT 166 MICROSEC       
/* 1A7C */ 0x8D, 0x44, 0x17,   //          STA   CLK1T       
/* 1A7F */ 0xA9, 0xA7,         //          LDA   #$A7      ; OUTPUT PB7=1       
/* 1A81 */ 0x8D, 0x42, 0x17,   //          STA   SBD       
/* 1A84 */ 0x2C, 0x47, 0x17,   //  PLL2    BIT   CLKRDI       
/* 1A87 */ 0x10, 0xFB,         //          BPL   PLL2        
/* 1A89 */ 0xA9, 0x9A,         //          LDA   #154       
/* 1A8B */ 0x8D, 0x44, 0x17,   //          STA   CLK1T       
/* 1A8E */ 0xA9, 0x27,         //          LDA   #$27      ; PB7=0       
/* 1A90 */ 0x8D, 0x42, 0x17,   //          STA   SBD       
/* 1A93 */ 0x4C, 0x75, 0x1A,   //          JMP   PLL1       
/* 1A96 */                     //         
/* 1A96 */                     //  ; Fill unused locations with $FF       
/* 1A96 */ 0xFF, 0xFF, 0xFF, 0xFF,  // .FILL 356, $FF
/* 1A9A */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1A9E */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1AA2 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1AA6 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1AAA */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1AAE */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1AB2 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1AB6 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1ABA */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1ABE */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1AC2 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1AC6 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1ACA */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1ACE */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1AD2 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1AD6 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1ADA */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1ADE */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1AE2 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1AE6 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1AEA */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1AEE */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1AF2 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1AF6 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1AFA */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1AFE */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B02 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B06 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B0A */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B0E */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B12 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B16 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B1A */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B1E */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B22 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B26 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B2A */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B2E */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B32 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B36 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B3A */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B3E */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B42 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B46 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B4A */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B4E */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B52 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B56 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B5A */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B5E */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B62 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B66 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B6A */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B6E */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B72 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B76 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B7A */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B7E */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B82 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B86 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B8A */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B8E */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B92 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B96 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B9A */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1B9E */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1BA2 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1BA6 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1BAA */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1BAE */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1BB2 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1BB6 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1BBA */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1BBE */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1BC2 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1BC6 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1BCA */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1BCE */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1BD2 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1BD6 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1BDA */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1BDE */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1BE2 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1BE6 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1BEA */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1BEE */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1BF2 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1BF6 */ 0xFF, 0xFF, 0xFF, 0xFF,  // 
/* 1BFA */                     //  ;       
/* 1BFA */                     //  ;       ** INTERRUPTS **       
/* 1BFA */                     //          .org  $1BFA       
/* 1BFA */ 0x6B, 0x1A,         //  NMIP27  .WORD PLLCAL       
/* 1BFC */ 0x6B, 0x1A,         //  RSTP27  .WORD PLLCAL       
/* 1BFE */ 0x6B, 0x1A          //  IRQP27  .WORD PLLCAL  
};

