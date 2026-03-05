/*
 * usart.h
 *
 *  Created on: Feb 13, 2026
 *      Author: iman
 */
#include "stm32f401xe.h"
#include "systick.h"
#include <stdbool.h>

#ifndef USART_H_
#define USART_H_

#define APB1CLK SYSCLK

#define USART2EN (1UL << 17)
#define USART6EN (1UL << 5)
#define USART1EN (1UL << 4)

#define ALTFUNC0 0
#define ALTFUNC1 1
#define ALTFUNC2 2
#define ALTFUNC3 3
#define ALTFUNC4 4
#define ALTFUNC5 5
#define ALTFUNC6 6
#define ALTFUNC7 7
#define ALTFUNC8 8
#define ALTFUNC9 9
#define ALTFUNC10 10
#define ALTFUNC11 11
#define ALTFUNC12 12
#define ALTFUNC13 13
#define ALTFUNC14 14
#define ALTFUNC15 15

void enable_int(void);

void disable_int(void);

void uart2_putchar_int(char charw);

void uart_txint(void);

void uart_putchar(char *sr);

void uart_putchar_integer(int n);

void uart2int_putchar(char charw);
void wait_empty_buff(void);
void wait_empty_buff6(void);



void uart2_tx (void);

void usart_init (USART_TypeDef *USARTx, GPIO_TypeDef *GPIOx, uint8_t PIN, uint8_t AF);

void uart2_putcharpoll(char charw);


void uart2_transmit(uint8_t *buff, uint16_t len); //usage in OTA


void uart6_putchar_int(char charw);

void uart6_txint(void);

void uart6_putchar(char *sr);

void uart6_putchar_integer(int n);




/*--------------------Receive function-------------------*/

void uart2rx_init(void);

void uart2_read_int(void);

bool uart2_read(uint8_t *charw);

bool uart2_receive(uint8_t *buffer, uint16_t idx); //usage in OTA

#endif /* USART_H_ */
