/*
 * usart.c
 *
 *  Created on: Feb 13, 2026
 *      Author: iman
 */

#include "usart.h"
#include "ring_buffer.h"

#define BUFFER_SIZE 16

static uint8_t tx6_rb_mem[RINGBUFFER_OBJ_SIZE];
static uint8_t tx_rb_mem[RINGBUFFER_OBJ_SIZE];

static uint8_t rx6_rb_mem[RINGBUFFER_OBJ_SIZE];
static uint8_t rx_rb_mem[RINGBUFFER_OBJ_SIZE];

static uint8_t tx6_array[BUFFER_SIZE];
static uint8_t tx_array[BUFFER_SIZE];

static ringbuffer *tx6_buff;
static ringbuffer *tx_buff;

static uint8_t rx6_array[BUFFER_SIZE];
static uint8_t rx_array[BUFFER_SIZE];

static ringbuffer *rx6_buff;
static ringbuffer *rx_buff;

volatile bool rx_on = false;
volatile bool rx_event = false;
volatile bool rx6_on = false;
volatile bool rx6_event = false;

volatile uint32_t usart2_isr_hit = 0;
volatile uint32_t rxusart2_isr_hit = 0;
volatile uint32_t usart6_isr_hit = 0;
volatile uint32_t rxusart6_isr_hit = 0;


volatile uint32_t hit_empty = 0;
volatile uint32_t hit_not_empty = 0;

volatile uint32_t usart6loopforever = 0;
volatile uint32_t usart2loopforever = 0;

volatile bool RXNE6 = false;
volatile bool RXNE = false;

volatile uint32_t i6 = 0;
volatile uint32_t i = 0;


volatile uint32_t r = 0;
volatile uint32_t txe_int = 0;

volatile bool tx6_event = false;
volatile bool tx_event = false;

volatile uint32_t myprinth = 0;

void disable_int(void){

	USART2->CR1 &= ~(1UL << 7);

}


void enable_int(void){
	USART2->CR1 &= ~(1UL << 13);
	USART2->CR1 |= (1UL << 7);
	USART2->CR1 |= (1UL << 13);
}

void enable_intrx(void){
	USART2->CR1 |= (1UL << 5);
}

void disable_intrx(void){
	USART2->CR1 &= ~(1UL << 13);
	USART2->CR1 &= ~(1UL << 5);
	USART2->CR1 |= (1UL << 13);
}


void disable_int6(void){

	USART6->CR1 &= ~(1UL << 7);

}


void enable_int6(void){
	USART6->CR1 &= ~(1UL << 13);
	USART6->CR1 |= (1UL << 7);
	USART6->CR1 |= (1UL << 13);
}

void enable_intrx6(void){
	USART6->CR1 |= (1UL << 5);
}

void disable_intrx6(void){
	USART6->CR1 &= ~(1UL << 13);
	USART6->CR1 &= ~(1UL << 5);
	USART6->CR1 |= (1UL << 13);
}


void USART2_IRQHandler(void){

	if ((USART2->SR & (1UL << 7)) && (USART2->CR1 & (1UL << 7))){
		usart2_isr_hit++;
		if (empty_ring_buff(tx_buff)){
			usart2loopforever++;
		 		while(1);
		}

		get_ring_buff(tx_buff);

		if(!(empty_ring_buff((tx_buff)))){
			if (peek_ring_buff(tx_buff) == '\n'){
					tx_event = true;
			}
			USART2->DR = peek_ring_buff(tx_buff);

		}
		//if didnt have this if, will have bug issue
		if (!(tailplus_ring_buff(tx_buff)) && (guard_ring_buff(tx_buff))){
			disable_int();
		}
	}

	if ((USART2->SR & (1UL << 5)) && (USART2->CR1 & (1UL << 5))){
		rxusart2_isr_hit++;

		if (full_ring_buff(rx_buff)){
			while(1);
		}

		if(!(full_ring_buff(rx_buff))){
			RXNE = true;
			rx_event = true;
			char c = USART2->DR;          // clears RXNE
			put_ring_buff(rx_buff, c);


		}


	}


}


