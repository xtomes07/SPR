/*
 * aic23.c
 *
 *  Created on: 3.10.2013
 *      Author: Student
 */

#include "hellocfg.h"

#include "aic23.h"

/* Compatability definitions */
#define NULL                 0

#define	AIC23_LEFTINVOL			  0
#define	AIC23_RIGHTINVOL		  1
#define	AIC23_LEFTHPVOL		  2
#define	AIC23_RIGHTHPVOL	  3
#define	AIC23_ANAPATH			  4
#define	AIC23_DIGPATH			  5
#define	AIC23_POWERDOWN	  6
#define	AIC23_DIGIF					  7
#define	AIC23_SAMPLERATE	  8
#define	AIC23_DIGACT				  9
#define	AIC23_RESET					15

/* Table of supported frequencies */
static Uint16 FreqTable[] =
{
    AIC23_FREQ_8KHZ,  0x06,  // 8000 Hz
    AIC23_FREQ_16KHZ, 0x2c,  // 16000 Hz
    AIC23_FREQ_24KHZ, 0x20,  // 24000 Hz
    AIC23_FREQ_32KHZ, 0x0c,  // 32000 Hz
    AIC23_FREQ_44KHZ, 0x11,  // 44100 Hz
    AIC23_FREQ_48KHZ, 0x00,  // 48000 Hz
    AIC23_FREQ_96KHZ, 0x0e,  // 96000 Hz
    0, 0                             // End of table
};

/* Internal codec state used to simulate read/write functionality */
static AIC23_ConfigTab CodecState = AIC23_DEFAULTCONFIG;

/* AIC23 McBSP handles */
MCBSP_Handle AIC23_ControlHnd;
MCBSP_Handle AIC23_DataHnd;

#define LENGTH	(1 << 4)

Int32 rcvBuffer[LENGTH];
int rcvRead = 0, rcvWrite = 0;

Int32 xmtBuffer[LENGTH];
int xmtRead = 0, xmtWrite = 0;

/* Previous global interrupt enable bit */
Uint32 gie;

/*
 *  ======== AIC23_RegGet ========
 *  Return value of codec register regnum
 */
static Uint16 AIC23_RegGet( Uint16 regnum)
{
    if (regnum < AIC23_NUMREGS)
        return CodecState.regs[regnum];
    else
        return 0;
}

/*
 *  ======== AIC23_RegSet ========
 *  Set codec register regnum to value regval
 */
static void AIC23_RegSet( Uint16 regnum, Uint16 regval)
{
    /* Mask off lower 9 bits */
    regval &= 0x1ff;

    /* Wait for XRDY signal before writing data to DXR */
    while (!MCBSP_xrdy(AIC23_ControlHnd));

    /* Write 16 bit data value to DXR */
    MCBSP_write(AIC23_ControlHnd, (regnum << 9) | regval);

    /* Save register value if regnum is in range */
    if (regnum < AIC23_NUMREGS)
        CodecState.regs[regnum] = regval;

#ifndef SIMULATOR
    /* Wait for XRDY, state machine will not update until next McBSP clock */
    while (MCBSP_xrdy(AIC23_ControlHnd));
#endif
}

/*
 *  ======== AIC23_Loopback ========
 *  Enable/disable codec Loopback mode
 */
void AIC23_Loopback( Int16 mode)
{
    int regval;

    /* Set bypass bit if mode is true */
    regval = (mode) ? 0x8 : 0x0;

    /* Write to codec register */
    AIC23_RegSet( AIC23_ANAPATH,
        (AIC23_RegGet( AIC23_ANAPATH) & 0xfff7 | regval));
}

/*
 *  ======== AIC23_Mute ========
 *  Enable/disable codec Mute mode
 */
