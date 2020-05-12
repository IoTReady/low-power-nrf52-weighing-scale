/*  Copyright (c) 2016, Appiko
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification,
 *  are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *  this list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *  this list of conditions and the following disclaimer in the documentation
 *  and/or other materials provided with the distribution.
 *
 *  3. Neither the name of the copyright holder nor the names of its contributors
 *  may be used to endorse or promote products derived from this software without
 *  specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 *  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 *  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 *  OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef DFU_BOARDS_H_
#define DFU_BOARDS_H_

/**
 * @addtogroup group_platform
 * @{
 *
 * @brief A @ref boards.h file includes the relevant platform based on
 *  macro from the compile time defines
 */

#if defined(DFU_BOARD_PCA10040)
  #define NRF_DFU_BLE_ADV_NAME "IoTReadyDFU"
  #define NRF_DFU_HW_VERSION 1
  #include "dfu_pca10040.h"
#elif defined(DFU_BOARD_W_SCALE_HX711)
  #define NRF_DFU_BLE_ADV_NAME "IoTReadyDFU"
  #define NRF_DFU_HW_VERSION 2
  #include "dfu_weighing_scale_hx711.h"
#elif defined(DFU_BOARD_CUSTOM)
  #include "custom_board.h"
#elif defined(DFU_BOARD_SENSEBE_REV1)
  #error "Invalid board : Board not supported anymore"
//  #include "dfu_sensebe_rev1.h"
#else
#error "Board is not defined"

#endif

#endif /* DFU_BOARDS_H_ */

/** @} */