void USART6_IRQHandler(void){

	if ((USART6->SR & (1UL << 7)) && (USART6->CR1 & (1UL << 7))){
		usart6_isr_hit++;
		if (empty_ring_buff(tx6_buff)){
			usart6loopforever++;
		 		while(1);
		}

		get_ring_buff(tx6_buff);

		if(!(empty_ring_buff((tx6_buff)))){
			if (peek_ring_buff(tx6_buff) == '\n'){
					tx6_event = true;
			}
			USART6->DR = peek_ring_buff(tx6_buff);

		}
		//if didnt have this if, will have bug issue
		if (!(tailplus_ring_buff(tx6_buff)) && (guard_ring_buff(tx6_buff))){
			disable_int6();
		}
	}

	if ((USART6->SR & (1UL << 5)) && (USART6->CR1 & (1UL << 5))){
		rxusart6_isr_hit++;

		if (full_ring_buff(rx6_buff)){
			while(1);
		}

		if(!(full_ring_buff(rx6_buff))){
			RXNE6 = true;
			rx6_event = true;
			char c = USART6->DR;          // clears RXNE
			put_ring_buff(rx6_buff, c);


		}


	}


}


void usart_rcc(USART_TypeDef *USARTx){

	if (USARTx == USART2){
		RCC->APB1ENR |= USART2EN;
	}
	else if (USARTx == USART6){
		RCC->APB2ENR |= USART6EN;
	}
	else{
		RCC->APB2ENR |= USART1EN;
	}


}

