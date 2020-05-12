/**
 *  pca10040.h : PCA10040 board definitions
 *  Copyright (C) 2019  Appiko
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * @addtogroup group_platform
 * @{
 *
 * @defgroup board_pca10040 PCA10040 nRF52-DK
 * @brief The nRF52 Development Kit (DK) platform PCA10040
 * @{
 */

#ifndef PLATFORM_PCA10040_H_
#define PLATFORM_PCA10040_H_

/** @anchor pca10040-leds
 * @name LED definitions for PCA10040
 * @{*/
#define DFU_LED_1          17
#define DFU_LED_2          18
#define DFU_LED_3          19
#define DFU_LED_4          20
#define DFU_LED_RED        17
#define DFU_LED_GREEN      18
/** The output level at which the LEDs shine */
#define DFU_LEDS_ACTIVE_STATE 0
/** @} */

/** @anchor pca10040-buttons
 * @name Button definitions for PCA10040
 * @{*/
#define DFU_BUTTON_1       13
#define DFU_BUTTON_2       14
#define DFU_BUTTON_3       15
#define DFU_BUTTON_4       16
#define DFU_BUTTON_PIN     13
/** The input level when the button is pressed */
#define DFU_BUTTONS_ACTIVE_STATE 0
/** The kind of internal resistors required for the button */
#define DFU_BUTTON_PULL    HAL_GPIO_PULLUP
/** @} */

/** @anchor pca10040-serial
 * @name Serial port definitions for PCA10040
 * @{*/
#define DFU_RX_PIN_NUMBER  8
#define DFU_TX_PIN_NUMBER  6
#define DFU_CTS_PIN_NUMBER 7
#define DFU_RTS_PIN_NUMBER 5

/** @} */

///Bool define if the circuitry is present for the internal DC-DC of nRF52
#define DFU_DC_DC_CIRCUITRY     true

///Bool define if a NFC Antenna circuitry is present
#define DFU_NFC_CIRCUITRY       true

///Bool define if the 32 kHz crystal is present for the LFCLK
#define DFU_LFCLK_XTAL_PRESENT  true

///Bool define if a crystal is present for the HFCLK
#define DFU_HFCLK_XTAL_PRESENT  true

/** Low frequency clock source used when initializing the SoftDevice */
#define DFU_BOARD_LFCLKSRC_STRUCT  {.source        = NRF_CLOCK_LF_SRC_XTAL,            \
                                    .rc_ctiv       = 0,                                \
                                    .rc_temp_ctiv  = 0,                                \
                                    .accuracy = NRF_CLOCK_LF_ACCURACY_20_PPM}

#define DFU_BOARD_LFCLKSRC         NRF_CLOCK_LFCLK_Xtal

#endif /* PLATFORM_PCA10040_H_ */

/** @} */
/** @} */
