#include "appiko_support.h"
#include "string.h"

#define NRF_UICR0_ADDR 0x10001080
                
void appiko_support_get_device_id (uint8_t * device_id)
{
    memcpy(device_id, (void *) NRF_UICR0_ADDR, 16);
}




