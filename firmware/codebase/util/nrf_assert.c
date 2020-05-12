/**
 *  nrf_assert.c : static and runtime assertion
 *  Copyright (C) 2019  Appiko
 */

#include "nrf_assert.h"

#ifdef DEBUG

#include "log.h"
#include <stdnoreturn.h>

noreturn void assert_nrf_callback(uint16_t line_num, const uint8_t * file_name)
{
    log_printf("Assertion at line %d in file %s.\n", line_num,
            file_name);
    while (1)
    {
    }
}

#else

void assert_nrf_callback(uint16_t line_num, const uint8_t * file_name)
{

}

#endif /* DEBUG flag as compiler flag */
