#include "modbus.h"
#include "communication.h"
#include "anemometer.h"

/* Table of CRC values for high order byte */
const uint8_t auchCRCHi[256] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40
} ;

/* Table of CRC values for low order byte */
const uint8_t auchCRCLo[256] = {
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
0x40
} ;
uint16_t modbusErrorCount = 0;

uint16_t calculateCRC(uint8_t *buff,uint16_t length);
uint16_t getU16(uint8_t buffer[], uint16_t index);
uint32_t getU32(uint8_t buffer[], uint16_t index);

//**** Modbus Master ****

uint16_t  creatReadIR(uint8_t buffer[], uint8_t id, uint16_t reg, uint16_t count)
{
    buffer[0] = id;
    buffer[1] = 0x04;
    buffer[2] = (uint8_t)(reg >> 8);
    buffer[3] = (uint8_t)reg;
    buffer[4] = (uint8_t)(count >> 8);
    buffer[5] = (uint8_t)count;
    uint16_t crc = calculateCRC(buffer, 6);
    buffer[6] = (uint8_t)(crc >> 8);
    buffer[7] = (uint8_t)crc;
    return 8;
}

uint16_t  creatReadHR(uint8_t buffer[], uint8_t id, uint16_t reg, uint16_t count)
{
    buffer[0] = id;
    buffer[1] = 0x03;
    buffer[2] = (uint8_t)(reg >> 8);
    buffer[3] = (uint8_t)reg;
    buffer[4] = (uint8_t)(count >> 8);
    buffer[5] = (uint8_t)count;
    uint16_t crc = calculateCRC(buffer, 6);
    buffer[6] = (uint8_t)(crc >> 8);
    buffer[7] = (uint8_t)crc;
    return 8;
}

uint16_t crc;

uint16_t parseFrame(uint8_t buffer[], uint16_t length)
{
    //uint16_t crc = calculateCRC(buffer,length - 2);
    crc = calculateCRC(buffer,length - 2);
    uint16_t crcRec = SwapBytes(getU16(buffer, length - 2));
    if(crc == crcRec)
    {
        //Process data and populate struct
        switch (buffer[1])
        {
            case 0x03://Read HR
                break;
            case 0x04://Read IR
                anemometerData.windSpeed = getU32(buffer , IR_MEAN_WIND_SPEED);
                anemometerData.windDirection = getU32(buffer , IR_MEAN_WIND_DIRECTION);
                anemometerData.precipitation = getU32(buffer , IR_PRECIPITATION);
                anemometerData.compass = getU32(buffer , IR_COMPASS);
                break;
            case 0x06://Single Write
                break;
            case 0x10://Block write
                break;
            default:
                break;
        }
    }
    else
        modbusErrorCount++;
    return 1;
}

//**** Modbus General ****
uint16_t calculateCRC(uint8_t *buff,uint16_t length) //Calculates 16 bit CRC
{
    uint16_t temp = 0xFFFF;
    uint8_t uchCRCHi = 0xFF ; 			/* high byte of CRC initialized */
    uint8_t uchCRCLo = 0xFF ; 			/* low byte of CRC initialized */
    uint8_t uIndex;/* will index into CRC lookup table */
    uint8_t usDataLen;
    uint8_t *puchMsg;
    usDataLen = length;
    puchMsg = buff;
    while (usDataLen) 						/* pass through message buffer */
    {
        uIndex = uchCRCHi ^ *puchMsg++ ; 		/* calculate the CRC */
        uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ;
        uchCRCLo = auchCRCLo[uIndex] ;
        usDataLen--;
    }
    temp = uchCRCLo;
    temp = temp << 8;
    temp = temp | uchCRCHi;
    return SwapBytes(temp);
    //return temp;// Use if CRC bytes dont need swapping
}

uint16_t getU16(uint8_t buffer[], uint16_t index)
{
    uint16_t retVal = buffer[index + 1];
    retVal = (retVal << 8) & 0xFF00;
    retVal = retVal | buffer[index];
    return retVal;
}

uint32_t getU32(uint8_t buffer[], uint16_t index)
{
    uint32_t retVal = getU16(buffer, index);
    retVal = (retVal << 16) & 0xFFFF0000;
    retVal |= getU16(buffer, index + 2);
    return retVal;
}

uint16_t getExpectedByteCount(uint8_t buffer[])
{
    uint16_t retVal = 4;
    switch (buffer[1])
    {
        case 0x03:
        case 0x04:
        case 0x17:
            retVal += buffer[2] + 1;
            break;
        case 0x06:
        case 0x10:
            retVal += 4;
            break;
        default:
            retVal = 0;
            break;
    }
    return retVal;
}