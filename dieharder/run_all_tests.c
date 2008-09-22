/*
 *========================================================================
 * $Id$
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *========================================================================
 */

/*
 *========================================================================
 * This should be a nice, big case switch where we add EACH test
 * we might want to do and either just configure and do it or
 * prompt for input (if absolutely necessary) and then do it.
 *========================================================================
 */

#include "dieharder.h"

void run_all_tests()
{

 /*
  * The nt variables control ntuple loops for the -a(ll) display only.
  */
 int i,ntmin,ntmax,ntsave;
 int error = 0;

 /*
  * This isn't QUITE a simple loop because -a is a dieharder-only function,
  * so that all running of ntuples etc has to be mediated here, per very
  * specific test.  Only certain tests are run over an ntuple range.
  */

 /*
  * No special ntuple tests in diehard
  */
 for(i=0;i<dh_num_diehard_tests;i++){
   if(dh_test_types[i]){
     execute_test(i);
   }
 }

 /*
  * No special ntuple tests in sts (yet)
  */
 for(i=100;i<100+dh_num_sts_tests;i++){
   if(dh_test_types[i]){
     execute_test(i);
   }
 }

 /*
  * Here we have to mess with ntuples for various rgb -- I mean "other"
  * tests. Sorry!  We do this in a case switch.  Note that we could just
  * loop over all tests from 0-899 with the same case switch, but since
  * we take the trouble to count the three categories of test, might as
  * well use them.
  */
 for(i=200;i<200+dh_num_other_tests;i++){

   switch(i){

     /*
      * Test 200 is rgb_bitdist, and needs an ntuple set/loop.
      */
     case 200:

       if(dh_test_types[i]){

         if(ntuple){
           /*
            * If ntuple is set to be nonzero, just use that value in "all".
	    * We might need to check to be sure it is "doable", but probably
	    * not...
            */
           execute_test(i);
         } else {
           /*
            * Default is to test 1 through 8 bits, which takes a while on my
            * (quite fast) laptop but is a VERY thorough test of randomness
            * out to byte level.
            */
           ntmin = 1;
           ntmax = 12;
           /* ntmax = 8; */
           /* printf("Setting ntmin = %d ntmax = %d\n",ntmin,ntmax); */
           for(ntuple = ntmin;ntuple <= ntmax;ntuple++){
             execute_test(i);
           }
           /*
            * This RESTORES ntuple = 0, which is the only way we could have
            * gotten here in the first place!
            */
           ntuple = 0;

	 }
       }
       break;

     /*
      * Test 201 is rgb_minimum_distance, and needs an ntuple set/loop.
      */
     case 201:

       if(dh_test_types[i]){

         if(ntuple){
           /*
            * If ntuple is set to be nonzero, just use that value in "all",
            * but only if it is in bounds.
            */
           if(ntuple < 2 || ntuple > 5){
             ntsave = ntuple;
             ntuple = 5;  /* This is the hardest test anyway */
             execute_test(i);
             ntuple = ntsave;
           } else {
             execute_test(i);
           }
         } else {
           /*
            * Default is to 2 through 5 dimensions, all that are supported by
            * the test.
            */
           ntmin = 2;
           ntmax = 5;
           /* printf("Setting ntmin = %d ntmax = %d\n",ntmin,ntmax); */
           for(ntuple = ntmin;ntuple <= ntmax;ntuple++){
	     execute_test(i);
	   }
           /*
            * This RESTORES ntuple = 0, which is the only way we could have
            * gotten here in the first place!
            */

           ntuple = 0;
	   
	 }
       }
       break;

     /*
      * Test 202 is rgb_permutations, and needs an ntuple set/loop.
      */
     case 202:

       if(dh_test_types[i]){

         if(ntuple){
           /*
            * If ntuple is set to be nonzero, just use that value in "all",
            * but only if it is in bounds.
            */
           if(ntuple < 2){
             ntsave = ntuple;
             ntuple = 5;  /* This is the default operm5 value */
             execute_test(i);
             ntuple = ntsave;
           } else {
             execute_test(i);
           }
         } else {
           /*
            * Default is to 2 through 5 permutations.  Longer than 5 takes
	    * a LONG TIME and must be done by hand.
            */
           ntmin = 2;
           ntmax = 5;
           /* printf("Setting ntmin = %d ntmax = %d\n",ntmin,ntmax); */
           for(ntuple = ntmin;ntuple <= ntmax;ntuple++){
	     execute_test(i);
	   }
           /*
            * This RESTORES ntuple = 0, which is the only way we could have
            * gotten here in the first place!
            */

           ntuple = 0;
	   
	 }
       }
       break;

     /*
      * Test 203 is rgb_lagged_sums, and needs an ntuple set/loop.
      */
     case 203:

       if(dh_test_types[i]){

         if(ntuple){
           /*
            * If ntuple is set to be nonzero, just use that value in "all".
            */
           execute_test(i);
         } else {
           /*
            * Do all lags from 0 to 32.
            */
           ntmin = 0;
           ntmax = 32;
           /* printf("Setting ntmin = %d ntmax = %d\n",ntmin,ntmax); */
           for(ntuple = ntmin;ntuple <= ntmax;ntuple++){
	     execute_test(i);
	   }
           /*
            * This RESTORES ntuple = 0, which is the only way we could have
            * gotten here in the first place!
            */

           ntuple = 0;
	   
	 }
       }
       break;

     default:
       printf("Preparing to run test %d.  ntuple = %d\n",i,ntuple);
       if(dh_test_types[i]){   /* This is the fallback to normal tests */
         execute_test(i);
       }
       break;

   }

 }

}
