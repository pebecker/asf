/**
 * \file
 *
 * \brief Chip-specific system clock management functions.
 *
 * Copyright (c) 2013 Atmel Corporation. All rights reserved.
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

#ifndef CHIP_SYSCLK_H_INCLUDED
#define CHIP_SYSCLK_H_INCLUDED

#include <osc.h>
#include <pll.h>

/**
 * \page sysclk_quickstart Quick Start Guide for the System Clock Management service (SAM4CP)
 *
 * This is the quick start guide for the \ref sysclk_group "System Clock Management"
 * service, with step-by-step instructions on how to configure and use the service for
 * specific use cases.
 *
 * \section sysclk_quickstart_usecases System Clock Management use cases
 * - \ref sysclk_quickstart_basic
 *
 * \section sysclk_quickstart_basic Basic usage of the System Clock Management service
 * This section will present a basic use case for the System Clock Management service.
 * This use case will configure the main system clock to 120MHz, using an internal PLLB
 * module to multiply the frequency of a crystal attached to the microcontroller.
 *
 * \subsection sysclk_quickstart_use_case_1_prereq Prerequisites
 *  - None
 *
 * \subsection sysclk_quickstart_use_case_1_setup_steps Initialization code
 * Add to the application initialization code:
 * \code
 *    sysclk_init();
 * \endcode
 *
 * \subsection sysclk_quickstart_use_case_1_setup_steps_workflow Workflow
 * -# Configure the system clocks according to the settings in conf_clock.h:
 *    \code sysclk_init(); \endcode
 *
 * \subsection sysclk_quickstart_use_case_1_example_code Example code
 *   Add or uncomment the following in your conf_clock.h header file, commenting out all other
 *   definitions of the same symbol(s):
 *   \code
 *   #define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_PLLBCK
 *
 *   // Fpll1 = (Fclk * PLL_mul) / PLL_div
 *   #define CONFIG_PLL1_SOURCE          PLLB_SRC_MAINCK_XTAL
 *   #define CONFIG_PLL1_MUL             (240000000UL / BOARD_FREQ_MAINCK_XTAL)
 *   #define CONFIG_PLL1_DIV             2
 *
 *   // Fbus = Fsys / BUS_div
 *   #define CONFIG_SYSCLK_PRES          SYSCLK_PRES_1
 *   \endcode
 *
 * \subsection sysclk_quickstart_use_case_1_example_workflow Workflow
 *  -# Configure the main system clock to use the output of the PLL module as its source:
 *   \code #define CONFIG_SYSCLK_SOURCE          SYSCLK_SRC_PLLBCK \endcode
 *  -# Configure the PLL module to use the external crystal oscillator as its source:
 *   \code #define CONFIG_PLL1_SOURCE            PLLB_SRC_MAINCK_XTAL \endcode
 *  -# Configure the PLL module to multiply the external crystal oscillator frequency up to 200MHz:
 *   \code
 *   #define CONFIG_PLL1_MUL             (240000000UL / BOARD_FREQ_MAINCK_XTAL)
 *   #define CONFIG_PLL1_DIV             1
 *   \endcode
 *   \note For user boards, \c BOARD_FREQ_MAINCK_XTAL should be defined in the board \c conf_board.h configuration
 *         file as the frequency of the fast crystal attached to the microcontroller.
 *  -# Configure the main clock to run at the full 120MHz, set prescaler to 1:
 *    \code
 *    #define CONFIG_SYSCLK_PRES         SYSCLK_PRES_1
 *    \endcode
 */

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/**
 * \weakgroup sysclk_group
 * @{
 */

//! \name Configuration Symbols
//@{

/**
 * \def CONFIG_SYSCLK_SOURCE
 * \brief Initial/static main system clock source for core 0
 *
 * The main system clock will be configured to use this clock during
 * initialization.
 */
#ifndef CONFIG_SYSCLK_SOURCE
# define CONFIG_SYSCLK_SOURCE   SYSCLK_SRC_MAINCK_4M_RC
#endif

/**
 * \def CONFIG_SYSCLK_PRES
 * \brief Initial CPU clock divider for core 0 (mck)
 *
 * The MCK will run at
 * \f[
 *   f_{MCK} = \frac{f_{sys}}{\mathrm{CONFIG\_SYSCLK\_PRES}}\,\mbox{Hz}
 * \f]
 * after initialization.
 */
