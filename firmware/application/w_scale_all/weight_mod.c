/**
 * License
 */
#include <string.h>

#include "weight_mod.h"
#include "log.h"
#include "hal_spim.h"

#ifdef HX_ADC    
#define EXTERNAL_ADC_RES 24
#elif defined INSTR_AMP
#define EXTERNAL_ADC_RES 16
#define INSTR_AMP_GAIN 128
#endif
int32_t g_weight_10g;

int32_t g_weigt_lc;

int32_t g_tare_10g = 0;

uint32_t g_batt_mvolt;

uint32_t g_batt_adc;

static weight_mod_hw_t g_hw;

volatile bool mod_is_busy;

uint8_t battery_li_level_in_percent(uint32_t mvolts)
{
    uint8_t battery_level;

    if (mvolts >= 3000)
    {
        battery_level = 100;
    }
    else if (mvolts > 2900)
    {
        battery_level = 100 - ((3000 - mvolts) * 58) / 100;
    }
    else if (mvolts > 2740)
    {
        battery_level = 42 - ((2900 - mvolts) * 24) / 160;
    }
    else if (mvolts > 2440)
    {
        battery_level = 18 - ((2740 - mvolts) * 12) / 300;
    }
    else if (mvolts > 2100)
    {
        battery_level = 6 - ((2440 - mvolts) * 6) / 340;
    }
    else
    {
        battery_level = 0;
    }

    return battery_level;
}


/**
 * @brief Function to convert milli-volts to percentage of battery remaining
 * @return Battery percentage
 */
uint32_t convert_mv_to_per ()
{
    return battery_li_level_in_percent ((uint16_t)(g_batt_mvolt & 0xFFFF));
}

/**
 * @brief Function to update display data
 */
void update_disp_data ()
{
    
    info_display_data_t l_data = 
    {
        .weight_10gm = g_weight_10g,
        .batt_per = convert_mv_to_per(),
        .charging_sts = hal_gpio_pin_read (g_hw.charging_sts_pin),
        
    };
    info_display_update_fields (INFO_DISPLAY_ALL, &l_data);
    info_display_show ();
}


uint32_t convert_lc_to_10g (void);
void extr_adc_init ();
void extr_adc_deinit ();
void get_load_cell_value ();

#ifdef HX_ADC    

/**
 * @brief Function to convert load-cell to weight in the units of 10grams
 * @return weight in 10grams
 */
uint32_t convert_lc_to_10g (void)
{    
    static int32_t l_wt = 1;
    l_wt++;
    g_weight_10g = l_wt;
    return l_wt;
}


/**
 * @brief Function to initialize external ADC.
 */
void extr_adc_init ()
{
    hal_gpio_pin_write (g_hw.wt_scale_hw.pwr_ldo_en, 1);
    hal_gpio_pin_write (g_hw.wt_scale_hw.pwr_boost_en, 1);
//    while (hal_gpio_pin_read (g_hw.wt_scale_hw.pwr_vlc) == 0);
    
    hal_gpio_pin_write (g_hw.wt_scale_hw.hx_clk, 0);
    hal_gpio_pin_write (g_hw.wt_scale_hw.hx_rate, 1);
}

/**
 * @brief Function to de-initialize external ADC.
 */
void extr_adc_deinit ()
{
    hal_gpio_pin_write (g_hw.wt_scale_hw.hx_rate, 0);
    hal_gpio_pin_write (g_hw.wt_scale_hw.hx_clk, 1);
    hal_gpio_pin_write (g_hw.wt_scale_hw.pwr_boost_en, 0);
    hal_gpio_pin_write (g_hw.wt_scale_hw.pwr_ldo_en, 0);
//    while (hal_gpio_pin_read (g_hw.wt_scale_hw.pwr_vlc));
    
}

/**
 * @brief Function to get Load Cell value
 */
void get_load_cell_value ()
{
    g_weigt_lc = 0;
    hal_gpio_pin_write (g_hw.wt_scale_hw.hx_clk, 0);
    for (uint32_t bit_n = EXTERNAL_ADC_RES; bit_n > 0; bit_n--)
    {
        hal_gpio_pin_write (g_hw.wt_scale_hw.hx_clk, 1);
        hal_nop_delay_us (2);
        g_weigt_lc |= ((hal_gpio_pin_read (g_hw.wt_scale_hw.hx_data) & 0x01)<< 
                           (EXTERNAL_ADC_RES));
        hal_gpio_pin_write (g_hw.wt_scale_hw.hx_clk, 0);
        hal_nop_delay_us (2);
    }
    hal_gpio_pin_write (g_hw.wt_scale_hw.hx_clk, 1);
    
    return;
}

#elif defined INSTR_AMP

/**
 * @brief Function to convert load-cell to weight in the units of 10grams
 * @return weight in 10grams
 */
uint32_t convert_lc_to_10g (void)
{
    static int32_t l_wt = 1;
    l_wt++;
    g_weight_10g = l_wt;
    return l_wt;
}

/**
 * @brief Function to initialize external ADC.
 */
void extr_adc_init ()
{
    uint8_t gain = INSTR_AMP_GAIN;
    static hal_spim_init_t l_spi_init; 
    {
        l_spi_init.csBar_pin = g_hw.wt_scale_hw.CS_amp;
        l_spi_init.sck_pin = g_hw.wt_scale_hw.common_clk;
        l_spi_init.miso_pin = g_hw.wt_scale_hw.D_adc;
        l_spi_init.mosi_pin = g_hw.wt_scale_hw.D_amp;
        l_spi_init.byte_order = HAL_SPIM_MSB_FIRST;
        l_spi_init.freq = HAL_SPIM_FREQ_2M;
        l_spi_init.spi_mode = HAL_SPIM_SPI_MODE0;
        l_spi_init.irq_priority = APP_IRQ_PRIORITY_HIGHEST;
    }
    hal_spim_deinit ();
    hal_spim_init (&l_spi_init);
    hal_spim_tx_rx (&gain, 1, NULL, 0);
    hal_spim_is_busy ();
    hal_spim_deinit ();

}

