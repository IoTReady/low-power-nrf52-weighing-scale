/*
 *  weighing_scale_ble.c : <Write brief>
 */


#include "weighing_scale_ble.h"
#include "weighing_scale_uuids.h"

#include "nrf_nvic.h"
#include "ble.h"
#include "nrf_sdm.h"
#include "app_error.h"
#include "boards.h"
#include "stddef.h"
#include "common_util.h"
#include "nrf_util.h"
#include "log.h"
#include "evt_sd_handler.h"
#include "string.h"




/**< Interval between advertisement packets (0.5 seconds). */
#define ADVERTISING_INTERVAL       MSEC_TO_UNITS(500, UNIT_0_625_MS)
/**< Minimum acceptable connection interval (0.2 seconds). */
#define MIN_CONN_INTERVAL          MSEC_TO_UNITS(200, UNIT_1_25_MS)
/**< Maximum acceptable connection interval (0.5 second). */
#define MAX_CONN_INTERVAL          MSEC_TO_UNITS(500, UNIT_1_25_MS)
/**< Slave latency (number of skippable conn events by the slave). */
#define SLAVE_LATENCY              0
/**< Connection supervisory timeout (4 seconds). */
#define CONN_SUP_TIMEOUT           MSEC_TO_UNITS(4000, UNIT_10_MS)


#define APP_BLE_CONN_CFG_TAG            1 /**< A tag identifying the SoftDevice BLE configuration. */

/** TODO Update device name */
#define DEVICE_NAME 'D','e','v',' ','N','a','m','e',' ','H','e','r','e','.','.','.'
//Dev Name Here..\0


#define LOW_8(x) (x & 0xFF)
#define HIGH_8(x) ((x & 0xFF00) >> 8)

const uint8_t app_device_name[] = {DEVICE_NAME};

/** Standard Advertisement data */
#define APP_ADV_DATA   {                                        \
                       0x02, BLE_GAP_AD_TYPE_FLAGS, BLE_GAP_ADV_FLAGS_LE_ONLY_LIMITED_DISC_MODE,    \
                       sizeof(app_device_name) + 1, BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME, DEVICE_NAME,   \
                       0x02, BLE_GAP_AD_TYPE_TX_POWER_LEVEL, /** TODO Update this */0x03, \
                       0x05, BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_COMPLETE, \
                       LOW_8(WEIGHT_READ_SERVICE), HIGH_8(WEIGHT_READ_SERVICE),\
                       LOW_8(SYSTEM_SETTING_SERVICE), HIGH_8(SYSTEM_SETTING_SERVICE),\
                       }
/** Standard Scan response data */
#define SCAN_RSP_DATA {                                         \
                       0x07,BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, \
                       0x05, BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_COMPLETE, \
                       LOW_8(WEIGHT_READ_SERVICE), HIGH_8(WEIGHT_READ_SERVICE),\
                       LOW_8(SYSTEM_SETTING_SERVICE), HIGH_8(SYSTEM_SETTING_SERVICE),\
                       0x05, BLE_GAP_AD_TYPE_SLAVE_CONNECTION_INTERVAL_RANGE, 0x10, 0x00, 0x10, 0x00\
                      }

#define WEIGHT_KG_INDEX 4
#define WEIGHT__G_INDEX 6

uint8_t g_adv_data1[] = APP_ADV_DATA;
uint8_t g_adv_data2[] = APP_ADV_DATA;

uint8_t g_scan_rsp_data1[] = SCAN_RSP_DATA;
uint8_t g_scan_rsp_data2[] = SCAN_RSP_DATA;

uint8_t h_adv;
uint16_t h_conn;

uint16_t h_weight_service;
ble_gatts_char_handles_t h_weight_data;

uint16_t h_setting_service;
ble_gatts_char_handles_t h_force_ota;
ble_gatts_char_handles_t h_set_tare;
ble_gatts_char_handles_t h_read_batt;
ble_gatts_char_handles_t h_sw_ver;
ble_gatts_char_handles_t h_controller;

