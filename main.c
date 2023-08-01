/******************************************************************************
 * File Name:   main.c
 *
 * Description: This code example demonstrates MQTT Subscribe and OTA from AWS
 * IoT Core with the help of the Cloud Connectivity Manager (CCM) evaluation kit.
 *
 * Related Document: See README.md
 *
 *
 ********************************************************************************
 * $ Copyright 2023 Cypress Semiconductor $
 ********************************************************************************/
#include "CCM.h"

/*******************************************************************************
 * Macros
 *******************************************************************************/
/* define MODIFY_SSID macro as 1 for disconnecting the existing Wi-Fi connection and
 * connect to different Access point*/
#define MODIFY_SSID_AFTER_CONNECTED (0)

/* define CIRRENT_APP_ONBOARDING macro as 1 for Wi-Fi Onboarding via Cirrent APP.*/
#define CIRRENT_APP_ONBOARDING (0)

/*define AWS_FLOW macro as 1 for choosing AWS flow and 0 for Cirrent flow*/
#define AWS_FLOW (1)

/* Max response delay in milliseconds for AT commands*/
#define RESPONSE_DELAY (120000)

#define GPIO_INTERRUPT_PRIORITY (7u)

#define SUCCESS 1
#define FAILURE 0

/* CCM evaluation kits event pin is connected to P5_5*/
#define EVENT_PIN P5_5

#define POLLING_DELAY (60000)

/* Set SSID, Passphrase and Endpoint as follows
 * AT+CONF SSID=XXXX\n; where XXXX is the required SSID
 * AT+CONF Passphrase=YYYY\n ; YYYY is the Passphrase
 * AT+CONF EndPoint=ZZZZ\n; ZZZZ is the endpoint
 */

#define SET_SSID "AT+CONF SSID=\n"
#define SET_PASSPHRASE "AT+CONF Passphrase=\n"
#define SET_ENDPOINT "AT+CONF Endpoint=\n"

/*******************************************************************************
 * Global Variables
 *******************************************************************************/
bool gpio_intr_flag = false;
int result = 0;
char *response = NULL;

/******************************************************************************
 * Function Prototypes
 *******************************************************************************/

static void wifionboarding(void);
static void gpio_interrupt_handler(void *, cyhal_gpio_event_t);
static void empty_event_queue(void);

/*******************************************************************************
 * Function Name: main
 *******************************************************************************
 * Summary:
 *  System entrance point. This function
 *  - performs initial setup of device
 *  - initializes UART peripherals to send AT Commands to CCM and view debug messages.
 *  - sends required AT Commands to CCM module
 *
 * Return:
 *  int
 *
 *******************************************************************************/
