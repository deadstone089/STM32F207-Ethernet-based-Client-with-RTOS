#define __CMS_CALLBACK_H__
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include <string.h>
#include "httpserver-netconn.h"
#include "gbtMessages.h"
#include "ocpp_cms.h"


/* Private function prototypes -----------------------------------------------*/
void CmsCallback(void);
static err_t callback_Connected(void *arg, struct tcp_pcb *tpcb, err_t error);
static err_t CmsServerAccept(void *arg, struct tcp_pcb *newpcb, err_t err);
void ProcessRcvdRspForCMS(void *ctx,char *receivedBuf,int cmsOperation);
void CmsRequestHeader(char *httpHeader);
static err_t ClientRecv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err);

void ftp(void);
void ProcessRcvdRspForCP(char *receivedData);
#define MAX_LENGTH 125
#define SIX_BYTE   6 
#define EIGHT_BYTE 8
#define COUNT_ZERO 0
#define COUNT_ONE  1
#define COUNT_TWO  2

#define REQUEST_STR_LEN     1000

#define HANDSHAKEHEADER                     "GET "HANDSHAKEPATH" HTTP/1.1\r\nHost:10.9.39.5:8081\r\nConnection:Upgrade\r\nUpgrade:websocket\r\nOrigin:http://10.9.39.16:8080\r\nSec-WebSocket-Version:13\r\nSec-WebSocket-Key:9iPJw3of+czlqg1MbLLnrA==\r\n\r\n"
//#define HANDSHAKEHEADER                     "GET "HANDSHAKEPATH" HTTP/1.1\r\nHost:10.9.39.5:8080\r\nConnection:Upgrade\r\nUpgrade:websocket\r\nOrigin:http://10.9.39.31:8080\r\nSec-WebSocket-Version:13\r\nSec-WebSocket-Key:9iPJw3of+czlqg1MbLLnrA==\r\n\r\n"
//#define HANDSHAKEHEADER                     "GET "HANDSHAKEPATH" HTTP/1.1\r\nHost:10.21.18.206:8080\r\nConnection:Upgrade\r\nUpgrade:websocket\r\nOrigin:http://10.20.1.187:8080\r\nSec-WebSocket-Version:13\r\nSec-WebSocket-Key:9iPJw3of+czlqg1MbLLnrA==\r\n\r\n"
#define HANDSHAKEPATH                       "/cims/websocketservices/ocpp1.6/14_1"
#define GETDIAGNOSTICSFTPCWD1               "CWD resin-pro-4.0.41/webapps/cims/WebClient/ftp/updatefirmware/ST/Nucleo-f207zg\r\n"
#define GETDIAGNOSTICSFTPCWD                "CWD resin-pro-4.0.41/webapps/cims/WebClient/ftp/diagnostics/CP3\r\n"
#define GETDIAGNOSTICSFTPPWD                "PWD \r\n"
#define GETDIAGNOSTICSFTPPASV               "PASV \r\n"
#define GETDIAGNOSTICSFTP                   "TYPE A\r\n"
#define CONSTANT                            "256"
#define GETDIAGNOSTICSUPLOAD                "STOR Diag_sample.txt\r\n"
#define GETDIAGNOSTICSFTPQUIT               "QUIT \r\n"
#define GETDIAGNOSTICSDOWNLOAD              "RETR updateFirmwareCP2.txt\r\n"
#define SERVER_PORT_A                         "21"
#define SERVER_PORT_B                         "18"