w_scale_ble_controllers_t g_default_controller;
w_scale_ble_sw_ver_t g_sw_ver;

ble_gap_adv_params_t adv_params;

/**
 * @brief Function to handle BLE events
 * @param evt BLE event
 */
void ble_evt_handler(ble_evt_t * evt);
/**
 * @brief Function to handle SOC events
 * @param evt_id SOC events
 */
void soc_evt_handler(uint32_t evt_id);

/**
 * @brief Function to initialize BLE stack
 */
void stack_init ();

/**
 * @brief Function to initialize Services
 */
void service_init ();

/**
 * @brief Function to start advertisement
 */
void start_advertisement ();

/**
 * @brief Function to load default values.
 */
void load_default_val ();


void (*p_update_ota_flag) (w_scale_ble_force_ota_t force_ota);
void (*p_tare_to_zero_set) (w_scale_ble_tare_to_zero_t set_tare);
void (*p_update_conn_status) (bool conn_status);

/**
 * @brief Handler which will address the BLE related events
 *  generated by the SoftDevice for BLE related activities.
 *  The event is passed on to the application for its activities.
 * @param evt The pointer to the buffer containing all the data
 *  related to the event
 */
void ble_evt_handler(ble_evt_t * evt)
{
    uint32_t err_code;
    switch(evt->header.evt_id)
    {
        case BLE_GAP_EVT_ADV_SET_TERMINATED:
        {
            start_advertisement ();
            break;
        }
        case BLE_GAP_EVT_CONNECTED:
        {
            h_conn = evt->evt.gap_evt.conn_handle;
            log_printf ("Connected\n");
            p_update_conn_status (true);
            load_default_val ();
            break;
        }
        case BLE_GAP_EVT_DISCONNECTED:
        {
            h_conn = BLE_CONN_HANDLE_INVALID;
            log_printf ("Disconnected\n");
            start_advertisement ();
            break;
        }
        case BLE_GAP_EVT_TIMEOUT:
        {
            start_advertisement ();
            break;
        }
        case BLE_GATTS_EVT_WRITE:
        {
            log_printf ("Write event\n");
            if (evt->evt.gatts_evt.params.write.handle == h_force_ota.value_handle)
            {
                p_update_ota_flag
                    ((w_scale_ble_force_ota_t)evt->evt.gatts_evt.params.write.data);

            }

            if (evt->evt.gatts_evt.params.write.handle == h_set_tare.value_handle)
            {
                p_tare_to_zero_set 
                    ((w_scale_ble_tare_to_zero_t)evt->evt.gatts_evt.params.write.data);

            }
            break;
        }
        case BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST:
        {
            uint16_t mtu_val = BLE_GATT_ATT_MTU_DEFAULT;
            err_code = sd_ble_gatts_exchange_mtu_reply(h_conn, mtu_val);
            APP_ERROR_CHECK(err_code);
            break;
        }
        case BLE_GAP_EVT_PHY_UPDATE_REQUEST:
        {
            ble_gap_phys_t const phys = {
                .rx_phys = BLE_GAP_PHY_AUTO,
                .tx_phys = BLE_GAP_PHY_AUTO,
            };

            err_code = sd_ble_gap_phy_update(h_conn, &phys);
            APP_ERROR_CHECK(err_code);
            break;
        }
        case BLE_GAP_EVT_PHY_UPDATE:
        {
            log_printf("Tx_get : %x  Rx_get : %x  Status : %x\n",
                       evt->evt.gap_evt.params.phy_update.tx_phy, 
                       evt->evt.gap_evt.params.phy_update.rx_phy,
                       evt->evt.gap_evt.params.phy_update.status);
            break;
        }
    }

}

void soc_evt_handler(uint32_t evt_id)
{
    log_printf("soc evt %x\n", evt_id);
}

