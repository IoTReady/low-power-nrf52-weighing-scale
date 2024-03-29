
BOARD           := BOARD_WEIGHING_SCALE_HX711
SLNO            := 681592281
BLE_REQD        := BLE_STACK_SUPPORT_REQD
#PINRESET_REQD   := CONFIG_GPIO_AS_PINRESET
PINRESET_REQD   := CONFIG_GPIO_AS_PIN
DEBUGGER        := JLINK
LOGGER          := LOG_SEGGER_RTT
FW_VER_VAL      := 0
MS_TIMER_FREQ   := 32768
#TSSP_DETECT_FREQ := 1000
SD_USED         := s112
DISPLAY_USED	:= WSEPD_1IN54_BW_V1
SD_VER          := 6.0.0
SD_ID		:= 0xA7
CONFIG_HEADER	:= 0
SHARED_RESOURCES := 0

ifeq ($(BOARD),BOARD_WEIGHING_SCALE_HX711)
WEIGHING_METHOD := HX_ADC
DFU_BOARD_ID := 2
endif
ifeq ($(BOARD),BOARD_WEIGHING_SCALE_LTC6915)
WEIGHING_METHOD := LTC_ADC
DFU_BOARD_ID := 3
endif

SDK_DIR         = ../../bootloader/components
DOC_DIR         = ../../doc
PLATFORM_DIR    = ../../platform
CODEBASE_DIR    = ../../codebase

INCLUDEDIRS	= .
INCLUDEDIRS	+= $(PLATFORM_DIR)
ifneq ($(SD_USED_LC),blank)
INCLUDEDIRS += $(SDK_DIR)/softdevice/$(SD_USED)/headers
endif
INCLUDEDIRS += $(CODEBASE_DIR)/hal
INCLUDEDIRS += $(CODEBASE_DIR)/segger_rtt
INCLUDEDIRS += $(CODEBASE_DIR)/sd_assist
INCLUDEDIRS += $(CODEBASE_DIR)/peripheral_modules
INCLUDEDIRS += $(CODEBASE_DIR)/util

INCLUDEDIRS += ./led_sequences

C_SRC_DIRS = .
C_SRC_DIRS += $(CODEBASE_DIR)/segger_rtt
C_SRC_DIRS += $(CODEBASE_DIR)/hal
C_SRC_DIRS += $(CODEBASE_DIR)/sd_assist
C_SRC_DIRS += $(CODEBASE_DIR)/peripheral_modules
C_SRC_DIRS += $(CODEBASE_DIR)/util


C_SRC = main.c
C_SRC += nrf_assert.c
C_SRC += hal_clocks.c ms_timer.c
C_SRC += nrf_util.c
ifeq ($(LOGGER), LOG_SEGGER_RTT)
C_SRC += SEGGER_RTT.c SEGGER_RTT_printf.c
else ifeq ($(LOGGER), LOG_UART_DMA_PRINTF)
C_SRC += uart_printf.c tinyprintf.c
else ifeq ($(LOGGER), LOG_UART_PRINTF)
C_SRC += hal_uart.c tinyprintf.c
else
endif
C_SRC += info_display.c
C_SRC += weighing_scale_ble.c
C_SRC += weight_mod.c
C_SRC += app_error.c
C_SRC += evt_sd_handler.c
C_SRC += simple_adc.c

ifneq ($(DISPLAY_USED), blank)
DISPLAY_DIR	= $(CODEBASE_DIR)/display_lib
include $(DISPLAY_DIR)/Makefile.display
endif

#Gets the name of the application folder
APPLN = $(shell basename $(PWD))

CFLAGS_APP = -DDEBUG
CFLAGS_APP += -D$(DISPLAY_USED)
CFLAGS_APP += -D$(BLE_REQD)
CFLAGS_APP += -D$(PINRESET_REQD)
CFLAGS_APP += -D$(LOGGER)
CFLAGS_APP += -D$(WEIGHING_METHOD)
CFLAGS_APP += -DFW_VER=$(FW_VER_VAL)
#CFLAGS_APP += -DMS_TIMER_FREQ=$(MS_TIMER_FREQ)
#CFLAGS_APP += -DTSSP_DETECT_FREQ=$(TSSP_DETECT_FREQ)
CFLAGS_APP += -DSYS_CFG_PRESENT=$(CONFIG_HEADER)
CFLAGS_APP += -DISR_MANAGER=$(SHARED_RESOURCES)

#Lower case of BOARD
BOARD_HEADER  = $(shell echo $(BOARD) | tr A-Z a-z)
include $(PLATFORM_DIR)/Makefile.$(BOARD_HEADER)

include ../Makefile.common
