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

#ifndef DFU_PLATFORM_PCA10040_H_
#define DFU_PLATFORM_PCA10040_H_

/** @anchor board-leds
 * @name LED definitions for Weighing Scale hx711
 * @{*/
#define DFU_GPIO_0          (17)
#define DFU_GPIO_1          (16)
#define DFU_GPIO_2          (15)
#define DFU_GPIO_3          (14)

#define DFU_LED_1           (GPIO_1)
#define DFU_LED_2           (GPIO_2)

#define DFU_LED_RED         (LED_1)
#define DFU_LED_GREEN       (LED_2)
/** The output level at which the LEDs shine */
#define DFU_LEDS_ACTIVE_STATE (0)
/** @} */

/** @anchor weighing_scale_button
 * @name Button Pins
 * @{*/
#define DFU_BUTTON_PIN      (7)
/** The input level when the button is pressed */
#define DFU_BUTTONS_ACTIVE_STATE (0)
/** The kind of internal resistors required for the button */
#define DFU_BUTTON_PULL     (HAL_GPIO_PULLUP)
/** @} */

/** @anchor weighing_scale_serial_interface
 * @name Serial port definitions for Weighing Scale HX711 board
 * @{*/
#define DFU_RX_PIN_NUMBER   (21)
#define DFU_TX_PIN_NUMBER   (20)
#define DFU_CTS_PIN_NUMBER  (7)
#define DFU_RTS_PIN_NUMBER  (5)
/** Bool to say if the hardware flow control is required */
#define DFU_HWFC            (false)
/** @} */

/** @anchor e_ink_paper_interface 
 * @name E-Ink Paper display Interface
 * @{*/
#define DFU_EINK_BUSY       (25)
#define DFU_EINK_RST        (26)
#define DFU_EINK_DC         (27)
#define DFU_EINK_CS         (28)
#define DFU_EINK_SCLK       (29)
#define DFU_EINK_MOSI       (30)
#define DFU_EINK_MISO       (31) //Unused
/** @} */

/** @anchor hx711_interface 
 * @name HX711 Interface
 * @{*/
#define DFU_HX711_CLK       (13)
#define DFU_HX711_DATA      (12)
#define DFU_HX711_RATE      (11)
#define DFU_VLC_PIN         (4)
#define DFU_LDO_EN          (8)
#define DFU_BOOST_EN        (9)
/** @} */

/** @anchor batt_level 
 * @name Battery Level
 * @{*/
#define DFU_BT_LV           (5)
#define DFU_BT_LV_EN        (10)
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
