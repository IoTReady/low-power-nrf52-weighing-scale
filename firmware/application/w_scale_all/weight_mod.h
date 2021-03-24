/**
 * License
 */

#ifndef WEIGHT_MOD_H
#define WEIGHT_MOD_H


#include "info_display.h"

typedef info_display_hw_t weight_mod_disp_hw_t;

typedef struct
{
#ifdef HX_ADC
    uint32_t hx_clk;
    uint32_t hx_rate;
    uint32_t hx_data;
#elif defined LTC_ADC
    uint32_t common_clk;
    uint32_t miso;
    uint32_t mosi;
    uint32_t CS_amp;
    uint32_t CS_adc;
    uint32_t SHDN_amp;
    uint32_t EN_ref;
#endif
    uint32_t pwr_ldo_en;
    uint32_t pwr_boost_en;
    uint32_t pwr_vlc;
}weight_mod_scale_t;

typedef struct
{
    uint32_t batt_lv;
    uint32_t batt_lv_en;
    uint32_t batt_chrg_status;
}weight_mod_batt_t;

typedef struct 
{
    weight_mod_disp_hw_t disp_hw;
    weight_mod_scale_t wt_scale_hw;
    weight_mod_batt_t batt_hw;
    void (*callback)(void);
}weight_mod_hw_t;

typedef enum
{
    FEILD_DISABLE,
    FEILD_ENABLE,
}weight_mod_disp_state_t;

/**
 * @brief Function to Initialize weight module. This module will also handle battery levels.
 * @param init_hw Structure pointer to the structure containing hardware information.
 */
void weight_mod_init (weight_mod_hw_t * init_hw);

/**
 * @brief Function to set the state of Weight field in display.
 * @param state State of weight field in state (Default : Enable)
 */
void weight_mod_disp_wt (weight_mod_disp_state_t state);

/**
 * @brief Function to set the state of Battery field in display
 * @param state State of Battery field (Default : Enable)
 */
void weight_mod_disp_batt (weight_mod_disp_state_t state);

/**
 * @brief FUnction to set the state of the charging status field on display
 * @param state State of the Charging status field.
 */
void weight_mod_disp_supply (weight_mod_disp_state_t state);

/**
 * @brief Function to set current weight as tare weight
 * @param set_tare Flag to set current weight as tare weight
 */
void weight_mod_set_tare (bool set_tare);

/**
 * @brief Function to get current weight
 * @return weight in grams (resolution 1g)
 */
int32_t weight_mod_get_wt ();

/**
 * @brief Function to get current weight value in raw ADC output
 * @return ADC value for weight
 */
uint32_t weight_mod_get_wt_adc ();

/**
 * @brief Function to get current battery voltage
 * @return Battery voltage in mv
 */
uint32_t weight_mod_get_batt_mv ();

/**
 * @brief Function to get current battery_voltage in raw ADC output
 * @return ADC value for battery voltage
 */
uint32_t weight_mod_get_batt_adc ();

/**
 * @brief Function to get current battery_voltage in percentage
 * @return Percentage of battery left
 */
uint8_t weight_mod_get_batt_percent ();

/**
 * @brief Function to send busy status to application.
 * @return Status of Weight module.
 * @retval 1 Module is busy
 * @retval 2 Module is free
 */
bool weight_mod_is_busy ();

#endif /* WEIGHT_MOD_H */
