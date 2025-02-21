/*
 * esp32_lib.c
 *
 *  Created on: Feb 4, 2025
 *      Author: Katja
 */
#include "esp32_lib.h"

static void esp32_startup_test(void){
	/* Clear ESP UART port */
	buffer_clear(esp32_port);

	/* Send the command */
	buffer_send_string("AT\r\n", esp32_port);

	/* Wait for "OK" response */
	while(!(is_response("OK\r\n"))){}

	/* Print confirming to the debug UART */
	buffer_send_string("AT startup test successful\r\n", debug_port);
}


static void esp32_reset(void){
	/* Clear ESP UART port */
	buffer_clear(esp32_port);

	/* Send the command */
	buffer_send_string("AT+RST\r\n", esp32_port);

	/* Delay for 1 second */
	delay(1000);

	/* Wait for "OK" response */
	while(!(is_response("ready"))){}

	/* Print confirming to the debug UART */
	buffer_send_string("Reset was successful\r\n", debug_port);
}


static void esp32_sta_mode(void){
	/* Clear ESP UART port */
	buffer_clear(esp32_port);

	/* Send the command */
	buffer_send_string("AT+CWMODE=1\r\n", esp32_port);

	/* Wait for "OK" response */
	while(!(is_response("OK\r\n"))){}

	/* Print confirming to the debug UART */
	buffer_send_string("STA mode set successful\r\n", debug_port);
}


static void esp32_ap_connect(char* ssid, char* password){      // ESP32 Access Point Connect

	char data[80];
	/* Clear ESP UART port */
	buffer_clear(esp32_port);

	buffer_send_string("Connecting to access point...\r\n", debug_port);

	/* Pass ssid, password and command into one string packet */
	sprintf(data, "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, password);
	/* Send the command */
	buffer_send_string(data, esp32_port);

	/* Wait for "OK" response */
	while(!(is_response("OK\r\n"))){}

	/* Print confirming to the debug UART */
	sprintf(data, "Connected:   \"%s\" \r\n", ssid);
	buffer_send_string(data, debug_port);
}


void esp32_init(char* ssid, char* password){
	circular_buffer_init();
	buffer_clear(SLAVE_DEV_PORT);
	esp32_startup_test();
	esp32_reset();
	delay(1000);
	esp32_sta_mode();
	esp32_ap_connect(ssid, password);
}
