/*
 * aic23.h
 *
 *  Created on: 3.10.2013
 *      Author: Student
 */

#ifndef AIC23_H_
#define AIC23_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <csl.h>
#include <csl_mcbsp.h>

/* Frequency Definitions */
#define AIC23_FREQ_8KHZ        1
#define AIC23_FREQ_16KHZ      2
#define AIC23_FREQ_24KHZ      3
#define AIC23_FREQ_32KHZ      4
#define AIC23_FREQ_44KHZ      5
#define AIC23_FREQ_48KHZ      6
#define AIC23_FREQ_96KHZ      7

/* Codec register tab definitions */
#define AIC23_NUMREGS         10

/* Parameter Structure for the AIC23 Codec */
typedef struct AIC23_ConfigTab {
    int regs[AIC23_NUMREGS];
} AIC23_ConfigTab;

#define AIC23_DEFAULTCONFIG { \
    0x001D, /* Set-Up Reg 0       Left line input channel volume control */  \
            /* LRS     0          simultaneous left/right volume: disabled */\
            /* LIM     0          left line input mute: disabled */          \
            /* XX      00         reserved */                                \
            /* LIV     10111      left line input volume: 0 dB */            \
                                                                             \
    0x001D, /* Set-Up Reg 1       Right line input channel volume control */ \
            /* RLS     0          simultaneous right/left volume: disabled */\
            /* RIM     0          right line input mute: disabled */         \
            /* XX      00         reserved */                                \
            /* RIV     10111      right line input volume: 0 dB */           \
                                                                             \
    0x00f9, /* Set-Up Reg 2       Left channel headphone volume control */   \
            /* LRS     1          simultaneous left/right volume: enabled */ \
            /* LZC     1          left channel zero-cross detect: enabled */ \
            /* LHV     1110000    left headphone volume: -25 dB */           \
                                                                             \
    0x00f9, /* Set-Up Reg 3       Right channel headphone volume control */  \
            /* RLS     1          simultaneous right/left volume: enabled */ \
            /* RZC     1          right channel zero-cross detect: enabled */\
            /* RHV     1110000    right headphone volume: -25 dB */          \
                                                                             \
    0x0011, /* Set-Up Reg 4       Analog audio path control */               \
            /* X       0          reserved */                                \
            /* STA     00         sidetone attenuation: -6 dB */             \
            /* STE     0          sidetone: disabled */                      \
            /* DAC     1          DAC: selected */                           \
            /* BYP     0          bypass: off */                             \
            /* INSEL   0          input select for ADC: line */              \
            /* MICM    0          microphone mute: disabled */               \
            /* MICB    1          microphone boost: enabled */               \
                                                                             \
    0x0000, /* Set-Up Reg 5       Digital audio path control */              \
            /* XXXXX   00000      reserved */                                \
            /* DACM    0          DAC soft mute: disabled */                 \
            /* DEEMP   00         deemphasis control: disabled */            \
            /* ADCHP   0          ADC high-pass filter: disabled */          \
                                                                             \
    0x0000, /* Set-Up Reg 6       Power down control */                      \
            /* X       0          reserved */                                \
            /* OFF     0          device power: on (i.e. not off) */         \
            /* CLK     0          clock: on */                               \
            /* OSC     0          oscillator: on */                          \
            /* OUT     0          outputs: on */                             \
            /* DAC     0          DAC: on */                                 \
            /* ADC     0          ADC: on */                                 \
            /* MIC     0          microphone: on */                          \
            /* LINE    0          line input: on */                          \
                                                                             \
    0x0043, /* Set-Up Reg 7       Digital audio interface format */          \
            /* XX      00         reserved */                                \
            /* MS      1          master/slave mode: master */               \
            /* LRSWAP  0          DAC left/right swap: disabled */           \
            /* LRP     0          DAC lrp: MSB on 1st BCLK */                \
            /* IWL     00         input bit length: 16 bit */                \
            /* FOR     11         data format: DSP format */                 \
                                                                             \
    0x0081, /* Set-Up Reg 8       Sample rate control */                     \
            /* X       0          reserved */                                \
            /* CLKOUT  1          clock output divider: 2 (MCLK/2) */        \
            /* CLKIN   0          clock input divider: 2 (MCLK/2) */         \
            /* SR,BOSR 00000      sampling rate: ADC  48 kHz DAC  48 kHz */  \
            /* USB/N   1          clock mode select (USB/normal): USB */     \
                                                                             \
    0x0001  /* Set-Up Reg 9       Digital interface activation */            \
            /* XX..X   00000000   reserved */                                \
            /* ACT     1          active */                                  \
}

/* Set the codec loopback mode */
void AIC23_Loopback( Int16 mode);

/* Enable/disable codec mute mode */
void AIC23_Mute( Int16 mode);

/* Set the codec output gain */
void AIC23_OutGain( Uint16 outGain);

/* Enable/disable codec powerdown modes for DAC, ADC */
void AIC23_PowerDown( Uint16 sect);

/* Set the codec sample rate frequency */
void AIC23_SetFreq( Uint32 freq);

/* Read a 32-bit value from the codec */
Int16 AIC23_Read( Int32 *val);

/* Write a 32-bit value to the codec */
Int16 AIC23_Write( Int32 val);

/* Configure the codec register values */
void AIC23_Config( AIC23_ConfigTab *Config);

/* Close the codec */
void AIC23_CloseCodec( void);

/* Open the codec with id and return handle */
Int32 AIC23_OpenCodec( AIC23_ConfigTab *Config);

#ifdef __cplusplus
}
#endif

#endif /* AIC23_H_ */
