/*
 * ring_buffer.c
 *
 *  Created on: Feb 13, 2026
 *      Author: iman
 */

#include "ring_buffer.h"
#include <stdbool.h>


struct ringbuffer{
	uint8_t *buffer;
	uint8_t size;
	uint8_t head;
	uint8_t tail;
};


ringbuffer *create_ring_buff(uint8_t *array, uint8_t size){

	static ringbuffer rb;
	rb.buffer = array;
	rb.size = size;
	rb.head = 0;
	rb.tail = 0;
	return &rb;

}


void put_ring_buff(ringbuffer *rb, uint8_t data){


	rb->buffer[rb->head] = data;
	rb->head++;

	if (rb->head == rb->size){
		rb->head = 0;
	}

}

uint8_t get_ring_buff(ringbuffer *rb){


	uint8_t data = rb->buffer[rb->tail];
	rb->tail++;

	if (rb->tail == rb->size){
		rb->tail = 0;
	}

	return data;
}


uint8_t peek_ring_buff(ringbuffer *rb){

	return rb->buffer[rb->tail];
}

bool empty_ring_buff(ringbuffer *rb){

	return rb->tail == rb->head;
}

bool full_ring_buff(ringbuffer *rb){


	uint8_t headadd = rb->head+1;

	if (headadd == rb->size){
		headadd = 0;
	}

	return headadd == rb->tail;

}

bool guard_ring_buff(ringbuffer *rb){
	uint8_t tailadd = rb->tail+1;

	if (tailadd == rb->size){
		tailadd = 0;
	}

	return tailadd == rb->head;
}
