/*
 *  info_display.c : <Write brief>
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


#include <string.h>

#include "info_display.h"
#include "EPD_1in54.h"
#include "stdbool.h"
#include "log.h"

#ifdef WSEPD_1IN54_BW_V1
#define X_DIM_PIX   (200)
#define Y_DIM_PIX   (200)
#define X_DIM_BYTE  (200)
#define Y_DIM_BYTE  (25)
#define EP1B_BLACK  (0)
#define EP1B_WHITE  (1)
#endif

UG_GUI g_gui;
static uint8_t g_arr_img [(X_DIM_BYTE * Y_DIM_BYTE)];
const uint8_t g_empty_img [(X_DIM_BYTE * Y_DIM_BYTE)];

UG_COLOR g_backg, g_foreg;

uint32_t g_disp_busy;

#define WT_X_POS    2
#define WT_Y_POS    2

#define UNIT_X_POS  160
#define UNIT_Y_POS  60

#define BATT_X_POS  2
#define BATT_Y_POS  100

#define SUP_X_POS   2
#define SUP_Y_POS   170

#define SEC1_X1_POS 0 
#define SEC1_Y1_POS 90

#define SEC1_X2_POS 199 
#define SEC1_Y2_POS 90

#define SEC2_X1_POS 0 
#define SEC2_Y1_POS 165

#define SEC2_X2_POS 199
#define SEC2_Y2_POS 165


static bool is_wt_enable = false, is_batt_enable = false, is_sup_enable = false;

static uint8_t g_part_refresh_cnt = 0;

#define MAX_PART_BEFORE_FULL 15

/**
 * @brief Function to check if given space should be blank in given string
 * @param ch Current character present in string
 * @param blank_flag Flag to keep track if character printing has started. 
 * @return Character or empty space according to condition check
 */
char chk_blanks (char ch, bool * blank_flag)
{
    if (*blank_flag)
    {
        if (ch  == '0')
        {
            return ' ';
        }
        else
        {
            *blank_flag = false;
            return ch;
        }
    }
    else
    {
        return ch;
    }
}

/**
 * @brief Function to set pixel
 * @param H_cmp X axis position of pixel
 * @param W_cmp Y axis position of pixel
 * @param pixel Pixel value (1 to display, 0 to hide).
 */
void set_pix_array (UG_S16 H_cmp,UG_S16 W_cmp,UG_COLOR pixel)
{
    UDOUBLE Addr = (H_cmp / 8) + (W_cmp * Y_DIM_BYTE);
    UBYTE Rdata = g_arr_img[Addr];
    if(pixel)
    {
        g_arr_img[Addr] = Rdata | (0x80 >> (H_cmp % 8));
    }
    else
    {
        g_arr_img[Addr] = Rdata & (~(0x80 >> (H_cmp % 8)));
    }
}

/**
 * @brief Function to set weight string
 * @param wt_10gm Weight in 10 grams
 */
void set_weight (int32_t wt_10gm)
{
    if ((wt_10gm > 99999) && (wt_10gm < -99990))
    {
        UG_FontSelect (&FONT_32X53);
        UG_PutString (WT_X_POS, WT_Y_POS, "ovr.wt\n");

        return;
    }
    else if (wt_10gm >= 0)
    {
        bool blanks = true;
        char wt_str[] = {'K','K','K','.','g','g', 0x00};
        wt_str[0] = ('0' + (uint8_t)(wt_10gm / 10000));
        wt_str[0] = chk_blanks (wt_str[0], &blanks);
        wt_10gm %= 10000;
        wt_str[1] = ('0' + (uint8_t)(wt_10gm / 1000));
        wt_str[1] = chk_blanks (wt_str[1], &blanks);
        wt_10gm %= 1000;
        wt_str[2] = ('0' + (uint8_t)(wt_10gm / 100));
        wt_str[2] = chk_blanks (wt_str[2], &blanks);
        wt_10gm %= 100;
        wt_str[4] = ('0' + (uint8_t)(wt_10gm / 10));
        wt_10gm %= 10;
        wt_str[5] = ('0' + (uint8_t)(wt_10gm / 1)); 
        
        UG_FontSelect (&FONT_32X53);
        log_printf("Weight : %s\n", wt_str);
        UG_PutString (WT_X_POS, WT_Y_POS, wt_str);
        return;
        
    }
    else 
    {
        bool blanks = true;
        char wt_str[] = {'-','K','K','K','.','g', 0x00};
        wt_str[1] = ('0' + (uint8_t)(wt_10gm / 10000));
        wt_str[1] = chk_blanks (wt_str[0], &blanks);
        wt_10gm %= 10000;
        wt_str[2] = ('0' + (uint8_t)(wt_10gm / 1000));
        wt_str[2] = chk_blanks (wt_str[0], &blanks);
        wt_10gm %= 1000;
        wt_str[3] = ('0' + (uint8_t)(wt_10gm / 100));
        wt_str[3] = chk_blanks (wt_str[0], &blanks);
        wt_10gm %= 100;
        wt_str[5] = ('0' + (uint8_t)(wt_10gm / 10));
        
        UG_FontSelect (&FONT_32X53);
        UG_PutString (WT_X_POS, WT_Y_POS, wt_str);
        return;
    
    }
}

/**
 * @brief Function to set Battery percent string
 * @param per Battery percentage
 */
