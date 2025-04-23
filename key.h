#ifndef __key_h__
#define __key_h__

#include "main.h"
#include <string.h>
#include <stdint.h>

#include "gpio.h"
#include "tim.h"

#define LONG_PRESS_TIME 500 //in thr long press time , unit:ms
#define DOUBLE_PRESS_TIME 400 // doubel press ok time , unit:ms 

typedef enum{
    PRESS,
    RELEASE,
    TICK,
}EVENT;

typedef enum{
    IDEL,
    FIRST_PRESS,
    WAIT_DOUBLE,
}STATE;

typedef struct{
    STATE state;
    
    GPIO_TypeDef* port;
    uint16_t pin;
    uint16_t time;
}KEY;

void key_init(KEY* me, GPIO_TypeDef* port, uint16_t pin);
void key_dispatch(KEY* me, EVENT e);

void key_short_press_callback(KEY* me);
void key_long_press_callback(KEY* me);
void key_double_press_callback(KEY* me);

#endif
