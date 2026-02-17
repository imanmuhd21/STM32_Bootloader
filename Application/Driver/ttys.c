/*
 * ttys.c
 *
 *  Created on: Feb 14, 2026
 *      Author: iman
 */


#include "ttys.h"
#include "usart.h"
#include "myprintf.h"
#include <ctype.h> //for isprint();


#define PROMPT '>'
#define CONSOLE_CMD_BUFR_SIZE 128



uint8_t ttys_buff[CONSOLE_CMD_BUFR_SIZE];

volatile uint32_t k = 0;

/*Linear Buffer*/
struct ttysbuffer{
	uint8_t *buffer;
	uint8_t size;
	uint8_t idx;
	uint8_t key;
	bool first_run_done;

};


/*Uses static to save the state of the struct.
Note that static will store our variable inside storage(.bss section)*/
static struct ttysbuffer ttysbuff = {ttys_buff, sizeof(ttys_buff), 0, 0, false};

/*ttys_init reset the idx and first_run_done, so each time intialize will not be mistaken*/
void ttys_init(void){
	gpio_init(GPIOA, PIN3, ALTFUNC);
	usart_init(USART2, GPIOA, PIN3, ALTFUNC7);
	uart2rx_init();
	ttysbuff.idx = 0;
	ttysbuff.key = 0;
	ttysbuff.first_run_done = false;
}



void ttys_run(void){

	char c;
	if (!ttysbuff.first_run_done){
		ttysbuff.first_run_done = true;
		myprintf("%c", PROMPT);
	}

	while (uart2_read(&c)){
		k++;
		if (k > 1){
			if (c == '\r' || c == '\n'){
				ttysbuff.buffer[ttysbuff.idx] = '\0';

				ttysbuff.idx = 0;
				ttysbuff.key = 0;
				myprintf("\n%c", PROMPT);
				continue;

			}

			if (c == '\b' || c == '\x7f'){

				if (ttysbuff.idx > 0){
					//The magic line
					myprintf("\b \b");
					ttysbuff.idx--;
					ttysbuff.key--;

				}
				continue;
			}



			if (isprint(c)){
				if (ttysbuff.buffer[ttysbuff.idx] < CONSOLE_CMD_BUFR_SIZE - 1){
					ttysbuff.buffer[ttysbuff.idx] = c;
					ttysbuff.idx++;
					ttysbuff.key++;
					uart2_putchar_int(c);
				}
				else{
					uart2_putchar_int('\a');
				}

			}

		}
	}

}
