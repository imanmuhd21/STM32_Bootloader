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

ringbuffer *create_ring_buff(uint8_t *array, uint8_t size);

void put_ring_buff(ringbuffer *rb, uint8_t data);

uint8_t get_ring_buff(ringbuffer *rb);

uint8_t peek_ring_buff(ringbuffer *rb);

bool empty_ring_buff(ringbuffer * rb);

bool full_ring_buff(ringbuffer *rb);

bool guard_ring_buff(ringbuffer *rb);

#endif /* RING_BUFFER_H_ */
