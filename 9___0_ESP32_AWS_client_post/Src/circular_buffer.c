/*
 * circular_buffer.c
 *
 *  Created on: Jan 31, 2025
 *      Author: Katja
 */

#include "circular_buffer.h"
#include <string.h>

#define CR1_RXNEIE          (1U<<5)
#define CR1_TXEIE           (1U<<7)

#define SR_RXNE             (1U<<5)
#define SR_TXE              (1U<<7)

// portType debug_port = 0;
// portType slave_dev_port = 1;

/* Buffer for Slave Device UAR */
circular_buffer rx_buffer1 = {{INIT_VAL}, INIT_VAL, INIT_VAL};  // Rx Buffer for Slave device
circular_buffer tx_buffer1 = {{INIT_VAL}, INIT_VAL, INIT_VAL};  // Tx Buffer for Slave device

/* Buffer for debug Device UAR */
circular_buffer rx_buffer1_debug = {{INIT_VAL}, INIT_VAL, INIT_VAL};  // Rx Buffer for Debug
circular_buffer tx_buffer1_debug = {{INIT_VAL}, INIT_VAL, INIT_VAL};  // Tx Buffer for Debug

/* Define pointers to the buffers */
circular_buffer* _rx_buffer1;
circular_buffer* _tx_buffer1;

circular_buffer* _rx_buffer1_debug;
circular_buffer* _tx_buffer1_debug;


void circular_buffer_init(void){
	/* Init buff pointers */
	_rx_buffer1 = &rx_buffer1;
	_rx_buffer1_debug = &rx_buffer1_debug;

	_tx_buffer1 = &tx_buffer1;
	_tx_buffer1_debug = &tx_buffer1_debug;

	/* Initial Rx interrupt */
	USART1->CR1 |= CR1_RXNEIE;
	USART2->CR1 |= CR1_RXNEIE;
	USART1->DR  = 0;
}

static void buff_store_char(unsigned char c, circular_buffer *buffer){
	int location = (uint32_t)(buffer->head + 1) % UART_BUFFER_SIZE;

	/* Check if no overflow will occur */
	if(location != buffer->tail){
		/* Store character */
		buffer->buffer[buffer->head] = c;

		/* Update head */
		buffer->head = location;
 	}
}

void buffer_clear(portType uart){

	switch(uart){

		case SLAVE_DEV_PORT:
			/* Set content to '\0' */
			memset(_rx_buffer1->buffer, '\0', UART_BUFFER_SIZE);
			_rx_buffer1->head = 0;
		break;

		case DEBUG_PORT:
			/* Set content to '\0' */
			memset(_rx_buffer1_debug->buffer, '\0', UART_BUFFER_SIZE);
			_rx_buffer1_debug->head = 0;
		break;
	}
}

/* Check the next value in the buffer
 * without removing it << Peeking >>
 */
int buffer_peek(portType uart){

	int ret;
	switch(uart){

		case SLAVE_DEV_PORT:
			if(_rx_buffer1->head == _rx_buffer1->tail){
				/* Return error */
				ret = -1;
			}
			else{
				ret = (int)_rx_buffer1->buffer[_rx_buffer1->tail];
			}
		break;

		case DEBUG_PORT:
			if(_rx_buffer1_debug->head == _rx_buffer1_debug->tail){
				/* Return error */
				ret = -1;
			}
			else{
				ret = (int)_rx_buffer1_debug->buffer[_rx_buffer1_debug->tail];
			}
		break;

		default:
			ret = -1;
		break;
	}
	return ret;
}


/* Function to read (and remove) the next value
 * in the buffer */
int buffer_read(portType uart){

	int ret;
	switch(uart){

		case SLAVE_DEV_PORT:
			if(_rx_buffer1->head == _rx_buffer1->tail){
				/* Return error */
				ret = -1;
			}
			else{
				unsigned char c = _rx_buffer1->buffer[_rx_buffer1->tail];
				_rx_buffer1->tail = (uint32_t)(_rx_buffer1->tail + 1) % UART_BUFFER_SIZE;
				ret = c;
			}
		break;

		case DEBUG_PORT:
			if(_rx_buffer1_debug->head == _rx_buffer1_debug->tail){
				/* Return error */
				ret = -1;
			}
			else{
				unsigned char c = _rx_buffer1_debug->buffer[_rx_buffer1_debug->tail];
				_rx_buffer1_debug->tail = (uint32_t)(_rx_buffer1_debug->tail + 1) % UART_BUFFER_SIZE;
				ret = c;
			}
		break;

		default:
			ret = -1;
		break;
	}
	return ret;
}


void buffer_write(unsigned char c, portType uart){

	int loc;
	switch(uart){

		case SLAVE_DEV_PORT:
			loc = (uint32_t)(_tx_buffer1->head + 1) % UART_BUFFER_SIZE;

			while(loc == _tx_buffer1->tail){}
			_tx_buffer1->buffer[_tx_buffer1->head] = c;
			_tx_buffer1->head = loc;

			/* Initial Tx interrupt */
			USART1->CR1 |= CR1_TXEIE;
		break;

		case DEBUG_PORT:
			loc = (uint32_t)(_tx_buffer1_debug->head + 1) % UART_BUFFER_SIZE;

			while(loc == _tx_buffer1_debug->tail){}
			_tx_buffer1_debug->buffer[_tx_buffer1_debug->head] = c;
			_tx_buffer1_debug->head = loc;

			/* Initial Tx interrupt */
			USART2->CR1 |= CR1_TXEIE;
		break;

		default:
			break;
	}
}


