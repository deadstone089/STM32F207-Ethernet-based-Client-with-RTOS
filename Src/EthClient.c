#include "EthClient.h"
#include "main.h"


#define TIME_CHECK_ETH_LINK_STATE 500U
static uint32_t gEhtLinkTickStart = 0;

//extern struct netif gnetif;
struct tcp_struct *tcp_client;
struct netconn *xNetConn=NULL;

ip_addr_t DestIPaddr;

int client_connect(){
 int rc2;
 
  if ( xNetConn == NULL ) { 
 
  /* No memory for new connection? */
    return 0;
  }

IP4_ADDR( &DestIPaddr, (uint8_t)3, (uint8_t)15, (uint8_t)19, (uint8_t)182 );

rc2 = netconn_connect ( xNetConn, &DestIPaddr, 10000 ); 
 
if (rc2 != ERR_OK )
{
  netconn_delete ( xNetConn );
    return 0;
}
return 1;
}

//int client_connect(void){
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
//    stop();
//    HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_7);
//      HAL_Delay(500);
//    return 0;
//  }
//  
///  
//  while(tcp_client->state == TCP_NONE) {
//    //stm32_eth_scheduler();
//		osDelay(2000);
//    if((tcp_client->state == TCP_CLOSING) /*|| (TIM6->CNT >= 9999)*/) {
//      stop();
//      return 0;
//    }
//  }
//
//  return 1;
//  
//}

void stm32_eth_scheduler(void) {
  /* Read a received packet from the Ethernet buffers and send it
  to the lwIP for handling */
#ifndef ETH_INPUT_USE_IT
  ethernetif_input(&gnetif);
#endif /* ETH_INPUT_USE_IT */

  /* Check ethernet link status */
  if((HAL_GetTick() - gEhtLinkTickStart) >= TIME_CHECK_ETH_LINK_STATE) {
    ethernetif_set_link(&gnetif);
    gEhtLinkTickStart = HAL_GetTick();
  }

  /* Handle LwIP timeouts */
  //sys_check_timeouts();

#if LWIP_DHCP
  stm32_DHCP_Periodic_Handle(&gnetif);
#endif /* LWIP_DHCP */
}

void stop() {
  if(tcp_client == NULL) {
    return;
  }
  
  // close tcp connection if not closed yet
  if(status() != TCP_CLOSING) {
    tcp_connection_close(tcp_client->pcb, tcp_client);
  }
}

uint8_t client_connected() {
  uint8_t s = status();
  return ((client_available() && (s == TCP_CLOSING)) ||
          (s == TCP_CONNECTED) || (s == TCP_ACCEPTED));
}

uint8_t status() {
  if(tcp_client == NULL) {
    return TCP_NONE;
  }
  return tcp_client->state;
}

int client_read() {
  uint8_t b;
  if((tcp_client != NULL) && (tcp_client->data.p != NULL)) {
    stm32_get_data(&(tcp_client->data), &b, 1);
    return b;
  }
  // No data available
  return -1;
}

void readString(char printstr[100],int len){
  
  int i=0;
  while(1){
      if (client_available()){
        printstr[i]=client_read();
        i++;
        }
      if (i>=len){
        printstr[i]='\0';
        break;
      }
      }
}

uint16_t stm32_get_data(struct pbuf_data *data, uint8_t *buffer, size_t size)
{
  uint16_t i;
  uint16_t offset;
  uint16_t nb;
  struct pbuf * ptr;

  if((data->p == NULL) || (buffer == NULL) || (size == 0) ||
      (data->available == 0) || (data->available > data->p->tot_len)) {
    return 0;
  }

  nb = 0;

  while((nb < size) && (data->p != NULL) && (data->available > 0)) {
    ptr = data->p;
    offset = ptr->tot_len - data->available;

    /* Get data from p */
    for(i = 0; (nb < size) && ((offset + i) < ptr->len) && (data->available > 0); i++) {
      buffer[nb] = pbuf_get_at(ptr, offset + i);
      nb++;
      data->available--;
    }

    if(nb < size) {
      /* continue with next pbuf in chain (if any) */
      data->p = ptr->next;

      if(data->p != NULL) {
        /* increment reference count for p */
        pbuf_ref(data->p);
      }

      /* chop first pbuf from chain */
      ptr = stm32_free_data(ptr);
    }
  }

  if(data->available == 0) {
    data->p = stm32_free_data(data->p);
  }

  return nb;
}

struct pbuf *stm32_free_data(struct pbuf *p)
{
  uint16_t n;

  if(p != NULL) {
    do {
      n = pbuf_free(p);
    } while(n == 0);
  }

  return NULL;
}

int client_available() {
  stm32_eth_scheduler();
  if(tcp_client != NULL) {
    return tcp_client->data.available;
  }
  else{
    return 0;
  }
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

  if(ERR_OK != tcp_write(tcp_client->pcb, buf, size, TCP_WRITE_FLAG_COPY)) {
    return 0;
  }

  //Force to send data right now!
  if(ERR_OK != tcp_output(tcp_client->pcb)) {
    return 0;
  }

  stm32_eth_scheduler();

  return size;
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
