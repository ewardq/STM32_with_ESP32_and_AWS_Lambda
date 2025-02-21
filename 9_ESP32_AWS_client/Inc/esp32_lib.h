/*
 * esp32_lib.h
 *
 *  Created on: Feb 4, 2025
 *      Author: Katja
 */

#ifndef ESP32_LIB_H_
#define ESP32_LIB_H_

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "circular_buffer.h"
#include "timebase.h"

#define esp32_port  SLAVE_DEV_PORT // Same as --> #define esp32_port  1
#define debug_port  DEBUG_PORT     // Same as --> #define esp32_port  0

void esp32_init(char* ssid, char* password);

#endif /* ESP32_LIB_H_ */
