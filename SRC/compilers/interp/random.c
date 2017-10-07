extern long time(long*);

/*  This is the minimal standard random number generator based on  */
/*  Schrage's method described on page 1195 of:                    */
/*     Random Number Generators: Good ones are hard to find        */
/*     Stephen K. Park & Keith W. Miller, CACM, v.31,10(Oct.88),   */
/*     pp. 1192 - 1201                                             */

static long int seed = 1;
static long int useed = 1;    /* user's random # generator */
const long A = 16807;
const long M = 2147483647;
const long Q = 127773;		/* M / A */
const long R = 2836;		/* M % A */

float strand()
  /* returns a pseudo-random float between 0.0 and 1.0 */
{
long int lo, hi, test;
float next;
   hi = seed / Q;
   lo = seed % Q;
   test = A * lo - R * hi;
   if (test > 0)
      seed = test; 
   else 
      seed = test + M; 
   next = (((float) seed)/M);
   return (next);
}	/* end of strand */


float ustrand()
  /* returns a pseudo-random float between 0.0 and 1.0             */
  /* for random number generator callable by user from interpreter */
{
long int lo, hi, test;
float next;
   hi = useed / Q;
   lo = useed % Q;
   test = A * lo - R * hi;
   if (test > 0)
      useed = test; 
   else 
      useed = test + M; 
   next = (((float) useed)/M);
   return (next);
}	/* end of ustrand */

void setseed(newseed)
long int newseed;
   /* sets a new seed, if 'newseed' is positive */
{
   if (newseed > 0) seed = newseed;
}

void __randomize()
   /* sets random number generators' seeds from the clock */
{
long int temp;
   time(&temp);
      /* take lower 16 bits of current time & make them significant */
      /* (but non-zero) for the initial seed                        */
   seed = ((temp & 0xffff) << 16) + 31;
   useed = temp+seed;   /* start user somewhere else */
}

unsigned int   __random(range)
unsigned int   range;
   /* returns a pseudo-random unsigned int between 0 and 'range' */
{
   return ((unsigned) ((float) range * strand()));
}

unsigned int   __urandom(range)
unsigned int   range;
   /* returns a pseudo-random unsigned int between 0 and 'range' */
   /* for user-callable random number generator in interpreter   */
{
   return ((unsigned) ((float) range * ustrand()));
}
/*
 *
 *  $Id: random.c,v 1.7 2007/06/01 18:58:44 bynum Exp $
 *
 */
