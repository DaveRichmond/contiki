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

#include "contiki.h"
#include "dev/leds.h"

PROCESS(red_process, "Red Blink Process");
PROCESS(green_process, "Green Blink Process");
AUTOSTART_PROCESSES(&green_process, &red_process);

static struct etimer etr;
PROCESS_THREAD(red_process, ev, data){
  PROCESS_BEGIN();

  while(1){
    leds_toggle(LEDS_RED);
    etimer_set(&etr, CLOCK_SECOND/8);
    PROCESS_WAIT_UNTIL(etimer_expired(&etr));
  }

  PROCESS_END();
}

static struct etimer etg;
PROCESS_THREAD(green_process, ev, data){
  PROCESS_BEGIN();

  while(1){
    leds_toggle(LEDS_GREEN);
    etimer_set(&etg, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&etg));
  }
  PROCESS_END();
}