void altfunc(GPIO_TypeDef *GPIOx, uint8_t PIN, uint8_t AF){

	if (PIN >= 8){
		switch (AF){

		case ALTFUNC0:
			GPIOx->AFR[1] &= ~(0xFUL << ((PIN-8)*4));
			GPIOx->AFR[1] |= ((ALTFUNC0) << ((PIN-8)*4));
			break;

		case ALTFUNC1:
			GPIOx->AFR[1] &= ~(0xFUL << ((PIN-8)*4));
			GPIOx->AFR[1] |= ((ALTFUNC1) << ((PIN-8)*4));
			break;

		case ALTFUNC2:
			GPIOx->AFR[1] &= ~(0xFUL << ((PIN-8)*4));
			GPIOx->AFR[1] |= ((ALTFUNC2) << ((PIN-8)*4));
			break;
		case ALTFUNC3:
			GPIOx->AFR[1] &= ~(0xFUL << ((PIN-8)*4));
			GPIOx->AFR[1] |= ((ALTFUNC3) << ((PIN-8)*4));
			break;

		case ALTFUNC4:
			GPIOx->AFR[1] &= ~(0xFUL << ((PIN-8)*4));
			GPIOx->AFR[1] |= ((ALTFUNC4) << ((PIN-8)*4));
			break;

		case ALTFUNC5:
			GPIOx->AFR[1] &= ~(0xFUL << ((PIN-8)*4));
			GPIOx->AFR[1] |= ((ALTFUNC5) << ((PIN-8)*4));
			break;

		case ALTFUNC6:
			GPIOx->AFR[1] &= ~(0xFUL << ((PIN-8)*4));
			GPIOx->AFR[1] |= ((ALTFUNC6) << ((PIN-8)*4));
			break;

		case ALTFUNC7:
			GPIOx->AFR[1] &= ~(0xFUL << ((PIN-8)*4));
			GPIOx->AFR[1] |= ((ALTFUNC7) << ((PIN-8)*4));
			break;

		case ALTFUNC8:
			GPIOx->AFR[1] &= ~(0xFUL << ((PIN-8)*4));
			GPIOx->AFR[1] |= ((ALTFUNC8) << ((PIN-8)*4));
			break;

		case ALTFUNC9:
			GPIOx->AFR[1] &= ~(0xFUL << ((PIN-8)*4));
			GPIOx->AFR[1] |= ((ALTFUNC9) << ((PIN-8)*4));
			break;

		case ALTFUNC10:
			GPIOx->AFR[1] &= ~(0xFUL << ((PIN-8)*4));
			GPIOx->AFR[1] |= ((ALTFUNC10) << ((PIN-8)*4));
			break;

		case ALTFUNC11:
			GPIOx->AFR[1] &= ~(0xFUL << ((PIN-8)*4));
			GPIOx->AFR[1] |= ((ALTFUNC11) << ((PIN-8)*4));
			break;

		case ALTFUNC12:
			GPIOx->AFR[1] &= ~(0xFUL << ((PIN-8)*4));
			GPIOx->AFR[1] |= ((ALTFUNC12) << ((PIN-8)*4));
			break;

		case ALTFUNC13:
			GPIOx->AFR[1] &= ~(0xFUL << ((PIN-8)*4));
			GPIOx->AFR[1] |= ((ALTFUNC13) << ((PIN-8)*4));
			break;

		case ALTFUNC14:
			GPIOx->AFR[1] &= ~(0xFUL << ((PIN-8)*4));
			GPIOx->AFR[1] |= ((ALTFUNC14) << ((PIN-8)*4));
			break;

		case ALTFUNC15:
			GPIOx->AFR[1] &= ~(0xFUL << ((PIN-8)*4));
			GPIOx->AFR[1] |= ((ALTFUNC15) << ((PIN-8)*4));
			break;

		}

	}
	else{
		switch (AF){

		case ALTFUNC0:
			GPIOx->AFR[0] &= ~(0xFUL << (PIN*4));
			GPIOx->AFR[0] |= ((ALTFUNC0) << (PIN*4));
			break;

		case ALTFUNC1:
			GPIOx->AFR[0] &= ~(0xFUL << (PIN*4));
			GPIOx->AFR[0] |= ((ALTFUNC1) << (PIN*4));
			break;

		case ALTFUNC2:
			GPIOx->AFR[0] &= ~(0xFUL << (PIN*4));
			GPIOx->AFR[0] |= ((ALTFUNC2) << (PIN*4));
			break;

		case ALTFUNC3:
			GPIOx->AFR[0] &= ~(0xFUL << (PIN*4));
			GPIOx->AFR[0] |= ((ALTFUNC3) << (PIN*4));
			break;

		case ALTFUNC4:
			GPIOx->AFR[0] &= ~(0xFUL << (PIN*4));
			GPIOx->AFR[0] |= ((ALTFUNC4) << (PIN*4));
			break;

		case ALTFUNC5:
			GPIOx->AFR[0] &= ~(0xFUL << (PIN*4));
			GPIOx->AFR[0] |= ((ALTFUNC5) << (PIN*4));
			break;

		case ALTFUNC6:
			GPIOx->AFR[0] &= ~(0xFUL << (PIN*4));
			GPIOx->AFR[0] |= ((ALTFUNC6) << (PIN*4));
			break;

		case ALTFUNC7:
			GPIOx->AFR[0] &= ~(0xFUL << (PIN*4));
			GPIOx->AFR[0] |= ((ALTFUNC7) << (PIN*4));
			break;

		case ALTFUNC8:
			GPIOx->AFR[0] &= ~(0xFUL << (PIN*4));
			GPIOx->AFR[0] |= ((ALTFUNC8) << (PIN*4));
			break;

		case ALTFUNC9:
			GPIOx->AFR[0] &= ~(0xFUL << (PIN*4));
			GPIOx->AFR[0] |= ((ALTFUNC9) << (PIN*4));
			break;

		case ALTFUNC10:
			GPIOx->AFR[0] &= ~(0xFUL << (PIN*4));
			GPIOx->AFR[0] |= ((ALTFUNC10) << (PIN*4));
			break;

		case ALTFUNC11:
			GPIOx->AFR[0] &= ~(0xFUL << (PIN*4));
			GPIOx->AFR[0] |= ((ALTFUNC11) << (PIN*4));
			break;

		case ALTFUNC12:
			GPIOx->AFR[0] &= ~(0xFUL << (PIN*4));
			GPIOx->AFR[0] |= ((ALTFUNC12) << (PIN*4));
			break;

		case ALTFUNC13:
			GPIOx->AFR[0] &= ~(0xFUL << (PIN*4));
			GPIOx->AFR[0] |= ((ALTFUNC13) << (PIN*4));
			break;

		case ALTFUNC14:
			GPIOx->AFR[0] &= ~(0xFUL << (PIN*4));
			GPIOx->AFR[0] |= ((ALTFUNC14) << (PIN*4));
			break;

		case ALTFUNC15:
			GPIOx->AFR[0] &= ~(0xFUL << (PIN*4));
			GPIOx->AFR[0] |= ((ALTFUNC15) << (PIN*4));
			break;

				}
	}

}

