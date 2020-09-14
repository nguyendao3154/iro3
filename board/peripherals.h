/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PERIPHERALS_H_
#define _PERIPHERALS_H_

/***********************************************************************************************************************
 * Included files
 **********************************************************************************************************************/
#include "fsl_common.h"
#include "fsl_lpuart.h"
#include "fsl_clock.h"
#include "fsl_lptmr.h"
#include "fsl_ftm.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/
/* Definitions for BOARD_InitPeripherals functional group */
/* Definition of peripheral ID */
#define LPUART_1_PERIPHERAL LPUART0
/* Definition of the clock source frequency */
#define LPUART_1_CLOCK_SOURCE 24000000UL
/* LPUART_1 interrupt vector ID (number). */
#define LPUART_1_SERIAL_RX_TX_IRQN LPUART0_IRQn
/* LPUART_1 interrupt handler identifier. */
#define LPUART_1_SERIAL_RX_TX_IRQHANDLER LPUART0_IRQHandler
/* BOARD_InitPeripherals defines for LPTMR0 */
/* Definition of peripheral ID */
#define LPTMR_1_PERIPHERAL LPTMR0
/* Definition of the clock source frequency */
#define LPTMR_1_CLK_FREQ 128000UL
/* Definition of the prescaled clock source frequency */
#define LPTMR_1_INPUT_FREQ 128000UL
/* Definition of the timer period in us */
#define LPTMR_1_USEC_COUNT 203UL
/* Definition of the timer period in number of ticks */
#define LPTMR_1_TICKS 26UL
/* LPTMR_1 interrupt vector ID (number). */
#define LPTMR_1_IRQN PWT_LPTMR0_IRQn
/* LPTMR_1 interrupt handler identifier. */
#define LPTMR_1_IRQHANDLER PWT_LPTMR0_IRQHandler
/* Definition of peripheral ID */
#define FTM_1_PERIPHERAL FTM0
/* Definition of the clock source frequency */
#define FTM_1_CLOCK_SOURCE 48000000UL
/* FTM_1 interrupt vector ID (number). */
#define FTM_1_IRQN FTM0_IRQn
/* FTM_1 interrupt handler identifier. */
#define FTM_1_IRQHANDLER FTM0_IRQHandler

/***********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/
extern const lpuart_config_t LPUART_1_config;
extern const lptmr_config_t LPTMR_1_config;
extern const ftm_config_t FTM_1_config;

/***********************************************************************************************************************
 * Initialization functions
 **********************************************************************************************************************/
void BOARD_InitPeripherals(void);

/***********************************************************************************************************************
 * BOARD_InitBootPeripherals function
 **********************************************************************************************************************/
void BOARD_InitBootPeripherals(void);

#if defined(__cplusplus)
}
#endif

#endif /* _PERIPHERALS_H_ */
