/*
 *  info_display.h : <Write brief>
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

#ifndef INFO_DISPLAY_H
#define INFO_DISPLAY_H

#include "stdbool.h"
#include "stdint.h"
#include "ugui.h"
#include "EPD_CONF_Config.h"

typedef enum 
{
    INFO_DISPLAY_WEIGHT = 0x01,
    INFO_DISPLAY_BATTERY = 0x02,
    INFO_DISPLAY_SUPPLY = 0x04,
    INFO_DISPLAY_ALL = (INFO_DISPLAY_WEIGHT | INFO_DISPLAY_BATTERY | INFO_DISPLAY_SUPPLY),
}info_display_fields_t;

typedef struct 
{
    int32_t weight_1g;
    uint8_t batt_per;
    uint8_t charging_sts;
}info_display_data_t;

typedef EPD_CONF_Config_t info_display_hw_t;


/**
 * @brief Function to Initialize
 * @param hw
 */
void info_display_hw_init (info_display_hw_t * hw);

/**
 * @brief Function to enable one or more display fields
 * @param fields
 */
void info_display_enable_fields (uint16_t fields);

/**
 * @brief Function to disable one or more display fields
 * @param fields
 */
void info_display_disable_fields (uint16_t fields);

/**
 * @brief Function to update one or more display fields
 * @param fields
 * @param p_new_data
 */
void info_display_update_fields (uint16_t fields, info_display_data_t * p_new_data);

/**
 * @brief Function to show current buffer values on screen
 */
void info_display_show ();

/**
 * @brief Function to refresh the screen. 
 */
void info_display_refresh ();

#endif /* INFO_DISPLAY_H */
