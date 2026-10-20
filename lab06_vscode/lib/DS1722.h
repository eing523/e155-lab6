// DS1722.h
// Emily Ing
// eing@g.hmc.edu
// 10/17/2026
// This file is the .h file for the DS implementation

#ifndef DS1722_H
#define DS1722_H

#include <stdint.h>

///////////////////////////////////////////////////////////////////////////////
// Function prototypes
///////////////////////////////////////////////////////////////////////////////

// Initializes the sensor
void initDS(void);

// Writes to the sensor
void writeDS(char res);

// Read the sensor
float readDS(void);

#endif