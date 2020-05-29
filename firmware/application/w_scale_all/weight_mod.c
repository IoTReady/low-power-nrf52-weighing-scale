/**
 * License
 */
#include <string.h>

#include "weight_mod.h"
#include "log.h"
#include "hal_spim.h"
#include "simple_adc.h"
#include "ms_timer.h"

#define WT_SAMPLING_INTERVAL_MS 1000

#ifndef HX_ADC
#define HX_ADC
#endif

#ifdef HX_ADC    
#define EXTERNAL_ADC_RES 24
#define ZERO_READING (140000-2000)
#define KG_READING (35600)
#define SAMPLER_WAKE_TIME_MS  (60)
#elif defined LTC_ADC
#define EXTERNAL_ADC_RES 16
#define INSTR_AMP_GAIN (0xFF)
#define ZERO_READING (140000-2000)
#define KG_READING (35600)
#define SAMPLER_WAKE_TIME_MS  (10)
#endif
int32_t g_weight_1g;

int32_t g_weigt_lc;

int32_t g_tare_1g = 0;

uint32_t g_batt_mvolt;

uint32_t g_batt_adc;

static weight_mod_hw_t g_hw;

volatile bool mod_is_busy;

uint8_t battery_li_level_in_percent(uint32_t mvolts)
{
    uint8_t battery_level;

    if (mvolts >= 4200)
    {
        battery_level = 100;
    }
    else if (mvolts > 4100)
    {
        battery_level = 90;
    }
    else if (mvolts > 4000)
    {
        battery_level = 80;
    }
    else if (mvolts > 3900)
    {
        battery_level = 60;
    }
    else if (mvolts > 3800)
    {
        battery_level = 40;
    }
    else if (mvolts > 3700)
    {
        battery_level = 20;
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
uint8_t convert_mv_to_per ()
{
    return battery_li_level_in_percent ((uint16_t)(g_batt_mvolt & 0xFFFF));
}

int32_t get_tare_wt(void)
{
    return (g_weight_1g - g_tare_1g);
}

/**
 * @brief Function to update display data
 */
void update_disp_data ()
{
    
    info_display_data_t l_data = 
    {
        .weight_1g = get_tare_wt(),
        .batt_per = convert_mv_to_per(),
        .charging_sts = hal_gpio_pin_read (g_hw.batt_hw.batt_chrg_status),
    };
    info_display_update_fields (INFO_DISPLAY_ALL, &l_data);
    info_display_show ();

    log_printf("weight %x, batt %d \n", get_tare_wt(), convert_mv_to_per());
}


void convert_lc_to_1g (void);
void sampler_init ();
void sampler_deinit ();
void get_load_cell_value ();
void weight_mod_process ();
void weight_mod_wakeup ();

#ifdef HX_ADC    

/**
 * @brief Function to convert load-cell to weight in the units of 10grams
 * @return weight in 10grams
 */
void convert_lc_to_1g (void)
{    
    g_weight_1g = ((g_weigt_lc-ZERO_READING)*1000)/KG_READING;
}


/**
 * @brief Function to initialize external ADC.
 */
void sampler_init ()
{
    hal_gpio_pin_write (g_hw.wt_scale_hw.hx_clk, 0);
    hal_gpio_pin_write (g_hw.wt_scale_hw.pwr_ldo_en, 1);
    hal_gpio_pin_write (g_hw.wt_scale_hw.pwr_boost_en, 1);
}

/**
 * @brief Function to de-initialize external ADC.
 */
void sampler_deinit ()
{
    hal_gpio_pin_write (g_hw.wt_scale_hw.hx_clk, 1);
    hal_gpio_pin_write (g_hw.wt_scale_hw.pwr_boost_en, 0);
    hal_gpio_pin_write (g_hw.wt_scale_hw.pwr_ldo_en, 0);
}

/**
 * @brief Function to get Load Cell value
 */
void get_load_cell_value ()
{
    g_weigt_lc = 0;
    hal_gpio_pin_write (g_hw.wt_scale_hw.hx_clk, 0);
    for (uint32_t bit_n = EXTERNAL_ADC_RES; bit_n ; bit_n--)
    {
        hal_gpio_pin_write (g_hw.wt_scale_hw.hx_clk, 1);
        hal_nop_delay_us (1);
        g_weigt_lc |= ((hal_gpio_pin_read (g_hw.wt_scale_hw.hx_data) & 0x01)<< 
                           (bit_n-1));
        hal_gpio_pin_write (g_hw.wt_scale_hw.hx_clk, 0);
        hal_nop_delay_us (1);
    }
}

#elif defined LTC_ADC

/**
 * @brief Function to convert load-cell to weight in the units of 10grams
 * @return weight in 10grams
 */
void convert_lc_to_1g (void)
{
    static int32_t l_wt = 1;
    l_wt++;
    g_weight_1g = l_wt;
}

/**
 * @brief Function to initialize external ADC.
 */
void sampler_init ()
{
    uint8_t gain = INSTR_AMP_GAIN;
    static hal_spim_init_t l_spi_init; 
    {
        l_spi_init.csBar_pin = g_hw.wt_scale_hw.CS_amp;
        l_spi_init.sck_pin = g_hw.wt_scale_hw.common_clk;
        l_spi_init.miso_pin = g_hw.wt_scale_hw.miso;
        l_spi_init.mosi_pin = g_hw.wt_scale_hw.mosi;
        l_spi_init.byte_order = HAL_SPIM_MSB_FIRST;
        l_spi_init.freq = HAL_SPIM_FREQ_1M;
        l_spi_init.spi_mode = HAL_SPIM_SPI_MODE0;
        l_spi_init.irq_priority = APP_IRQ_PRIORITY_LOW;
    }
    hal_spim_deinit ();
    hal_spim_init (&l_spi_init);
    hal_spim_tx_rx (&gain, 1, NULL, 0);
    while (hal_spim_is_busy ()) {}
    hal_spim_deinit ();

}

/**
 * @brief Function to de-initialize external ADC.
 */
void sampler_deinit ()
{
    hal_spim_deinit ();
}

/**
 * @brief Function to get Load Cell value
 */
void get_load_cell_value ()
{
    uint32_t adc_val = 0;

    static hal_spim_init_t l_spi_init; 
    {
        l_spi_init.csBar_pin = g_hw.wt_scale_hw.CS_adc;
        l_spi_init.sck_pin = g_hw.wt_scale_hw.common_clk;
        l_spi_init.miso_pin = g_hw.wt_scale_hw.miso;
        l_spi_init.mosi_pin = g_hw.wt_scale_hw.mosi;
        l_spi_init.byte_order = HAL_SPIM_MSB_FIRST;
        l_spi_init.freq = HAL_SPIM_FREQ_1M;
        l_spi_init.spi_mode = HAL_SPIM_SPI_MODE0;
        l_spi_init.irq_priority = APP_IRQ_PRIORITY_LOW;
    }

    hal_spim_deinit ();
    hal_spim_init (&l_spi_init);
    hal_spim_tx_rx (NULL, 0, &adc_val, 3);
    while (hal_spim_is_busy ()) {}
    hal_spim_deinit ();
    
    //TODO to get the ADC value from 3 bytes
    g_weigt_lc = adc_val;
}

#endif


/**
 * @brief Function to calculate weight from load-cell value.
 */
void measure_weight ()
{
    get_load_cell_value ();
    sampler_deinit ();
    convert_lc_to_1g ();
}

/**
 * @brief Function to measure battery voltage
 */
void measure_batt_per ()
{
    hal_gpio_pin_set(g_hw.batt_hw.batt_lv_en);
    g_batt_adc = simple_adc_get_value(SIMPLE_ADC_GAIN1_4, ANALOG_PIN_5);
    //4096 as its a 12 bit ADC, Vref is 600 mV and the voltage scaling is 1/8
    //with 1/4 from ADC and 1/2 from voltage divider
    g_batt_mvolt = (g_batt_adc*8*600)/4096;
    hal_gpio_pin_clear(g_hw.batt_hw.batt_lv_en);
}

void weight_mod_init (weight_mod_hw_t * init_hw)
{
    g_tare_1g = 0;
    memcpy (&g_hw, init_hw, sizeof(weight_mod_hw_t)); 
    info_display_hw_init ((info_display_hw_t * )&init_hw->disp_hw);
    info_display_enable_fields (INFO_DISPLAY_ALL);

#ifdef HX_ADC    
    hal_gpio_cfg_output (g_hw.wt_scale_hw.hx_rate, 1);  //80 samples per second
    hal_gpio_cfg_output (g_hw.wt_scale_hw.hx_clk, 1);
    hal_gpio_cfg_output (g_hw.wt_scale_hw.pwr_boost_en, 0);
    hal_gpio_cfg_output (g_hw.wt_scale_hw.pwr_ldo_en, 0);
    
    hal_gpio_cfg_input (g_hw.wt_scale_hw.hx_data, HAL_GPIO_PULL_DISABLED);

    hal_gpio_cfg_output(g_hw.batt_hw.batt_lv_en, 0);
#elif defined LTC_ADC
    hal_gpio_cfg_output (g_hw.wt_scale_hw.EN_ref, 1);
    hal_gpio_cfg_output (g_hw.wt_scale_hw.SHDN_amp, 0);
    hal_gpio_cfg_output (g_hw.wt_scale_hw.pwr_ldo_en, 1);
    hal_gpio_cfg_output (g_hw.wt_scale_hw.pwr_boost_en, 1);

    hal_gpio_cfg_input (g_hw.wt_scale_hw.miso, HAL_GPIO_PULL_UP);
#endif

    ms_timer_start (MS_TIMER0, MS_REPEATED_CALL,
            MS_TIMER_TICKS_MS (WT_SAMPLING_INTERVAL_MS), weight_mod_wakeup);
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


void weight_mod_set_tare (bool set_tare)
{
    if(set_tare)
    {
        g_tare_1g = g_weight_1g;
    }
    else
    {
        g_tare_1g = 0;
    }
}

int32_t weight_mod_get_wt ()
{
    return get_tare_wt();
}

uint32_t weight_mod_get_wt_adc ()
{
    return g_weigt_lc;
}

uint32_t weight_mod_get_batt_mv ()
{
    return g_batt_mvolt;
}

uint32_t weight_mod_get_batt_adc ()
{
    return g_batt_adc;
}

uint8_t weight_mod_get_batt_percent ()
{
    return (convert_mv_to_per());
}

void weight_mod_wakeup(void)
{
    sampler_init ();
    ms_timer_start (MS_TIMER1, MS_SINGLE_CALL,
            MS_TIMER_TICKS_MS (SAMPLER_WAKE_TIME_MS), weight_mod_process);
}

void weight_mod_process (void)
{
    mod_is_busy = true;
    measure_weight ();
    measure_batt_per ();
    update_disp_data ();
    g_hw.callback();
    mod_is_busy = false;
}

bool weight_mod_is_busy ()
{
    return mod_is_busy;
}
