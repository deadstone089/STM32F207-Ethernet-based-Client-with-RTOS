#ifndef EthClient_h
#define EthClient_h

#include "lwip.h"
#include "lwip/ip_addr.h"
#include "lwip/dhcp.h"
#include "ethernetif.h"
#include "lwip/tcp.h"
#include "lwip/ip4_addr.h"
#include "lwip/api.h"


#include<stdint.h>
#include<String.h>
#include<stdio.h>

extern struct netif gnetif;
extern struct tcp_struct *tcp_client;
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

extern struct tcp_struct *tcp_client;

err_t tcp_connected_callback(void *arg, struct tcp_pcb *tpcb, err_t err);
static err_t tcp_recv_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static err_t tcp_sent_callback(void *arg, struct tcp_pcb *tpcb, u16_t len);
static void tcp_err_callback(void *arg, err_t err);
int client_connect(void);
void stm32_eth_scheduler(void);
void stop();
uint8_t client_connected();
void stm32_eth_scheduler(void);
uint8_t status();
int client_read();
uint16_t stm32_get_data(struct pbuf_data *, uint8_t *, size_t);
struct pbuf *stm32_free_data(struct pbuf *);
int client_available(void);
size_t client_write(const uint8_t *, size_t);
void tcp_connection_close(struct tcp_pcb *, struct tcp_struct *);
void readString(char printstr[100],int len);

#endif