/**
 * \file
 *
 * \brief TWI Slave Example configuration.
 *
 * Copyright (c) 2011-2012 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

/* Configuration of TWI Slave Example */

#ifndef CONF_TWI_SLAVE_EXAMPLE_H_INCLUDED
#define CONF_TWI_SLAVE_EXAMPLE_H_INCLUDED

/** Definition of TWI interrupt ID on board. */
#define BOARD_TWI_IRQn          TWI0_IRQn
#define BOARD_TWI_Handler       TWI0_Handler

/** TWI ID for SLAVE application to use */
#define BOARD_ID_TWI_SLAVE          ID_TWI0
/** TWI Base for TWI SLAVE application to use */
#define BOARD_BASE_TWI_SLAVE        TWI0
/** TWI pins for TWI SLAVE application to use */
#define BOARD_PINS_TWI_SLAVE        PINS_TWI0

/** TWI Base for TWI MASTER application to use */
#define BOARD_BASE_TWI_MASTER        TWI0

#define BOARD_ID_USART             ID_USART0

#define BOARD_USART                USART0

#define BOARD_USART_BAUDRATE       115200

#define USART_Handler              USART0_Handler

#define USART_IRQn                 USART0_IRQn

#endif /* CONF_TWI_SLAVE_EXAMPLE_H_INCLUDED */
