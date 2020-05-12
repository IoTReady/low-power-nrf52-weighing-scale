/*
 *  main.c : Application to test display library
 *  Copyright (C) 2020  Appiko
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
 * @addtogroup group_appln
 * @{
 *
 *
 * @{
 *
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "boards.h"
#include "hal_nop_delay.h"
#include "weighing_scale_ble.h"
#include "weight_mod.h"
#include "log.h"
#include "nrf_sdm.h"
#include "nrf_nvic.h"
#include "ms_timer.h"
#include "nrf_util.h"
#include "hal_gpio.h"

#define MS_TIMER_PROCESS 1000

#define BUTTON_GPIOTE_CH 0

#define BUTTON_USED BUTTON_1

bool g_is_ble_connected = false;

void button_handler ();


void GPIOTE_IRQHandler (void)
{
    if (NRF_GPIOTE->EVENTS_IN[BUTTON_GPIOTE_CH])
    {
        NRF_GPIOTE->INTENCLR = (1 << BUTTON_GPIOTE_CH);
        NRF_GPIOTE->EVENTS_IN[BUTTON_GPIOTE_CH] = 0;
        button_handler ();
        NRF_GPIOTE->INTENSET = (1 << BUTTON_GPIOTE_CH);
    }

}




void reset_and_ota ()
{
    NRF_POWER->GPREGRET = 0xB1;
    uint8_t is_sd_enabled;
    sd_softdevice_is_enabled(&is_sd_enabled);
    if(is_sd_enabled == 0)
    {
        sd_nvic_SystemReset();
    }
    else
    {
        NVIC_SystemReset ();
    }
}

void connection_status (bool new_status)
{
    g_is_ble_connected = new_status;
}

void OTA_flag_update (w_scale_ble_force_ota_t ota)
{
    if (ota)
    {
        reset_and_ota ();
    }
}

void tare_set_zero (w_scale_ble_tare_to_zero_t set_zero)
{
    if(set_zero)
    {
        weight_mod_set_tare ();
    }
}

void app_process ()
{
    static uint32_t cnt = 0;
    if ((cnt++ < 25) && (weight_mod_is_busy () == false))
    {
        weight_mod_process ();   
    }
    if(g_is_ble_connected)
    {
        w_scale_ble_update_weight ((w_scale_ble_weight_t) weight_mod_get_wt ());
        w_scale_ble_update_batt_adc ((w_scale_ble_batt_adc_t) weight_mod_get_batt_adc ());
    }
    else
    {
        w_scale_ble_update_adv ((w_scale_ble_weight_t) weight_mod_get_wt ());
    }
}

weight_mod_hw_t dev_config = 
{
    .disp_hw.busy_pin = 19,
    .disp_hw.rst_pin = 20,
    .disp_hw.dc_pin = 22,
    .disp_hw.cs_pin = 23,
    .disp_hw.clk_pin = 24,
    .disp_hw.din_pin = 25,
    .disp_hw.dout_pin = 26,
    
    
    .charging_sts_pin = 27,
    
    .wt_scale_hw.hx_clk = 3,
    .wt_scale_hw.hx_data = 4,
    .wt_scale_hw.hx_rate = 28,
    .wt_scale_hw.pwr_boost_en = 29,
    .wt_scale_hw.pwr_ldo_en = 30,
    .wt_scale_hw.pwr_vlc = 31,
    
        
        
        
};

void leds_init ()
{
    
}

void button_handler ()
{
    log_printf("%s\n", __func__);
    weight_mod_set_tare ();    
}

void button_init (app_irq_priority_t irq_priority)
{
    hal_gpio_cfg_input (BUTTON_USED, HAL_GPIO_PULL_UP);
    NRF_GPIOTE->CONFIG[BUTTON_GPIOTE_CH] = 
        (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos|
        ((BUTTON_USED << GPIOTE_CONFIG_PSEL_Pos) & GPIOTE_CONFIG_PSEL_Msk));
    
    if (BUTTONS_ACTIVE_STATE)
    {
        NRF_GPIOTE->CONFIG[BUTTON_GPIOTE_CH] |= 
            GPIOTE_CONFIG_POLARITY_LoToHi << GPIOTE_CONFIG_POLARITY_Pos; 
    }
    else
    {
        NRF_GPIOTE->CONFIG[BUTTON_GPIOTE_CH] |= 
            GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos; 
    }

    NRF_GPIOTE->INTENSET = (1 << BUTTON_GPIOTE_CH);

    NVIC_ClearPendingIRQ(GPIOTE_IRQn);
    NVIC_SetPriority(GPIOTE_IRQn, irq_priority);
    NVIC_EnableIRQ(GPIOTE_IRQn);
}

int main(void)
{
    lfclk_init (LFCLK_SRC_Xtal);
    ms_timer_init (APP_IRQ_PRIORITY_LOWEST);
    
    log_printf("Hello World from RTT\n");
    weight_mod_init (&dev_config);
    leds_init ();
    button_init (APP_IRQ_PRIORITY_LOW);
    w_scale_ble_init (OTA_flag_update,tare_set_zero,connection_status);
    ms_timer_start (MS_TIMER0, MS_REPEATED_CALL, MS_TIMER_TICKS_MS (MS_TIMER_PROCESS), app_process);
    while (true)
    {
//        ms_timer_handler ();
        __WFI();
    }
}

/** @} */
/** @} */
