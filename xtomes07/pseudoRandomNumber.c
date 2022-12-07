#include <std.h>
#include <math.h>
#include "pseudoRandomNumber.h"

#define LENGTH 2
#define	FLOAT2FIXED(x)	((x) >= 0 ? ((Int16)((x)*32768+0.5)) : ((Int16)((x)*32768-0.5)))

/*** constants and variables ***/

Int32 a = 8145821;
Int32 b = 3545443;
Int32 accu;

static Int16 sample[LENGTH] = {-16549, 0};
static int read = 0;
static int write = LENGTH/2;
float sub;


/*** functions ***/

Int32 getRandomNumber (){
	Int32 m = pow(2,16);

	accu = _smpyhl(a, sample[read]);
	accu = _sadd(accu,b);

/*
	accu = (a * sample[read]);
	accu = accu + b;
*/
	accu = accu % m;

	sample[write] = accu;

	read = (read + 1) & (LENGTH - 1);
	write = (write + 1) & (LENGTH - 1);

	return sample[read];

}




