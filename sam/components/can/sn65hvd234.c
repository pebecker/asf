/**
 * \file
 *
 * \brief CAN transceiver SN65HVD234 driver.
 *
 * Copyright (c) 2011 - 2012 Atmel Corporation. All rights reserved.
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

/**
 * \defgroup sam_component_sn65hvd234_transceiver_group CAN - SN65HVD234 Transceiver
 *
 * This driver provides access to the main features of the SN65HVD234 transceiver.
 * Control the RS and EN pin level for SN65HVD234.
 *
 * \{
 */

#include "board.h"
#include "sn65hvd234.h"
#include "gpio.h"

/**
 * \brief Initialize RS pin for transceiver.
 *
 * \param p_component Pointer to SN65HVD234 control structure.
 * \param pin_idx     The pin index value for transceiver RS pin.
 */
void sn65hvd234_set_rs(sn65hvd234_ctrl_t *p_component, uint32_t pin_idx)
{
	p_component->pio_rs_idx = pin_idx;
}

/**
 * \brief Initialize EN pin for transceiver.
 *
 * \param p_component Pointer to SN65HVD234 control structure.
 * \param pin_idx     The pin index value for transceiver EN pin.
 */
void sn65hvd234_set_en(sn65hvd234_ctrl_t *p_component, uint32_t pin_idx)
{
	p_component->pio_en_idx = pin_idx;
}

/**
 * \brief Enable transceiver.
 *
 * \param p_component Pointer to SN65HVD234 control structure.
 */
void sn65hvd234_enable(sn65hvd234_ctrl_t *p_component)
{
	/* Raise EN pin of SN65HVD234 to High Level (Vcc). */
	gpio_set_pin_high(p_component->pio_en_idx);
}

/**
 * \brief Disable transceiver.
 *
 * \param p_component Pointer to SN65HVD234 control structure.
 */
void sn65hvd234_disable(sn65hvd234_ctrl_t *p_component)
{
	/* Lower EN pin of SN65HVD234 to Low Level (0.0v). */
	gpio_set_pin_low(p_component->pio_en_idx);
}

/**
 * \brief Turn the component into low power mode, listening only.
 *
 * \param p_component Pointer to SN65HVD234 control structure.
 */
void sn65hvd234_enable_low_power(sn65hvd234_ctrl_t *p_component)
{
	/* Raise RS pin of SN65HVD234 to more than 0.75v. */
	gpio_set_pin_high(p_component->pio_rs_idx);
}

/**
 * \brief Resume to Normal mode by exiting from low power mode.
 *
 * \param p_component Pointer to SN65HVD234 control structure.
 */
void sn65hvd234_disable_low_power(sn65hvd234_ctrl_t *p_component)
{
	/* Lower RS pin of SN65HVD234 to 0.0v~0.33v. */
	gpio_set_pin_low(p_component->pio_rs_idx);
}

/**
 * \}
 */
