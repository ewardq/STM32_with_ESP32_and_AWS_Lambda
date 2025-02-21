/*
 * esp32_client.h
 *
 *  Created on: Feb 5, 2025
 *      Author: EMART429
 */

#ifndef ESP32_CLIENT_H_
#define ESP32_CLIENT_H_

void aws_client_get_file(char* dest_buffer, const char* storage_url);
void aws_client_post(char *dest_buffer, const char* endpoint, const char* json_payload);
void aws_client_get(char* dest_buffer, const char* endpoint);

#endif /* ESP32_CLIENT_H_ */
