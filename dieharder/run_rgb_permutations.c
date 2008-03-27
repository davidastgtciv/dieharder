/*
 * See copyright in copyright.h and the accompanying file COPYING
 */

/*
 *========================================================================
 *  run_rgb_permutations()
 *========================================================================
 */

#include "dieharder.h"

void run_rgb_permutations()
{

 /*
  * Declare the results struct.
  */
 Test **rgb_permutations_test;
 uint ntmin,ntmax;

 /*
  * Set any GLOBAL data used by the test.  rgb_ntuple is the value
  * assigned by -n ntuple (max) on the command line, or default 0 which
  * means -- do all ntuples in range 1 to 8.  rgb_permutations_dtest.nkps is
  * the number of pvalues to be returned, which is just ntuple.
  *
  * Note that we have to do this BEFORE creating the test because
  * dtest.nkps depends on the value of ntuple!
  */
 if(ntuple){
   ntmax = ntuple;
   ntmin = ntuple;
 } else {
   /*
    * Default is to test 2 through n permutations, but n > 7 is really
    * long.  We quit at n = 7.
    */
   ntmin = 2;
   ntmax = 7;
 }
 if(all == YES) ntmin = 2;

 /* printf("Setting ntmin = %d ntmax = %d\n",ntmin,ntmax); */

 /*
  * We could work on speeding this up.  One round of calls and working
  * down?
  */
 for(rgb_permutations_k = ntmin;rgb_permutations_k<=ntmax;rgb_permutations_k++){

   /*
    * First we create the test (to set some values displayed in test header
    * correctly).
    */
   rgb_permutations_test = create_test(&rgb_permutations_dtest,tsamples,psamples,&rgb_permutations);

   /*
    * This particular test we need to pre-initialize the pvlabel for
    * each test, in order.
    */
   snprintf(rgb_permutations_test[0]->pvlabel,LINE,"# Normal p-value for RGB Permutations test for n = %u\n",rgb_permutations_k);

   /*
    * Show the standard test header for this test.
    */
   show_test_header(&rgb_permutations_dtest,rgb_permutations_test);
   /*
    * Append the following SPECIAL line
    */
   printf("# Testing %d-permutations \n",rgb_permutations_k);
   fflush(stdout);

   /*
    * Set any GLOBAL data used by the test.  Then call the test itself
    * This fills in the results in the Test struct.
    */
   std_test(&rgb_permutations_dtest,rgb_permutations_test);

   /*
    * This almost certainly belongs in the show_test_results section,
    * possibly with additional conditionals rejecting test results involving
    * rewinds, period.
    */
   if(strncmp("file_input",gsl_rng_name(rng),10) == 0){
     printf("# %u rands were used in this test\n",file_input_get_rtot(rng));
     printf("# The file %s was rewound %u times\n",gsl_rng_name(rng),file_input_get_rewind_cnt(rng));
     fflush(stdout);
   }

   /*
    * Show standard test results, for all statistics generated by a single run.
    */
   show_test_results(&rgb_permutations_dtest,rgb_permutations_test);

   /*
    * Free any GLOBAL data used by the test.
    */
   destroy_test(&rgb_permutations_dtest,rgb_permutations_test);

 }

}
