/*
 * myprintf.c
 *
 *  Created on: Feb 13, 2026
 *      Author: iman
 */


#ifndef MYPRINTF_C_
#define MYPRINTF_C_


#include "myprintf.h"

void myprintf_init(void){
    // 1. Enable clocks
    gpio_rcc(GPIOA);

    gpio_init(GPIOA, PIN11, ALTFUNC);

    usart_init(USART6, GPIOA, PIN11, ALTFUNC8);

    uart6_txint();
}

void myprintf(char *sr, ...){
	va_list args;
	va_start (args, sr);

	for (int i = 0; sr[i] != '\0'; i++){

		if (sr[i] == '%'){
			i++;

			if (sr[i] == 'c'){
				char c = (char)va_arg(args, int);
				uart6_putchar_int(c);
			}
			else if (sr[i] == 'd'){
				int d = va_arg(args, int);
				uart6_putchar_integer(d);
			}
			else if (sr[i] == 's'){
				char *word = va_arg(args, char*);
				uart6_putchar(word);
			}
			else{
				uart6_putchar_int(sr[i]);
			}
		}
		else if (sr[i] == '\n'){
			uart6_putchar_int('\n');
			uart6_putchar_int('\r');
		}
		else{

			uart6_putchar_int(sr[i]);

		}


	}
	va_end(args);
}


#endif /* MYPRINTF_C_ */
