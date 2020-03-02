/**
 ******************************************************************************
  * File Name          : LWIP.c
  * Description        : This file provides initialization code for LWIP
  *                      middleWare.
  ******************************************************************************
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
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "lwip.h"
#include "lwip/init.h"
#include "lwip/netif.h"

/* USER CODE BEGIN 0 */
#include <string.h>
osSemaphoreId Netif_LinkSemaphore = NULL;
/* Ethernet link thread Argument */
struct link_str link_arg;
struct netconn *xNetConn;
/* USER CODE END 0 */

/* ETH Variables initialization ----------------------------------------------*/

/* USER CODE BEGIN 1 */
struct tcp_struct *tcp_client;
/* USER CODE END 1 */

/* Variables Initialization */
struct netif gnetif;
struct ip_addr ipaddr;
struct ip_addr netmask;
struct ip_addr gw;
uint8_t IP_ADDRESS[4];
uint8_t NETMASK_ADDRESS[4];
uint8_t GATEWAY_ADDRESS[4];

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */

/* init function */
void MX_LWIP_Init(void)
{
  IP_ADDRESS[0] = ipaddr0;
  IP_ADDRESS[1] = ipaddr1;
  IP_ADDRESS[2] = ipaddr2;
  IP_ADDRESS[3] = ipaddr3;
  NETMASK_ADDRESS[0] = netmask0;
  NETMASK_ADDRESS[1] = netmask1;
  NETMASK_ADDRESS[2] = netmask2;
  NETMASK_ADDRESS[3] = netmask3;
  GATEWAY_ADDRESS[0] = gateway0;
  GATEWAY_ADDRESS[1] = gateway1;
  GATEWAY_ADDRESS[2] = gateway2;
  GATEWAY_ADDRESS[3] = gateway3;
   
  tcpip_init( NULL, NULL );	
 
  IP4_ADDR(&ipaddr, IP_ADDRESS[0], IP_ADDRESS[1], IP_ADDRESS[2], IP_ADDRESS[3]);
  IP4_ADDR(&netmask, NETMASK_ADDRESS[0], NETMASK_ADDRESS[1] , NETMASK_ADDRESS[2], NETMASK_ADDRESS[3]);
  IP4_ADDR(&gw, GATEWAY_ADDRESS[0], GATEWAY_ADDRESS[1], GATEWAY_ADDRESS[2], GATEWAY_ADDRESS[3]);  
  

  /* add the network interface */
  netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);
 
 
  /*  Registers the default network interface */
  netif_set_default(&gnetif);

  if (netif_is_link_up(&gnetif))
  {
    /* When the netif is fully configured this function must be called */
    netif_set_up(&gnetif);
  }
  else
  {
    /* When the netif link is down this function must be called */
       netif_set_down(&gnetif);
  }  
  
  /* Set the link callback function, this function is called on change of link status*/
  netif_set_link_callback(&gnetif, ethernetif_update_config);

/* USER CODE BEGIN 3 */
  /* create a binary semaphore used for informing ethernetif of frame reception */
  osSemaphoreDef(Netif_SEM);
  Netif_LinkSemaphore = osSemaphoreCreate(osSemaphore(Netif_SEM) , 1 );

  link_arg.netif = &gnetif;
  link_arg.semaphore = Netif_LinkSemaphore;
  /* Create the Ethernet link handler thread */
  osThreadDef(LinkThr, ethernetif_set_link, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);
  osThreadCreate (osThread(LinkThr), &link_arg);
  
    xNetConn = netconn_new ( NETCONN_TCP ); 
  
  netconn_bind ( xNetConn, &ipaddr, 0 );
/* USER CODE END 3 */
}

/* USER CODE BEGIN 4 */

