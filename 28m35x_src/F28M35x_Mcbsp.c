//###########################################################################
// TITLE:   F28M35x Device McBSP Initialization & Support Functions.
//###########################################################################
// $TI Release: F28M35x Support Library v220 $
// $Release Date: Tue Sep 26 15:35:11 CDT 2017 $
// $Copyright: Copyright (C) 2011-2017 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

// Device and example header files
#include "F28M35x_Device.h"
#include "F28M35x_Examples.h"

//---------------------------------------------------------------------------
// MCBSP_INIT_DELAY determines the amount of CPU cycles in 2 sample rate
// generator (SRG) cycles required for the McBSP initialization routine.
// MCBSP_CLKG_DELAY determines the amount of CPU cycles in 2 clock
// generator (CLKG) cycles required for the McBSP initialization routine.
//---------------------------------------------------------------------------

#define CPU_FRQ_150MHZ    1

//#define CPU_FRQ_100MHZ        1
//#define CPU_FRQ_60MHZ         1

// For 150 MHz SYSCLKOUT(default)
// SRG input is LSPCLK (SYSCLKOUT/4) for examples
#if CPU_FRQ_150MHZ
    #define CPU_SPD           150E6
    #define MCBSP_SRG_FREQ    CPU_SPD/4
#endif

// For 100 MHz SYSCLKOUT
// SRG input is LSPCLK (SYSCLKOUT/4) for examples
#if CPU_FRQ_100MHZ
    #define CPU_SPD           100E6
    #define MCBSP_SRG_FREQ    CPU_SPD/4
#endif

// For 60 MHz SYSCLKOUT
// SRG input is LSPCLK (SYSCLKOUT/4) for examples
#if CPU_FRQ_60MHZ
    #define CPU_SPD           60E6
    #define MCBSP_SRG_FREQ    CPU_SPD/4
#endif

// # of CPU cycles in 2 SRG cycles
// # of CPU cycles in 2 CLKG cycles
#define CLKGDV_VAL            15
#define MCBSP_INIT_DELAY      2*(CPU_SPD/MCBSP_SRG_FREQ)
#define MCBSP_CLKG_DELAY      2*(CPU_SPD/(MCBSP_SRG_FREQ/(1+CLKGDV_VAL)))

//*****************************************************************************
// Delay function used for CLKG initialization
// Delay function used for SRG initialization
//*****************************************************************************
void delay_loop(void);
void clkg_delay_loop(void);

void InitMcbsp(void)
{
    InitMcbspa();
}

//*****************************************************************************
// McBSP initialization routine for examples
//*****************************************************************************
void InitMcbspa(void)
{

    // Reset the McBSP
    // Disable all interrupts
    // Frame sync generator reset
    // Sample rate generator reset
    // Transmitter reset
    // Receiver reset
    McbspaRegs.SPCR2.bit.FRST = 0;
    McbspaRegs.SPCR2.bit.GRST = 0;
    McbspaRegs.SPCR2.bit.XRST = 0;
    McbspaRegs.SPCR1.bit.RRST = 0;

    // Enable loop back mode
    // This does not require external hardware
    McbspaRegs.SPCR2.all = 0x0000;
    McbspaRegs.SPCR1.all = 0x8000;

    // RX data delay is 1 bit
    // TX data delay is 1 bit
    McbspaRegs.RCR2.bit.RDATDLY = 1;
    McbspaRegs.XCR2.bit.XDATDLY = 1;

    // No clock sync for CLKG
    // Frame-synchronization period
    McbspaRegs.SRGR2.bit.GSYNC = 0;
    McbspaRegs.SRGR2.bit.FPER = 320;

    // Frame-synchronization pulses from
    // the sample rate generator
    McbspaRegs.SRGR2.bit.FSGM = 1;

    // Sample rate generator input clock is LSPCLK
    McbspaRegs.SRGR2.bit.CLKSM = 1;
    McbspaRegs.PCR.bit.SCLKME = 0;

    // Divide-down value for CLKG
    // Frame-synchronization pulse width
    McbspaRegs.SRGR1.bit.CLKGDV = CLKGDV_VAL;
    McbspaRegs.SRGR1.bit.FWID = 1;

    // CLKX is driven by the sample rate generator
    // Transmit frame synchronization generated by internal
    // sample rate generator
    McbspaRegs.PCR.bit.CLKXM = 1;
    McbspaRegs.PCR.bit.FSXM = 1;

    // Wait at least two SRG clock cycles
    delay_loop();

    // Enable Sample rate generator and
    // wait at least 2 CLKG clock cycles
    McbspaRegs.SPCR2.bit.GRST = 1;
    clkg_delay_loop();

    // Release from reset
    // RX, TX and frame sync generator
    McbspaRegs.SPCR2.bit.XRST = 1;
    McbspaRegs.SPCR1.bit.RRST = 1;
    McbspaRegs.SPCR2.bit.FRST = 1;
}

