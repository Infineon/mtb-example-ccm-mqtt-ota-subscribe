/******************************************************************************
 * File Name: CCM.h
 *
 * Description: This file is the public interface of CCM.c source file.
 *
 * Related Document: README.md
 *
 ********************************************************************************
 * $ Copyright 2022 Cypress Semiconductor $
 *******************************************************************************/

#include "stdio.h"
#include "stdint.h"
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "stdlib.h"
#include "cy_retarget_io.h"

/*******************************************************************************
 * Function prototypes
 *******************************************************************************/
void bsp_init(void);

void uart_init(void);

void at_command_send(char *);

char *at_command_response_receive(uint32_t delay);

uint8_t is_wifi_connected(void);

uint8_t is_aws_connected(void);

void handle_error(void);

void delay_ms(int);

char *at_command_send_receive(char *, int, int *, char*);
