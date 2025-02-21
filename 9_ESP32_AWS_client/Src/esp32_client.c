/*
 * esp32_client.c
 *
 *  Created on: Feb 5, 2025
 *      Author: EMART429
 */
#include "esp32_client.h"
#include "circular_buffer.h"
#include "string.h"
#include "esp32_lib.h"
#include <stdio.h>

#define TEMP_BUFF_LNG_SZ    600
#define TEMP_BUFF2_SHT_SZ   30

#define SERVER_ADDRESS "5v9czg8uzi.execute-api.us-east-2.amazonaws.com" /* Put server here */
#define OK_RESPONSE "OK\r\n"
#define SEND_PROMPT ">"
#define SEND_OK_RESPONSE "SEND OK\r\n"
#define END_OF_HEADERS "\r\n\r\n"
#define LINE_TERMINATOR "\r\n"
#define CLOSED_RESPONSE "CLOSED\r\n"
#define CIPSEND_COMMAND "AT+CIPSEND=%d\r\n"  /* C I P command */


void aws_client_post(char *dest_buffer, const char* endpoint, const char* json_payload){
	/* Buffers for the HTTP request and AT commands */
	char request_buffer[TEMP_BUFF_LNG_SZ] = {0};
	char send_command_buffer[TEMP_BUFF2_SHT_SZ] = {0};

	/* Clear any previous data in the ESP UART buffer */
	buffer_clear(esp32_port);

	/* Establish an SSL connection to the server on port 443 */
	snprintf(request_buffer, sizeof(request_buffer), "AT+CIPSTART=\"SSL\",\"%s\",443\r\n", SERVER_ADDRESS);
	buffer_send_string(request_buffer, esp32_port);

	/* Wait until SSL connection is successfully established */
	while(!is_response(OK_RESPONSE));

	/* Prepare the HTTP POST request */
	snprintf(request_buffer, sizeof(request_buffer),
			 "POST %s HTTP/1.1\r\nHost: %s\r\nContent-Type: application/json\r\nContent-Length: %d\r\nConnection: close\r\n\r\n%s",
			 endpoint, SERVER_ADDRESS, strlen(json_payload), json_payload);

	/* Get the length of the prepared request */
	int request_length = strlen(request_buffer);

	/* Send the POST request length */
	snprintf(send_command_buffer, sizeof(send_command_buffer), CIPSEND_COMMAND, request_length);
	buffer_send_string(send_command_buffer, esp32_port);

	/* Wait for the ESP to prompt for data with ">" */
	while(!is_response(SEND_PROMPT)){}

	/* Send the POST request */
	buffer_send_string(request_buffer, esp32_port);

	/* Wait to confirm that the data was sent successfully */
	while(!is_response(SEND_OK_RESPONSE)){}

	/* Wait until the HTTP response header has been received */
	while(!is_response(END_OF_HEADERS)){}

	/* Copy response data into destination buffer */
	while(!copy_up_to_string(CLOSED_RESPONSE, dest_buffer)){}

	/* Close SSL connection */
	buffer_send_string("AT+CIPCLOSE\r\n", esp32_port);
}

void aws_client_get_file(char* dest_buffer, const char* storage_url){
	/* Buffers for the HTTP request and AT commands */
	char request_buffer[TEMP_BUFF_LNG_SZ] = {0};
	char send_command_buffer[TEMP_BUFF2_SHT_SZ] = {0};

	/* Clear any previous data in the ESP UART buffer */
	buffer_clear(esp32_port);

	/* Establish an SSL connection to the server on port 443 */
	snprintf(request_buffer, sizeof(request_buffer), "AT+CIPSTART=\"SSL\",\"%s\",443\r\n", SERVER_ADDRESS);
	buffer_send_string(request_buffer, esp32_port);

	/* Wait until SSL connection is successfully established */
	while(!is_response(OK_RESPONSE));

	/* Prepare GET request */
	snprintf(request_buffer, sizeof(request_buffer), "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", storage_url, SERVER_ADDRESS);

	/* Get the length of the prepared request */
	int request_length = strlen(request_buffer);


	/* Send the GET request length */
	snprintf(send_command_buffer, sizeof(send_command_buffer), CIPSEND_COMMAND, request_length);
	buffer_send_string(send_command_buffer, esp32_port);

	/* Wait for the ESP to prompt for data with ">" */
	while(!is_response(SEND_PROMPT)){}

	/* Send the GET request */
	buffer_send_string(request_buffer, esp32_port);

	/* Wait to confirm that the data was sent successfully */
	while(!is_response(SEND_OK_RESPONSE)){}

	/* Wait until the HTTP response header has been received */
	while(!is_response(END_OF_HEADERS)){}

	/* Copy response data into destination buffer */
	while(!copy_up_to_string(CLOSED_RESPONSE, dest_buffer)){}

	/* Close SSL connection */
	buffer_send_string("AT+CIPCLOSE\r\n", esp32_port);
}











