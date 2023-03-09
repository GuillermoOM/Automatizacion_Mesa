/*
 Hecho por: Guillermo Ochoa

TODO:
 * checar distancias y resolucion
 */

#define _XTAL_FREQ 8000000
#include "confbitspic.h"
#include <xc.h>
#include <htc.h>
#include <stdio.h>
#include <math.h>
#include <p18f4550.h>
#include "UART.h"

#define rs LATDbits.LATD1
#define en LATDbits.LATD0
#define lcdport LATB

int steps = 0; //NO BORRAR VARIABLE

unsigned char leer_teclado() {
    unsigned char tecla = 0;
    LATB = 0;
    TRISB = 0b00001111;
    for (int j = 0; j < 4; j++) {
        LATB7 = 1;
        __delay_ms(5);
        if (PORTBbits.RB0) { //A
            tecla = 'A';
            while (PORTBbits.RB0);
            break;
        }
        if (PORTBbits.RB1) { //3
            tecla = '3';
            while (PORTBbits.RB1);
            break;
        }
        if (PORTBbits.RB2) { //2
            tecla = '2';
            while (PORTBbits.RB2);
            break;
        }
        if (PORTBbits.RB3) { //1
            tecla = '1';
            while (PORTBbits.RB3);
            break;
        }
        LATB7 = 0;
        LATB6 = 1;
        __delay_ms(5);
        if (PORTBbits.RB0) { //B
            tecla = 'B';
            while (PORTBbits.RB0);
            break;
        }
        if (PORTBbits.RB1) { //6
            tecla = '6';
            while (PORTBbits.RB1);
            break;
        }
        if (PORTBbits.RB2) { //5
            tecla = '5';
            while (PORTBbits.RB2);
            break;
        }
        if (PORTBbits.RB3) { //4
            tecla = '4';
            while (PORTBbits.RB3);
            break;
        }
        LATB6 = 0;
        LATB5 = 1;
        __delay_ms(5);
        if (PORTBbits.RB0) { //C
            tecla = 'C';
            while (PORTBbits.RB0);
            break;
        }
        if (PORTBbits.RB1) { //9
            tecla = '9';
            while (PORTBbits.RB1);
            break;
        }
        if (PORTBbits.RB2) { //8
            tecla = '8';
            while (PORTBbits.RB2);
            break;
        }
        if (PORTBbits.RB3) { //7
            tecla = '7';
            while (PORTBbits.RB3);
            break;
        }
        LATB5 = 0;
        LATB4 = 1;
        __delay_ms(5);
        if (PORTBbits.RB0) { //D
            tecla = 'D';
            while (PORTBbits.RB0);
            break;
        }
        if (PORTBbits.RB1) { //#
            tecla = '#';
            while (PORTBbits.RB1);
            break;
        }
        if (PORTBbits.RB2) { //0
            tecla = '0';
            while (PORTBbits.RB2);
            break;
        }
        if (PORTBbits.RB3) { //*
            tecla = '*';
            while (PORTBbits.RB3);
            break;
        }
        LATB4 = 0;
        __delay_ms(5);
    }
    LATB = 0;
    TRISB = 0;
    return tecla;
}

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

