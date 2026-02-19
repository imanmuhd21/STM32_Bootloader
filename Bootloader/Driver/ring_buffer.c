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

size_t ringbuffer_obj_size(void){
	return sizeof(ringbuffer);
}

void init_ring_buff(void *mem, uint8_t *array, uint8_t size){

	struct ringbuffer *rb = mem;
	rb->buffer = array;
	rb->size = size;
	rb->head = 0;
	rb->tail = 0;

}

ringbuffer *mem_to_ringbuff(void *mem){
	return (ringbuffer *)mem;
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


bool full_read_ringbuff(ringbuffer *rb){

	uint8_t tailadd = rb->tail+1;

	if(tailadd == rb->size){
		tailadd = 0;
	}

	return tailadd == rb->head;
}

uint8_t predict_ring_buff(ringbuffer *rb){

	uint8_t tailadd = rb->tail+1;
	if (tailadd == rb->size){
		tailadd = 0;
	}
	uint8_t taildadd = tailadd+1;
	if (taildadd == rb->size){
		taildadd = 0;
	}
	return taildadd == rb->head;

}

bool tailplus_ring_buff(ringbuffer *rb){
	uint8_t tailadd = rb->tail+2;

	if (tailadd == rb->size){
		tailadd = 0;
	}
	if (tailadd == rb->size+1){
		tailadd = 1;
	}

	return tailadd == rb->head;
}

bool guard_ring_buff(ringbuffer *rb){
	uint8_t tailadd = rb->tail+1;

	if (tailadd == rb->size){
		tailadd = 0;
	}

	return tailadd == rb->head;
}
