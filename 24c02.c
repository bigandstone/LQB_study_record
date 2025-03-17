#include "24c02.h"

void eeprom_write_byte(uint8_t address, uint8_t data){
    I2CStart();
    
    I2CSendByte(EPROM_ADDRESS_W);
    I2CWaitAck();
    
    I2CSendByte(address);
    I2CWaitAck();
    
    I2CSendByte(data);
    I2CWaitAck();
    
    I2CStop();
    
    HAL_Delay(10);
}

uint8_t eeprom_read_byte(uint8_t address){
    I2CStart();
    
    I2CSendByte(EPROM_ADDRESS_W);
    I2CWaitAck();
    
    I2CSendByte(address);
    I2CWaitAck();
    
    I2CStart();
    
    I2CSendByte(EPROM_ADDRESS_R);
    I2CWaitAck();
    
    uint8_t data = I2CReceiveByte();
    I2CSendNotAck();
    
    I2CStop();
    return data;
}

void eeprom_write(uint8_t address, uint16_t num, uint8_t* pdata){
    uint8_t begin_col = address%8;//col in arrage of 0~7
    uint8_t begin_line = address/8;//line in arrage of 0~31
    uint8_t end_col = (address+num-1)%8;//col in arrage of 0~7
    uint8_t end_line = (address+num-1)/8;//line in arrage of 0~31
    uint8_t index = 0;
    
    while(1){
        if(end_line !=begin_line){
            eeprom_write_page(begin_line*8+begin_col,8-begin_col,pdata,index);
            begin_col = 0;
            begin_line +=1;
            index += 8-begin_col;
        }else{
            eeprom_write_page(begin_line*8+begin_col,end_col-begin_col+1,pdata,index);
            break;
        }
    }
    
    
}
void eeprom_read(uint8_t address, uint16_t num, uint8_t* pdata){
    if(num == 0){ return; }
    
    I2CStart();
    
    I2CSendByte(EPROM_ADDRESS_W);
    I2CWaitAck();
    
    I2CSendByte(address);
    I2CWaitAck();
    
    I2CStart();
    
    I2CSendByte(EPROM_ADDRESS_R);
    I2CWaitAck();
    
    for (uint16_t i = 0; i < num; i++) {
        pdata[i] = I2CReceiveByte();
        if (i < num - 1) {
            I2CSendAck();    // 发送ACK继续读取
        } else {
            I2CSendNotAck(); // 末字节发送NACK
        }
    }
    
    I2CStop();
}

void eeprom_write_page(uint8_t address, uint16_t num, uint8_t* pdata,uint8_t begin_index){
    //--------------------------------------------------------
    I2CStart();
    
    I2CSendByte(EPROM_ADDRESS_W);
    I2CWaitAck();
    
    I2CSendByte(address);
    I2CWaitAck();
    
    for(uint16_t i = 0; i < num ; i++){
        I2CSendByte(pdata[i+begin_index]);
        I2CWaitAck();
    }
    
    I2CStop();
    
    HAL_Delay(10);
    //--------------------------------------------------------
}