/* Function to check if there is data in the buffer */
int is_data(portType uart){

	int ret;
	switch(uart){

	case SLAVE_DEV_PORT:
		ret = ((uint32_t)(UART_BUFFER_SIZE + _rx_buffer1->head - _rx_buffer1->tail) % UART_BUFFER_SIZE);
	break;

	case DEBUG_PORT:
		ret = ((uint32_t)(UART_BUFFER_SIZE + _rx_buffer1_debug->head - _rx_buffer1_debug->tail) % UART_BUFFER_SIZE);
	break;

	default:
		break;
	}

	return ret;
}


/* Get first character of a specified string from buffer */
static void get_first_char(char *str){
	/* Make sure there is data in the buffer */
	while(!is_data(SLAVE_DEV_PORT)){}

	while(buffer_peek(SLAVE_DEV_PORT) != str[0]){
		_rx_buffer1->tail = (uint16_t)(_rx_buffer1->tail + 1) % UART_BUFFER_SIZE;

		while(!is_data(SLAVE_DEV_PORT)){}
	}
}


/* Function to check if a certain response is present in the buffer */
int is_response(char *str){
	int curr_pos = 0;
	int len = strlen(str);

	while(curr_pos != len){
		curr_pos = 0;
		get_first_char(str);

		while(buffer_peek(SLAVE_DEV_PORT) == str[curr_pos]){
			curr_pos++;
			buffer_read(SLAVE_DEV_PORT);
			if(curr_pos == len){
				/* Success */
				return 1;
			}

			while(!is_data(SLAVE_DEV_PORT)){}
		}
	}
	return -1;
}


/* Function to get specified number of characters from buffer */
void get_strg(uint8_t num_of_char, char *dest_buffer){
	for( int indx = 0; indx < num_of_char; indx++ ){
		for( int i = 0; i < 9000; i++){}

		dest_buffer[indx] = buffer_read(SLAVE_DEV_PORT);
	}
}

/* Function to send a string to the buffer */
void buffer_send_string(const char *s, portType uart){
	while( *s != '\0'){
		buffer_write(*s++, uart);
	}
}

static int8_t process_copy(char* str, char* dest_buffer, int start_pos){
	int curr_pos = 0;
	int len = strlen(str);
	int indx = start_pos;

	while(!is_data(SLAVE_DEV_PORT)){}

	while(buffer_peek(SLAVE_DEV_PORT) != str[curr_pos]){
		dest_buffer[indx] = _rx_buffer1->buffer[_rx_buffer1->tail];
		_rx_buffer1->tail = (uint16_t)(_rx_buffer1->tail + 1) % UART_BUFFER_SIZE;
		indx++;

		while(!is_data(SLAVE_DEV_PORT)){}

	}

	while(buffer_peek(SLAVE_DEV_PORT) == str[curr_pos]){
		curr_pos++;
		dest_buffer[indx++] = buffer_read(SLAVE_DEV_PORT);

		if(curr_pos == len){
			return 1;
		}

		while(!is_data(SLAVE_DEV_PORT)){}

	}

	if(curr_pos != len){
		curr_pos = 0;
		process_copy(str, dest_buffer, indx);
	}

	if(curr_pos == len){
		return 1;
	}
	else{
		return -1;
	}
}

int8_t copy_up_to_string(char* str, char* dest_buffer){
	/* Process copy */
	return (process_copy(str, dest_buffer, 0));
}


void slave_dev_uart_callback(void){
	/* Check if RXNE is raised and RXNEIE is enabled */
	if( ((USART1->SR & SR_RXNE) != 0) && ((USART1->CR1 & CR1_RXNEIE) != 0) ){
		unsigned char c = USART1->DR;
		buff_store_char(c, _rx_buffer1);
	}

	/* Check if TXE is raised and TXEIE is enabled */
	if(((USART1->SR & SR_TXE) != 0) && ((USART1->CR1 & CR1_TXEIE) != 0)){
		if( tx_buffer1.head == tx_buffer1.tail ){
			USART1->CR1 &= ~CR1_TXEIE;
		}
		else{
			/* Get character from buffer */
			unsigned char c = tx_buffer1.buffer[tx_buffer1.tail];

			/* Update position */
			tx_buffer1.tail = (uint32_t)(tx_buffer1.tail + 1) % UART_BUFFER_SIZE;

			/* Transmit character */
			USART1->DR = c;
		}
	}
}


void debug_uart_callback(void){
	/* Check if RXNE is raised and RXNEIE is enabled */
	if( ((USART2->SR & SR_RXNE) != 0) && ((USART2->CR1 & CR1_RXNEIE) != 0) ){
		unsigned char c = USART2-> DR;
		buff_store_char(c, _rx_buffer1_debug);
	}

	/* Check if TXE is raised and TXEIE is enabled */
	if(((USART2->SR & SR_TXE) != 0) && ((USART2->CR1 & CR1_TXEIE) != 0)){
		if( tx_buffer1_debug.head == tx_buffer1_debug.tail ){
			USART2->CR1 &= ~CR1_TXEIE;
		}
		else{
			/* Get character from buffer */
			unsigned char c = tx_buffer1_debug.buffer[tx_buffer1_debug.tail];

			/* Update position */
			tx_buffer1_debug.tail = (uint32_t)(tx_buffer1_debug.tail + 1) % UART_BUFFER_SIZE;

			/* Transmit character */
			USART2->DR = c;
		}
	}
}

void USART2_IRQHandler(void){
	debug_uart_callback();
}

void USART1_IRQHandler(void){
	slave_dev_uart_callback();
}
