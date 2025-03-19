#include "key.h"

void key_init(KEY* me, GPIO_TypeDef* port, uint16_t pin){
    me->state = IDEL;
    
    me->port = port;
    me->pin = pin;
    HAL_TIM_Base_Start_IT(&htim3);
}

void key_dispatch(KEY* me, EVENT e){
    switch (me->state){
        case IDEL:
            if(e == PRESS){
                me->state = FIRST_PRESS;
                me->time = 0;
            }
            break;
        case FIRST_PRESS:
            if(e == TICK){
                me->time++;
                if(me->time == LONG_PRESS_TIME){
                    me->state = IDEL;
                    key_long_press_callback(me);
                }
            }
            if(e == RELEASE){
                me->state = WAIT_DOUBLE;
                me->time = 0;
            }
            break;
        case WAIT_DOUBLE:
            if(e == PRESS){
                me->state = IDEL;
                key_double_press_callback(me);
            }
            if(e == TICK){
                me->time++;
                if(me->time == DOUBLE_PRESS_TIME){
                    me->state = IDEL;
                    key_short_press_callback(me);
                }
            }
            break;
    }
}
__weak void key_short_press_callback(KEY* me){
    
}
__weak void key_long_press_callback(KEY* me){
    
}
__weak void key_double_press_callback(KEY* me){
    
}