void AIC23_Mute( Int16 mode)
{
    int regval;

    /* Enable Mute if mode is true */
    regval = (mode) ? 0x08 : 0x00;

    /* Write to codec registers (left and right) */
    AIC23_RegSet( AIC23_DIGPATH,
        (AIC23_RegGet( AIC23_DIGPATH) & 0xfff7) | regval);
}

/*
 *  ======== AIC23_OutGain ========
 *  Set the output gain on the codec
 */
void AIC23_OutGain( Uint16 OutGain)
{
    /* Write to codec registers (left and right) */
    AIC23_RegSet( AIC23_LEFTHPVOL,
        (AIC23_RegGet( AIC23_LEFTHPVOL) & 0xff80) | (OutGain & 0x7f));
    AIC23_RegSet( AIC23_RIGHTHPVOL,
        (AIC23_RegGet( AIC23_RIGHTHPVOL) & 0xff80) | (OutGain & 0x7f));
}

/*
 *  ======== AIC23_PowerDown ========
 *  Enable/disable powerdown modes for the DAC and ADC codec subsections
 */
void AIC23_PowerDown( Uint16 sect)
{
    /* Write to codec register */
    AIC23_RegSet( AIC23_POWERDOWN,
        (AIC23_RegGet( AIC23_POWERDOWN) & 0xff00) | (sect & 0xff));
}

/*
 *  ======== AIC23_SetFreq ========
 *  Set the codec sample rate frequency
 */
void AIC23_SetFreq( Uint32 freq)
{
    Uint16 regval, curr;

    /* Calculate codec clock control register setting, assume USB Mode (12MHz) */
    /* regval will contain CLKIN,SR3..SR0,BOSR */
    curr = 0;
    while(1)
    {
        /* Do nothing if frequency doesn't match */
        if (FreqTable[curr] == 0)
            return;

        /* Check for match */
        if (FreqTable[curr] == freq)
        {
            regval = FreqTable[curr + 1];
            break;
        }

        /* Set up for next pair */
        curr += 2;
    }

    /* Write to codec register */
    AIC23_RegSet( AIC23_SAMPLERATE,
        (AIC23_RegGet( AIC23_SAMPLERATE) & 0xff81) | ((regval & 0x3f) << 1));
}

void AIC23_rcvIsr(void)
{
	if( !MCBSP_rrdy( AIC23_DataHnd))
		return;

	if((( rcvWrite + 1) & (LENGTH - 1)) == rcvRead)
                rcvRead = (rcvRead + 1) & (LENGTH - 1);

	rcvBuffer[rcvWrite] = MCBSP_read( AIC23_DataHnd);
	rcvWrite = (rcvWrite + 1) & (LENGTH - 1);

	SEM_postBinary(&SEM_rcv);
}

/*
 *  ======== AIC23_read ========
 *  Read a 32-bit value from the codec
 */
Int16 AIC23_Read( Int32 *val)
{
    IRQ_globalDisable();
	while( rcvRead == rcvWrite){
	    IRQ_globalEnable();
		SEM_pendBinary(&SEM_rcv, 1);
		IRQ_globalDisable();
    }

	*val = rcvBuffer[rcvRead];
	rcvRead = (rcvRead + 1) & (LENGTH - 1);

	IRQ_globalEnable();

    return ( TRUE);
}

void AIC23_xmtIsr( void)
{
	if( !MCBSP_xrdy( AIC23_DataHnd))
		return;

	if( xmtRead == xmtWrite)
		MCBSP_write( AIC23_DataHnd, 0);
	else {
		MCBSP_write( AIC23_DataHnd, xmtBuffer[xmtRead]);
		xmtRead = (xmtRead + 1) & (LENGTH - 1);
		SEM_postBinary(&SEM_xmt);
	}
}

/*
 *  ======== AIC23_write ========
 *  Write a 32-bit value to the codec
 */
