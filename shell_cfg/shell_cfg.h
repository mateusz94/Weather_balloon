/*
 * shellcfg.h
 *
 *  Created on: 14 paź 2017
 *      Author: krystian
 */

#ifndef SHELL_CFG_H_
#define SHELL_CFG_H_

#include "ch.h"
#include "hal.h"
#include "shell.h"

#define SHELL_WA_SIZE               (2048)

void shell_init(BaseSequentialStream *stream);

#endif /* SHELL_CFG_H_ */
