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

#include "contiki.h"
#include "button.h"
#include "isr_compat.h"

#define DEBOUNCE_TIME (CLOCK_SECOND/16)
static struct timer debounce_timer;

void button_init(){
  button_event = process_alloc_event();

  dint();
  BUTTON_PORT(SEL) &= ~BUTTON_PINS;
  BUTTON_PORT(DIR) &= ~BUTTON_PINS;

#if BUTTON_INTERRUPT
  BUTTON_PORT(IE) |= BUTTON_PINS;
  BUTTON_PORT(IFG) &= ~BUTTON_PINS;
#endif
  eint();

  timer_set(&debounce_timer, CLOCK_SECOND/CLOCK_SECOND);
}


uint8_t button_pressed(void){
  return BUTTON_PORT(IN) & BUTTON_PINS;
}

#if BUTTON_INTERRUPT
volatile uint8_t btnp = 0;
ISR(PORT3, button_interrupt){
  if(BUTTON_PORT(IFG) & BUTTON_PINS){
    BUTTON_PORT(IFG) &= ~BUTTON_PINS;
    if(timer_expired(&debounce_timer)){
      btnp = BUTTON_PORT(IN);
      timer_set(&debounce_timer, DEBOUNCE_TIME);
      process_post(PROCESS_BROADCAST, button_event, (void *)&btnp);

      LPM4_EXIT;
    }
  }
}
#else
PROCESS(button_poll_process, "Button polling process");

static inline uint8_t read_buttons(){ return (BUTTON_PORT(IN) & BUTTON_PINS) ^ BUTTON_PINS; }
uint8_t button_data, new_button_data;
static struct etimer et;
PROCESS_THREAD(button_poll_process, ev, data){
  PROCESS_BEGIN();

  button_data = 0;

  while(1){
    new_button_data = read_buttons();

    if(new_button_data != button_data){
      process_post(PROCESS_BROADCAST, button_event, &new_button_data);
    }

    button_data = new_button_data;

    etimer_set(&et, 10);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  }

  PROCESS_END();
}
#endif
