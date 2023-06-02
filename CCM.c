/*******************************************************************************
 * $ Copyright 2023 Cypress Semiconductor $
 *******************************************************************************/
#include "CCM.h"

#define DATA_BITS_8 (8)
#define STOP_BITS_1 (1)
#define BAUD_RATE (115200)
#define RX_BUF_SIZE (32)
#define AWS_CONNECT_RESPONSE_DELAY (4000)  /* milliseconds*/
#define WIFI_CONNECT_RESPONSE_DELAY (4000) /* milliseconds*/
#define DELAY (8000)                       /* milliseconds*/
#define BUF_SIZE (256)
#define NUMBER_OF_CHARACTERS (10)
#define AT_COMMAND_SIZE (22)

/*global variable for storing the AT COMMAND response */
char global_command_response[BUF_SIZE];

/*uart-object */
cyhal_uart_t uart_obj;

/*rx-buffer*/
uint8_t rx_buf[RX_BUF_SIZE];

/*baud rate*/
uint32_t actualbaud;

/* UART configuration*/
const cyhal_uart_cfg_t uart_config =
    {
        .data_bits = DATA_BITS_8,
        .stop_bits = STOP_BITS_1,
        .parity = CYHAL_UART_PARITY_NONE,
        .rx_buffer = rx_buf,
        .rx_buffer_size = RX_BUF_SIZE};

/*******************************************************************************
 * Function Name: Bsp_Init
 ********************************************************************************
 * Summary:
 * BSP initialization.
 * While porting to any other microcontroller,
 * replace the cybsp_init() with your microcontroller specific BSP init API's.
 *******************************************************************************/
void bsp_init()
{
    cybsp_init();
}

/*******************************************************************************
 * Function Name: Uart_Init
 ********************************************************************************
 * Summary:
 * Initialize two UART peripherals.
    - To receive debug messages
    - To send AT commands to CCM module
 * While porting to any other microcontroller,
 * replace UART initialization API's with your microcontroller specific UART API's
 *******************************************************************************/
void uart_init()
{

    /*Initialize UART to communicate with CCM via pins P12_0 and P12_1 */
    cyhal_uart_init(&uart_obj, P12_1, P12_0, NC, NC, NULL, &uart_config);
    cyhal_uart_set_baud(&uart_obj, BAUD_RATE, &actualbaud);

    /*Initialize Debug UART */
    cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX,
                        CY_RETARGET_IO_BAUDRATE);

    /* Enable global interrupts */
    __enable_irq();

    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
    printf("\x1b[2J\x1b[;H");
}

/*******************************************************************************
 * Function Name: at_command_send
 ********************************************************************************
 * Summary:
 * Sending AT Commands to CCM module via UART interface.
 *
 * while porting to any other microcontroller,
 * replace the cyhal_uart_write() API with your microcontroller specific UART API
 *
 * parameter: str
 * Address of AT Command in string format
 *
 *******************************************************************************/
void at_command_send(char *str)
{
    printf("\rSending %s \n", str);

    size_t length = strlen(str);

    /* UART API for sending data to CCM */
    cyhal_uart_write(&uart_obj, (void *)str, &length);
}

/*******************************************************************************
 * Function Name: at_command_response_receive
 ********************************************************************************
 * Summary:
 * Receive AT Command response from CCM module via UART interface.
 *
 * While porting to any other microcontroller,
 * replace the cyhal_uart_getc() API with your microcontroller specific UART API
 *
 * parameter: uint32_t delay
 * The amount of time(ms) the receive UART function should wait if there is no response
 * from CCM module
 *
 * return: char *
 *         Pointer to the response stored in a global variable.
 *
 *******************************************************************************/
char *at_command_response_receive(uint32_t delay)
{

    uint8_t read_data = 0;

    uint8_t resp_char_count = 0;

    memset(global_command_response, '\0', BUF_SIZE);

    /*cyhal_uart_getc() : Get a character. This is a blocking call which waits till a character is received*/
    while (CY_RSLT_SUCCESS == cyhal_uart_getc(&uart_obj, &read_data, delay))
    {

        global_command_response[resp_char_count++] = read_data;

        printf("%c", read_data);

        /*The below if statement breaks out of while loop
         * once you get the response for AT commands */

        if (read_data == '\n' && (!cyhal_uart_readable(&uart_obj)))
        {
            printf("\r");
            break;
        }
    }

    return global_command_response;
}

