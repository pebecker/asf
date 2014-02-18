/**
 * \file
 *
 * \brief SAM D20 NVM Unit test
 *
 * Copyright (C) 2013 Atmel Corporation. All rights reserved.
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
 * \mainpage SAM D20 NVM Unit Test
 * See \ref appdoc_main "here" for project documentation.
 * \copydetails appdoc_preface
 *
 *
 * \page appdoc_preface Overview
 * This unit test carries out tests for NVM driver.
 * It consists of test cases for the following functionalities:
 *      - Test for driver initialization.
 *      - Test for NVM parameter retrieval.
 *      - Test for FLASH row erasure.
 *      - Test for FLASH page read and write.
 *      - Test for FLASH partial page update.
 */

/**
 * \page appdoc_main SAM D20 NVM Unit Test
 *
 * Overview:
 * - \ref appdoc_samd20_nvm_unit_test_intro
 * - \ref appdoc_samd20_nvm_unit_test_setup
 * - \ref appdoc_samd20_nvm_unit_test_usage
 * - \ref appdoc_samd20_nvm_unit_test_compinfo
 * - \ref appdoc_samd20_nvm_unit_test_contactinfo
 *
 * \section appdoc_samd20_nvm_unit_test_intro Introduction
 * \copydetails appdoc_preface
 *
 * The following kit is required for carrying out the test:
 *      - SAM D20 Xplained Pro board
 *
 * \section appdoc_samd20_nvm_unit_test_setup Setup
 * The following connections has to be made using wires:
 *  - None
 *
 * To run the test:
 *  - Connect the SAM D20 Xplained Pro board to the computer using a
 *    micro USB cable.
 *  - Open the virtual COM port in a terminal application.
 *    \note The USB composite firmware running on the Embedded Debugger (EDBG)
 *          will enumerate as debugger, virtual COM port and EDBG data
 *          gateway.
 *  - Build the project, program the target and run the application.
 *    The terminal shows the results of the unit test.
 *
 * \section appdoc_samd20_nvm_unit_test_usage Usage
 *  - The unit tests are carried out using the internal NVM controller.
 *
 * \section appdoc_samd20_nvm_unit_test_compinfo Compilation Info
 * This software was written for the GNU GCC and IAR for ARM.
 * Other compilers may or may not work.
 *
 * \section appdoc_samd20_nvm_unit_test_contactinfo Contact Information
 * For further information, visit
 * <a href="http://www.atmel.com">http://www.atmel.com</a>.
 */

#include <asf.h>
#include <stdio_serial.h>
#include <string.h>
#include "conf_test.h"

/**
 * \brief Generates byte pattern 1 for a given address
 */
#define BYTE_PATTERN1(x) ((uint8_t)(((x) * 5 + 1) & 0xff))

/**
 * \brief Generates byte pattern 2 for a given address
 */
#define BYTE_PATTERN2(x) \
	((uint8_t)(((x) * 3) & 0xff))

/* Test page is set to 8th from last */
#define TEST_PAGE_ADDR     (NVMCTRL_PAGES - 8) * NVMCTRL_PAGE_SIZE

/* Flag to indicate NVM initialization status */
static volatile bool nvm_init_success = false;

/* Structure for UART module connected to EDBG (used for unit test output) */
struct usart_module cdc_uart_module;

/**
 * \brief Test get flash parameter function
 *
 * This test gets the nvm parameters from the function and checks the
 * NVM page count and NVM page size
 *
 * \param test Current test case.
 */
static void run_nvm_parameter_test(const struct test_case *test)
{
	struct nvm_parameters parameters;

	/* Get the NVM parameters */
	nvm_get_parameters(&parameters);

	/* Validate the page size */
	test_assert_true(test, parameters.page_size == NVMCTRL_PAGE_SIZE,
			"Pagesize incorrect (read: 0x%02x,"
			" expected: 0x%02x)", parameters.page_size,
			NVMCTRL_PAGE_SIZE);

	/* Validate the page count */
	test_assert_true(test, parameters.nvm_number_of_pages == NVMCTRL_PAGES,
			"Number of Pages incorrect (read: 0x%02x,"
			" expected: 0x%02x)", parameters.nvm_number_of_pages,
			NVMCTRL_PAGES);
}

/**
 * \brief Test NVM initialization
 *
 * This function will write the default configuration to NVM
 * and returns an error in case of failure.
 *
 * \param test Current test case.
 */
