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

/**************************************************
 *
 * This is a very simple LwM2M Client demonstrating
 * IOWA ease-of-use over ESP32
 *
 **************************************************/

// IOWA headers
#include "iowa_client.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ESP32 specific
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"

/* Example_connect()
 * This is needed for setting up the wifi/lwip stack
 */
#include "protocol_examples_common.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "driver/gpio.h"        /* To enable LED pin */

#include "esp_log.h"

#include "iowa_ipso.h"
#include "iowa_platform.h"
#include "iowa_light_control.h"

static const char *TAG = "IOWA_EXAMPLE_02";

iowa_context_t g_iowaH;     /* global handle */

// Global structure to store the values of the custom object
typedef struct
{
    bool state;
    int  onTime;
} custom_object_values_t;


/* Can use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define LED_GPIO CONFIG_LED_GPIO

// LwM2M Server details, setup from menuconfig tool
// Default values: see Kconfig.projbuild
#define SERVER_SHORT_ID     CONFIG_IOWA_SAMPLE_SERVER_SHORT_ID  // 1234
#define SERVER_LIFETIME     CONFIG_IOWA_SAMPLE_LIFETIME         // 50
#define SERVER_URI          CONFIG_IOWA_SERVER_URI              // "coap://iowa-server.ioterop.com"
#define OBJECT_NAME_PREFIX  CONFIG_IOWA_SAMPLE_NAME             //"IOWA_sample_client"

#define DEVICE_NAME_SIZE   64

// As this sample does not use security, the LwM2M Server relies only
// on the endpoint name to identify the LwM2M Client. Thus we need an
// unique name.
// This function generates one from your network MAC address on ESP32
static void prv_generate_unique_name(char *name)
{
    long id= 0;
    uint8_t mac[6];

    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    id = mac[0];
   for ( uint8_t i = 1; i < 6; i++ )
        id += (mac[i] << (( i & 1 ) * 8 ));

    snprintf(name, DEVICE_NAME_SIZE-1, "%s_%lX", OBJECT_NAME_PREFIX, id);
}

/* --------------------------------------------------------- */
void led_off (void)
{
    gpio_set_level(LED_GPIO, 0);
}

/* --------------------------------------------------------- */
void led_on (void)
{
    gpio_set_level(LED_GPIO, 1);
}


/* The callback called by IOWA to perform operations on the custom object
 */
iowa_status_t prv_LEDControlCallback(iowa_dm_operation_t operation,
                                       iowa_lwm2m_data_t *dataP,
                                       size_t numData,
                                       void *userData,
                                       iowa_context_t iowaH)
{
    size_t i;
    custom_object_values_t *objectValuesP;

    objectValuesP = (custom_object_values_t *)userData;

    switch (operation)
    {
    case IOWA_DM_READ:
        for (i = 0; i < numData; i++)
        {
            switch (dataP[i].resourceID)
            {
            case 5850:  // On/Off
                dataP[i].value.asBoolean = objectValuesP->state;
                break;
            case 5852:  // On Time
                dataP[i].value.asInteger =  objectValuesP->onTime;
                break;

            default:
                // Already handled by IOWA stack
                break;
            }
        }
        break;

    case IOWA_DM_WRITE:
        ESP_LOGI(TAG, "IOWA_DM_WRITE");
        for (i = 0; i < numData; i++)
        {
            switch (dataP[i].resourceID)
            {
            case 5850:
                objectValuesP->state = dataP[i].value.asBoolean;
                break;

            case 5852: // On Time
                if (dataP[i].value.asInteger != 0)
                {
                    return IOWA_COAP_406_NOT_ACCEPTABLE;
                }

                objectValuesP->onTime = 0;
                break;
            default:
                // Already handled by IOWA stack
                break;
            }
        }
        break;

    default:
        // Already handled by IOWA stack
        break;
    }

    return IOWA_COAP_NO_ERROR;
}


/*---------------------------------------------------------
 * Main code
 */
