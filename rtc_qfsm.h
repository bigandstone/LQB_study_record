#ifndef __rtc_qfsm_h__
#define __rtc_qfsm_h__

#include "main.h"
#include <string.h>
#include <stdio.h>

#include "rtc.h"
#include "lcd.h"
#include "usart.h"

#define CMD_LEN 4
#define GET_LEN(x) (strlen((char*)(x)))

typedef enum{
    ONE_SEC_EVT,
    RX_EVT
}EVENT;

typedef enum{
    ONLY_ONE
}STATE;

typedef struct{
    STATE state;
    
    uint8_t cmd_buffer[CMD_LEN];
    uint8_t lcd_buffer[20];
    RTC_TimeTypeDef time;
    RTC_DateTypeDef date;
}rtc_qfsm;


extern rtc_qfsm rtc1;

void rtc_qfsm_init(rtc_qfsm* me);
void rtc_qfsm_dispatch(rtc_qfsm* me, EVENT event);

#endif
