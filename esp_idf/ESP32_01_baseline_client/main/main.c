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

#include "esp_log.h"

static const char *TAG = "IOWA_EXAMPLE_01";

iowa_context_t g_iowaH;     /* global handle */

/* LwM2M Server details, setup from menuconfig tool
 * Default values: see Kconfig.projbuild
 */
#define SERVER_SHORT_ID CONFIG_IOWA_SAMPLE_SERVER_SHORT_ID // 1234
#define SERVER_LIFETIME CONFIG_IOWA_SAMPLE_LIFETIME        // 50
#define SERVER_URI CONFIG_IOWA_SERVER_URI                  // "coap://iowa-server.ioterop.com"
#define OBJECT_NAME_PREFIX CONFIG_IOWA_SAMPLE_NAME         //"IOWA_sample_client"

#define DEVICE_NAME_SIZE   64

/* As this sample does not use security, the LwM2M Server relies only
 * on the endpoint name to identify the LwM2M Client. Thus we need an
 * unique name.
 * This function generates one from your network MAC address on ESP32
 */
static void prv_generate_unique_name(char *name)
{
    long id = 0;
    uint8_t mac[6];

    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    id = mac[0];
    for (uint8_t i = 1; i < 6; i++)
        id += (mac[i] << ((i & 1) * 8));

    snprintf(name, DEVICE_NAME_SIZE-1, "%s_%lX", OBJECT_NAME_PREFIX, id);
}

/*---------------------------------------------------------
 * Main code
 */
int iowa_app_start(void)
{
    iowa_status_t result;
    char endpoint_name[DEVICE_NAME_SIZE];
    iowa_device_info_t devInfo;

    ESP_LOGI(TAG, "===================================");
    ESP_LOGI(TAG, "This a simple LwM2M Client.");

    prv_generate_unique_name(endpoint_name); // The device name

    /* Initialize the IOWA stack. */
    g_iowaH = iowa_init(NULL);
    if (g_iowaH == NULL)
    {
        ESP_LOGE(TAG, "IOWA context initialization failed.");
        goto cleanup;
    }

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

    /* Add a LwM2M Server to connect to */
    result = iowa_client_add_server(g_iowaH, SERVER_SHORT_ID, SERVER_URI, SERVER_LIFETIME, 0, IOWA_SEC_NONE);
    if (result != IOWA_COAP_NO_ERROR)
    {
        ESP_LOGE(TAG, "Adding a server failed (%u.%02u)", (result & 0xFF) >> 5, (result & 0x1F));
        goto cleanup;
    }

	ESP_LOGI(TAG, "Registering to the LwM2M server at \"" SERVER_URI "\" under the Endpoint name [%s]", endpoint_name);
	ESP_LOGI(TAG, "Use Ctrl-C to stop.");

    /* Let IOWA run for two minutes */
    result = iowa_step(g_iowaH, 120);

cleanup:
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

    iowa_app_start();
}
