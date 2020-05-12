/**
 *  EPD_CONF_Config.c : SPI Abstraction layer
 *  Copyright (C) 2020  Appiko
 */
#include "EPD_CONF_Config.h"
#include "hal_spim.h"
#include "nrf_util.h"
#include "stddef.h"
#include "log.h"

uint32_t EPD_RST_PIN;
uint32_t EPD_DC_PIN;
uint32_t EPD_CS_PIN;
uint32_t EPD_BUSY_PIN;

void EPD_CONF_SPI_WriteByte(UBYTE value)
{
    hal_spim_tx_rx (&value, 1, NULL, 0);
    while (hal_spim_is_busy ());
}

static hal_spim_init_t spim_init; 

void EPD_CONF_Module_Init(EPD_CONF_Config_t * init_config)
{
    hal_gpio_cfg_output (init_config->cs_pin, 1);
    hal_gpio_cfg_output (init_config->clk_pin, 0);
    hal_gpio_cfg_output (init_config->din_pin, 0);
    hal_gpio_cfg_input (init_config->dout_pin, HAL_GPIO_PULL_DISABLED);
    
    {
        spim_init.sck_pin = init_config->clk_pin;
        spim_init.csBar_pin = init_config->cs_pin;
        spim_init.miso_pin = init_config->dout_pin;
        spim_init.mosi_pin = init_config->din_pin;
        spim_init.byte_order = HAL_SPIM_MSB_FIRST;
        spim_init.freq = HAL_SPIM_FREQ_2M;
        spim_init.spi_mode = HAL_SPIM_SPI_MODE0;
        spim_init.irq_priority = APP_IRQ_PRIORITY_HIGHEST;
    }
    
    hal_gpio_cfg_output (init_config->dc_pin, 1);
    hal_gpio_cfg_output (init_config->rst_pin, 1);
    hal_gpio_cfg_input (init_config->busy_pin, HAL_GPIO_PULL_DISABLED);
    
    EPD_BUSY_PIN = init_config->busy_pin;
    EPD_CS_PIN = init_config->cs_pin;
    EPD_DC_PIN = init_config->dc_pin;
    EPD_RST_PIN = init_config->rst_pin;
    
    return;
}

void EPD_CONF_Module_Exit(void)
{
    EPD_CONF_Digital_Write(EPD_DC_PIN, 0);
    EPD_CONF_Digital_Write(EPD_CS_PIN, 0);

    //close 5V
    EPD_CONF_Digital_Write(EPD_RST_PIN, 0);
}

void EPD_CONF_SPI_Init ()
{
    hal_spim_deinit ();
    hal_spim_init (&spim_init);
}

void EPD_CONF_SPI_Deinit ()
{
    hal_spim_deinit ();
}