uint32_t compute_uart_baudrate(uint32_t PeriphClk, uint32_t Baudrate){
	return ((PeriphClk +Baudrate/2)/Baudrate);
}

void uart_set_baudrate115200(USART_TypeDef *USARTx, uint32_t PeriphClk){

	USARTx->BRR = compute_uart_baudrate(PeriphClk, 115200UL);

}

void usart_init(USART_TypeDef *USARTx, GPIO_TypeDef *GPIOx, uint8_t PIN, uint8_t AF){

	usart_rcc(USARTx);

	// 3. AF7 for USART2_TX
	altfunc(GPIOx, PIN, AF);

	uart_set_baudrate115200(USARTx,APB1CLK);
}

void uart2_tx(void){
	USART2->CR1 &= ~(1UL << 13);
	USART2->CR1 |= (1UL << 3);
	USART2->CR1 |= (1UL << 13);
}

void uart2_putcharpoll(char charw){
	while(!(USART2->SR & (1UL << 7)));
	USART2->DR = charw;

	if (charw == '\n'){
		uart2_putcharpoll('\r');
	}

}

/*ring_buffer.h usage section*/

void uart_txint(void){
	USART2->CR1 &= ~(1UL << 13);
	USART2->CR1 |= (1UL << 3);
	USART2->CR1 |= (1UL << 13);
	if (sizeof(tx_rb_mem) < ringbuffer_obj_size()) {
	        while (1); // configuration error
	 }

	init_ring_buff(tx_rb_mem, tx_array, sizeof(tx_array));
	tx_buff = mem_to_ringbuff(tx_rb_mem);
	NVIC_EnableIRQ(USART2_IRQn);
}

/*This is for handling ttys_run()*/
void uart2_putchar_int(char charw){

	while (full_ring_buff(tx_buff));

	disable_int();

	const bool tx_ongoing = !(empty_ring_buff(tx_buff));

	put_ring_buff(tx_buff, charw);

	if (!tx_ongoing){
		USART2->DR = peek_ring_buff(tx_buff);

	}

	i++;
	if (i > 1){
		if (rx_event == rx_on){
			//rx_event = false; this is a bug when we dont make a tty
			USART2->CR1 |= (1UL << 7);
		}

		if (i  == sizeof(uint32_t)){
			i = 1;
		}
	}

}

/*This is for handling myprintf()*/
void uart2int_putchar(char charw){
	myprinth++;
	while (full_ring_buff(tx_buff));

	//if didnt have this if, will have bug issue
	if (!(tailplus_ring_buff(tx_buff))){
		disable_int();
	}


	const bool tx_ongoing = !(empty_ring_buff(tx_buff));

	put_ring_buff(tx_buff, charw);

	if (!tx_ongoing){
		USART2->DR = peek_ring_buff(tx_buff);

	}

	USART2->CR1 |= (1UL << 7);


	if (charw == '\n'){
		uart2int_putchar('\r');
	}
}

void uart_putchar(char *sr){

	for (int i = 0; sr[i] != '\0'; i++){

		uart2_putchar_int(sr[i]);
	}

}


void uart_putchar_integer(int n){

	uint8_t buff[4];

	int i = 0;

	if (n == '0'){
		uart2int_putchar(n);
	}

	while (n > 0){
		buff[i++] = n%10 + '0';
		n = n/10;
	}

	while (i > 0){
		i--;
		uart2int_putchar((char)buff[i]);
	}
}


/*Wait until tail+1 == head*/
void wait_empty_buff(void){
	while(!(guard_ring_buff(tx_buff)));
}

