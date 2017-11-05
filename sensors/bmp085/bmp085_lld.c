/*
 * bmp085.c
 *
 *  Created on: Oct 27, 2017
 *      Author: mateusz
 */

#include "bmp085_lld.h"
#include "hal.h"
// #define BMP085_I2C_ADDR (0xEE >> 1)


/* buffers depth */
#define PRESS_RX_DEPTH 6
#define PRESS_TX_DEPTH 4
#define PRESS_CTRL_REG1 BMP085_CTRL_MEAS_REG

static uint8_t rxbuf[PRESS_RX_DEPTH];
static uint8_t txbuf[PRESS_TX_DEPTH];
static msg_t status = MSG_OK;
static systime_t tmo = MS2ST(4);
static i2cflags_t errors = 0;

/* I2C interface #2 */
static const I2CConfig i2cfg2 = {
    OPMODE_I2C,
    400000,
    FAST_DUTY_CYCLE_2,
};

int bmp085_lld_init( bmp085_t* bmp085 )
{
    // palSetPadMode(GPIOB, GPIOB_ARD_D15 , PAL_MODE_ALTERNATE(4) | PAL_STM32_OTYPE_OPENDRAIN) ;
    // palSetPadMode(GPIOB, GPIOB_ARD_D14, PAL_MODE_ALTERNATE(4) | PAL_STM32_OTYPE_OPENDRAIN) ;
    bmp085->bus_write = bus_write;
    bmp085->bus_read = bus_read;
    bmp085->delay_msec = delay_msec;
    i2cStart(&I2CD1, &i2cfg2);
    status = bmp085_init( bmp085 );
    chThdSleepMilliseconds(100);
    return status;
}

int bmp085_lld_stop(bmp085_t* bmp085)
{
    i2cstop(&I2CD1);
}

bool bmp085_lld_get_pressure( long* res )
{
    unsigned long up = bmp085_get_up();
    if(up < 0) return false;
    *res = bmp085_get_pressure(up);
    return true;
}

BMP085_BUS_WR_RETURN_TYPE bus_write( unsigned char device_addr, 
                                     unsigned char register_addr, 
                                     unsigned char* register_data, 
                                     unsigned char write_length)
{
    static unsigned char tx[2];
    tx[0] = register_addr;
    tx[1] = *(register_data);
    i2cAcquireBus(&I2CD1);
    status = i2cMasterTransmitTimeout(&I2CD1, device_addr, tx, 2, NULL, 0, tmo);
    i2cReleaseBus(&I2CD1);
    return status;
}
 
BMP085_BUS_RD_RETURN_TYPE bus_read( unsigned char device_addr, 
                                    unsigned char register_addr, 
                                    unsigned char* register_data, 
                                    unsigned char read_length)
{
    static unsigned char tx[1];
    tx[0] = register_addr;

    i2cAcquireBus(&I2CD1);
    status = i2cMasterTransmitTimeout(&I2CD1, device_addr, tx, 1, register_data, read_length, tmo);
    i2cReleaseBus(&I2CD1);
    return status;
}

BMP085_MDELAY_RETURN_TYPE delay_msec( unsigned int delay)
{
    chThdSleep(MS2ST(delay));
}

