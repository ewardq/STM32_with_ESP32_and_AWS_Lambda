/*
 * circular_buffer.h
 *
 *  Created on: Jan 31, 2025
 *      Author: Katja
 */

#ifndef CIRCULAR_BUFFER_H_
#define CIRCULAR_BUFFER_H_

#include "uart.h"

#define UART_BUFFER_SIZE    100
#define INIT_VAL            0

typedef enum{
	DEBUG_PORT = 0,
	SLAVE_DEV_PORT = 1      // Automatically becomes 1
}portType;

typedef struct
{
	unsigned char buffer[UART_BUFFER_SIZE];
	__IO uint32_t head;
	__IO uint32_t tail;

}circular_buffer;

void circular_buffer_init(void);
void buffer_send_string(const char *s, portType uart);

void buffer_clear(portType uart);
int buffer_peek(portType uart);
int buffer_read(portType uart);
void buffer_write(unsigned char c, portType uart);
int is_data(portType uart);
int is_response(char *str);
void get_strg(uint8_t num_of_char, char *dest_buffer);
void buffer_send_string(const char *s, portType uart);

int8_t copy_up_to_string(char* str, char* dest_buffer);

void slave_dev_uart_callback(void);
void debug_uart_callback(void);
void USART2_IRQHandler(void);
void USART1_IRQHandler(void);


#endif /* CIRCULAR_BUFFER_H_ */
