/*
 * Copyright (c) 2013
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 *  Created on: Jun 20, 2013
 *      Author: David Richmond <dave@prstat.org>
 */

#include <stdio.h>
#include <string.h>

#include "contiki.h"
#include "dev/watchdog.h"
#include "dev/uart0.h"

static void print_processes(struct process * const processes[]){
  printf("Starting");
  while(*processes != NULL){
    printf(" '%s'", (*processes)->name);
    processes++;
  }
  printf("\r\n");
}

int main(int argc, char **argv){
  msp430_cpu_init();
  watchdog_stop();

  uart0_init(115200);
  leds_init();

  clock_init();
  rtimer_init();

  process_init();
  process_start(&etimer_process, NULL);

  printf("****** Booting %s *******\n", CONTIKI_VERSION_STRING);

  print_processes(autostart_processes);
  autostart_start(autostart_processes);

  while(1){
    int r;

    do {
      r = process_run();
    } while(r > 0);
  }
}