#ifndef CONFIG_SYSCLK_PRES
# define CONFIG_SYSCLK_PRES  SYSCLK_PRES_1
#endif

/**
 * \def CONFIG_CPCLK_SOURCE
 * \brief Initial/static main system clock source for core 1
 *
 * The main system clock will be configured to use this clock during
 * initialization.
 */
#ifndef CONFIG_CPCLK_SOURCE
# define CONFIG_CPCLK_SOURCE   CPCLK_SRC_MCK
#endif

/**
 * \def CONFIG_CPCLK_PRES
 * \brief Initial CPU clock divider for core 1 (mck)
 *
 * The MCK will run at
 * \f[
 *   f_{MCK} = \frac{f_{sys}}{\mathrm{CONFIG\_CPCLK\_PRES}}\,\mbox{Hz}
 * \f]
 * after initialization.
 */
#ifndef CONFIG_CPCLK_PRES
# define CONFIG_CPCLK_PRES  1
#endif

//@}

//! \name Master Clock Sources (MCK)
//@{
#define SYSCLK_SRC_SLCK_RC              0       //!< Internal 32kHz RC oscillator as master source clock
#define SYSCLK_SRC_SLCK_XTAL            1       //!< External 32kHz crystal oscillator as master source clock
#define SYSCLK_SRC_SLCK_BYPASS          2       //!< External 32kHz bypass oscillator as master source clock
#define SYSCLK_SRC_MAINCK_4M_RC         3       //!< Internal 4MHz RC oscillator as master source clock
#define SYSCLK_SRC_MAINCK_8M_RC         4       //!< Internal 8MHz RC oscillator as master source clock
#define SYSCLK_SRC_MAINCK_12M_RC        5       //!< Internal 12MHz RC oscillator as master source clock
#define SYSCLK_SRC_MAINCK_XTAL          6       //!< External crystal oscillator as master source clock
#define SYSCLK_SRC_MAINCK_BYPASS        7       //!< External bypass oscillator as master source clock
#define SYSCLK_SRC_PLLACK               8       //!< Use PLLACK as master source clock
#define SYSCLK_SRC_PLLBCK               9       //!< Use PLLBCK as master source clock
//@}

//! \name Master Clock Prescalers (MCK)
//@{
#define SYSCLK_PRES_1           PMC_MCKR_PRES_CLK_1     //!< Set master clock prescaler to 1
#define SYSCLK_PRES_2           PMC_MCKR_PRES_CLK_2     //!< Set master clock prescaler to 2
#define SYSCLK_PRES_4           PMC_MCKR_PRES_CLK_4     //!< Set master clock prescaler to 4
#define SYSCLK_PRES_8           PMC_MCKR_PRES_CLK_8     //!< Set master clock prescaler to 8
#define SYSCLK_PRES_16          PMC_MCKR_PRES_CLK_16    //!< Set master clock prescaler to 16
#define SYSCLK_PRES_32          PMC_MCKR_PRES_CLK_32    //!< Set master clock prescaler to 32
#define SYSCLK_PRES_64          PMC_MCKR_PRES_CLK_64    //!< Set master clock prescaler to 64
#define SYSCLK_PRES_3           PMC_MCKR_PRES_CLK_3     //!< Set master clock prescaler to 3
//@}

//! \name Coprocessor Master Clock Sources (CPMCK)
//@{
#define CPCLK_SRC_SLCK           0   //!< 32kHz Slow Clock as coprocessor master source clock
#define CPCLK_SRC_MAINCK         1   //!< Main Clock as coprocessor master source clock
#define CPCLK_SRC_PLLACK         2   //!< Use PLLACK as coprocessor master source clock
#define CPCLK_SRC_PLLBCK         3   //!< Use PLLBCK as coprocessor master source clock
#define CPCLK_SRC_MCK            4   //!< Use PLLBCK as coprocessor master source clock
//@}

//! \name Coprocessor Master Clock Prescalers (CPMCK)
//@{
#define CPCLK_PRES_MIN          1     //!< Minimal value for coprocessor master clock prescaler
#define CPCLK_PRES_MAX          16    //!< Maximal value for coprocessor master clock prescaler
//@}

/**
 * \name Querying the system clock
 *
 * The following functions may be used to query the current frequency of
 * the system clock and the CPU and bus clocks derived from it.
 * sysclk_get_main_hz() and sysclk_get_cpu_hz() can be assumed to be
 * available on all platforms, although some platforms may define
 * additional accessors for various chip-internal bus clocks. These are
 * usually not intended to be queried directly by generic code.
 */
