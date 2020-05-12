/*****************************************************************************
* | File      	:	Debug.h
* | Author      :   Waveshare team
* | Function    :	debug with printf
* | Info        :
*   Image scanning
*      Please use progressive scanning to generate images or fonts
*----------------
* |	This version:   V1.0
* | Date        :   2018-01-11
* | Info        :   Basic version
*
******************************************************************************/
/**
 *  Debug.h : Abstraction for Debug message printing
 *  Copyright (C) 2020  Appiko
 */

#ifndef __DEBUG_H
#define __DEBUG_H
#if DEBUG
#include "log.h"
	#define Debug(__info,...) log_printf("Debug: " __info,##__VA_ARGS__)
#else
	#define Debug(__info,...)  
#endif

#endif
