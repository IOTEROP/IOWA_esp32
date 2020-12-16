/**********************************************
 *
 * Copyright (c) 2016-2020 IoTerop.
 * All rights reserved.
 *
 * This program and the accompanying materials
 * are made available under the terms of
 * IoTerop’s IOWA License (LICENSE.TXT) which
 * accompany this distribution.
 *
 **********************************************/

/**********************************************
*
* In this file, you can define the compilation
* flags instead of specifying them on the
* compiler command-line.
*
**********************************************/

#ifndef _IOWA_CONFIG_INCLUDE_
#define _IOWA_CONFIG_INCLUDE_

#include "sdkconfig.h"

/**********************************************
*
* Platform configuration.
*
**********************************************/

/**********************************************
* To specify the endianness of your platform.
* One and only one must be defined.
*/
#ifdef CONFIG_LWM2M_BIG_ENDIAN
    #define LWM2M_BIG_ENDIAN
#elif CONFIG_LWM2M_LITTLE_ENDIAN
    #define LWM2M_LITTLE_ENDIAN
#else
    #error "Missing endianness"
#endif

/************************************************
* To specify the size of the static buffer used
* to received datagram packets.
*/

#ifdef CONFIG_IOWA_BUFFER_SIZE
    #define IOWA_BUFFER_SIZE CONFIG_IOWA_BUFFER_SIZE
#else
    #define IOWA_BUFFER_SIZE 512
#endif

/**********************************************
*
* IOWA configuration.
*
**********************************************/

/**********************************************
* Support of transports.
*/
#ifdef CONFIG_IOWA_UDP_SUPPORT
    #define IOWA_UDP_SUPPORT
#elif CONFIG_IOWA_TCP_SUPPORT
    #define IOWA_TCP_SUPPORT
#elif CONFIG_IOWA_LORAWAN_SUPPORT
    #define IOWA_LORAWAN_SUPPORT
#elif CONFIG_IOWA_SMS_SUPPORT
    #define IOWA_SMS_SUPPORT
#else
    #error "Missing transport"
#endif

/***********************************************
* To enable logs
* By level:
*     - IOWA_LOG_LEVEL_NONE (default)
*     - IOWA_LOG_LEVEL_ERROR
*     - IOWA_LOG_LEVEL_WARNING
*     - IOWA_LOG_LEVEL_INFO
*     - IOWA_LOG_LEVEL_TRACE
*/

#ifdef CONFIG_IOWA_LOG_LEVEL_NONE
	#define IOWA_LOG_LEVEL IOWA_LOG_LEVEL_NONE
#elif CONFIG_IOWA_LOG_LEVEL_ERROR
	#define IOWA_LOG_LEVEL IOWA_LOG_LEVEL_ERROR
#elif CONFIG_IOWA_LOG_LEVEL_WARNING
	#define IOWA_LOG_LEVEL IOWA_LOG_LEVEL_WARNING
#elif CONFIG_IOWA_LOG_LEVEL_INFO
	#define IOWA_LOG_LEVEL IOWA_LOG_LEVEL_INFO
#elif CONFIG_IOWA_LOG_LEVEL_TRACE
	#define IOWA_LOG_LEVEL IOWA_LOG_LEVEL_TRACE
#endif

/* ... and by components:
*     - IOWA_PART_ALL (default)
*     - IOWA_PART_BASE
*     - IOWA_PART_COAP
*     - IOWA_PART_COMM
*     - IOWA_PART_DATA
*     - IOWA_PART_LWM2M
*     - IOWA_PART_OBJECT
*     - IOWA_PART_SECURITY
*     - IOWA_PART_SYSTEM
*/

#ifndef CONFIG_IOWA_IOWA_PART_ENABLE
    #define IOWA_LOG_PART 0
#elif CONFIG_IOWA_PART_ALL
	#define IOWA_LOG_PART IOWA_PART_ALL
#elif CONFIG_IOWA_PART_BASE
	#define IOWA_LOG_PART IOWA_PART_BASE
#elif CONFIG_IOWA_PART_COAP
	#define IOWA_LOG_PART IOWA_PART_COAP
#elif CONFIG_IOWA_PART_COMM
	#define IOWA_LOG_PART IOWA_PART_COMM
#elif CONFIG_IOWA_PART_DATA
	#define IOWA_LOG_PART IOWA_PART_DATA
#elif CONFIG_IOWA_PART_LWM2M
	#define IOWA_LOG_PART IOWA_PART_LWM2M
#elif CONFIG_IOWA_PART_OBJECT
	#define IOWA_LOG_PART IOWA_PART_OBJECT
#elif CONFIG_IOWA_PART_SECURITY
	#define IOWA_LOG_PART IOWA_PART_SECURITY
#elif CONFIG_IOWA_PART_SYSTEM
	#define IOWA_LOG_PART IOWA_PART_SYSTEM
#endif


/**********************************************
* To enable LWM2M features.
**********************************************/

/**********************************************
* To specify the LWM2M role of your device.
* Several of them can be defined at the same time.
*/
#ifdef CONFIG_LWM2M_CLIENT_MODE
    #define LWM2M_CLIENT_MODE
#elif CONFIG_LWM2M_SERVER_MODE
    #define LWM2M_SERVER_MODE
#elif CONFIG_LWM2M_BOOTSTRAP_SERVER_MODE
    #define LWM2M_BOOTSTRAP_SERVER_MODE
#else
    #error "No LWM2M role specified"
#endif

#endif