//@{

/**
 * \brief Return the current rate in Hz of the main system clock (Core 0)
 *
 * \todo This function assumes that the main clock source never changes
 * once it's been set up, and that PLL always runs at the compile-time
 * configured default rate. While this is probably the most common
 * configuration, which we want to support as a special case for
 * performance reasons, we will at some point need to support more
 * dynamic setups as well.
 */
#if (defined CONFIG_SYSCLK_DEFAULT_RETURNS_SLOW_OSC)
extern uint32_t sysclk_initialized;
#endif
static inline uint32_t sysclk_get_main_hz_core0(void)
{
#if (defined CONFIG_SYSCLK_DEFAULT_RETURNS_SLOW_OSC)
	if (!sysclk_initialized ) {
		return OSC_MAINCK_4M_RC_HZ;
	}
#endif

	/* Config system clock setting */
	if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_SLCK_RC) {
		return OSC_SLCK_32K_RC_HZ;
	} else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_SLCK_XTAL) {
		return OSC_SLCK_32K_XTAL_HZ;
	} else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_SLCK_BYPASS) {
		return OSC_SLCK_32K_BYPASS_HZ;
	} else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_MAINCK_4M_RC) {
		return OSC_MAINCK_4M_RC_HZ;
	} else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_MAINCK_8M_RC) {
		return OSC_MAINCK_8M_RC_HZ;
	} else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_MAINCK_12M_RC) {
		return OSC_MAINCK_12M_RC_HZ;
	} else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_MAINCK_XTAL) {
		return OSC_MAINCK_XTAL_HZ;
	} else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_MAINCK_BYPASS) {
		return OSC_MAINCK_BYPASS_HZ;
	}
#ifdef CONFIG_PLL0_SOURCE
	else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_PLLACK) {
		return pll_get_default_rate(0);
	}
#endif

#ifdef CONFIG_PLL1_SOURCE
	else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_PLLBCK) {
		if (CONFIG_PLL1_SOURCE == PLLB_SRC_PLLA) {
			return (PLLA_OUTPUT_HZ * CONFIG_PLL1_MUL / CONFIG_PLL1_DIV);
		} else {
			return pll_get_default_rate(1);
		}
	}
#endif

	else {
		/* unhandled_case(CONFIG_SYSCLK_SOURCE); */
		return 0;
	}
}

#ifdef CONFIG_CPCLK_ENABLE
/**
 * \brief Return the current rate in Hz of the main system clock (Core 1)
 *
 * \todo This function assumes that the main clock source never changes
 * once it's been set up, and that PLL always runs at the compile-time
 * configured default rate. While this is probably the most common
 * configuration, which we want to support as a special case for
 * performance reasons, we will at some point need to support more
 * dynamic setups as well.
 */
static inline uint32_t sysclk_get_main_hz_core1(void)
{
	/* Config system clock setting */
	if (CONFIG_CPCLK_SOURCE == CPCLK_SRC_SLCK) {
		if (SUPC->SUPC_SR & SUPC_SR_OSCSEL) {
			return CHIP_FREQ_XTAL_32K;
		} else {
			return CHIP_FREQ_SLCK_RC;
		}
	} else if (CONFIG_CPCLK_SOURCE == CPCLK_SRC_MAINCK) {
		if (PMC->CKGR_MOR & CKGR_MOR_MOSCSEL) {
			return CHIP_FREQ_PLLA_TYPICAL;
		} else {
			uint32_t mor_moscrcf = PMC->CKGR_MOR & CKGR_MOR_MOSCRCF_Msk;
			if (mor_moscrcf == CKGR_MOR_MOSCRCF_4_MHz) {
				return CHIP_FREQ_MAINCK_RC_4MHZ;
			} else if (mor_moscrcf == CKGR_MOR_MOSCRCF_8_MHz) {
				return CHIP_FREQ_MAINCK_RC_8MHZ;
			} else if (mor_moscrcf == CKGR_MOR_MOSCRCF_12_MHz) {
				return CHIP_FREQ_MAINCK_RC_12MHZ;
			} else {
				/* unhandled_case(CONFIG_CPSCLK_SOURCE); */
				return 0;
			}
		}
#ifdef CONFIG_PLL0_SOURCE
	} else if (CONFIG_CPCLK_SOURCE == CPCLK_SRC_PLLACK) {
		return pll_get_default_rate(0);
#endif
#ifdef CONFIG_PLL1_SOURCE
	} else if (CONFIG_CPCLK_SOURCE == CPCLK_SRC_PLLBCK) {
		if (CONFIG_PLL1_SOURCE == PLLB_SRC_PLLA) {
			return (PLLA_OUTPUT_HZ * CONFIG_PLL1_MUL / CONFIG_PLL1_DIV);
		} else {
			return pll_get_default_rate(1);
		}
#endif
	} else if (CONFIG_CPCLK_SOURCE == CPCLK_SRC_MCK) {
		return sysclk_get_main_hz_core0() /
				((CONFIG_SYSCLK_PRES == SYSCLK_PRES_3) ? 3 :
				(1 << (CONFIG_SYSCLK_PRES >> PMC_MCKR_PRES_Pos)));
	}

	else {
		/* unhandled_case(CONFIG_CPSCLK_SOURCE); */
		return 0;
	}
}
#endif