/*******************************************************************************
 * Function Name: handle_error
 ********************************************************************************
 * Summary:
 * User defined error handling function
 *
 *******************************************************************************/
void handle_error(void)
{
    /* Disable all interrupts. */
    __disable_irq();

    CY_ASSERT(0);
}

/*******************************************************************************
 * Function Name: Is_WiFi_Connected
 ********************************************************************************
 * Summary:
 * Check if CCM module is connected to Wi-Fi network
 *
 * While porting to any other microcontroller,
 * replace the cyhal_uart_getc() and cyhal_uart_write() API with your microcontroller specific UART API's
 *
 * return : uint8_t
 *          1 if connected to Wi-Fi network,
 *          0 otherwise.
 *******************************************************************************/
uint8_t is_wifi_connected()
{

    char *wifi_status = NULL;

    at_command_send("AT+DIAG PING 8.8.8.8\n");

    wifi_status = at_command_response_receive(WIFI_CONNECT_RESPONSE_DELAY);

    if (!strcmp(wifi_status, "OK Not connected to AP\r\n"))
    {
        return 0;
    }

    /* Compare 10 characters */
    else if (!strncmp(wifi_status, "OK Received ping", NUMBER_OF_CHARACTERS))
    {
        return 1;
    }

    return 0;
}

/*******************************************************************************
 * Function Name: Is_AWS_Connected
 ********************************************************************************
 * Summary:
 * Check if CCM module is connected to AWS IoT core.
 *
 * While porting to any other microcontroller,
 * replace the cyhal_uart_getc() and cyhal_uart_write() API with your microcontroller specific UART API's
 *
 * return : uint8_t
 *          1 if connected to AWS IoT core,
 *          0 otherwise.
 *******************************************************************************/
uint8_t is_aws_connected()
{

    char *aws_status = NULL;

    /* UART API for sending data to CCM*/
    at_command_send("AT+CONNECT?\n");

    aws_status = at_command_response_receive(AWS_CONNECT_RESPONSE_DELAY);

    if (!strcmp(aws_status, "OK 1 1 CONNECTED CUSTOMER\r\n"))
    {
        return 1;
    }

    else if (!strcmp(aws_status, "OK 0 1 DISCONNECTED CUSTOMER\r\n"))
    {
        return 0;
    }

    else if (!strcmp(aws_status, "OK 0 0 DISCONNECTED STAGING\r\n"))
    {
        return 0;
    }

    else if (!strcmp(aws_status, "OK 1 0 CONNECTED STAGING\r\n"))
    {
        return 0;
    }

    return 0;
}
/*******************************************************************************
 * Function Name: delay_ms
 ********************************************************************************
 * Summary:
 *
 * While porting to any other microcontroller,
 * replace the cyhal_uart_getc() and cyhal_uart_write() API with your microcontroller specific UART API's.
 *
 * return : void
 *
 *******************************************************************************/
void delay_ms(int delay)
{
    cyhal_system_delay_ms(delay);
}

/*******************************************************************************
 * Function Name: at_command_send_receive
 ********************************************************************************
 * Summary:
 *          Send AT commands to CCM Evaluation kit , receive response
 *          ,compare it with the desired_response if compare is 1 and update
 *          the result variable.
 *
 *
 * input parameter: char *str
 *                     AT command sent in string format.
 *
 * input parameter: int delay
 *                    The amount of time(ms) the receive UART function should wait if there is no response
 *                  from CCM module
 *
 * output parameter: int *result
 *                  if the desired_response is not NULL then desired_response and
 *                  response for the AT command is compared ,the value 1 is assigned to result
 *                  if they are same else value 0 is assigned to result.
 *
 * input parameter: char *desired_response
 *                  the desired response for the AT command sent in string format
 *
 * return :
 *             Address of the AT command response received in string format.
 *
 *******************************************************************************/

char *at_command_send_receive(char *str, int delay, int *result, char *desired_response)
{

    char *local_response = NULL;

    at_command_send(str);

    local_response = at_command_response_receive(delay);

    if (desired_response)
    {
        if (!strcmp(desired_response, local_response))
        {
            *result = 1;
        }

        else
        {

            *result = 0;
        }
    }

    else
    {
        *result = 0;
    }

    return local_response;
}
