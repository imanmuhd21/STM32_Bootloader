/*
 * ring_buffer.h
 *
 *  Created on: Feb 13, 2026
 *      Author: iman
 */

#include "stm32f401xe.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h> //for size_t

#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_


typedef struct ringbuffer ringbuffer;

#define RINGBUFFER_OBJ_SIZE 16

void init_ring_buff(void *mem, uint8_t *array, uint8_t size);

size_t ringbuffer_obj_size(void);

ringbuffer *mem_to_ringbuff(void *mem);

void put_ring_buff(ringbuffer *rb, uint8_t data);

uint8_t get_ring_buff(ringbuffer *rb);

uint8_t peek_ring_buff(ringbuffer *rb);

bool empty_ring_buff(ringbuffer * rb);

bool full_ring_buff(ringbuffer *rb);


bool full_read_ringbuff(ringbuffer *rb);

uint8_t predict_ring_buff(ringbuffer *rb);

bool tailplus_ring_buff(ringbuffer *rb);

bool guard_ring_buff(ringbuffer *rb);

#endif /* RING_BUFFER_H_ */