//int client_connect(){
//  ip_addr_t DestIPaddr;
//    char str[]="h,AA00\r\n";
// int rc2;
// 
//  if ( xNetConn == NULL ) { 
// 
//  /* No memory for new connection? */
//    return 0;
//  }
//
//IP4_ADDR( &DestIPaddr, (uint8_t)3, (uint8_t)15, (uint8_t)19, (uint8_t)182 );
//
//rc2 = netconn_connect ( xNetConn, &DestIPaddr, 10000 ); 
// 
//if (rc2 != ERR_OK )
//{
//  netconn_delete ( xNetConn );
//    return 0;
//}
//while(1){
//  netconn_write(xNetConn,str,strlen(str),NETCONN_NOFLAG);
//  osDelay(2000);
//}
//return 1;
//}

//int client_connect(void){
//  ip_addr_t DestIPaddr;
//  
//  if(tcp_client == NULL) {
//    /* Allocates memory for client */
//    tcp_client = (struct tcp_struct *)mem_malloc(sizeof(struct tcp_struct));
//
//    if(tcp_client == NULL) {
//        return 0;
//    }
//  }
//
//  /* Creates a new TCP protocol control block */
//  tcp_client->pcb = tcp_new();
//
//  if(tcp_client->pcb == NULL) {
//    return 0;
//  }
//
//  tcp_client->data.p = NULL;
//  tcp_client->data.available = 0;
//  tcp_client->state = TCP_NONE;
//
//  //ip_addr_t DestIPaddr;
//  tcp_arg(tcp_client->pcb, tcp_client);
//  IP4_ADDR( &DestIPaddr, (uint8_t)3, (uint8_t)15, (uint8_t)19, (uint8_t)182 );
//  if(ERR_OK != tcp_connect(tcp_client->pcb, &DestIPaddr, 10000, tcp_connected_callback)) {
//    //stop();
//    HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_7);
//      HAL_Delay(500);
//    return 0;
//  }
//  
//
//  
//  //HAL_TIM_Base_Start(&htim6);
//  while(tcp_client->state == TCP_NONE) {
//    //stm32_eth_scheduler();
//    HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_14);
//    osDelay(2000);
//    if((tcp_client->state == TCP_CLOSING) /*|| (TIM6->CNT >= 9999)*/) {
//      //stop();
//      return 0;
//    }
//  }
//
//  return 1;
//  
//}

err_t tcp_connected_callback(void *arg, struct tcp_pcb *tpcb, err_t err)
{
  struct tcp_struct *tcp_arg = (struct tcp_struct *)arg;

  if (err == ERR_OK)
  {
    if ((tcp_arg != NULL) && (tcp_arg->pcb == tpcb))
    {
      tcp_arg->state = TCP_CONNECTED;

      /* initialize LwIP tcp_recv callback function */
      tcp_recv(tpcb, tcp_recv_callback);

      /* initialize LwIP tcp_sent callback function */
      tcp_sent(tpcb, tcp_sent_callback);

      /* initialize LwIP tcp_err callback function */
      tcp_err(tpcb, tcp_err_callback);

      return ERR_OK;
    }
    else
    {
      /* close connection */
      tcp_connection_close(tpcb, tcp_arg);

      return ERR_ARG;
    }
  }
  else
  {
    /* close connection */
    tcp_connection_close(tpcb, tcp_arg);
  }
  return err;
}

/**
  * @brief tcp_receiv callback
  * @param arg: argument to be passed to receive callback
  * @param tpcb: tcp connection control block
  * @param err: receive error code
  * @retval err_t: retuned error
  */
static err_t tcp_recv_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
  struct tcp_struct *tcp_arg = (struct tcp_struct *)arg;
  err_t ret_err;

  /* if we receive an empty tcp frame from server => close connection */
  if (p == NULL)
  {
    /* we're done sending, close connection */
    tcp_connection_close(tpcb, tcp_arg);
    ret_err = ERR_OK;
  }
  /* else : a non empty frame was received from echo server but for some reason err != ERR_OK */
  else if(err != ERR_OK)
  {
    /* free received pbuf*/
    if (p != NULL)
    {
      pbuf_free(p);
    }
    ret_err = err;
  }
  else if((tcp_arg->state == TCP_CONNECTED) || (tcp_arg->state == TCP_ACCEPTED))
  {
    /* Acknowledge data reception */
    tcp_recved(tpcb, p->tot_len);

    if(tcp_arg->data.p == NULL) {
      tcp_arg->data.p = p;
    }
    else {
      pbuf_chain(tcp_arg->data.p, p);
    }

    tcp_arg->data.available += p->len;
    ret_err = ERR_OK;
  }
  /* data received when connection already closed */
  else
  {
    /* Acknowledge data reception */
    tcp_recved(tpcb, p->tot_len);

    /* free pbuf and do nothing */
    pbuf_free(p);
    ret_err = ERR_OK;
  }
  return ret_err;
}

