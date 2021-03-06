/*
 * \file
 *
 * \brief utility module for ATSHA204 examples
 *
 * This file contains the main function and some helper functions.
 *
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

#include <string.h>
#include <stdio.h>
#include "asf.h"
#include "compiler.h"
#include "preprocessor.h"
#include "sha204_example_log.h"
#include "serial.h"
#include "conf_usart_serial.h"


void usart_write_crlf(void);
uint8_t convert_nibble_to_hexascii(uint8_t nibble);


/** 
 * \brief This function initializes the logging hardware.
 * 
 */
void log_init(void)
{
	usart_rs232_options_t USART_SERIAL_OPTIONS = {
		.baudrate = USART_SHA204_BAUDRATE,
		.charlength = USART_SHA204_CHAR_LENGTH,
		.paritytype = USART_SHA204_PARITY,
		.stopbits = USART_SHA204_STOP_BIT
	};
	// Initialize the serial port for CDC communication.
	usart_serial_init(USART_SHA204, &USART_SERIAL_OPTIONS);
}


/** \brief This function sends a carriage return and linefeed over USART.
 */
void usart_write_crlf(void)
{
	usart_serial_write_packet(USART_SHA204, (uint8_t *) "\r\n", 2);
}


/** \brief This function converts a nibble to Hex-ASCII.
 * \param[in] nibble nibble value to be converted
 * \return ASCII value
**/
uint8_t convert_nibble_to_hexascii(uint8_t nibble)
{
    nibble &= 0x0F;
    if (nibble <= 0x09 )
        nibble += '0';
    else
        nibble += ('A' - 10);
    return nibble;
}


/** 
 * \brief This function logs the title of a SHA204 transaction.
 * \param[in] title pointer to title string
 */
void log_sha204_title(char *title)
{
	usart_write_crlf();
	usart_serial_write_packet(USART_SHA204, (uint8_t *) title, strlen(title));
	usart_write_crlf();
}


/** 
 * \brief This function logs rx or tx data as hex-ascii.
 * \param[in] length number of characters
 * \param[in] buffer pointer to data
 * \param[in] is_rx true if rx buffer, false if tx_buffer is logged
 */
void log_sha204(uint8_t length, uint8_t *buffer, bool is_rx)
{
	uint8_t i;
	uint8_t nibble;
	
	usart_serial_write_packet(USART_SHA204, (uint8_t *) (is_rx ? "r" : "t"), 1);
	usart_serial_write_packet(USART_SHA204, (uint8_t *) "x: ", 3);
	for (i = 0; i < length; i++) {
		nibble = convert_nibble_to_hexascii(buffer[i] >> 4);
		usart_putchar(USART_SHA204, nibble);
		nibble = convert_nibble_to_hexascii(buffer[i] & 0x0F);
		usart_putchar(USART_SHA204, nibble);
		usart_putchar(USART_SHA204, ' ');
	}
	usart_write_crlf();
}