/**
 * @brief Function to de-initialize external ADC.
 */
void extr_adc_deinit ()
{
    hal_spim_deinit ();
}

/**
 * @brief Function to get Load Cell value
 */
void get_load_cell_value ()
{
    uint16_t adc_val = 0;

    static hal_spim_init_t l_spi_init; 
    {
        l_spi_init.csBar_pin = g_hw.wt_scale_hw.CS_adc;
        l_spi_init.sck_pin = g_hw.wt_scale_hw.common_clk;
        l_spi_init.miso_pin = g_hw.wt_scale_hw.D_adc;
        l_spi_init.mosi_pin = g_hw.wt_scale_hw.D_amp;
        l_spi_init.byte_order = HAL_SPIM_MSB_FIRST;
        l_spi_init.freq = HAL_SPIM_FREQ_2M;
        l_spi_init.spi_mode = HAL_SPIM_SPI_MODE0;
        l_spi_init.irq_priority = APP_IRQ_PRIORITY_HIGHEST;
    }

    hal_spim_deinit ();
    hal_spim_init (&l_spi_init);
    hal_spim_tx_rx (NULL, 0, &adc_val, 2);
    hal_spim_is_busy ();
    hal_spim_deinit ();
    
    g_weigt_lc = adc_val;
}

#endif


/**
 * @brief Function to calculate weight from load-cell value.
 */
void measure_weight ()
{
    extr_adc_init ();
    get_load_cell_value ();
    extr_adc_deinit ();
    convert_lc_to_10g ();
    g_weight_10g -= g_tare_10g;
    return;
}

/**
 * @brief Function to measure battery voltage
 */
void measure_batt_per ()
{
    static uint32_t batt_per = 3000;
    g_batt_mvolt = batt_per;
    batt_per -= 5;
    if (batt_per)
    {
        return;
    }
    else
    {
        batt_per = 3000;
        return;
    }
}

void weight_mod_init (weight_mod_hw_t * init_hw)
{
    g_tare_10g = 0;
    memcpy (&g_hw, init_hw, sizeof(weight_mod_hw_t)); 
    info_display_hw_init ((info_display_hw_t * )&init_hw->disp_hw);
    info_display_enable_fields (INFO_DISPLAY_ALL);
    

#ifdef HX_ADC    
    log_printf ("Here\n");
    hal_gpio_cfg_output (g_hw.wt_scale_hw.hx_rate, 0);
    hal_gpio_cfg_output (g_hw.wt_scale_hw.hx_clk, 1);
    hal_gpio_cfg_output (g_hw.wt_scale_hw.pwr_boost_en, 0);
    hal_gpio_cfg_output (g_hw.wt_scale_hw.pwr_ldo_en, 0);
    
    hal_gpio_cfg_input (g_hw.wt_scale_hw.hx_data, HAL_GPIO_PULL_DISABLED);
#elif defined INSTR_AMP
    hal_gpio_cfg_output (g_hw.wt_scale_hw.common_clk, 0);
    hal_gpio_cfg_output (g_hw.wt_scale_hw.CS_amp, 1);
    hal_gpio_cfg_output (g_hw.wt_scale_hw.CS_adc, 1);
    hal_gpio_cfg_output (g_hw.wt_scale_hw.D_amp, 0);
    
    hal_gpio_cfg_input (g_hw.wt_scale_hw.D_adc, HAL_GPIO_PULL_DISABLED);
    
#endif
}

void weight_mod_disp_wt (weight_mod_disp_state_t state)
{
    if(state)
    {
        info_display_enable_fields (INFO_DISPLAY_WEIGHT);
    }
    else
    {
        info_display_disable_fields (INFO_DISPLAY_WEIGHT);
    }
}

void weight_mod_disp_batt (weight_mod_disp_state_t state)
{
    if(state)
    {
        info_display_enable_fields (INFO_DISPLAY_BATTERY);
    }
    else
    {
        info_display_disable_fields (INFO_DISPLAY_BATTERY);
    }
}


void weight_mod_disp_supply (weight_mod_disp_state_t state)
{
    if(state)
    {
        info_display_enable_fields (INFO_DISPLAY_SUPPLY);
    }
    else
    {
        info_display_disable_fields (INFO_DISPLAY_SUPPLY);
    }
}


void weight_mod_set_tare ()
{
    measure_weight ();
    g_tare_10g = g_weight_10g + g_tare_10g;
    g_weight_10g = 0;
    update_disp_data ();
}

uint32_t weight_mod_get_wt ()
{
    return g_weight_10g*10;
}

uint32_t weight_mod_get_wt_adc ()
{
    return g_weigt_lc;
}

uint32_t weight_mod_get_batt ()
{
    return g_batt_mvolt;
}

uint32_t weight_mod_get_batt_adc ()
{
    return g_batt_adc;
}

void weight_mod_process ()
{
    mod_is_busy = true;
    measure_weight ();
    measure_batt_per ();
    update_disp_data ();
    mod_is_busy = false;
}

bool weight_mod_is_busy ()
{
    return mod_is_busy;
}
