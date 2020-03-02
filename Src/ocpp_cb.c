#include <string.h>
#include <ocpp_cb.h>

int client_connect(){
  ip_addr_t DestIPaddr;
    char str[]="h,AA00\r\n";
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

void ethernetif_notify_conn_changed(struct netif *netif)
{
  struct ip_addr ipaddr;
  struct ip_addr netmask;
  struct ip_addr gw;

  if(netif_is_link_up(netif))
  {
    IP4_ADDR(&ipaddr, ipaddr0, ipaddr1, ipaddr2, ipaddr3);
    IP4_ADDR(&netmask, netmask0, netmask1 , netmask2, netmask3);
    IP4_ADDR(&gw, gateway0, gateway1, gateway2, gateway3);     
    netif_set_addr(netif, &ipaddr , &netmask, &gw);
    /* When the netif is fully configured this function must be called.*/
    netif_set_up(netif); 
    
    
    xNetConn = netconn_new ( NETCONN_TCP ); 
  
  netconn_bind ( xNetConn, &ipaddr, 0 );    
  }
//  else
//  {
//#ifdef USE_DHCP
//    /* Update DHCP state machine */
//    DHCP_state = DHCP_LINK_DOWN;
//#endif /* USE_DHCP */
//    
//    /*  When the netif link is down this function must be called.*/
//    netif_set_down(netif);
//    
//#ifdef USE_LCD
//    LCD_UsrLog ("The network cable is not connected \n");
//#else
//    BSP_LED_Off(LED1);
//    BSP_LED_On(LED2);
//#endif /* USE_LCD */    
//  }
}