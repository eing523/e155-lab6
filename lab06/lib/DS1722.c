// DS1722.c
// Emily Ing
// eing@g.hmc.edu
// 10/17/2026
// This file is the .c file that contains the DS implementation

#include "DS1722.h"
#include "STM32L432KC_SPI.h"
#include "main.h"
#include "STM32L432KC_GPIO.h"

// initialize
void initDS(void){
    // enabling the CE pin on and off so that data can be sent/received
    digitalWrite(CE_PIN, PIO_HIGH);
    spiSendReceive(0x80); // from DS datasheet, register address
    digitalWrite(CE_PIN, PIO_LOW);

}

// write (changing sensor resolution)
void writeDS(char res){
    
    digitalWrite(CE_PIN, PIO_HIGH);
    spiSendReceive(0x80); // from DS datasheet, register address
    spiSendReceive(res); // changing resolution (from ds datasheet p.6 - leftmost 3 bits must be 1) - 0xE0 + res
    digitalWrite(CE_PIN, PIO_LOW);

}

// read 
float readDS(){
    // reading lsb address
    digitalWrite(CE_PIN, PIO_HIGH);
    spiSendReceive(0x01); // from DS datasheet
    float lsb = spiSendReceive(0x00); // need to send a dummy byte since SPI sends and receives at same time, so something must be outputted to get sensor's data back
    digitalWrite(CE_PIN, PIO_LOW);

    // reading msb address
    digitalWrite(CE_PIN, PIO_HIGH);
    spiSendReceive(0x02); // from DS datasheet
    float msb = spiSendReceive(0x00);
    digitalWrite(CE_PIN, PIO_LOW);

    return msb + (lsb / 256.0); // from DS datasheet; dividing lsb by 256 since we have 2^8 positions and lsb is a fraction

}