void lcdclear() {
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

void lcdshiftCursor(unsigned char value) {
    if (value == 'L')lcdcmd(0b00010000); //LEFT
    if (value == 'R')lcdcmd(0b10010100); //RIGHT
    __delay_ms(2);
}

void lcdReturn() {
    lcdcmd(0b00000010); //Return Home
    __delay_ms(2);
}

void *calibracion(char *Input) {
    char *stepx = Input;
    int i = 0;
    lcdclear();
    lcdtext("Calibrando...");
    UART_Write('c');
    while (!UART_Data_Ready()) {
        if (leer_teclado() == 'B') {
            UART_Write('s');
            break;
        }
    }
    switch (UART_Read()) {
        case 'd':
            do {
                stepx[i] = UART_Read();
                i += 1;
            } while (stepx[i - 1] != 'd');
            stepx[i - 1] = '\0';
            lcdclear();
            lcdtext("Calibracion");
            lcdsetCursor(0x40);
            lcdtext("Exitosa");
            __delay_ms(2000);
            lcdclear();
            lcdtext("Distancia:");
            lcdsetCursor(0x40);
            lcdtext(stepx);
            lcdtext(" mm");
            __delay_ms(2000);
            break;
        case 'e':
            lcdclear();
            lcdtext("Cancelado...");
            break;
        case '\0':
            lcdclear();
            lcdtext("Error!");
            break;
    }
    __delay_ms(2000);
    return *stepx;
}

void trabajo() {
    unsigned char num = 0;
    unsigned char cuts = 0;
    char cutx[20];
    char mx[20];
    char outx[20];
    int distancias[50];
    lcdclear();
    lcdtext("Ingrese Numero");
    lcdsetCursor(0x40);
    lcdtext("de Cortes:");
    while (num != 'A') { //esperando a aceptar
        num = leer_teclado();
        if (num >= '0' && num <= '9') { //multiplicar por 10 y agregar unidad
            if (!(cuts == 0 && num == '0')) {
                lcdwrite(num);
                cuts *= 10;
                cuts += num - '0';
            }
        }
        if ((num == 'C') && (cuts > 0)) { //borrar una unidad
            lcdshiftCursor('L');
            lcdwrite(' ');
            lcdshiftCursor('L');
            cuts /= 10;
            trunc(cuts);
        }
        if (num == 'B')break; //cancelar operacion
    }
    if (num == 'B') { //volviendo al menu
        lcdclear();
        lcdtext("Operacion");
        lcdsetCursor(0x40);
        lcdtext("Cancelada");
        __delay_ms(2000);
    } else { //proceder a distancias ------------------------------------
        sprintf(cutx, "%d", cuts);
        for (int m = 0; m < cuts; m++) {
            distancias[m] = 0;
            num = 0;
            lcdclear();
            lcdtext("Corte ");
            lcdsetCursor(0x06);
            sprintf(mx, "%d", m + 1);
            lcdtext(mx);
            lcdtext("/");
            lcdtext(cutx);
            lcdsetCursor(0x40);
            lcdtext("Distancia: ");
            while (num != 'A') { //esperando a aceptar
                num = 0;
                num = leer_teclado();
                if (num >= '0' && num <= '9') { //multiplicar por 10 y agregar unidad
                    if (!(distancias[m] == 0 && num == '0')) {
                        lcdwrite(num);
                        distancias[m] = 10 * distancias[m] + num - '0';
                        if (distancias[m] > (steps)) {
                            lcdclear();
                            lcdtext("Limite excedido");
                            distancias[m] = 0;
                            __delay_ms(2000);
                            lcdclear();
                            lcdtext("Corte ");
                            lcdsetCursor(0x06);
                            sprintf(mx, "%d", m + 1);
                            lcdtext(mx);
                            lcdtext("/");
                            lcdtext(cutx);
                            lcdsetCursor(0x40);
                            lcdtext("Distancia: ");
                        }
                    }
                }
                if ((num == 'C') && (distancias[m] > 0)) { //borrar una unidad
                    lcdshiftCursor('L');
                    lcdwrite(' ');
                    lcdshiftCursor('L');
                    distancias[m] /= 10;
                    trunc(distancias[m]);
                }
                if (num == 'B')break; //cancelar operacion
            }
            if (num == 'B')break;
        }
        if (num == 'B') { //volviendo al menu
            lcdclear();
            lcdtext("Operacion");
            lcdsetCursor(0x40);
            lcdtext("Cancelada");
            __delay_ms(2000);
        } else { //proceder a cortar -----------------------------------
            lcdclear();
            lcdtext("Corte ");
            for (int m = 0; m < cuts; m++) {
                lcdsetCursor(0x06);
                sprintf(mx, "%d", m + 1);
                lcdtext(mx);
                lcdtext("/");
                lcdtext(cutx);
                UART_Write('a');
                //UART Enviar distancia del corte
                sprintf(outx, "%d", (distancias[m] - 353 + 142));
                for (int i = 0; outx[i] != '\0'; i++) {
                    __delay_ms(100);
                    UART_Write(outx[i]);
                    outx[i] = '\0';
                }
                __delay_ms(100);
                UART_Write('a');
                lcdsetCursor(0x40);
                lcdtext("Moviendo...         ");
                while (!UART_Data_Ready()) {
                    num = leer_teclado();
                    if (num == 'B') {
                        UART_Write('s');
                        break;
                    }
                }
                if (num != 'B') {
                    if (UART_Read() == 'd') {
                        lcdsetCursor(0x40);
                        lcdtext("Listo          ");
                        __delay_ms(2000);
                    } else {
                        lcdclear();
                        lcdtext("Error!         ");
                        __delay_ms(2000);
                    }
                    num = 0;
                    if (m < cuts) {
                        if ((m + 1) == cuts) {
                            lcdsetCursor(0x40);
                            lcdtext("Terminar (A)");
                        } else {
                            lcdsetCursor(0x40);
                            lcdtext("Siguiente (A)");
                        }
                    }
                    while (num != 'A') {
                        num = leer_teclado();
                        if (num == 'B')break;
                    }
                }
                if (num == 'B')break;
            }
            if (num == 'B') {
                while (UART_Read() != 'e');
                lcdclear();
                lcdtext("Cancelado...");
                __delay_ms(2000);
            } else {
                lcdclear();
                lcdtext("Terminado...");
                __delay_ms(2000);
            }
        }
    }
}

void menu() { //selección de comandos
    const char *comandos = "   1)Trabajo 2)Calibracion"; //lista de comandos
    int l;
    char stepx[10];
    unsigned char c = 0; //seleccion del teclado
    for (l = 0; comandos[l] != '\0'; l++); //longitud de string comandos
    lcdclear();
    lcdtext("Elija un Comando:");
    while (c == 0) { //Esperar Comando, escribir en LCD (scrolling text)
        for (int i = 0; comandos[i + 13] != '\0'; i++) { //i + 13 para recorrido
            lcdsetCursor(0x40);
            lcdtext(comandos + i);
            lcdsetCursor(0x40 + l - i);
            lcdwrite(' ');
            c = leer_teclado();
            switch (c) {
                case '1':
                    if (steps != 0)trabajo();
                    else {
                        lcdclear();
                        lcdtext("Ejecutar");
                        lcdsetCursor(0x40);
                        lcdtext("Calibracion!");
                        __delay_ms(2000);
                    }
                    break;
                case '2': //Calibración
                    steps = 0;
                    *stepx = calibracion(stepx);
                    for (int k = 0; stepx[k] != '\0'; k++) {
                        steps = steps * 10 + stepx[k] - '0';
                    }
                    break;
                default:
                    c = 0;
                    break;
            }
            if (c != 0)break;
        }
        if (c != 0)break;
    }
}

void main(void) {
    OSCCON = 0b01111110;
    TRISB = 0;
    TRISDbits.RD0 = 0;
    TRISDbits.RD1 = 0;
    lcdinitialize();
    UART_Init(9600);
    lcdtext("   Bienvenido");
    __delay_ms(2000);
    while (1)menu();
}
