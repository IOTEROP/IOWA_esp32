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
 * This file implements simple IOWA system
 * abstraction functions for Linux and Windows.
 *
 **********************************************/

// IOWA header
#include "iowa_platform.h"

// Platform specific headers
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "esp_log.h"

// We bind this function directly to malloc().
void * iowa_system_malloc(size_t size)
{
    return malloc(size);
}

// We bind this function directly to free().
void iowa_system_free(void *pointer)
{
    free(pointer);
}

// We return the number of seconds since Epoch.
int32_t iowa_system_gettime(void)
{
    return (int32_t)time(NULL);
}


#ifdef MOVED_CODE
/* This function is moved to main.c
 * for convenient reasons (unregister IOWA)
 */
void iowa_system_reboot(void *userData)
{
    (void)userData;
    ESP_LOGD(TAG, "\tFaking a reboot.");
    exit(0);
}
#endif

// Traces are output on stderr.
void iowa_system_trace(const char *format,
                       va_list varArgs)
{
    vfprintf(stderr, format, varArgs);
}