//*****************************************************************************
// Enable TX and RX interrupts
//*****************************************************************************
void InitMcbspaInt(void)
{
    // Reset TX and RX
    // Enable interrupts for TX and RX
    // Release TX and RX
    McbspaRegs.SPCR2.bit.XRST = 0;
    McbspaRegs.SPCR1.bit.RRST = 0;
    McbspaRegs.MFFINT.bit.XINT = 1;
    McbspaRegs.MFFINT.bit.RINT = 1;
    McbspaRegs.SPCR2.bit.XRST = 1;
    McbspaRegs.SPCR1.bit.RRST = 1;
}

//*****************************************************************************
// McBSP uses an 8-bit word for both TX and RX
//*****************************************************************************
void InitMcbspa8bit(void)
{
    McbspaRegs.RCR1.bit.RWDLEN1 = 0;
    McbspaRegs.XCR1.bit.XWDLEN1 = 0;
}

//*****************************************************************************
// McBSP uses an 12-bit word for both TX and RX
//*****************************************************************************
void InitMcbspa12bit(void)
{
    McbspaRegs.RCR1.bit.RWDLEN1 = 1;
    McbspaRegs.XCR1.bit.XWDLEN1 = 1;
}

//*****************************************************************************
// McBSP uses an 16-bit word for both TX and RX
//*****************************************************************************
void InitMcbspa16bit(void)
{
    McbspaRegs.RCR1.bit.RWDLEN1 = 2;
    McbspaRegs.XCR1.bit.XWDLEN1 = 2;
}

//*****************************************************************************
// McBSP uses an 20-bit word for both TX and RX
//*****************************************************************************
void InitMcbspa20bit(void)
{
    McbspaRegs.RCR1.bit.RWDLEN1 = 3;
    McbspaRegs.XCR1.bit.XWDLEN1 = 3;
}

//*****************************************************************************
// McBSP uses an 24-bit word for both TX and RX
//*****************************************************************************
void InitMcbspa24bit(void)
{
    McbspaRegs.RCR1.bit.RWDLEN1 = 4;
    McbspaRegs.XCR1.bit.XWDLEN1 = 4;
}

//*****************************************************************************
// McBSP uses an 32-bit word for both TX and RX
//*****************************************************************************
void InitMcbspa32bit(void)
{
    McbspaRegs.RCR1.bit.RWDLEN1 = 5;
    McbspaRegs.XCR1.bit.XWDLEN1 = 5;
}

void InitMcbspGpio(void)
{
    InitMcbspaGpio();
}

//*****************************************************************************
// Assign pins to the McBSP peripheral
// Note:
//      This function assumes that the pin was assigned to the
//      C28x MUX by the M3.
//      Pull-up enable/disable is controlled only by the M3
//*****************************************************************************
void InitMcbspaGpio(void)
{
    EALLOW;

    //*************************************************************************
    // This specifies which of the possible GPIO pins will be
    // McBSP functional pins. Comment out unwanted connections.
    // Set qualification for selected input pins to asynchronous only
    // This will select asynchronous (no qualification) for the selected pins.
    //*************************************************************************
    // Select one of the following for MDXA
    // GPIO20
    GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 2;

    // MDRA
    // Qualification is asynchronous
    GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 2;
    GpioCtrlRegs.GPAQSEL2.bit.GPIO21 = 3;

    // MCLKXA
    // Qualification is asynchronous
    GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 2;
    GpioCtrlRegs.GPAQSEL2.bit.GPIO22 = 3;

    // MCLKRA
    // Select one of the following
    // GPIO7 with asynchronous qualification
    // GPIO58 with asynchronous qualification
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 2;
    GpioCtrlRegs.GPAQSEL1.bit.GPIO7 = 3;

    //GpioCtrlRegs.GPBMUX2.bit.GPIO58 = 1;
    //GpioCtrlRegs.GPBQSEL2.bit.GPIO58 = 3;
    // MFSXA
    // Qualification is asynchronous
    GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 2;
    GpioCtrlRegs.GPAQSEL2.bit.GPIO23 = 3;

    // MFSRA
    // Select one of the following
    // GPIO5 with asynchronous qualification
    // GPIO59 with asynchronous qualification
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 2;
    GpioCtrlRegs.GPAQSEL1.bit.GPIO5 = 3;

    //GpioCtrlRegs.GPBMUX2.bit.GPIO59 = 1;
    //GpioCtrlRegs.GPBQSEL2.bit.GPIO59 = 3;

    EDIS;

    //*************************************************************************
    // Enable internal pull-up for the selected pins
    // Pull-up enable should be set in the
    // GPIO_O_PUR register in the M3 code.
    //*************************************************************************

}

//*****************************************************************************
// Delay function: at least 2 SRG cycles
// Required in McBSP initialization
//*****************************************************************************
void delay_loop(void)
{
    long i;
    for (i = 0; i < MCBSP_INIT_DELAY; i++) {}
}

//*****************************************************************************
// Delay function: at least 2 CLKG cycles
// Required in McBSP init
//*****************************************************************************
void clkg_delay_loop(void)
{
    long i;
    for (i = 0; i < MCBSP_CLKG_DELAY; i++) {}
}

// No more.




