// STM32L432KC_SPI.c
// Emily Ing
// eing@g.hmc.edu
// 10/17/2026
// This file is the .c file that contains the SPI implementation

#include "stm32l432xx.h"
#include "STM32L432KC_GPIO.h"
#include "STM32L432KC_SPI.h"
#include "main.h"

void initSPI(int br, int cpol, int cpha){
    
    // configure gpio for mosi, miso, sck
    pinMode(MISO_PIN, GPIO_ALT);
    pinMode(MOSI_PIN, GPIO_ALT);
    pinMode(SCK_PIN, GPIO_ALT);
    pinMode(CE_PIN, GPIO_OUTPUT); // clip select aka chip enable


    // from datasheet we get the pin name that corresponds to miso, sck, and mosi (p. 56). we look at the device header for the functions of the others
    GPIOB->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL3, 0b101); // configure PB3 to SCK
    GPIOB->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL4, 0b101); // configure PB4 to MISO
    GPIOB->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL5, 0b101); // configure PB5 to MOSI

    // clear bits
    SPI1->CR1 = 0;

    // writing to the cr1 register
    // step a
    // enable serial clock baud rate using BR[2:0] bits
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_BR, br);

    // step b
    // enable clock phase of cpol
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_CPOL, cpol);

    // enable clock polarity of cpha
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_CPHA, cpha);

    // step c
    // enable half-duplex (transmit and receive)
    //SPI1->CR1 |= _VAL2FLD(USART_CR3_HDSEL, 1);

    // step g
    // enable master configuration
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_MSTR, 1);

    // writing to the cr2 register
    // step b
    // configure data size to 8 bits
    SPI1->CR2 |= _VAL2FLD(SPI_CR2_DS, 0b0111); 

    // configure SSOE
    SPI1->CR2 |= _VAL2FLD(SPI_CR2_SSOE, 1);

    // step e
    // configure frxth bit. configuring to 1 means RXNE event generated if FIFO level is >= 1/4 (8 bit)
    SPI1->CR2 |= _VAL2FLD(SPI_CR2_FRXTH, 1);

    // enable spi
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_SPE, 1);

}

char spiSendReceive(char send){
    // txe flag is set when txfifo has enough space to store data to send. therefore, while SPI_SR_TXE isn't set, we are still waiting for space.
    while(!(SPI1->SR & SPI_SR_TXE)); // (p.1324 of RM talks about SPI status flags TXE, RXNE, BSY)
    // writing send to the data register
    *((volatile char *) (&SPI1->DR)) = send;

    // waiting for FIFO to be >= 1/4 (8-bit). when this happens, something is received.
    while(!(SPI1->SR & SPI_SR_RXNE));

    // char is sent out
    char received = *((volatile char *) (&SPI1->DR));
    return received;
    // busy flag clear
    //SPI1->SR &= _VAL2FLD(SPI_CR1_SPE, 1);
}