void tcp_connection_close(struct tcp_pcb *tpcb, struct tcp_struct *tcp)
{
  /* remove callbacks */
  tcp_recv(tpcb, NULL);
  tcp_sent(tpcb, NULL);
  tcp_poll(tpcb, NULL,0);
  tcp_err(tpcb, NULL);
  tcp_accept(tpcb, NULL);

  /* close tcp connection */
  tcp_close(tpcb);

  tcp->pcb = NULL;
  tcp->state = TCP_CLOSING;
}

size_t client_write(const uint8_t *buf, size_t size) {
  if( (tcp_client == NULL) || (tcp_client->pcb == NULL) ||
      (buf == NULL) || (size == 0)) {
    return 0;
  }

  /* If client not connected or accepted, it can't write because connection is
  not ready */
  if((tcp_client->state != TCP_ACCEPTED) &&
    (tcp_client->state != TCP_CONNECTED)) {
    return 0;
  }

  if(ERR_OK != tcp_write(tcp_client->pcb, buf, size,1)) {
    return 0;
  }

  //Force to send data right now!
  if(ERR_OK != tcp_output(tcp_client->pcb)) {
    return 0;
  }
  pbuf_ref(tcp_client->data.p);
      
      /* free pbuf: will free pbufs up to es->p (because es->p has a reference count > 0) */
      pbuf_free(tcp_client->data.p);
  //stm32_eth_scheduler();
      mem_free(tcp_client->data.p);

  return size;
}

/**
  * @brief  This function implements the tcp_sent LwIP callback (called when ACK
  *         is received from remote host for sent data)
  * @param  arg: pointer on argument passed to callback
  * @param  tcp_pcb: tcp connection control block
  * @param  len: length of data sent
  * @retval err_t: returned error code
  */
static err_t tcp_sent_callback(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
  struct tcp_struct *tcp_arg = (struct tcp_struct *)arg;

  LWIP_UNUSED_ARG(len);

  if((tcp_arg != NULL) && (tcp_arg->pcb == tpcb))
  {
    
    return ERR_OK;
  }

  return ERR_ARG;
}

/** Function prototype for tcp error callback functions. Called when the pcb
 * receives a RST or is unexpectedly closed for any other reason.
 *
 * @note The corresponding pcb is already freed when this callback is called!
 *
 * @param arg Additional argument to pass to the callback function (@see tcp_arg())
 * @param err Error code to indicate why the pcb has been closed
 *            ERR_ABRT: aborted through tcp_abort or by a TCP timer
 *            ERR_RST: the connection was reset by the remote host
 */
static void tcp_err_callback(void *arg, err_t err)
{
  struct tcp_struct *tcp_arg = (struct tcp_struct *)arg;

  if(tcp_arg != NULL) {
    if(ERR_OK != err) {
      tcp_arg->pcb = NULL;
      tcp_arg->state = TCP_CLOSING;
    }
  }
}

/**
 * ----------------------------------------------------------------------
 * Function given to help user to continue LwIP Initialization
 * Up to user to complete or change this function ...
 * Up to user to call this function in main.c in while (1) of main(void) 
 *-----------------------------------------------------------------------
 * Read a received packet from the Ethernet buffers 
 * Send it to the lwIP stack for handling
 * Handle timeouts if NO_SYS_NO_TIMERS not set and without RTOS
 */
void MX_LWIP_Process(void)
{
  ethernetif_input(&gnetif);
       
  /* Handle timeouts */
  #if !NO_SYS_NO_TIMERS && NO_SYS
    sys_check_timeouts();
  #endif
    
}
/* USER CODE END 4 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
