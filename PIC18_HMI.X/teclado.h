unsigned char leer_teclado(){
	unsigned char tecla;
	while(1){
		LATB7 = 1;
		__delay_ms(5);
		if (PORTBbits.RB0){ //A
		tecla = 'A';
		while (PORTBbits.RB0);
		break;
		}
		if (PORTBbits.RB1){ //3
		tecla = '3';
		while (PORTBbits.RB1);
		break;
		}
		if (PORTBbits.RB2){ //2
		tecla = '2';
		while (PORTBbits.RB2);
		break;
		}
		if (PORTBbits.RB3){ //1
		tecla = '1';
		while (PORTBbits.RB3);
		break;
		}
		LATB7 = 0;
		LATB6 = 1;
		__delay_ms(5);
		if (PORTBbits.RB0){ //B
		tecla = 'B';
		while (PORTBbits.RB0);
		break;
		}
		if (PORTBbits.RB1){ //6
		tecla = '6';
		while (PORTBbits.RB1);
		break;
		}
		if (PORTBbits.RB2){ //5
		tecla = '5';
		while (PORTBbits.RB2);
		break;
		}
		if (PORTBbits.RB3){ //4
		tecla = '4';
		while (PORTBbits.RB3);
		break;
		}
		LATB6 = 0;
		LATB5 = 1;
		__delay_ms(5);
		if (PORTBbits.RB0){ //C
		tecla = 'C';
		while (PORTBbits.RB0);
		break;
		}
		if (PORTBbits.RB1){ //9
		tecla = '9';
		while (PORTBbits.RB1);
		break;
		}
		if (PORTBbits.RB2){ //8
		tecla = '8';
		while (PORTBbits.RB2);
		break;
		}
		if (PORTBbits.RB3){ //7
		tecla = '7';
		while (PORTBbits.RB3);
		break;
		}
		LATB5 = 0;
		LATB4 = 1;
		__delay_ms(5);
		if (PORTBbits.RB0){ //D
		tecla = 'D';
		while (PORTBbits.RB0);
		break;
		}
		if (PORTBbits.RB1){ //#
		tecla = '#';
		while (PORTBbits.RB1);
		break;
		}
		if (PORTBbits.RB2){ //0
		tecla = '0';
		while (PORTBbits.RB2);
		break;
		}
		if (PORTBbits.RB3){ //*
		tecla = '*';
		while (PORTBbits.RB3);
		break;
		}
		LATB4 = 0;
		__delay_ms(5);
	}
	return tecla;
}