/**
 * \brief Return the current rate in Hz of the main system clock
 *
 * \todo This function assumes that the main clock source never changes
 * once it's been set up, and that PLL always runs at the compile-time
 * configured default rate. While this is probably the most common
 * configuration, which we want to support as a special case for
 * performance reasons, we will at some point need to support more
 * dynamic setups as well.
 */
static inline uint32_t sysclk_get_main_hz(void)
{
#if (SAM4CP_0)
	return sysclk_get_main_hz_core0();
#elif (SAM4CP_1) && (defined CONFIG_CPCLK_ENABLE)
	return sysclk_get_main_hz_core1();
#else
#error "No specifid core 0 or core 1 for clock service."
#endif
}

/**
 * \brief Return the current rate in Hz of the CPU clock
 *
 * \todo This function assumes that the CPU always runs at the system
 * clock frequency. We want to support at least two more scenarios:
 * Fixed CPU/bus clock dividers (config symbols) and dynamic CPU/bus
 * clock dividers (which may change at run time). Ditto for all the bus
 * clocks.
 *
 * \return Frequency of the CPU clock, in Hz.
 */
static inline uint32_t sysclk_get_cpu_hz(void)
{
#if (SAM4CP_0)
	/* CONFIG_SYSCLK_PRES is the register value for setting the expected */
	/* prescaler, not an immediate value. */
	return sysclk_get_main_hz() /
		((CONFIG_SYSCLK_PRES == SYSCLK_PRES_3) ? 3 :
			(1 << (CONFIG_SYSCLK_PRES >> PMC_MCKR_PRES_Pos)));
#elif (SAM4CP_1)
	return sysclk_get_main_hz() / CONFIG_CPCLK_PRES;
#else
#error "No specifid core 0 or core 1 for clock service."
#endif
}

/**
 * \brief Retrieves the current rate in Hz of the peripheral clocks.
 *
 * \return Frequency of the peripheral clocks, in Hz.
 */
static inline uint32_t sysclk_get_peripheral_hz(void)
{
	return sysclk_get_cpu_hz();
}

/**
 * \brief Retrieves the current rate in Hz of the Peripheral Bus clock attached
 *        to the specified peripheral.
 *
 * \param module Pointer to the module's base address.
 *
 * \return Frequency of the bus attached to the specified peripheral, in Hz.
 */
static inline uint32_t sysclk_get_peripheral_bus_hz(const volatile void *module)
{
	UNUSED(module);
	return sysclk_get_peripheral_hz();
}
//@}

//! \name Enabling and disabling synchronous clocks
//@{

/**
 * \brief Enable a peripheral's clock.
 *
 * \param ul_id Id (number) of the peripheral clock.
 */
static inline void sysclk_enable_peripheral_clock(uint32_t ul_id)
{
	pmc_enable_periph_clk(ul_id);
}

/**
 * \brief Disable a peripheral's clock.
 *
 * \param ul_id Id (number) of the peripheral clock.
 */
static inline void sysclk_disable_peripheral_clock(uint32_t ul_id)
{
	pmc_disable_periph_clk(ul_id);
}

//@}

//! \name System Clock Source and Prescaler configuration
//@{

extern void sysclk_set_prescalers(uint32_t ul_pres);
extern void sysclk_set_source(uint32_t ul_src);

//@}

extern void sysclk_init(void);

//! @}

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond

#endif /* CHIP_SYSCLK_H_INCLUDED */