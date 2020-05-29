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

#define BUTTON_GPIOTE_CH 0

#define BUTTON_USED BUTTON_PIN

w_scale_ble_sw_ver_t sw_vers =
{
    .sw_ver = "0.1.0",
};

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
    weight_mod_set_tare (set_zero);
}

void app_process ()
{
    if(g_is_ble_connected)
    {
        w_scale_ble_update_weight ((w_scale_ble_weight_t) weight_mod_get_wt ());
        w_scale_ble_update_batt_percent ((w_scale_ble_batt_percent_t) weight_mod_get_batt_percent ());
    }
    else
    {
        w_scale_ble_update_adv ((w_scale_ble_weight_t) weight_mod_get_wt ());
    }
}

weight_mod_hw_t dev_config = 
{
    .disp_hw.busy_pin = EINK_BUSY,
    .disp_hw.rst_pin = EINK_RST,
    .disp_hw.dc_pin = EINK_DC,
    .disp_hw.cs_pin = EINK_CS,
    .disp_hw.clk_pin = EINK_SCLK,
    .disp_hw.din_pin = EINK_MOSI,
    .disp_hw.dout_pin = EINK_MISO,
    
    .batt_hw.batt_lv = BT_LV,
    .batt_hw.batt_lv_en = BT_LV_EN,
    .batt_hw.batt_chrg_status = GPIO_0,
    
    .wt_scale_hw.pwr_boost_en = BOOST_EN,
    .wt_scale_hw.pwr_ldo_en = LDO_EN,
    .wt_scale_hw.pwr_vlc = VLC_PIN,

#ifdef HX_ADC
    .wt_scale_hw.hx_clk = HX711_CLK,
    .wt_scale_hw.hx_data = HX711_DATA,
    .wt_scale_hw.hx_rate = HX711_RATE,
#elif defined LTC_ADC
    .wt_scale_hw.common_clk = AMP_ADC_CLK,
    .wt_scale_hw.miso = AMP_ADC_MISO,
    .wt_scale_hw.mosi = AMP_ADC_MOSI,
    .wt_scale_hw.CS_amp = LTC6915_AMP_CS,
    .wt_scale_hw.CS_adc = ADS8320_ADC_CS,
    .wt_scale_hw.SHDN_amp = LTC6915_SHDN,
    .wt_scale_hw.EN_ref = LM4140_REF_EN,
#endif

    .callback = app_process,
};

void leds_init ()
{
    
}

void button_handler ()
{
    log_printf("%s\n", __func__);
    weight_mod_set_tare (true);
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

void boot_pwr_config(void)
{
//Enable the DCDC converter if the board supports it
#if DC_DC_CIRCUITRY == true  //Defined in the board header file
    NRF_POWER->DCDCEN = POWER_DCDCEN_DCDCEN_Enabled << POWER_DCDCEN_DCDCEN_Pos;
#endif
    NRF_POWER->TASKS_LOWPWR = 1;
}

int main(void)
{
    log_printf("Hello World from RTT\n");
    lfclk_init (BOARD_LFCLKSRC);
    boot_pwr_config();
    ms_timer_init (APP_IRQ_PRIORITY_LOWEST);
    
    w_scale_ble_init (OTA_flag_update,tare_set_zero,connection_status);
    w_scale_ble_set_sw_ver(&sw_vers);
    button_init (APP_IRQ_PRIORITY_LOW);
    weight_mod_init (&dev_config);
    leds_init ();
    while (true)
    {
        __WFI();
    }
}

/** @} */
/** @} */
