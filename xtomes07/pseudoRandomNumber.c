#include <std.h>
#include <math.h>
#include "pseudoRandomNumber.h"

#define LENGTH 2

/*** constants and variables ***/

Int32 a = 8145821;
Int32 b = 3545443;
Long accu;

static Int16 sample[LENGTH] = {-16549, 0};
static int read = 0;
static int write = LENGTH/2;


/*** functions ***/

Int32 getRandomNumber (){
	Int32 m = pow(2,16);

/*
	accu = _smpyhl(a, sample[read]);
	accu = _sadd(accu,b);
*/

/*
	accu = (a * sample[read]) << 1;
	accu = accu + b;
	accu = accu % m;
*/

	accu = (a * sample[read]);
	accu = accu << 1;
	accu = accu + b;
	accu = accu % m;


	sample[write] = accu;

	read = (read + 1) & (LENGTH - 1);
	write = (write + 1) & (LENGTH - 1);

	return sample[read];

}




