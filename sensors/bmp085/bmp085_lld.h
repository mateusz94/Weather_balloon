/*
 * bmp085_lld.h
 *
 *  Created on: Oct 27, 2017
 *      Author: mateusz
 */

#ifndef SENSORS_BMP085_BMP085_LLD_H_
#define SENSORS_BMP085_BMP085_LLD_H_

#include "bmp085.h"
#include "ch.h"

int bmp085_lld_init( bmp085_t* );
int bmp085_lld_stop( bmp085_t* );
bool bmp085_lld_get_pressure( long * );

BMP085_BUS_WR_RETURN_TYPE bus_write( BMP085_BUS_WR_PARAM_TYPES );
BMP085_BUS_RD_RETURN_TYPE bus_read( BMP085_BUS_RD_PARAM_TYPES );
BMP085_MDELAY_RETURN_TYPE delay_msec( BMP085_MDELAY_DATA_TYPE );

#endif /* SENSORS_BMP085_BMP085_LLD_H_ */