static void run_nvm_init_test(const struct test_case *test)
{
	struct nvm_config config;
	enum status_code status;

	/* Get the default configuration */
	nvm_get_config_defaults(&config);

	/* Set wait state to 1 */
	config.wait_states = 1;

	/* Set the NVM configuration */
	status = nvm_set_config(&config);

	/* Validate the initialization */
	test_assert_true(test, status == STATUS_OK,
			"NVM Initialization error");

	if (status == STATUS_OK) {
		nvm_init_success = true;
	}
}

/**
 * \brief Test NVM erasing
 *
 * This test erases test row and verifies it
 *
 * \param test Current test case.
 */
static void run_nvm_erase_test(const struct test_case *test)
{
	uint16_t offset = 0;
	volatile uint32_t test_page_address;
	test_page_address = TEST_PAGE_ADDR;
	volatile uint32_t *test_address;
	test_address = (uint32_t *) test_page_address;
	enum status_code status;

	/* Skip test if NVM initialization failed */
	test_assert_true(test, nvm_init_success == true,
			"NVM initialization failed, skipping test");

	/* Erase the row at test address */
	status = nvm_erase_row(test_page_address);

	/* Validate erase operation is complete */
	test_assert_true(test, status == STATUS_OK,
			"Erase operation not completed");

	/* Validate whether the data in NVM is 0xff after erasing */
	for (offset  = 0; offset < ((NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE) / 4);
			offset++) {
		test_assert_true(test, ((uint32_t)test_address[offset]) == 0xffffffff,
				"Flash erase error @ %d offset", offset);
	}
}

/**
 * \brief Test NVM writing
 *
 * This test writes erased test page with a pattern, reads it back
 * and checks the integrity
 *
 * \param test Current test case.
 */
static void run_nvm_read_and_write_test(const struct test_case *test)
{
	uint8_t buffer[NVMCTRL_PAGE_SIZE], i;
	enum status_code status;

	/* Skip test if NVM initialization failed */
	test_assert_true(test, nvm_init_success == true,
			"NVM initialization failed, skipping test");

	/* Fill the buffer with Pattern 1 */
	for (i = 0; i < NVMCTRL_PAGE_SIZE; i++) {
		buffer[i] = BYTE_PATTERN1(i);
	}

	/* Write the buffer to Test address in NVM */
	status = nvm_write_buffer(TEST_PAGE_ADDR, buffer, NVMCTRL_PAGE_SIZE);

	/* Validate whether write operation is complete */
	test_assert_true(test, status == STATUS_OK,
			"Write operation error");

	/* Flush the buffer */
	for (i = 0; i < NVMCTRL_PAGE_SIZE; i++) {
		buffer[i] = 0;
	}

	/* Read the NVM contents at test address to buffer */
	status = nvm_read_buffer(TEST_PAGE_ADDR, buffer, NVMCTRL_PAGE_SIZE);

	/* Validate whether read operation is complete */
	test_assert_true(test, status == STATUS_OK,
			"Read operation error");

	/* Check the integrity of data in NVM */
	for (i = 0; i < NVMCTRL_PAGE_SIZE; i++) {
		test_assert_true(test, buffer[i] == BYTE_PATTERN1(i),
			"Value not expected @ byte %d (read: 0x%02x,"
			" expected: 0x%02x)", i, buffer[i], BYTE_PATTERN1(i));
	}
 }

/**
 * \brief Test NVM update
 *
 * This test modifies first half of the test page with different pattern,
 * reads it back and checks whether the values are updated
 *
 * \param test Current test case.
 */
static void run_nvm_update_test(const struct test_case *test)
{
	uint8_t buffer[NVMCTRL_PAGE_SIZE], i;
	enum status_code status;

	/* Skip test if NVM initialization failed */
	test_assert_true(test, nvm_init_success == true,
			"NVM initialization failed, skipping test");

	/* Fill half of the buffer with pattern 2 */
	for (i = 0; i < (NVMCTRL_PAGE_SIZE / 2); i++) {
		buffer[i] = BYTE_PATTERN2(i);
	}

	/* Update first half of the test page with new data */
	status = nvm_update_buffer(TEST_PAGE_ADDR, buffer, 0,
		(NVMCTRL_PAGE_SIZE / 2));

	/* Validate whether the update operation is complete */
	test_assert_true(test, status == STATUS_OK,
			"Update operation error");

	/* Flush the buffer */
	for (i = 0; i < NVMCTRL_PAGE_SIZE; i++) {
		buffer[i] = 0;
	}

	/* Read the NVM contents at test address to buffer */
	status = nvm_read_buffer(TEST_PAGE_ADDR, buffer, NVMCTRL_PAGE_SIZE);

	/* Validate whether the read operation is complete */
	test_assert_true(test, status == STATUS_OK,
			"Read operation error");

	/* Check the integrity of data in NVM */
	for (i = 0; i < (NVMCTRL_PAGE_SIZE / 2); i++) {
		test_assert_true(test, buffer[i] == BYTE_PATTERN2(i),
				"Value not expected @ byte %d (read: 0x%02x,"
				" expected: 0x%02x)", i, buffer[i], BYTE_PATTERN2(i));
	}
	for (i = (NVMCTRL_PAGE_SIZE / 2); i < NVMCTRL_PAGE_SIZE; i++) {
		test_assert_true(test, buffer[i] == BYTE_PATTERN1(i),
				"Value not expected @ byte %d (read: 0x%02x,"
				" expected: 0x%02x)", i, buffer[i], BYTE_PATTERN1(i));
	}
}