Int16 AIC23_Write( Int32 val)
{
    IRQ_globalDisable();
	while(((xmtWrite + 1) & (LENGTH - 1)) == xmtRead){
	    IRQ_globalEnable();
		SEM_pendBinary(&SEM_xmt, 10);
		IRQ_globalDisable();
    }

	xmtBuffer[xmtWrite] = val;
	xmtWrite = (xmtWrite + 1) & (LENGTH - 1);

	IRQ_globalEnable();

    return(TRUE);
}

/*
 *  ======== AIC23_Config ========
 *  Set the default codec register Config values
 */
void AIC23_Config( AIC23_ConfigTab *Config)
{
    int i;

    /* Use default parameters if none are given */
    if (Config == NULL)
    	Config = &CodecState;

    /* Configure power down register first */
    AIC23_RegSet( AIC23_POWERDOWN,
        Config -> regs[AIC23_POWERDOWN]);

    /* Assign each register */
    for (i = 0; i < AIC23_NUMREGS; i++)
        if (i != AIC23_POWERDOWN)
            AIC23_RegSet( i, Config -> regs[i]);
}

void AIC23_termIrq( void)
{
	IRQ_globalDisable();

	IRQ_disable( IRQ_EVT_RINT2);
	IRQ_disable( IRQ_EVT_XINT2);

	IRQ_clear( IRQ_EVT_RINT2);
	IRQ_clear( IRQ_EVT_XINT2);

	IRQ_globalRestore(gie);
}

void AIC23_initIrq( void)
{
	gie = IRQ_globalDisable();

	IRQ_clear( IRQ_EVT_RINT2);
	IRQ_clear( IRQ_EVT_XINT2);

	IRQ_enable( IRQ_EVT_RINT2);
	IRQ_enable( IRQ_EVT_XINT2);

	IRQ_globalEnable();
}

/*
 *  ======== AIC23_closeCodec ========
 *  Close the codec
 */
void AIC23_CloseCodec( void)
{
    AIC23_termIrq();

    /* Turn the codec off */
    AIC23_RegSet( AIC23_POWERDOWN, 0xff);

    /* Close McBSPs */
    MCBSP_close(AIC23_ControlHnd);
    MCBSP_close(AIC23_DataHnd);
}

/*
 *  ======== AIC23_OpenCodec ========
 *  Open the codec and return a codec handle
 */
