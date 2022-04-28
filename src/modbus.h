/* 
 * File:   modbus.h
 * Author: RFord7
 *
 * Created on 26 April 2022, 12:50
 */

#ifndef MODBUS_H
#define	MODBUS_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <stdint.h>

#define SwapBytes(x) (((x) >> 8) | ((x) << 8)) 

    uint16_t  creatReadIR(uint8_t buffer[], uint8_t id, uint16_t reg, uint16_t count);
    uint16_t parseFrame(uint8_t buffer[], uint16_t length);
    uint16_t getExpectedByteCount(uint8_t buffer[]);


#ifdef	__cplusplus
}
#endif

#endif	/* MODBUS_H */