/**
 * \brief Test NVM fuses functions
 *
 * This test will test the fuse API functions
 * and returns an error in case of failure.
 *
 * \param test Current test case.
 */
static void run_nvm_fuses_test(const struct test_case *test)
{
	struct nvm_fusebits fuses;
	enum status_code status;
	uint8_t eeprom_fuse_value;

	/* Get the original configuration */
	nvm_get_fuses(&fuses);

	/* Set EEPROM size */
	eeprom_fuse_value = fuses.eeprom_size;
	if (eeprom_fuse_value == 7) {
		eeprom_fuse_value = NVM_EEPROM_EMULATOR_SIZE_16384;
	} else {
		eeprom_fuse_value++;
	}
	fuses.eeprom_size = (enum nvm_eeprom_emulator_size)(eeprom_fuse_value);

	/* Set the NVM configuration */
	status = nvm_set_fuses(&fuses);

	/* Validate whether the set configuration is complete */
	test_assert_true(test, status == STATUS_OK,
			"NVM fuses set error");

	/* Get the configuration after change. */
	nvm_get_fuses(&fuses);

	/* Validate the changed EEPROM fuse value */
	test_assert_true(test, fuses.eeprom_size == eeprom_fuse_value,
			"NVM fuses set error");
}

/**
 * \brief Initialize USART for unit tests
 *
 * Initializes the USART used by the unit test. The USART connected to
 * embedded debugger is used for outputting the results.
 *
 */
static void cdc_uart_init(void)
{
	struct usart_config usart_conf;

	/* Configure USART for unit test output */
	usart_get_config_defaults(&usart_conf);
	usart_conf.mux_setting = CONF_STDIO_MUX_SETTING;
	usart_conf.pinmux_pad0 = CONF_STDIO_PINMUX_PAD0;
	usart_conf.pinmux_pad1 = CONF_STDIO_PINMUX_PAD1;
	usart_conf.pinmux_pad2 = CONF_STDIO_PINMUX_PAD2;
	usart_conf.pinmux_pad3 = CONF_STDIO_PINMUX_PAD3;
	usart_conf.baudrate    = CONF_STDIO_BAUDRATE;

	stdio_serial_init(&cdc_uart_module, CONF_STDIO_USART, &usart_conf);
	usart_enable(&cdc_uart_module);
}

/**
 * \brief Run NVM unit tests
 *
 * Initializes the system and serial output, then sets up the
 * NVM unit test suite and runs it.
 */
int main(void)
{
	system_init();
	cdc_uart_init();

	/* Define Test Cases */
	DEFINE_TEST_CASE(nvm_paramter_test, NULL,
			run_nvm_parameter_test, NULL,
			"NVM parameter check");

	DEFINE_TEST_CASE(nvm_init_test, NULL,
			run_nvm_init_test, NULL,
			"NVM initialization");

	DEFINE_TEST_CASE(nvm_erase_test, NULL,
			run_nvm_erase_test, NULL,
			"NVM erase row");

	DEFINE_TEST_CASE(nvm_read_and_write_test, NULL,
			run_nvm_read_and_write_test, NULL,
			"NVM page read and write");

	DEFINE_TEST_CASE(nvm_update_test, NULL,
			run_nvm_update_test, NULL,
			"NVM page update");

	DEFINE_TEST_CASE(nvm_fuses_test, NULL,
			run_nvm_fuses_test, NULL,
			"NVM fuses test");

	/* Put test case addresses in an array */
	DEFINE_TEST_ARRAY(nvm_tests) = {
			&nvm_paramter_test,
			&nvm_init_test,
			&nvm_erase_test,
			&nvm_read_and_write_test,
			&nvm_update_test,
			&nvm_fuses_test,
			};

	/* Define the test suite */
	DEFINE_TEST_SUITE(nvm_suite, nvm_tests,
			"SAM D20 NVM driver test suite");

	/* Run all tests in the suite*/
	test_suite_run(&nvm_suite);

	while (1) {
		/* Intentionally left empty */
	}

}
