/*
 * sensor_hub.h
 *
 *  Created on: Oct 27, 2017
 *      Author: mateusz
 */

#ifndef SENSORS_SENSOR_HUB_H_
#define SENSORS_SENSOR_HUB_H_
#include "ch.h"

#define SENSOR_HUB_EVT_PRESS_NEW_DATA		((eventmask_t)(1 << 0))
#define SENSOR_HUB_EVT_ACCEL_NEW_DATA		((eventmask_t)(1 << 1))

void sensor_hub_start();
thread_t *sensor_hub_get_thread_ptr();


#endif /* SENSORS_SENSOR_HUB_H_ */
