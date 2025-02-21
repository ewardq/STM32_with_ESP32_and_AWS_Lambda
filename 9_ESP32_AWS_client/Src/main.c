#include <stdio.h>
#include "stm32f4xx.h"
#include "fpu.h"
#include "uart.h"
#include "timebase.h"
#include "bsp.h"
#include "adc.h"
#include "esp32_lib.h"
#include "esp32_client.h"

/*
#define SSID_NAME "Totalplay-E7AA"
#define PASSKEY   "E7AA1038HpVJkVef"
*/
#define SSID_NAME "GalaxyHtspt"
#define PASSKEY   "f2tjdyiybu4i3nx"

#define POST_ENDPOINT "/dev/data"

char response[10000];

int main(void)
{
    /* Enable FPU */
	fpu_enable();

	/* Initialize debug UART */
	debug_uart_init();
	slave_dev_uart_init();

	/* Initialize timebase */
	timebase_init();

	/* Initialize LED */
	//led_init();

	/* Initialize push button */
	//button_init();

	/* Initialize ADC */
	pa1_adc_init();

	/* Start conversion */
	//start_conversion();


	esp32_init(SSID_NAME, PASSKEY);

	aws_client_post(response, POST_ENDPOINT, "{\"device_id\": \"STM32_077\", \"temperature\": 12, \"humidity\": 345}");

	while(1){

	}


	/*
	esp32_init(SSID_NAME, PASSKEY);
	while(1){

		//btn_state = get_btn_state();
	}
	*/
}