int iowa_app_start(void)
{
    iowa_status_t result;
    char endpoint_name[DEVICE_NAME_SIZE];
    iowa_device_info_t devInfo;

    custom_object_values_t objectValues = {.state = false, .onTime = 0};

    /*Resources declaration used for the Custom Object 3311
     * 5850 is mandatory (On/Off)
     * 5852 is optional (On Time)
     */
    iowa_lwm2m_resource_desc_t resources[2] = {
        {5850, IOWA_LWM2M_TYPE_BOOLEAN, IOWA_DM_READ | IOWA_DM_WRITE, IOWA_RESOURCE_FLAG_MANDATORY},
        {5852, IOWA_LWM2M_TYPE_INTEGER, IOWA_DM_READ | IOWA_DM_WRITE, IOWA_RESOURCE_FLAG_OPTIONAL}
    };
    uint16_t singleInstanceId[1] = {0};

    ESP_LOGI(TAG, "===================================");
    ESP_LOGI(TAG, "This a simple LwM2M Client.");

    prv_generate_unique_name(endpoint_name);    /* The device name */

    /* Initialize the IOWA stack. */
    g_iowaH = iowa_init(NULL);
    if (g_iowaH == NULL)
    {
        ESP_LOGE(TAG, "IOWA context initialization failed");
        goto cleanup;
    }

    /* Save the handle */
    g_iowaH = g_iowaH;

    /* Set the information presented in Device Object (ID: 3)
     * all of these are optional
     */
    memset(&devInfo, 0, sizeof(iowa_device_info_t));
    devInfo.manufacturer = "https://ioterop.com";
    devInfo.deviceType = "IOWA ESP32 sample from https://github.com/IOTEROP";
    devInfo.modelNumber = "ESP32 baseline_client";

    /* Configure the LwM2M Client */
    result = iowa_client_configure(g_iowaH, endpoint_name, &devInfo, NULL);
    if (result != IOWA_COAP_NO_ERROR)
    {
        ESP_LOGE(TAG, "IOWA Client configuration failed (%u.%02u)", (result & 0xFF) >> 5, (result & 0x1F));
        goto cleanup;
    }
    ESP_LOGI(TAG, "IOWA Client configuration OK");

    /* Create of a custom object. Actual values are stored in a custom_object_values_t structure */
    result = iowa_client_add_custom_object(g_iowaH,
                                           3311,    //Light Control (http://www.openmobilealliance.org/wp/omna/lwm2m/lwm2mregistry.html)
                                           1, singleInstanceId,
                                           2, resources,
                                           prv_LEDControlCallback, NULL, NULL,
                                           &objectValues);
    if (result != IOWA_COAP_NO_ERROR)
    {
        fprintf(stderr, "Adding a custom object failed (%u.%02u)", (result & 0xFF) >> 5, (result & 0x1F));
        goto cleanup;
    }

    /* Add a LwM2M Server to connect to */
    result = iowa_client_add_server(g_iowaH, SERVER_SHORT_ID, SERVER_URI, SERVER_LIFETIME, 0, IOWA_SEC_NONE);
    if (result != IOWA_COAP_NO_ERROR)
    {
        ESP_LOGE(TAG,"Adding a server failed (%u.%02u)", (result & 0xFF) >> 5, (result & 0x1F));
        goto cleanup;
    }

    ESP_LOGI(TAG, "Registering to the LwM2M server at \"" SERVER_URI "\" under the Endpoint name [%s]", endpoint_name);
	ESP_LOGI(TAG, "Use Ctrl-C to stop.");

    /* Let IOWA run for 1 second */
    while (result == IOWA_COAP_NO_ERROR)
    {
        if (objectValues.state == true)
        {
            led_on();
            objectValues.onTime++;
            iowa_client_notification_lock(g_iowaH, true);
            iowa_client_object_resource_changed(g_iowaH, 3311, 0, 5852);
            iowa_client_notification_lock(g_iowaH, false);
        } else {
            led_off();
        }

        result = iowa_step(g_iowaH, 1);     /* 1 second */
    }

cleanup:
    iowa_client_remove_custom_object(g_iowaH, 3311);
    iowa_client_remove_server(g_iowaH, SERVER_SHORT_ID);
    iowa_close(g_iowaH);

    return 0;
}

/*---------------------------------------------------------
 * We moved this function from connection_abstraction.c for a
 * cleaner reboot (e.g.: unregister client, ...)
 */
void iowa_system_reboot(void *userData)
{
    ESP_LOGI(TAG, "Ready to initiate a reboot...");
    iowa_client_remove_custom_object(g_iowaH, 3311);
    iowa_client_remove_server(g_iowaH, SERVER_SHORT_ID);
    iowa_close(g_iowaH);

    /* delay 1 second before the restart */
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    ESP_LOGI(TAG, "Restart the device....");
    esp_restart(); //Bye
}

/*---------------------------------------------------------
 * Entry point
 */
void app_main(void)
{
    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("IOWA_CLIENT", ESP_LOG_VERBOSE);

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    ESP_ERROR_CHECK(example_connect());

    /* Configure the IOMUX register for pad LED_GPIO (some pads are
       muxed to GPIO on reset already, but some default to other
       functions and need to be switched to GPIO. Consult the
       Technical Reference for a list of pads and their default
       functions.)
       We configure GPIO_MODE_OUTPUT | GPIO_MODE_INPUT to be able to read and write
       the GPIO state.
    */
    gpio_pad_select_gpio(LED_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT | GPIO_MODE_INPUT);

    /* Let's go */
    iowa_app_start();
}
