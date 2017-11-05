/*
 * sensor_hub.c
 *
 *  Created on: Oct 27, 2017
 *      Author: mateusz
 */
#include "sensor_hub.h"
#include "bmp085.h"
#include "hal.h" /*Blink LED*/

static thread_t *sensor_hub_ptr = NULL;
static THD_WORKING_AREA(sensor_hub_thread_wa, 128);

/* sensors struct */
static bmp085_t bmp085;

/*
 * Private functions
 */
static bool sensor_hub_read_accel_data();
static bool sensor_hub_read_press_data( long* );
static void sensor_hub_process_events(eventmask_t eventmask);
static bool sensor_hub_init();
static THD_FUNCTION(sensor_hub_thread, arg);

/*
 * Public API implementation
 */
void sensor_hub_start()
{
	sensor_hub_ptr = chThdCreateStatic(sensor_hub_thread_wa,
									   sizeof(sensor_hub_thread_wa),
									   NORMALPRIO,
									   sensor_hub_thread,
									   NULL);
}

thread_t *sensor_hub_get_thread_ptr()
{
	return sensor_hub_ptr;
}


/*
 * Privates functions implementation
 */
static bool sensor_hub_read_press_data( long *press)
{
	if ( bmp085_lld_get_pressure( press ) <= 0 ) return false;
	return true;
}

static bool sensor_hub_read_accel_data()
{

	return true;
}

static bool sensor_hub_init()
{
	if(bmp085_lld_init( &bmp085 ) <= 0) {
		return false;
	}
	return true;
}

static void sensor_hub_process_events(eventmask_t eventmask)
{
	long press = 0;
	if(eventmask & SENSOR_HUB_EVT_PRESS_NEW_DATA) {
		if(sensor_hub_read_press_data(&press)) {
			chprintf((BaseSequentialStream*)&SD2, "pressure: %ld\n\r", press);
		} else {
			chprintf((BaseSequentialStream*)&SD2, "sensor_hub_read_press_data failed!\n\r");
		}
		palSetPad(GPIOD, GPIOD_PIN13);
	} else if(eventmask & SENSOR_HUB_EVT_ACCEL_NEW_DATA) {
		palClearPad(GPIOD, GPIOD_PIN13);
		palClearPad(GPIOD, GPIOD_PIN14);
		palClearPad(GPIOD, GPIOD_PIN15);
	}
}

static THD_FUNCTION(sensor_hub_thread, arg)
{
	eventmask_t eventmask = 0;
	chRegSetThreadName("sensor_hub_thread");

	if (sensor_hub_init() == false) {
		chprintf((BaseSequentialStream*)&SD2, "sensor_hub_init failed!\n\r");
		return;
	}

	chEvtSignal(sensor_hub_get_thread_ptr(), SENSOR_HUB_EVT_PRESS_NEW_DATA);
	chEvtSignal(sensor_hub_get_thread_ptr(), SENSOR_HUB_EVT_ACCEL_NEW_DATA);

	while(true){
		eventmask = chEvtWaitAny(ALL_EVENTS);
		sensor_hub_process_events(eventmask);
	}
}