void wait_empty_buff6(void){
	while(!(guard_ring_buff(tx6_buff)));
}

void uart6_txint(void){
	USART6->CR1 &= ~(1UL << 13);
	USART6->CR1 |= (1UL << 3);
	USART6->CR1 |= (1UL << 13);
	if (sizeof(tx6_rb_mem) < ringbuffer_obj_size()) {
	        while (1); // configuration error
	 }

	init_ring_buff(tx6_rb_mem, tx6_array, sizeof(tx6_array));
	tx6_buff = mem_to_ringbuff(tx6_rb_mem);
	NVIC_EnableIRQ(USART6_IRQn);
}


void uart6_putchar_int(char charw){

	while (full_ring_buff(tx6_buff));

	//if didnt have this if, will have bug issue
	if (!(tailplus_ring_buff(tx_buff))){
		disable_int6();
	}

	const bool tx_ongoing = !(empty_ring_buff(tx6_buff));

	put_ring_buff(tx6_buff, charw);

	if (!tx_ongoing){
		USART6->DR = peek_ring_buff(tx6_buff);

	}

	i6++;
	if (i6 > 1){
		if (rx6_event == rx6_on){
			rx6_event = false;
			USART6->CR1 |= (1UL << 7);
		}

		if (i6  == sizeof(uint32_t)){
			i6 = 1;
		}
	}

}


void uart6_putchar(char *sr){

	for (int i = 0; sr[i] != '\0'; i++){
		uart6_putchar_int(sr[i]);
	}
}


void uart6_putchar_integer(int n){

	uint8_t buff[4];

	int i = 0;

	if (n == '0'){
		uart6_putchar_int(n);
	}

	while (n > 0){
		buff[i++] = n%10 + '0';
		n = n/10;
	}

	while (i > 0){
		i--;
		uart6_putchar_int((char)buff[i]);
	}
}


/*----------------Receiving function-------------------*/

void uart2rx_init(void){
	USART2->CR1 &= ~(1UL << 13);
	USART2->CR1 |= (1UL << 2);
	USART2->CR1 |= (1UL << 13);

	if (sizeof(rx_rb_mem) < ringbuffer_obj_size()) {
		        while (1); // configuration error
		    }

	init_ring_buff(rx_rb_mem, rx_array, sizeof(rx_array));
	rx_buff = mem_to_ringbuff(rx_rb_mem);
	NVIC_EnableIRQ(USART2_IRQn);
	enable_intrx();

}


/* Normal interrupt driven function*/
void uart2_read_int(void){

	if (!empty_ring_buff(rx_buff)) {

		char c = get_ring_buff(rx_buff);
		uart2_putchar_int(c);
	}


}

/* Modification of RX interrupt driven function to suit TTY
 * Return True if there's character in ringbuffer ready to be fetched
 * Return False if there's no character in ringbuffer*/
bool uart2_read(uint8_t *charw){

	rx_on = true;
	/*This IF statement just to make head = 1, tail = 0*/
	if (empty_ring_buff(rx_buff)){
		hit_empty++;
		uint8_t Tharw;
		Tharw = USART2->DR;
		put_ring_buff(rx_buff, Tharw);//it's a giberrish char, no need to return the value
		return false;
		/*So this is only for the first iteration, and since we didnt specify any data inside *charw
		 * *charw will have undefined data, thus the uart2_putchar_int()will just echo random number
		 * to avoid this, we only uart2_putchar_int at second iteration only when using this uart2_read() function*/

	}

	while(guard_ring_buff(rx_buff));
	if (tailplus_ring_buff(rx_buff)) {

		get_ring_buff(rx_buff);
		*charw = peek_ring_buff(rx_buff);
		return true;

	}
	return false;

}


bool uart2_receive(uint8_t *buffer, uint16_t idx){


	uint16_t i = 0;
	bool ret = false;
	while(i < idx){

		uint8_t data;
		ret = uart2_read(&data);
		buffer[i++] = data;

	}
	return ret;

}
void uart2_transmit(uint8_t *buff, uint16_t len){


	for (uint16_t i = 0; i < len; i++){
		uart2_putchar_int(buff[i]);
	}
}
