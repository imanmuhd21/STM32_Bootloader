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

    gpio_init(GPIOA, PIN2, ALTFUNC);

    usart_init(USART2, GPIOA, PIN2, ALTFUNC7);
    usart_rcc(USART2);

    uart_txint();
}

void myprintf(char *sr, ...){
	va_list args;
	va_start (args, sr);

	for (int i = 0; sr[i] != '\0'; i++){

		if (sr[i] == '%'){
			i++;

			if (sr[i] == 'c'){
				char c = (char)va_arg(args, int);
				uart2_putchar_int(c);
			}
			else if (sr[i] == 'd'){
				int d = va_arg(args, int);
				uart_putchar_integer(d);
			}
			else if (sr[i] == 's'){
				char *word = va_arg(args, char*);
				uart_putchar(word);
			}
			else{
				uart2_putchar_int(sr[i]);
			}
		}
		else if (sr[i] == '\n'){
			uart2_putchar_int('\n');
			uart2_putchar_int('\r');
		}
		else{

			uart2_putchar_int(sr[i]);

		}


	}
	va_end(args);
}


#endif /* MYPRINTF_C_ */
