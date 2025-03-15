#include "rtc_qfsm.h"

rtc_qfsm rtc1;

void rtc_qfsm_init(rtc_qfsm* me){
    LCD_Init();
    LCD_SetBackColor(Blue2);
    LCD_SetTextColor(Black);
    LCD_Clear(Blue2);
    
    me->state = ONLY_ONE;
    LCD_DisplayStringLine(Line2,(u8*)"    Current Time    ");
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, me->cmd_buffer, CMD_LEN);
}

void rtc_qfsm_dispatch(rtc_qfsm* me, EVENT event){
    switch (me->state)
    {
        case ONLY_ONE:
            switch (event)
            {
                case ONE_SEC_EVT:
                    HAL_RTC_GetTime(&hrtc, &me->time,RTC_FORMAT_BIN);
                    HAL_RTC_GetDate(&hrtc, &me->date, RTC_FORMAT_BIN);
                    sprintf((char*)me->lcd_buffer,"      %02d:%02d:%02d      ",me->time.Hours,me->time.Minutes,me->time.Seconds);
                    LCD_DisplayStringLine(Line4,(u8*)me->lcd_buffer);
                    HAL_UART_Transmit_IT(&huart1, me->lcd_buffer, GET_LEN(me->lcd_buffer));
                    break;
                case RX_EVT:
                    if(me->cmd_buffer[0] != '#'){
                        HAL_UART_Transmit_IT(&huart1,(uint8_t*)"illegal cmd",11);
                        return;
                    }
                    uint8_t val = (me->cmd_buffer[2] - 0x30)*10 + me->cmd_buffer[3] - 0x30;
                    switch (me->cmd_buffer[1])
                    {
                        case 'H':me->time.Hours = val;
                            break;
                        case 'M':me->time.Minutes = val;
                            break;
                        case 'S':me->time.Seconds = val;
                            break;
                    }
                    HAL_RTC_SetTime(&hrtc, &me->time, RTC_FORMAT_BIN);
                    break;
            }
            break;
    }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
    rtc_qfsm_dispatch(&rtc1,RX_EVT);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
    HAL_UART_Transmit(&huart1,(uint8_t*)"\r\n",2,HAL_MAX_DELAY);
}

void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc){
    rtc_qfsm_dispatch(&rtc1, ONE_SEC_EVT);
}