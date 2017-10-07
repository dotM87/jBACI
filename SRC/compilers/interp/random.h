/*  This is the minimal standard random number generator based on  */
/*  Schrage's method described on page 1195 of:                    */
/*     Random Number Generators: Good ones are hard to find        */
/*     Stephen K. Park & Keith W. Miller, CACM, v.31,10(Oct.88),   */
/*     pp. 1192 - 1201                                             */
/*  Implemented by Bill Bynum, September, 1993                     */

extern void __randomize();
   /* __randomize();                                               */
   /* sets the seed for the random number generator from the clock */

extern void setseed();
   /* setseed(long newseed);                       */
   /* sets the seed to 'newseed', if it's positive */
   /* otherwise the default seed (1) is used       */

extern float strand();
   /* strand();                                                        */
   /* returns a randomly distributed 'random' real between 0.0 and 1.0 */

extern unsigned __random();
   /* __random(range);                                              */
   /* returns a 'randomly' chosen integer between 0 and 'range' */


   /* the next two are for the user-callable random number generator */
   /* in the interpreter                                             */
extern float ustrand();
   /* strand();                                                        */
   /* returns a randomly distributed 'random' real between 0.0 and 1.0 */

extern unsigned __urandom();
   /* __urandom(range);                                             */
   /* returns a 'randomly' chosen integer between 0 and 'range' */

/*
 *
 *  $Id: random.h,v 1.6 2007/06/01 18:58:44 bynum Exp $
 *
 */
