#define _XTAL_FREQ 8000000
#include "confbitspic.h"
#include <xc.h>
#include <htc.h>
#include <stdio.h>
#include <p18f4550.h>
#include <math.h>
#include "UART.h"

void step() {
    PORTDbits.RD1 = 1;
    __delay_us(1200);
    PORTDbits.RD1 = 0;
    __delay_us(1200);
}

char stopcheck() {
    if (UART_Data_Ready()) {
        if (UART_Read() == 's') {
            return 1;
        } else return 0;
    }
    return 0;
}
//boton a tope 3364
//quitar 353
void main(void) {
    int i = 0;
    float steps = 0;
    int move = 0;
    float pos = 0;
    float resolucion = 0.59375;
    int dir = 0;
    char stop = 0;
    char stepx[10];
    OSCCON = 0b01111110;
    TRISDbits.RD3 = 1;
    TRISDbits.RD2 = 1;
    TRISDbits.RD1 = 0;
    TRISDbits.RD0 = 0;
    UART_Init(9600);

    while (1) {
        if (UART_Data_Ready()) {
            switch (UART_Read()) {
                case 'c':
                    steps = 0;
                    PORTDbits.RD0 = 1; //hacia sierra
                    while (!PORTDbits.RD2) {
                        if (stopcheck()) {
                            stop = 1;
                            break;
                        }
                        step();
                    }
                    PORTDbits.RD0 = 0; //desde sierra
                    if (!stop) {
                        while (!PORTDbits.RD3) {
                            if (stopcheck()) {
                                stop = 1;
                                break;
                            }
                            step();
                            steps += resolucion;
                        }
                        if (!stop) {
                            UART_Write('d');
                            trunc(steps);
                            sprintf(stepx, "%d", ((int)steps + 353 - 43));
                            for (int u = 0; stepx[u] != '\0'; u++) {
                                __delay_ms(100);
                                UART_Write(stepx[u]);
                                stepx[u] = '\0';
                            }
                            __delay_ms(100);
                            UART_Write('d');
                            pos = steps;
                        }
                    }
                    if (stop)UART_Write('e');
                    stop = 0;
                    break;
                case 'a':
                    i = 0;
                    move = 0;
                    do { //leer string de numero
                        stepx[i] = UART_Read();
                        i += 1;
                    } while (stepx[i - 1] != 'a');
                    stepx[i - 1] = '\0';
                    for (int k = 0; stepx[k] != '\0'; k++) { //convertir a int
                        move = move * 10 + stepx[k] - '0';
                    }
                    if (move < pos) {
                        PORTDbits.RD0 = 1;
                        dir = 0;
                    }
                    if (move > pos) {
                        PORTDbits.RD0 = 0;
                        dir = 1;
                    }
                    switch (dir) {
                        case 1:
                            while (move > pos) {
                                step();
                                pos += resolucion * 1;
                                if (stopcheck()) {
                                    stop = 1;
                                    break;
                                }
                            }
                            break;
                        case 0:
                            while(move < pos){
                                step();
                                pos += resolucion * -1;
                                if (stopcheck()) {
                                    stop = 1;
                                    break;
                                }
                            }
                            PORTDbits.RD0 = 0;
                            step();
                            break;
                        default:
                            break;
                    }
                    dir = 0;
                    if (!stop)UART_Write('d');
                    if (stop)UART_Write('e');
                    stop = 0;
                    break;
            }
        }
    }
}