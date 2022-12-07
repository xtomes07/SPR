#include <std.h>

#include <log.h>
#include <sts.h>
#include <clk.h>

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "pseudoRandomNumber.h"

#define LENGTH 2

#ifdef SIMULATOR

/* Simulace DIP pøepínaèù a LED diod na kitu */

#define DSK6416_DIP_get(k)      (dip[k] > 0)
#define DSK6416_LED_on(k)       (led[k] = 1)
#define DSK6416_LED_off(k)      (led[k] = 0)
#define DSK6416_LED_toggle(k)   (led[k] = led[k] ? 0 : 1)

void DSK6416_init(void)     {}
Int16 DSK6416_getVersion()  {return 1;}
void DSK6416_DIP_init(void) {}
void DSK6416_LED_init(void) {}

char led[4] = {0};
char dip[4] = {0};

#else

#include <dsk6416.h>
#include <dsk6416_led.h>
#include <dsk6416_dip.h>

#endif

#include "hellocfg.h"

#include "aic23.h"

static AIC23_ConfigTab CodecCfg = AIC23_DEFAULTCONFIG;

void tskProcess( void)
{
	int k;
	static Int32 output;
	static Int16 sample;

	while( 1)
	{

#ifdef SIMULATOR
		IRQ_set(IRQ_EVT_RINT2);
		IRQ_set(IRQ_EVT_XINT2);
#endif


		TSK_sleep(1);
		/* simulace nároèného zpracování */
		if( !DSK6416_DIP_get(3) && rand() < 1000)
			for( k = 0; k < 50000; k++)
				;


/**********************************************************/

		STS_set(&STS_left, CLK_gethtime());

        sample = getRandomNumber();

        STS_delta( &STS_left, CLK_gethtime());

		output = _extu (sample, 16, 16);
		output = output | _extu (sample, 16, 0);

		AIC23_Write(output);
/**********************************************************/
	}

	AIC23_CloseCodec();
}


void tskStat( void)
{
    while( 1){
#ifdef SIMULATOR
        TSK_sleep(100);
#else
        TSK_sleep(10000);
#endif
        printf( "Narocnost generovani %d\n", STS_left.acc / STS_left.num);
        fflush(stdout);
    }
}


void tskCheck( Arg par)
{
	static int last[2];

	while( 1){
		last[par] = !DSK6416_DIP_get(par);
		if( last[par]){
			DSK6416_LED_on(par);
		} else
			DSK6416_LED_off(par);

		switch(par){
		case 0:
			AIC23_Mute(last[par]);
			break;
		case 1:
			AIC23_Loopback(last[par]);
			break;
		}

#ifdef SIMULATOR
       IRQ_set( IRQ_EVT_RINT2);
       IRQ_set( IRQ_EVT_XINT2);
#endif

		while( last[par] == !DSK6416_DIP_get(par))
			TSK_sleep(1000);
	}
}

void idlLive( void){
	static int count = 0;

	if( 500000 > ++count)
		return;

	DSK6416_LED_toggle(3);
	count = 0;
}

/*
 *  ======== main ========
 */
Void main()
{
   int ver;

    LOG_printf(&trace, "hello world!");

	CSL_init();
	DSK6416_init();
	DSK6416_DIP_init();
	DSK6416_LED_init();

    ver = DSK6416_getVersion();
    if( 1 == ver){
        printf( "Spuštìno v simulátoru.\n");
#ifndef SIMULATOR
        printf( "V simulátoru není audio kodek a proto verze pro kit nebude fungovat.\n");
        exit(-1);
#endif
    } else {
        printf( "Spuštìno v kitu verze %d.\n", ver);
#ifdef SIMULATOR
        printf( "Verzi pro simulátor nelze spouštìt na kitu.\n");
        exit( -1);
#endif
    }

	if( AIC23_OpenCodec( &CodecCfg) < 0)
		exit(-1);

	AIC23_SetFreq( AIC23_FREQ_8KHZ);

    /* fall into DSP/BIOS idle loop */
    return;
}
