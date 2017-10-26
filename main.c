/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"
#include "ch_test.h"
#include "chprintf.h"
#include "shell_cfg.h"

#define EVT_TURN_ON			((eventmask_t)(1 << 0))
#define EVT_TURN_OFF		((eventmask_t)(1 << 1))

thread_t *listener;

static THD_WORKING_AREA(waThread2, 128);
static THD_FUNCTION(Trigger, arg)
{

  (void)arg;
  chRegSetThreadName("trigger");
  eventmask_t events = 0;
  while(true){
	  chThdSleepMilliseconds(500);
	  events = EVT_TURN_ON;
	  chEvtSignal(listener, events);
	  chThdSleepMilliseconds(2000);
	  events = EVT_TURN_OFF;
	  chEvtSignal(listener, events);
  }

}


static void ProcessEvents(eventmask_t eventmask)
{
	/* Process events */
	if(eventmask & EVT_TURN_ON) {
		palSetPad(GPIOD, GPIOD_PIN13);
		palSetPad(GPIOD, GPIOD_PIN14);
		palSetPad(GPIOD, GPIOD_PIN15);
	} else
	if(eventmask & EVT_TURN_OFF) {
		palClearPad(GPIOD, GPIOD_PIN13);
		palClearPad(GPIOD, GPIOD_PIN14);
		palClearPad(GPIOD, GPIOD_PIN15);
	}
}

/*
 * Green LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(ThreadBlinker, arg) {

  (void)arg;
  eventmask_t eventmask;

  chRegSetThreadName("blinker");

  while(true) {
  		eventmask = chEvtWaitAny(ALL_EVENTS);
  		ProcessEvents(eventmask);
  }

  chThdExit(0);
}

/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /*
   * Activates the serial driver 2 using the driver default configuration. MINICOM
   */
  sdStart(&SD2, NULL);

  /*
   * Creates the blinker thread.
   */
  listener = chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, ThreadBlinker, NULL);
  chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO, Trigger, NULL);

  shell_init((BaseSequentialStream*)&SD2);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state.
   */
  while (true) {
    if (!palReadPad(GPIOC, GPIOC_BUTTON))
      test_execute((BaseSequentialStream *)&SD2);
    chThdSleepMilliseconds(500);
    //chprintf((BaseSequentialStream*)&SD2, "Hello world!\n\r");
  }
}