Int32 AIC23_OpenCodec( AIC23_ConfigTab *Config)
{
    MCBSP_Config mcbspCfgControl = {
        MCBSP_FMKS(SPCR, FREE, NO)              |
        MCBSP_FMKS(SPCR, SOFT, NO)              |
        MCBSP_FMKS(SPCR, FRST, YES)             |
        MCBSP_FMKS(SPCR, GRST, YES)             |
        MCBSP_FMKS(SPCR, XINTM, XRDY)           |
        MCBSP_FMKS(SPCR, XSYNCERR, NO)          |
        MCBSP_FMKS(SPCR, XRST, YES)             |
        MCBSP_FMKS(SPCR, DLB, OFF)              |
        MCBSP_FMKS(SPCR, RJUST, RZF)            |
        MCBSP_FMKS(SPCR, CLKSTP, NODELAY)       |
        MCBSP_FMKS(SPCR, DXENA, OFF)            |
        MCBSP_FMKS(SPCR, RINTM, RRDY)           |
        MCBSP_FMKS(SPCR, RSYNCERR, NO)          |
        MCBSP_FMKS(SPCR, RRST, YES),

        MCBSP_FMKS(RCR, RPHASE, DEFAULT)        |
        MCBSP_FMKS(RCR, RFRLEN2, DEFAULT)       |
        MCBSP_FMKS(RCR, RWDLEN2, DEFAULT)       |
        MCBSP_FMKS(RCR, RCOMPAND, DEFAULT)      |
        MCBSP_FMKS(RCR, RFIG, DEFAULT)          |
        MCBSP_FMKS(RCR, RDATDLY, DEFAULT)       |
        MCBSP_FMKS(RCR, RFRLEN1, DEFAULT)       |
        MCBSP_FMKS(RCR, RWDLEN1, DEFAULT)       |
        MCBSP_FMKS(RCR, RWDREVRS, DEFAULT),

        MCBSP_FMKS(XCR, XPHASE, SINGLE)         |
        MCBSP_FMKS(XCR, XFRLEN2, OF(0))         |
        MCBSP_FMKS(XCR, XWDLEN2, 8BIT)          |
        MCBSP_FMKS(XCR, XCOMPAND, MSB)          |
        MCBSP_FMKS(XCR, XFIG, NO)               |
        MCBSP_FMKS(XCR, XDATDLY, 1BIT)          |
        MCBSP_FMKS(XCR, XFRLEN1, OF(0))         |
        MCBSP_FMKS(XCR, XWDLEN1, 16BIT)         |
        MCBSP_FMKS(XCR, XWDREVRS, DISABLE),

        MCBSP_FMKS(SRGR, GSYNC, FREE)           |
        MCBSP_FMKS(SRGR, CLKSP, RISING)         |
        MCBSP_FMKS(SRGR, CLKSM, INTERNAL)       |
        MCBSP_FMKS(SRGR, FSGM, DXR2XSR)         |
        MCBSP_FMKS(SRGR, FPER, OF(0))           |
        MCBSP_FMKS(SRGR, FWID, OF(19))          |
        MCBSP_FMKS(SRGR, CLKGDV, OF(99)),

        MCBSP_MCR_DEFAULT,
        MCBSP_RCERE0_DEFAULT,
        MCBSP_RCERE1_DEFAULT,
        MCBSP_RCERE2_DEFAULT,
        MCBSP_RCERE3_DEFAULT,
        MCBSP_XCERE0_DEFAULT,
        MCBSP_XCERE1_DEFAULT,
        MCBSP_XCERE2_DEFAULT,
        MCBSP_XCERE3_DEFAULT,

        MCBSP_FMKS(PCR, XIOEN, SP)              |
        MCBSP_FMKS(PCR, RIOEN, SP)              |
        MCBSP_FMKS(PCR, FSXM, INTERNAL)         |
        MCBSP_FMKS(PCR, FSRM, EXTERNAL)         |
        MCBSP_FMKS(PCR, CLKXM, OUTPUT)          |
        MCBSP_FMKS(PCR, CLKRM, INPUT)           |
        MCBSP_FMKS(PCR, CLKSSTAT, DEFAULT)      |
        MCBSP_FMKS(PCR, DXSTAT, DEFAULT)        |
        MCBSP_FMKS(PCR, FSXP, ACTIVELOW)        |
        MCBSP_FMKS(PCR, FSRP, DEFAULT)          |
        MCBSP_FMKS(PCR, CLKXP, FALLING)         |
        MCBSP_FMKS(PCR, CLKRP, DEFAULT)
    };

    /* Configure structure of Data MCBSP */
    MCBSP_Config mcbspCfgData = {
    	/* SPCR */
    	MCBSP_FMKS(SPCR, FRST, YES)             |
    	MCBSP_FMKS(SPCR, GRST, YES)             |
#ifdef SIMULATOR
    	MCBSP_FMKS(SPCR, DLB, ON)               |
    	MCBSP_FMKS(SPCR, RINTM, RSYNCERR)       |
    	MCBSP_FMKS(SPCR, XINTM, XSYNCERR)       |
#endif
    	MCBSP_FMKS(SPCR, XRST, YES)             |
    	MCBSP_FMKS(SPCR, RJUST, RZF),
		/* RCR */
		MCBSP_FMKS(RCR, RCOMPAND, MSB)          |
		MCBSP_FMKS(RCR, RFRLEN1, OF(0))         |
		MCBSP_FMKS(RCR, RWDLEN1, 32BIT)         |
		MCBSP_FMKS(RCR, RWDREVRS, DISABLE),
		/* XCR */
		MCBSP_FMKS(XCR, XCOMPAND, MSB)          |
		MCBSP_FMKS(XCR, XFRLEN1, OF(0))         |
		MCBSP_FMKS(XCR, XWDLEN1, 32BIT)         |
		MCBSP_FMKS(XCR, XWDREVRS, DISABLE),
		/* SRGR */
		MCBSP_FMKS(SRGR, CLKSP, RISING)        |
		MCBSP_FMKS(SRGR, CLKSM, CLKS)          |
		MCBSP_FMKS(SRGR, FWID, OF(0))          |
		MCBSP_FMKS(SRGR, CLKGDV, OF(1)),

        MCBSP_MCR_DEFAULT,
        MCBSP_RCERE0_DEFAULT,
        MCBSP_RCERE1_DEFAULT,
        MCBSP_RCERE2_DEFAULT,
        MCBSP_RCERE3_DEFAULT,
        MCBSP_XCERE0_DEFAULT,
        MCBSP_XCERE1_DEFAULT,
        MCBSP_XCERE2_DEFAULT,
        MCBSP_XCERE3_DEFAULT,
        /* PCR */
        MCBSP_FMKS(PCR, XIOEN, SP)              |
        MCBSP_FMKS(PCR, RIOEN, SP)              |
#ifdef SIMULATOR
        MCBSP_FMKS(PCR, FSXM, INTERNAL)         |
#else
        MCBSP_FMKS(PCR, FSXM, EXTERNAL)         |
#endif
        MCBSP_FMKS(PCR, FSRM, EXTERNAL)         |
#ifdef SIMULATOR
        MCBSP_FMKS(PCR, CLKXM, OUTPUT)          |
#else
        MCBSP_FMKS(PCR, CLKXM, INPUT)           |
#endif
        MCBSP_FMKS(PCR, CLKRM, INPUT)           |
        MCBSP_FMKS(PCR, FSXP, ACTIVEHIGH)       |
        MCBSP_FMKS(PCR, FSRP, ACTIVEHIGH)       |
        MCBSP_FMKS(PCR, CLKXP, FALLING)         |
        MCBSP_FMKS(PCR, CLKRP, RISING)
    };

    /* Open codec control handle */
    AIC23_ControlHnd = MCBSP_open(MCBSP_DEV1, MCBSP_OPEN_RESET);
    if (AIC23_ControlHnd == INV)
        return (Int32)INV;

    /* Configure codec control McBSP */
    MCBSP_config(AIC23_ControlHnd, &mcbspCfgControl);

    /* Start McBSP1 as the codec control channel */
    MCBSP_start(AIC23_ControlHnd, MCBSP_XMIT_START | MCBSP_RCV_START |
	    MCBSP_SRGR_START | MCBSP_SRGR_FRAMESYNC, 100);

    /* Reset the AIC23 */
    AIC23_RegSet( AIC23_RESET, 0);

    /* Configure the rest of the AIC23 registers */
    AIC23_Config( Config);

    /* Open codec data handle */
    AIC23_DataHnd = MCBSP_open(MCBSP_DEV2, MCBSP_OPEN_RESET);
    if (AIC23_DataHnd == INV)
        return (Int32)INV;

    /* Configure codec data McBSP */
    MCBSP_config(AIC23_DataHnd, &mcbspCfgData);

    /* Clear any garbage from the codec data port */
    if (MCBSP_rrdy(AIC23_DataHnd))
        MCBSP_read(AIC23_DataHnd);

    AIC23_initIrq();
 
    MCBSP_start(AIC23_DataHnd, MCBSP_XMIT_START | MCBSP_RCV_START |
    		MCBSP_SRGR_START | MCBSP_SRGR_FRAMESYNC, 220);

    /* Start by dummy write */
    MCBSP_write( AIC23_DataHnd, 0);

    return 0;
}
