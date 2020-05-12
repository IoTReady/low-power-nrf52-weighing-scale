/**
 *  evt_sd_handler.c : Softdevice event handler
 *  Copyright (C) 2019  Appiko
 */

#include "stddef.h"
#include "stdbool.h"
#include "nrf_sdm.h"
#include "app_error.h"
#include "common_util.h"
#include "stdint.h"

#include "evt_sd_handler.h"
#include "nrf_assert.h"
#include "nrf_nvic.h"

#if ISR_MANAGER == 1
#include "isr_manager.h"
#endif

#define SWI_USED SWI_USED_EVT_SD_HANDLER

/** @anchor swi_defines
 * @name Defines for the specific SWI peripheral used for evt_sd_handler
 * @{*/
#define SWI_ID              CONCAT_2(NRF_SWI, SWI_USED)
#define SWI_IRQN             SWI_IRQN_a(SWI_USED)
#define SWI_IRQ_Handler      SWI_IRQ_Handler_a(SWI_USED)

#define SWI_IRQN_a(n)        SWI_IRQN_b(n)
#define SWI_IRQN_b(n)        SWI##n##_EGU##n##_IRQn

#define SWI_IRQ_Handler_a(n) SWI_IRQ_Handler_b(n)
#define SWI_IRQ_Handler_b(n) SWI##n##_EGU##n##_IRQHandler
/** @} */

///The buffer where the ble event data is stored by sd_evt_get
uint32_t ble_evt_buffer[
          CEIL_DIV(BLE_EVT_LEN_MAX(BLE_GATT_ATT_MTU_DEFAULT)
          ,sizeof(uint32_t))];

void (*ble_handler)(ble_evt_t * evt);
void (*soc_handler)(uint32_t evt);

void evt_sd_handler_init
            (void (* ble_evt_handler)(ble_evt_t * ble_evt),
            void (* soc_evt_handler)(uint32_t soc_evt_id))
{
    ASSERT(ble_evt_handler != NULL);
    ASSERT(soc_evt_handler != NULL);
    ble_handler = ble_evt_handler;
    soc_handler = soc_evt_handler;

    uint32_t err_code = sd_nvic_EnableIRQ(SWI_IRQN);
    APP_ERROR_CHECK(err_code);
}

#if ISR_MANAGER == 1
void evt_sd_handler_swi_Handler ()
#else
void SWI_IRQ_Handler(void)
#endif
{
    bool soc_evts_handled = false;
    while(soc_evts_handled == false)
    {
        uint32_t err_code, evt_id;
        err_code = sd_evt_get(&evt_id);
        if (err_code == NRF_ERROR_NOT_FOUND)
        {
            //No more events
            soc_evts_handled = true;
        }
        else if (err_code != NRF_SUCCESS)
        {
            APP_ERROR_CHECK(err_code);
        }
        else
        {
            // Call the SoC event handler.
            soc_handler(evt_id);
        }
    }

    bool ble_evts_handled = false;
    while(ble_evts_handled == false)
    {
        // Pull event from stack
        uint16_t evt_len = sizeof(ble_evt_buffer);

        uint32_t err_code = sd_ble_evt_get((uint8_t*)ble_evt_buffer, &evt_len);
        if (err_code == NRF_ERROR_NOT_FOUND)
        {
            ble_evts_handled = true;
        }
        else if (err_code != NRF_SUCCESS)
        {
            APP_ERROR_CHECK(err_code);
        }
        else
        {
            // Call the BLE stack event handler.
            ble_handler((ble_evt_t *) ble_evt_buffer);
        }
    }
}
