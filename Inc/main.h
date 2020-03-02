/**
  ******************************************************************************
  * @file    FatFs/FatFs_RAMDisk/Inc/main.h 
  * @author  EV Team
  * @version V1.0
  * @date    June 2016
  * @brief   Header for main.c module
  ******************************************************************************
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 TCoE- IITM
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H


  /* EVAL includes component */
  #include "../Drivers/BSP/STM32F2xx_Nucleo_144/STM32F2xx_Nucleo_144.h"

  /* FatFs includes component 
  #include "../Middlewares/Third_Party/FatFs/src/ff_gen_drv.h"
  #include "../Middlewares/Third_Party/FatFs/src/drivers/sram_diskio.h"
*/
 /* GBT includes */
  #include "gbtMessages.h" 
#include "chademoevse.h"

  /* CLI includes */
  #include "FreeRTOS_CLI.h"
void BC_DC_Messages(char *input);

  
    /* Uncomment to enable the adaquate RTC Clock Source */
  #define RTC_CLOCK_SOURCE_LSE
#include "httpserver-netconn.h"


#ifdef RTC_CLOCK_SOURCE_LSI
  #define RTC_ASYNCH_PREDIV  0x7F
  #define RTC_SYNCH_PREDIV   0x0130
#endif

#ifdef RTC_CLOCK_SOURCE_LSE
  #define RTC_ASYNCH_PREDIV  0x7F
  #define RTC_SYNCH_PREDIV   0x00FF
#endif
  
void Error_Handler(void);

#define ENTER_KEY 13
#define UART_TX_TIMEOUT (uint32_t) 10
#define UART_RX_TIMEOUT (uint32_t) 1000

//#define CLI_ENABLED

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT TCoE - IITM  **************************/
