/**
 *  EPD_CONF_Config.h : SPI Abstraction layer
 *  Copyright (C) 2020  Appiko
 */
#ifndef _EPD_CONF_CONFIG_H_
#define _EPD_CONF_CONFIG_H_

#include "hal_gpio.h"
#include "hal_nop_delay.h"
#include <stdint.h>

/**
 * data
**/
#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t

/**
 * e-Paper GPIO
**/
extern uint32_t EPD_RST_PIN;
extern uint32_t EPD_DC_PIN;
extern uint32_t EPD_CS_PIN;
extern uint32_t EPD_BUSY_PIN;

/**
 * GPIO read and write
**/
#define EPD_CONF_Digital_Write(_pin, _value) hal_gpio_pin_write(_pin, _value)
#define EPD_CONF_Digital_Read(_pin) hal_gpio_pin_read(_pin)

/** Structure to store pins required for E-paper display */
typedef struct 
{
    /** Busy Pin */
    uint32_t busy_pin;
    /** Reset Pin */
    uint32_t rst_pin;
    /** Data/Command Pin */
    uint32_t dc_pin;
    /** Chip Select Pin */
    uint32_t cs_pin;
    /** Clock Pin */
    uint32_t clk_pin;
    /** Data In Pin */
    uint32_t din_pin;
    /** Data Out Pin : Unused. required for SPI communication */
    uint32_t dout_pin; //not used
}EPD_CONF_Config_t;
/**
 * delay x ms
**/
#define EPD_CONF_Delay_ms(__xms) hal_nop_delay_ms(__xms);

/**
 * @brief Fucntion to write byte over SPI
 * @param value Byte value which is to be written
 */
void EPD_CONF_SPI_WriteByte(UBYTE value);

/**
 * @brief Function to initialize the E-Paper display hardware
 * @param init_config Structure pointer to the structure storing pin numbers required.
 */
void EPD_CONF_Module_Init(EPD_CONF_Config_t * init_config);

/**
 * @brief Function to de-initialize E-paper display hardware
 */
void EPD_CONF_Module_Exit(void);

/**
 * @brief Function to enable SPI communication
 */
void EPD_CONF_SPI_Init ();

/**
 * @brief Function to disable SPI communication
 */
void EPD_CONF_SPI_Deinit ();
#endif