void set_battery_per (uint8_t per)
{
    char per_str[] = {'c','c','c',0x00};
    if(per > 100)
    {
        UG_FontSelect (&FONT_32X53);
        UG_PutString (BATT_X_POS, BATT_Y_POS, "    ");
        UG_PutString (BATT_X_POS, BATT_Y_POS, "INVLD\n");
        return;
    }
    else
    {
        bool blanks = true;
        UG_PutString (BATT_X_POS, BATT_Y_POS, "   %");
        per_str[0] = ('0' + (uint8_t)(per/100));
        per_str[0] = chk_blanks (per_str[0], &blanks);
        per %= 100;
        per_str[1] = ('0' + (uint8_t)(per/10));
        per_str[1] = chk_blanks (per_str[1], &blanks);
        per %= 10;
        per_str[2] = ('0' + (uint8_t)(per));
        UG_FontSelect (&FONT_32X53);
        UG_PutString (BATT_X_POS, BATT_Y_POS, per_str);
    }
}

/**
 * @brief Function to set charging status string
 * @param status Charging status
 */
void set_supply_status (uint8_t status)
{
    UG_FontSelect (&FONT_12X20);
    if(status)
    {
        UG_PutString (SUP_X_POS, SUP_Y_POS, "              ");
        UG_PutString (SUP_X_POS, SUP_Y_POS, "Charging...");
    }
    else
    {
        UG_PutString (SUP_X_POS, SUP_Y_POS, "              ");
        UG_PutString (SUP_X_POS, SUP_Y_POS, "Discharging...");
    }
}

/**
 * @brief Function to do full refresh of the display
 */
void disp_refresh ()
{
    EPD_1IN54_Init (0);
    EPD_1IN54_Clear ();
    EPD_1IN54_Display ((uint8_t *)g_empty_img);
    EPD_CONF_Delay_ms (1000);
    
    EPD_1IN54_Init (1);
    EPD_1IN54_Clear ();
    
    g_part_refresh_cnt = 0;

}


void info_display_hw_init (info_display_hw_t * hw)
{
    EPD_CONF_Module_Init ((EPD_CONF_Config_t *)hw);
    
    g_backg = EP1B_WHITE;
    g_foreg = EP1B_BLACK;
    g_disp_busy = hw->busy_pin;
    
    disp_refresh ();

    UG_Init (&g_gui, set_pix_array, X_DIM_PIX, Y_DIM_PIX);
    UG_SetForecolor (g_foreg);
    UG_SetBackcolor (g_backg);
    
    UG_FillFrame (0, 0, X_DIM_PIX-1, X_DIM_PIX-1, g_backg);
    EPD_1IN54_Display (g_arr_img);
}


void info_display_enable_fields (uint16_t fields)
{
    if (fields & INFO_DISPLAY_WEIGHT)
    {
        is_wt_enable = true;
        UG_FontSelect (&FONT_12X20);
        UG_PutString (UNIT_X_POS, UNIT_Y_POS, "kgs\n");
        UG_DrawLine (SEC1_X1_POS, SEC1_Y1_POS, SEC1_X2_POS, SEC1_Y2_POS, g_foreg);
    }
    if (fields & INFO_DISPLAY_BATTERY)
    {
        is_batt_enable = true;
        UG_FontSelect (&FONT_32X53);
        UG_PutString (BATT_X_POS, BATT_Y_POS, "   %\n");
        UG_DrawLine (SEC2_X1_POS, SEC2_Y1_POS, SEC2_X2_POS, SEC2_Y2_POS, g_foreg);
    }
    if (fields & INFO_DISPLAY_SUPPLY)
    {
        is_sup_enable = true;
    }

}

void info_display_disable_fields (uint16_t fields)
{
    if (fields & INFO_DISPLAY_WEIGHT)
    {
        is_wt_enable = false;
        UG_FontSelect (&FONT_12X20);
        UG_PutString (UNIT_X_POS, UNIT_Y_POS, "   \n");
        UG_DrawLine (SEC1_X1_POS, SEC1_Y1_POS, SEC1_X2_POS, SEC1_Y2_POS, g_backg);
    }
    if (fields & INFO_DISPLAY_BATTERY)
    {
        is_batt_enable = false;
        UG_FontSelect (&FONT_32X53);
        UG_PutString (BATT_X_POS, BATT_Y_POS, "    \n");
        UG_DrawLine (SEC2_X1_POS, SEC2_Y1_POS, SEC2_X2_POS, SEC2_Y2_POS, g_backg);
    }
    if (fields & INFO_DISPLAY_SUPPLY)
    {
        is_sup_enable = false;
    }    
}


void info_display_update_fields (uint16_t fields, info_display_data_t * p_new_data)
{
    if ((fields & INFO_DISPLAY_WEIGHT) && is_wt_enable)
    {
        set_weight (p_new_data->weight_10gm);
    }
    if ((fields & INFO_DISPLAY_BATTERY) && is_batt_enable)
    {
        set_battery_per (p_new_data->batt_per);
    }
    if ((fields & INFO_DISPLAY_SUPPLY) && is_sup_enable)
    {
        set_supply_status (p_new_data->charging_sts);
    }
}

void info_display_show ()
{
    if (g_part_refresh_cnt < MAX_PART_BEFORE_FULL)
    {
        EPD_1IN54_Display (g_arr_img);
        g_part_refresh_cnt++;
    }
    else
    {
        disp_refresh ();
        EPD_1IN54_Display (g_arr_img);
    }
}
