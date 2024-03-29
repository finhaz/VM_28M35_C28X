//###########################################################################
// FILE:    F28M35x_I2C.c
// TITLE:    F28M35x SCI Initialization & Support Functions.
//###########################################################################
// $TI Release: F28M35x Support Library v220 $
// $Release Date: Tue Sep 26 15:35:11 CDT 2017 $
// $Copyright: Copyright (C) 2011-2017 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#include "F28M35x_Device.h"     // F28M35x Headerfile Include File
#include "F28M35x_Examples.h"   // F28M35x Examples Include File

//---------------------------------------------------------------------------
// InitI2C:
//---------------------------------------------------------------------------
// This function initializes the I2C to a known state.
void InitI2C(void)
{
    // Initialize I2C-A:

    //tbd...
}

//---------------------------------------------------------------------------
// Example: InitI2CGpio:
//---------------------------------------------------------------------------
// This function initializes GPIO pins to function as I2C pins
// Each GPIO pin can be configured as a GPIO pin or up to 3 different
// peripheral functional pins. By default all pins come up as GPIO
// inputs after reset.
// Caution:
// Only one GPIO pin should be enabled for SDAA operation.
// Only one GPIO pin shoudl be enabled for SCLA operation.
// Comment out other unwanted lines.

void InitI2CGpio()
{

    EALLOW;

/* Enable internal pull-up for the selected pins */

// Pull-ups can be enabled or disabled disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    //GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;    // Enable pull-up for GPIO32 (SDAA)
    //GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;       // Enable pull-up for GPIO33
    // (SCLA)

/* Set qualification for selected pins to asynch only */

// This will select asynch (no qualification) for the selected pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 3;  // Asynch input GPIO32 (SDAA)
    GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3;  // Asynch input GPIO33 (SCLA)

/* Configure I2C pins using GPIO regs*/

// This specifies which of the possible GPIO pins will be I2C functional pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 1;   // Configure GPIO32 for SDAA
                                           // operation
    GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 1;   // Configure GPIO33 for SCLA
                                           // operation

    EDIS;
}




