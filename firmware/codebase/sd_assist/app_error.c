/**
 *  app_error.c Application error handler
 *  Copyright (C) 2019  Appiko
 */

#include "app_error.h"
#include "nrf.h"


#ifdef DEBUG

#include "log.h"
#include "stdnoreturn.h"

noreturn void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name)
{
    // This call can be used for ONLY debug purposes during application development.
    log_printf("Error of 0x%X at %d in  %s\n", error_code, line_num, p_file_name);

    // On error, the system can only recover with a reset.
//    NVIC_SystemReset();
    for(;;);
}

noreturn void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t info)
{
    // This call can be used for ONLY debug purposes during application development.
    log_printf("Error of 0x%X ID at 0x%X PC with 0x%X info\n", id, pc, info);

    // On error, the system can only recover with a reset.
//    NVIC_SystemReset();
    for(;;);
}

#else

void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name)
{

}

void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t info)
{

}

#endif  /* DEBUG flag as compiler flag */
