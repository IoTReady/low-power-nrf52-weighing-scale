/*
 *  weighing_scale_ble.c : <Write brief>
 */

#ifndef WEIGHING_SCALE_BLE_H
#define WEIGHING_SCALE_BLE_H

#include "stdint.h"
#include "stdbool.h"

/** Weight in grams */
typedef uint32_t w_scale_ble_weight_t;

/** Flag : force OTA */
typedef bool w_scale_ble_force_ota_t;

/** Software version */
typedef uint32_t w_scale_ble_sw_ver_t;

/** Flag : set Tare to zero */
typedef bool w_scale_ble_tare_to_zero_t;

/** Battery voltage */
typedef uint16_t w_scale_ble_batt_adc_t;

/** List of Controllers */
typedef enum
{
    /** ESP32 */
    W_SCALE_BLE_CONTROL_ESP32 = 0,
    /** NRF52 */
    W_SCALE_BLE_CONTROL_NRF52 = 1
}w_scale_ble_controllers_t;

/**
 * @brief Function to initialize BLE module and start advertisement.
 * @param p_ota_flag_update Function pointer to the function which is to be called when OTA update flag is received.
 * @param p_set_tare_to_zero Function pointer to the function which is to be called when tare is to be set to zero.
 * @param p_conn_status_update Function pointer to the function which keeps track of connection status at application level.
 */
void w_scale_ble_init (void (*p_ota_flag_update)(w_scale_ble_force_ota_t force_ota),
        void (*p_set_tare_to_zero)(w_scale_ble_tare_to_zero_t set_tare), 
        void (*p_conn_status_update)(bool conn_status));

/**
 * @brief Function to update the advertisement data (weight).
 * @param adv_weight Weight which is to be displayed in advertisement.
 */
void w_scale_ble_update_adv (w_scale_ble_weight_t adv_weight);


/**
 * @brief Function to set controller. (Called once).
 * @param controller Controller that's being used. @ref w_scale_ble_controllers_t.
 */
void w_scale_ble_set_controller (w_scale_ble_controllers_t controller);

/**
 * @brief Function to set software version (Called once).
 * @param sw_ver Current software version.
 */
void w_scale_ble_set_sw_ver (w_scale_ble_sw_ver_t sw_ver);

/**
 * @brief Function to update the battery voltage (ADC value).
 * @param batt_update ADC reading of battery voltage.
 */
void w_scale_ble_update_batt_adc (w_scale_ble_batt_adc_t batt_update);

/**
 * @brief Function to update weight.
 * @param weight_update Weight in grams
 */
void w_scale_ble_update_weight (w_scale_ble_weight_t weight_update);

#endif /* WEIGHING_SCALE_BLE_H */
