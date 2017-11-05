
#include "ch.h"
#include "hal.h"
#include "extconfig.h"
#include "sensor_hub.h"

void EXTI6_callback(EXTDriver *extp, expchannel_t channel);

static const EXTConfig ext_config = {{
    {EXT_CH_MODE_DISABLED, NULL}, /* EXTI0 */
    {EXT_CH_MODE_DISABLED, NULL}, /* EXTI1 */
    {EXT_CH_MODE_DISABLED, NULL}, /* EXTI2 */
    {EXT_CH_MODE_DISABLED, NULL}, /* EXTI3 */
    {EXT_CH_MODE_DISABLED, NULL}, /* EXTI4 */
    {EXT_CH_MODE_DISABLED, NULL}, /* EXTI5 */
    {EXT_CH_MODE_AUTOSTART | EXT_CH_MODE_RISING_EDGE | EXT_MODE_GPIOE, EXTI6_callback}, /* EXTI6 */
    {EXT_CH_MODE_DISABLED, NULL}, /* EXTI7 */
    {EXT_CH_MODE_DISABLED, NULL}, /* EXTI8 */
    {EXT_CH_MODE_DISABLED, NULL}, /* EXTI9 */
    {EXT_CH_MODE_DISABLED, NULL}, /* EXTI10 */
    {EXT_CH_MODE_DISABLED, NULL}, /* EXTI11 */
    {EXT_CH_MODE_DISABLED, NULL}, /* EXTI12 */
    {EXT_CH_MODE_DISABLED, NULL}, /* EXTI13 */
    {EXT_CH_MODE_DISABLED, NULL}, /* EXTI14 */
    {EXT_CH_MODE_DISABLED, NULL}, /* EXTI15 */
    {EXT_CH_MODE_DISABLED, NULL}, /* EXTI16 (PVD output) */
    {EXT_CH_MODE_DISABLED, NULL}, /* EXTI17 (RTC Alarm event) */
    {EXT_CH_MODE_DISABLED, NULL}, /* EXTI18 (USB OTG FS Wakeup event) */
    {EXT_CH_MODE_DISABLED, NULL}, /* EXTI19 (Ethernet Wakeup event) */
    {EXT_CH_MODE_DISABLED, NULL}, /* EXTI20 (USB OTG HS Wakeup event) */
    {EXT_CH_MODE_DISABLED, NULL}, /* EXTI21 (RTC Tamper and TimeStamp events) */
    {EXT_CH_MODE_DISABLED, NULL} /* EXTI22 (RTC Wakeup event) */
}};

/*
 * Exported function
 */
void ext_config_start(void)
{
    extStart(&EXTD1, &ext_config);
}

/*
 * Callbacks
 */
void EXTI6_callback(EXTDriver *extp, expchannel_t channel)
{
	(void)extp;
	(void)channel;
	thread_t *thread = sensor_hub_get_thread_ptr();

	if(thread != NULL) {
		chSysLockFromISR();
		chEvtSignalI(thread, SENSOR_HUB_EVT_PRESS_NEW_DATA);
		chSysUnlockFromISR();
	}
}