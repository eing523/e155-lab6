/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : main.c
Purpose : Generic application start

*/

#include <stdio.h>

/*********************************************************************
*
*       main()
*
*  Function description
*   Application entry point.
*/
int main(void) {
    int test_lsb = 0x00;
    int test_msb = 0xC9;
  
    if (test_msb >= 128) {
      test_msb -= 256;
    }

    float temperature = test_msb + (test_lsb / 256.0f); // from DS datasheet; dividing lsb by 256 since we have 2^8 positions and lsb is a fraction

    return temperature;
}

/*************************** End of file ****************************/