int main()
{

    cyhal_gpio_callback_data_t gpio_event_callback = {
        .callback = gpio_interrupt_handler,
        .callback_arg = NULL};

    bsp_init();

    uart_init();

    cyhal_gpio_init(EVENT_PIN, CYHAL_GPIO_DIR_INPUT,
                    CYHAL_GPIO_DRIVE_NONE, CYBSP_LED_STATE_OFF);

    cyhal_gpio_register_callback(EVENT_PIN, &gpio_event_callback);

    cyhal_gpio_enable_event(EVENT_PIN, CYHAL_GPIO_IRQ_RISE,
                            GPIO_INTERRUPT_PRIORITY, true);

    printf("\r ******************AIROC™ CCM MQTT OTA AND SUBSCRIBE******************\n");

#if MODIFY_SSID_AFTER_CONNECTED

    /* AT command for disconnecting from Wi-Fi network */
    at_command_send_receive("AT+DISCONNECT\n", RESPONSE_DELAY, &result, NULL);

#endif

#if AWS_FLOW

    if (!is_aws_connected())
    {

        /*AT command for sending Device Endpoint*/
        at_command_send_receive(SET_ENDPOINT, RESPONSE_DELAY, &result, NULL);

        /*Connect to Wi-Fi network if it is not connected already*/
        if (!is_wifi_connected())
        {
            wifionboarding();
        }

        /*AT command for Connecting to AWS Cloud*/
        at_command_send_receive("AT+CONNECT\n", RESPONSE_DELAY, &result, "OK 1 CONNECTED\r\n");

        if (result != SUCCESS)
        {
            handle_error();
        }
    }

#else

    /*Check if CCM module already connected to AWS*/
    if (!is_aws_connected())
    {

        /*Connect to Wi-Fi network if it is not connected already*/
        if (!is_wifi_connected())
        {
            wifionboarding();
        }

        /*AT command for Connecting CCM device to AWS staging*/
        at_command_send_receive("AT+CONNECT\n", RESPONSE_DELAY, &result, NULL);

        /*AT command for Getting Endpoint from Cirrent Cloud*/
        at_command_send_receive("AT+CLOUD_SYNC\n", RESPONSE_DELAY, &result, NULL);

        /* Check in Cirrent console if the Job executed succesfully */
        printf("\nThe Connection Automatically switches to the new endpoint after 120 seconds\n\n");

        delay_ms(MAX_CONNECT_DELAY);

        while (!is_aws_connected())
            ;
    }

#endif
    /* AT commands for storing the topic name and subscribing to it*/
    at_command_send_receive("AT+CONF Topic1=data\n", RESPONSE_DELAY, &result, NULL);

    at_command_send_receive("AT+SUBSCRIBE1\n", RESPONSE_DELAY, &result, NULL);

    empty_event_queue();

    while (1)
    {

        if (gpio_intr_flag)
        {
            /* AT command for checking the events queued in CCM module*/
            response = at_command_send_receive("AT+EVENT?\n", RESPONSE_DELAY, &result, NULL);

            /* New message available in the subscribed topic event*/
            if (!strcmp("OK 1 1 MSG\r\n", response))
            {
                printf("\nNew message notification on the subscribed topic\n\n\r");
                /*AT command to receive the message from the subscribed topic */
                at_command_send_receive("AT+GET1\n", RESPONSE_DELAY, &result, NULL);
            }

            /*OTA event*/
            else if (!strcmp("OK 5 1 OTA\r\n", response))
            {
                printf("\nNew OTA firmware available notificaiton\n\n\r");
                /*Download the firmware*/
                at_command_send_receive("AT+OTA ACCEPT\n", RESPONSE_DELAY, &result, NULL);
            }

            /*Event denoting that the downloaded OTA firmware is Verified */
            else if (!strcmp("OK 5 4 OTA\r\n", response))
            {
                printf("\nThe new OTA firmware image verified notification\n\n\r");
                /*Apply the new firmware*/
                at_command_send_receive("AT+OTA APPLY\n", RESPONSE_DELAY, &result, NULL);
            }

            /* Boot up event*/
            else if (!strcmp("OK 2 0 STARTUP\r\n", response))
            {
                printf("\nStart up event notification\n\n\r");
                /*Host software reset*/
                NVIC_SystemReset();
            }

            gpio_intr_flag = false;
        }
    }
}

/*******************************************************************************
 * Function Name: wifionboarding
 *******************************************************************************
 * Summary: Send AT commands to set SSID and Passphrase for CCM module.
 *                                  or
 *          Send AT command to enter Onboarding mode and connect to Wi-Fi via Cirrent APP
 * Return:
 *  void
 *
 *******************************************************************************/

static void wifionboarding()
{

#if CIRRENT_APP_ONBOARDING

    /* AT command to enter Wi-Fi onboarding mode*/
    at_command_send_receive("AT+CONFMODE\n", RESPONSE_DELAY, &result, NULL);

    printf("\n\rOpen Cirrent APP on your mobile device and choose your Wi-Fi SSID. \n\rThe program continues after successfully connecting to Wi-Fi SSID.\n\r");

    while (!is_wifi_connected())
    {
        delay_ms(POLLING_DELAY);
    }

#else

    /* AT command for sending SSID */
    at_command_send_receive(SET_SSID, RESPONSE_DELAY, &result, NULL);

    /*AT command for sending Passphrase*/
    at_command_send_receive(SET_PASSPHRASE, RESPONSE_DELAY, &result, NULL);

#endif
}

static void gpio_interrupt_handler(void *handler_arg, cyhal_gpio_event_t event)
{
    gpio_intr_flag = true;
}

static void empty_event_queue()
{
    int result = 0;
    /* AT command for checking the events queued in CCM module*/
    while (!result)
    {
        at_command_send_receive("AT+EVENT?\n", RESPONSE_DELAY, &result, "OK\r\n");
    }
}

/* [] END OF FILE */
