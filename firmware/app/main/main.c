/*  Moisture Sensor main application
*
*   Description: Send the value of a moisture capacitive sensor to the internet.
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event_legacy.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"

#define BLINK_GPIO 2				// Use GPIO 2 to blink LED
#define NO_OF_SAMPLES 64

static const adc_channel_t channel = ADC_CHANNEL_6;     //GPIO34 if ADC1, GPIO14 if ADC2
static const adc_atten_t atten = ADC_ATTEN_DB_11;		// 0 - 3.6V





void app_main(void)
{
    /* Configure the IOMUX register for pad BLINK_GPIO (some pads are
       muxed to GPIO on reset already, but some default to other
       functions and need to be switched to GPIO. Consult the
       Technical Reference for a list of pads and their default
       functions.)
    */
    gpio_pad_select_gpio(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

    printf("Hello world!\n");

    /* Initialize wifi */
    wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&config);

    /* Scan for possible Access Point (AP) */
    esp_wifi_scan_start();

    /* Wait for scanning to be completed */
   // while(! SYSTEM_EVENT_SCAN_DONE);

    uint16_t accesspoint_number;
    accesspoint_number = esp_wifi_scan_get_ap_num();


    /* Configure GPIO 34 for ADC acquisition*/
    printf("Configure ADC channel");
    adc1_config_width(ADC_WIDTH_BIT_12);
	adc1_config_channel_atten(channel, atten);

    while(1){

    	/* Blink LED to know the program is working */
	//	printf("Turning off the LED\n");
		gpio_set_level(BLINK_GPIO, 0);
		vTaskDelay(1000 / portTICK_PERIOD_MS);

	//	printf("Turning on the LED\n");
		gpio_set_level(BLINK_GPIO, 1);
		vTaskDelay(1000 / portTICK_PERIOD_MS);


		/* Make a ADC acquisition */

        uint32_t adc_reading = 0;

        //Multisampling
        for (int i = 0; i < NO_OF_SAMPLES; i++) {
        	adc_reading += adc1_get_raw((adc1_channel_t)channel);
        }
        adc_reading /= NO_OF_SAMPLES;

		printf("Moist value: %d\n", adc_reading);
		vTaskDelay(1000 / portTICK_PERIOD_MS);

		/* TODO
	    Dry: (520 430]
	    Wet: (430 350]
	    Water: (350 260]
		*/

    }
}

