/**
  ******************************************************************************
  * File Name          : LWIP.h
  * Description        : This file provides code for the configuration
  *                      of the LWIP.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2020 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  *************************************************************************  

  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __mx_lwip_H
#define __mx_lwip_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#define ipaddr0 (uint8_t)192
#define ipaddr1 (uint8_t)168
#define ipaddr2 (uint8_t)1
#define ipaddr3 (uint8_t)225
   
#define netmask0 (uint8_t)255
#define netmask1 (uint8_t)255
#define netmask2 (uint8_t)255
#define netmask3 (uint8_t)0
   
#define gateway0 (uint8_t)192
#define gateway1 (uint8_t)168
#define gateway2 (uint8_t)1
#define gateway3 (uint8_t)1
   
#include "lwip/opt.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "lwip/netif.h"
#include "lwip/lwip_timers.h"
#include "ethernetif.h"

/* Includes for RTOS ---------------------------------------------------------*/
#if WITH_RTOS
#include "lwip/tcpip.h"
#endif /* WITH_RTOS */

/* USER CODE BEGIN 0 */

#include "lwip/ip_addr.h"
#include "lwip/dhcp.h"
#include "ethernetif.h"
#include "lwip/tcp.h"

extern struct netconn *xNetConn;

/* TCP connection state */
typedef enum
{
  TCP_NONE = 0,
  TCP_CONNECTED,
  TCP_RECEIVED,
  TCP_SENT,
  TCP_ACCEPTED,
  TCP_CLOSING,
}tcp_client_states;

/* Struct to store received data */
struct pbuf_data {
  struct pbuf *p;     // the packet buffer that was received
  uint16_t available; // number of data
};

/* TCP structure */
struct tcp_struct {
  struct tcp_pcb *pcb;          /* pointer on the current tcp_pcb */
  struct pbuf_data data;
  tcp_client_states state;      /* current connection state */
};

/* USER CODE END 0 */

/* Global Variables ----------------------------------------------------------*/
extern ETH_HandleTypeDef heth;

/* LWIP init function */	
void MX_LWIP_Init(void);

/* USER CODE BEGIN 1 */

//int client_connect(void);
err_t tcp_connected_callback(void *arg, struct tcp_pcb *tpcb, err_t err);
static void tcp_err_callback(void *arg, err_t err);
static err_t tcp_recv_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static err_t tcp_sent_callback(void *arg, struct tcp_pcb *tpcb, u16_t len);
void tcp_connection_close(struct tcp_pcb *tpcb, struct tcp_struct *tcp);
size_t client_write(const uint8_t *buf, size_t size);

/* Function defined in lwip.c to:
 *   - Read a received packet from the Ethernet buffers 
 *   - Send it to the lwIP stack for handling
 *   - Handle timeouts if NO_SYS_NO_TIMERS not set
 */ 
void MX_LWIP_Process(void);

/* USER CODE END 1 */

#ifdef __cplusplus
}
#endif
#endif /*__ mx_lwip_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
