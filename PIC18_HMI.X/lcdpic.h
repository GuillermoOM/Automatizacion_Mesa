#define rs LATDbits.LATD1
#define en LATDbits.LATD0
#define lcdport LATB

void lcdcmd(unsigned char cmdout) {
    lcdport = cmdout;
    rs = 0;
    en = 1;
    __delay_ms(10);
    en = 0;
}

void lcdinitialize() {
    __delay_ms(50);
    lcdcmd(0b00111000); //Function set
    lcdcmd(0b00001111); //Screen On
    lcdcmd(0b00000010); //Return Home
    lcdcmd(0b00000001); //Clear Display
}

void lcdclear(){
    lcdcmd(0b00000001); //Clear Display
    __delay_ms(2);
}

void lcdwrite(char dataout) {
    lcdport = dataout;
    rs = 1;
    en = 1;
    __delay_ms(10);
    en = 0;
}

void lcdtext(char *t) {
    for (int i = 0; t[i] != '\0'; i++) {
        lcdwrite(t[i]);
    }
}

void lcdsetCursor(unsigned char value) {
    lcdcmd(0b10000000 | value); //Set DDRAM Address
    __delay_ms(2);
}