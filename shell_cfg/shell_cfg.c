/*
 * shellcfg.c
 *
 *  Created on: 14 paź 2017
 *      Author: krystian
 */

#include "shell_cfg.h"

static char shell_history_buffer[SHELL_MAX_HIST_BUFF];
static char *shell_completion_buffer[SHELL_MAX_COMPLETIONS];

static const ShellCommand shell_commands[] = {
        {NULL, NULL},
};

static ShellConfig shell_cfg = {
        NULL,
        shell_commands,
        shell_history_buffer,
        sizeof(shell_history_buffer),
        shell_completion_buffer
};

static thread_t *shell_thd_ptr = NULL;


void shell_init(BaseSequentialStream *stream) {
    if (!stream) return;

    shell_cfg.sc_channel = stream;

    shell_thd_ptr = chThdCreateFromHeap(NULL, SHELL_WA_SIZE, "shell",
            NORMALPRIO - 1, shellThread, (void*)&shell_cfg);
}