void load_default_val ()
{
    uint32_t err_code;
    ble_gatts_value_t val =
    {
        .len = sizeof(w_scale_ble_controllers_t),
        .offset = 0,
        .p_value = (uint8_t *) &g_default_controller
    };
    err_code = sd_ble_gatts_value_set(h_conn,
            h_controller.value_handle, &val);
    APP_ERROR_CHECK(err_code);
    ble_gatts_value_t val1 =
    {
        .len = sizeof(w_scale_ble_sw_ver_t),
        .offset = 0,
        .p_value = (uint8_t *) &g_sw_ver
    };
    err_code = sd_ble_gatts_value_set(h_conn,
            h_sw_ver.value_handle, &val1);
    APP_ERROR_CHECK(err_code);
}


void start_advertisement ()
{
    p_update_conn_status (false);
    uint32_t err_code;
    /** Start Adv */
    err_code = sd_ble_gap_adv_start(h_adv, BLE_CONN_CFG_TAG_DEFAULT);
    APP_ERROR_CHECK(err_code);
}


void service_init ()
{
    uint32_t err_code;
    ble_uuid_t ble_uuid;
    uint8_t uuid_type;

    ble_uuid128_t base_uuid = {
        WEIGHING_SCALE_BASE_UUID
    };
    err_code = sd_ble_uuid_vs_add(&base_uuid, &uuid_type);
    APP_ERROR_CHECK(err_code);

    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t attr_char_value;
    ble_gatts_attr_md_t attr_md;

    /**** Create the Weight service *****/

    ble_uuid.type = uuid_type;
    ble_uuid.uuid = WEIGHT_READ_SERVICE;

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
            &ble_uuid, &h_weight_service);
    APP_ERROR_CHECK(err_code);

    /**** Create the weight characteristic *****/
    ble_gatts_attr_md_t cccd_md;
    memset(&cccd_md, 0, sizeof(cccd_md));
    memset(&char_md, 0, sizeof (char_md));

    char_md.char_props.read = 1;
    char_md.char_props.notify = 1;
    char_md.p_char_user_desc = NULL;
    char_md.p_char_pf = NULL;
    char_md.p_user_desc_md = NULL;
    char_md.p_cccd_md = NULL;
    char_md.p_sccd_md = NULL;

    ble_uuid.type = uuid_type;
    ble_uuid.uuid = (WEIGHT_READ_DATA);

    memset(&attr_md, 0, sizeof (attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);
    attr_md.vloc = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen = 0;

    memset(&attr_char_value, 0, sizeof (attr_char_value));

    attr_char_value.p_uuid = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len = sizeof (w_scale_ble_weight_t);
    attr_char_value.init_offs = 0;
    attr_char_value.max_len = sizeof (w_scale_ble_weight_t);
    attr_char_value.p_value = NULL;

    err_code = sd_ble_gatts_characteristic_add(
            h_weight_service, &char_md, &attr_char_value, &h_weight_data);
    APP_ERROR_CHECK(err_code);
        

    /**** Create the Setting service *****/

    ble_uuid.type = uuid_type;
    ble_uuid.uuid = SYSTEM_SETTING_SERVICE;

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
            &ble_uuid, &h_setting_service);
    APP_ERROR_CHECK(err_code);
    
    /**** Create the Force OTA characteristic *****/

    memset(&char_md, 0, sizeof (char_md));

    char_md.char_props.read = 0;
    char_md.char_props.write = 1;
    char_md.p_char_user_desc = NULL;
    char_md.p_char_pf = NULL;
    char_md.p_user_desc_md = NULL;
    char_md.p_cccd_md = NULL;
    char_md.p_sccd_md = NULL;

    ble_uuid.type = uuid_type;
    ble_uuid.uuid = (FORCE_OTA);

    memset(&attr_md, 0, sizeof (attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    attr_md.vloc = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen = 0;

    memset(&attr_char_value, 0, sizeof (attr_char_value));

    attr_char_value.p_uuid = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len = sizeof (w_scale_ble_force_ota_t);
    attr_char_value.init_offs = 0;
    attr_char_value.max_len = sizeof (w_scale_ble_force_ota_t);
    attr_char_value.p_value = NULL;

    err_code = sd_ble_gatts_characteristic_add(
            h_setting_service, &char_md, &attr_char_value, &h_force_ota);
    APP_ERROR_CHECK(err_code);
    
    /**** Create the Software Version characteristic *****/

    memset(&char_md, 0, sizeof (char_md));

    char_md.char_props.read = 1;
    char_md.char_props.write = 0;
    char_md.p_char_user_desc = NULL;
    char_md.p_char_pf = NULL;
    char_md.p_user_desc_md = NULL;
    char_md.p_cccd_md = NULL;
    char_md.p_sccd_md = NULL;

    ble_uuid.type = uuid_type;
    ble_uuid.uuid = (SOFTWARE_VERSION);

    memset(&attr_md, 0, sizeof (attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);
    attr_md.vloc = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen = 0;

    memset(&attr_char_value, 0, sizeof (attr_char_value));

    attr_char_value.p_uuid = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len = sizeof (w_scale_ble_sw_ver_t);
    attr_char_value.init_offs = 0;
    attr_char_value.max_len = sizeof (w_scale_ble_sw_ver_t);
    attr_char_value.p_value = NULL;

    err_code = sd_ble_gatts_characteristic_add(
            h_setting_service, &char_md, &attr_char_value, &h_sw_ver);
    APP_ERROR_CHECK(err_code);
    
    /**** Create the Set Tare to Zero characteristic *****/

    memset(&char_md, 0, sizeof (char_md));

    char_md.char_props.read = 0;
    char_md.char_props.write = 1;
    char_md.p_char_user_desc = NULL;
    char_md.p_char_pf = NULL;
    char_md.p_user_desc_md = NULL;
    char_md.p_cccd_md = NULL;
    char_md.p_sccd_md = NULL;

    ble_uuid.type = uuid_type;
    ble_uuid.uuid = (TARE_TO_ZERO);

    memset(&attr_md, 0, sizeof (attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    attr_md.vloc = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen = 0;

    memset(&attr_char_value, 0, sizeof (attr_char_value));

    attr_char_value.p_uuid = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len = sizeof (w_scale_ble_tare_to_zero_t);
    attr_char_value.init_offs = 0;
    attr_char_value.max_len = sizeof (w_scale_ble_tare_to_zero_t);
    attr_char_value.p_value = NULL;

    err_code = sd_ble_gatts_characteristic_add(
            h_setting_service, &char_md, &attr_char_value, &h_set_tare);
    APP_ERROR_CHECK(err_code);
    
    /**** Create the Get Battery ADC value characteristic *****/

    memset(&char_md, 0, sizeof (char_md));

    char_md.char_props.read = 1;
    char_md.char_props.write = 0;
    char_md.p_char_user_desc = NULL;
    char_md.p_char_pf = NULL;
    char_md.p_user_desc_md = NULL;
    char_md.p_cccd_md = NULL;
    char_md.p_sccd_md = NULL;

    ble_uuid.type = uuid_type;
    ble_uuid.uuid = (BATTERY_ADC);

    memset(&attr_md, 0, sizeof (attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);
    attr_md.vloc = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen = 0;

    memset(&attr_char_value, 0, sizeof (attr_char_value));

    attr_char_value.p_uuid = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len = sizeof (w_scale_ble_batt_adc_t);
    attr_char_value.init_offs = 0;
    attr_char_value.max_len = sizeof (w_scale_ble_batt_adc_t);
    attr_char_value.p_value = NULL;

    err_code = sd_ble_gatts_characteristic_add(
            h_setting_service, &char_md, &attr_char_value, &h_read_batt);
    APP_ERROR_CHECK(err_code);
    
    /**** Create the Controller Select characteristic *****/

    memset(&char_md, 0, sizeof (char_md));

    char_md.char_props.read = 1;
    char_md.char_props.write = 0;
    char_md.p_char_user_desc = NULL;
    char_md.p_char_pf = NULL;
    char_md.p_user_desc_md = NULL;
    char_md.p_cccd_md = NULL;
    char_md.p_sccd_md = NULL;

    ble_uuid.type = uuid_type;
    ble_uuid.uuid = (CONTROLLER_IDEDNTIFIER);

    memset(&attr_md, 0, sizeof (attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);
    attr_md.vloc = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen = 0;

    memset(&attr_char_value, 0, sizeof (attr_char_value));

    attr_char_value.p_uuid = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len = sizeof (w_scale_ble_controllers_t);
    attr_char_value.init_offs = 0;
    attr_char_value.max_len = sizeof (w_scale_ble_controllers_t);
    attr_char_value.p_value = NULL;

    err_code = sd_ble_gatts_characteristic_add(
            h_setting_service, &char_md, &attr_char_value, &h_controller);
    APP_ERROR_CHECK(err_code);
    
}

void stack_init ()
{
    uint32_t err_code;
    nrf_clock_lf_cfg_t cfg =
    {
        .source        = NRF_CLOCK_LF_SRC_XTAL,
        .rc_ctiv       = 0,
        .rc_temp_ctiv  = 0,
        .accuracy = NRF_CLOCK_LF_ACCURACY_20_PPM
    };
    err_code = sd_softdevice_enable(&cfg, app_error_fault_handler);
    APP_ERROR_CHECK(err_code);

    uint32_t app_ram_start = 0x200022f0;

    err_code = sd_ble_enable(&app_ram_start);

    evt_sd_handler_init(ble_evt_handler, soc_evt_handler);
    APP_ERROR_CHECK(err_code);
//
    err_code = sd_radio_notification_cfg_set(
            NRF_RADIO_NOTIFICATION_TYPE_INT_ON_INACTIVE,
            NRF_RADIO_NOTIFICATION_DISTANCE_NONE);
    APP_ERROR_CHECK(err_code);

    
    /** GAP Init */
    ble_gap_conn_params_t   gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    err_code = sd_ble_gap_device_name_set(&sec_mode,
                                          (const uint8_t *)app_device_name,
                                          sizeof(app_device_name));
    APP_ERROR_CHECK(err_code);

    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    APP_ERROR_CHECK(err_code);
    
}

void w_scale_ble_init (void (*p_ota_flag_update)(w_scale_ble_force_ota_t force_ota),
        void (*p_set_tare_to_zero)(w_scale_ble_tare_to_zero_t set_tare), 
        void (*p_conn_status_update)(bool conn_status))
{
    /** Stack Init*/
    p_update_conn_status = p_conn_status_update;
    p_tare_to_zero_set = p_set_tare_to_zero;
    p_update_ota_flag = p_ota_flag_update;
    
    uint32_t err_code;
    
    stack_init ();

    /** Service Init */
    service_init ();
    
    /** Set advertisement data */

    h_adv = BLE_GAP_ADV_SET_HANDLE_NOT_SET;

    ble_gap_adv_data_t adv_payload;

    adv_payload.adv_data.p_data = g_adv_data1;
    adv_payload.adv_data.len = sizeof (g_adv_data1);

    adv_payload.scan_rsp_data.p_data = g_scan_rsp_data1;
    adv_payload.scan_rsp_data.len = sizeof(g_scan_rsp_data1);


    memset(&adv_params, 0, sizeof (adv_params));

    //Set channel 37, 38 and 39 as advertising channels
    memset(adv_params.channel_mask, 0, 5);

    //Set the advertising to timeout in 180s
    adv_params.duration = BLE_GAP_ADV_TIMEOUT_LIMITED_MAX;

    //Any device can scan request and connect
    adv_params.filter_policy = BLE_GAP_ADV_FP_ANY;

    //Period between sending advertising packets
    adv_params.interval = ADVERTISING_INTERVAL;

    //Use 1Mbps physical layer to be backward compatible
    adv_params.primary_phy = BLE_GAP_PHY_1MBPS;

    //The advertisement would be unidirected connectable and scannable
    adv_params.properties.type =
            BLE_GAP_ADV_TYPE_CONNECTABLE_SCANNABLE_UNDIRECTED;

    adv_params.p_peer_addr = NULL;
    adv_params.scan_req_notification = 0;

    err_code = sd_ble_gap_adv_set_configure(&h_adv,
            (ble_gap_adv_data_t const *) &adv_payload,
            (ble_gap_adv_params_t const *) &adv_params);
    APP_ERROR_CHECK(err_code);


    start_advertisement ();
}

void w_scale_ble_update_adv (w_scale_ble_weight_t adv_weight)
{
    uint32_t err_code;
    
    uint16_t l_kgs = 0;
    uint16_t l_grams = 0;
   
    l_kgs = adv_weight / 1000;
    l_grams = adv_weight % 1000;
    
    static bool adv1 = false;
      
    ble_gap_adv_data_t adv_payload;
    if (adv1)
    {

        g_scan_rsp_data1[WEIGHT_KG_INDEX] = (l_kgs & 0xFF);
        g_scan_rsp_data1[WEIGHT_KG_INDEX+1] = ((l_kgs & 0xFF00) >> 8);

        g_scan_rsp_data1[WEIGHT__G_INDEX] = (l_grams & 0xFF);
        g_scan_rsp_data1[WEIGHT__G_INDEX+1] = ((l_grams & 0xFF00) >> 8);

        adv_payload.adv_data.p_data = g_adv_data1;
        adv_payload.adv_data.len = sizeof (g_adv_data1);

        adv_payload.scan_rsp_data.p_data = g_scan_rsp_data1;
        adv_payload.scan_rsp_data.len = sizeof (g_scan_rsp_data1);

    }
    else
    {

        g_scan_rsp_data2[WEIGHT_KG_INDEX] = (l_kgs & 0xFF);
        g_scan_rsp_data2[WEIGHT_KG_INDEX+1] = ((l_kgs & 0xFF00) >> 8);

        g_scan_rsp_data2[WEIGHT__G_INDEX] = (l_grams & 0xFF);
        g_scan_rsp_data2[WEIGHT__G_INDEX+1] = ((l_grams & 0xFF00) >> 8);

        adv_payload.adv_data.p_data = g_adv_data2;
        adv_payload.adv_data.len = sizeof (g_adv_data2);

        adv_payload.scan_rsp_data.p_data = g_scan_rsp_data2;
        adv_payload.scan_rsp_data.len = sizeof (g_scan_rsp_data2);

    }
    
    err_code = sd_ble_gap_adv_set_configure(&h_adv, &adv_payload, NULL);
    APP_ERROR_CHECK(err_code);

    adv1 = !adv1;

}

void w_scale_ble_set_controller (w_scale_ble_controllers_t controller)
{
    g_default_controller = controller;
}

void w_scale_ble_set_sw_ver (w_scale_ble_sw_ver_t sw_ver)
{
    g_sw_ver = sw_ver;
}

void w_scale_ble_update_batt_adc (w_scale_ble_batt_adc_t batt_update)
{
    uint32_t err_code;
    ble_gatts_value_t val =
    {
        .len = sizeof(w_scale_ble_batt_adc_t),
        .offset = 0,
        .p_value = (uint8_t *) &batt_update
    };
    err_code = sd_ble_gatts_value_set(h_conn,
            h_read_batt.value_handle, &val);
    APP_ERROR_CHECK(err_code);
}

void w_scale_ble_update_weight (w_scale_ble_weight_t weight_update)
{
    /** Set values */
    uint32_t err_code;
    uint16_t data_size = sizeof(w_scale_ble_weight_t); 
    err_code = sd_ble_gatts_sys_attr_set(h_conn, NULL, 0, 0);
    APP_ERROR_CHECK(err_code);
    ble_gatts_hvx_params_t hvx_params = 
    {
        .handle = h_weight_data.value_handle,
        .p_data = (uint8_t *)&weight_update,
        .p_len = (uint16_t *)&data_size,
        .type = BLE_GATT_HVX_NOTIFICATION,
        .offset = 0,
    };
    err_code = sd_ble_gatts_hvx (h_conn, &hvx_params);
    if(err_code != NRF_ERROR_INVALID_STATE)
    {
        APP_ERROR_CHECK(err_code);
    }

}
