/*
 * $Id$
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *
 */

/*
 *========================================================================
 * This is the Diehard Count a Stream of 1's test, rewritten from the
 * description in tests.txt on George Marsaglia's diehard site.
 *
 *     This is the COUNT-THE-1's TEST for specific bytes.        ::
 * Consider the file under test as a stream of 32-bit integers.  ::
 * From each integer, a specific byte is chosen , say the left-  ::
 * most::  bits 1 to 8. Each byte can contain from 0 to 8 1's,   ::
 * with probabilitie 1,8,28,56,70,56,28,8,1 over 256.  Now let   ::
 * the specified bytes from successive integers provide a string ::
 * of (overlapping) 5-letter words, each "letter" taking values  ::
 * A,B,C,D,E. The letters are determined  by the number of 1's,  ::
 * in that byte::  0,1,or 2 ---> A, 3 ---> B, 4 ---> C, 5 ---> D,::
 * and  6,7 or 8 ---> E.  Thus we have a monkey at a typewriter  ::
 * hitting five keys with with various probabilities::  37,56,70,::
 * 56,37 over 256. There are 5^5 possible 5-letter words, and    ::
 * from a string of 256,000 (overlapping) 5-letter words, counts ::
 * are made on the frequencies for each word. The quadratic form ::
 * in the weak inverse of the covariance matrix of the cell      ::
 * counts provides a chisquare test::  Q5-Q4, the difference of  ::
 * the naive Pearson  sums of (OBS-EXP)^2/EXP on counts for 5-   ::
 * and 4-letter cell counts.                                     ::
 *
 *                       Comment
 *
 * For the less statistically trained amongst us, the translation
 * of the above is:
 *  Generate a string of base-5 digits derived as described from
 * specific (randomly chosen) byte offsets in a random integer.
 *  Turn four and five digit base 5 numbers (created from these digits)
 * into indices and incrementally count the frequency of occurrence of
 * each index.
 *  Compute the expected value of these counts given tsamples samples,
 * and thereby (from the vector of actual vs expected counts) generate
 * a chisq for 4 and 5 digit numbers separately.  These chisq's are
 * expected, for a large number of trials, to be equal to the number of
 * degrees of freedom of the vectors, (5^5 - 1) or (5^4 - 1).  Generate
 * the mean DIFFERENCE = 2500 as the expected value of the difference
 * chisq_5 - chisq_4 and compute a pvalue based on this expected value and
 * the associated standard deviation sqrt(2*2500).
 *
 * Note:  The byte offset is systematically cycled PER tsample, so enough
 * tsamples gives one a reasonable chance of discovering "bad" offsets
 * in any exist.  How many is enough?  Difficult to say a priori -- it
 * depends on how bit the failure is and how many offsets create a
 * failure.  Play around with it.
 *
 * Note also that the code itself is a bit simpler than the stream
 * version (no need to worry about e.g. overlap or modulus 4/5 when
 * getting the next int/byte) but that it generates 4x as many rands
 * as non-overlapping stream: tsamples*psamples*5, basically.
 *
 * This test is actually LESS stringent than the stream version overall,
 * and is much closer to rgb_bitdist.  In fact, if rgb_bitdist for 8 bits
 * fails the diehard_count_1s_byte test MUST fail as it too samples all
 * the different offsets systematically.  However, as before, MOST failures
 * at 8 bits are derived from failures at smaller numbers of bits (this
 * is an assertion that can be made precise in terms of contributing
 * permutations) and again, the test is vastly less sensitive than
 * rgb_bitdist and is less senstive that diehard_count_1s_stream EXCEPT
 * that it samples more rands and might reveal problems with specific
 * offsets ignored by the stream test.  Honestly, I could fix the stream
 * test to cycle through the possible bitlevel offsets and make this
 * test completely obsolete.
 *========================================================================
 */


#include "dieharder.h"

double diehard_count_1s_byte()
{

 double *pvalue,pks;
 uint tempsamples;
 uint est_num_rands;

 /*
  * This is the merest shell to set any test-specific variables, call
  * the main test routine (which fills one or more slots in ks_pvalue[]
  * and increments kspi accordingly), and run a Kuiper Kolmogorov-Smirnov
  * test on the vector of pvalues produced and turn it into a single,
  * cumulative p-value (pks) for the entire test.  If the test parameters
  * are set properly, this will USUALLY yield an unambiguous signal of
  * failure.
  */

 /*
  * If this test is run by itself, we can ignore tsamples.  If it is
  * part of a "standard run", we have to use specific values.  Either
  * way, we have to adjust the sizes of e.g. the list of integers to
  * be generated and sampled, and (re)allocate memory accordingly.
  * Then at the bottom, we have to put it all back.
  */
 if(all == YES){
   tempsamples = tsamples;
   tsamples = 256000;  /* Standard value from diehard */
 }

 /*
  * Allocate space for ks_pvalue.  Free it below
  */
 ks_pvalue = (double *)malloc((size_t) psamples*sizeof(double));

 if(!quiet){
   help_diehard_count_1s_byte();
   printf("#                        Run Details\n");
   if(strncmp("file_input",gsl_rng_name(rng),10) == 0){
     printf("# Random number generator tested: %s\n",gsl_rng_name(rng));
     printf("# File %s contains %u rands of %c type.\n",filename,filecount,filetype);
   } else {
     printf("# Random number generator tested: %s\n",gsl_rng_name(rng));
   }
   printf("# Samples per test = %u.  Diehard recommends 256000\n",tsamples);
   printf("# Test run %u times to cumulate p-values for KS test.\n",psamples);
   printf("# Number of rands required is around 2^28 for 100 samples.\n");
   /* 1.25 uints required per five digit sample */
   est_num_rands = (int)(tsamples*psamples*1.25);
   printf("# Number of rands required is around %u for %u\n",est_num_rands,psamples);
   printf("# non-overlapping p-samples containing %u samples.\n",tsamples);
 }

 kspi = 0;  /* Always zero first */
 pks = sample((void *)diehard_count_1s_byte_test);

 /*
  * Display histogram of ks p-values (optional)
  */
 if(hist_flag){
   histogram(ks_pvalue,psamples,0.0,1.0,10,"p-values");
 }
 if(!quiet){
   if(strncmp("file_input",gsl_rng_name(rng),10) == 0){
     printf("# %u rands were used in this test\n",file_input_get_rtot(rng));
     printf("# The file %s was rewound %u times\n",gsl_rng_name(rng),file_input_get_rewind_cnt(rng));
   }
 }
 printf("#                          Results\n");
 printf("# p = %8.6f for diehard_count_1s_byte test (mean) from\n",pks);
 printf("#     Kuiper Kolmogorov-Smirnov test on %u pvalues.\n",kspi);
 /* Work through some ranges here */
 if(pks < 0.0001){
   printf("# Generator %s FAILED at < 0.01%% for diehard_count_1s_byte.\n",gsl_rng_name(rng));
 } else if(pks < 0.01){
   printf("# Generator %s POOR at < 1%% for diehard_count_1s_byte.\n",gsl_rng_name(rng));
   printf("# Recommendation:  Repeat test to verify failure.\n");
 } else if(pks < 0.05){
   printf("# Generator %s POSSIBLY WEAK at < 5%% for diehard_count_1s_byte.\n",gsl_rng_name(rng));
   printf("# Recommendation:  Repeat test to verify failure.\n");
 } else {
   printf("# Generator %s PASSED at > 5%% for diehard_count_1s_byte.\n",gsl_rng_name(rng));
 }
 printf("#==================================================================\n");

 /*
  * Put back tsamples, free ks_pvalue.
  */
 if(all == YES){
   tsamples = tempsamples;
 }
 free(ks_pvalue);

 return(pks);

}

/*
 * This table was generated using the following code fragment.
 {
   char table[256];
   table[i] = 0;
   for(j=0;j<8*sizeof(uint);j++){
     table[i] += get_int_bit(i,j);
   }
   switch(table[i]){
     case 0:
     case 1:
     case 2:
       table[i] = 0;
       break;
     case 3:
       table[i] = 1;
       break;
     case 4:
       table[i] = 2;
       break;
     case 5:
       table[i] = 3;
       break;
     case 6:
     case 7:
     case 8:
       table[i] = 4;
       break;
     default:
       fprintf(stderr,"Hahahahah\n");
       exit(0);
       break;
   }
 }
 */

char b5b[] = {
0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 2,
0, 0, 0, 1, 0, 1, 1, 2, 0, 1, 1, 2, 1, 2, 2, 3,
0, 0, 0, 1, 0, 1, 1, 2, 0, 1, 1, 2, 1, 2, 2, 3,
0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
0, 0, 0, 1, 0, 1, 1, 2, 0, 1, 1, 2, 1, 2, 2, 3,
0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 4,
0, 0, 0, 1, 0, 1, 1, 2, 0, 1, 1, 2, 1, 2, 2, 3,
0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 4,
0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 4,
1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 4,
2, 3, 3, 4, 3, 4, 4, 4, 3, 4, 4, 4, 4, 4, 4, 4};

/*
 * The following is needed to generate the test statistic
 *
 * Vector of probabilities for each integer.
 * 37.0/256.0,56.0/256.0,70.0/256.0,56.0/256.0,37.0/256.0
 */
const double pb[]={
0.144531250,
0.218750000,
0.273437500,
0.218750000,
0.144531250};


 
/*
 * Useful macro for building 4 and 5 digit base 5 numbers from
 * base 5 digits
 */
#define LSHIFT5(old,new) (old*5 + new)

void diehard_count_1s_byte_test()
{

 uint i,j,k,index5,index4,letter,t;
 uint boffset;
 uint count5[3125],count4[625];
 Btest btest4,btest5;
 Xtest ptest;

 /*
  * count_1s in specific bytes is straightforward after looking over
  * count_1s in a byte.  The statistic is identical; we just have to
  * cycle the offset of the bytes selected and generate 1 random uint
  * per digit.
  */

 /*
  * I'm leaving this in so the chronically bored can validate that
  * the table exists and is correctly loaded and addressable.
  */
 if(verbose == -1){
   for(i=0;i<256;i++){
     printf("%u, ",b5b[i]);
     /* dumpbits(&i,8); */
     if((i+1)%16 == 0){
       printf("\n");
     }
   }
   exit(0);
 }

 /*
  * This is basically a pair of parallel btests, with a final test
  * statistic generated from their difference (somehow).  We therefore
  * create two btests, one for four digit base 5 integers and one for
  * five digit base 5 integers, and generate their expected values for
  * tsamples trials.
  */

 ptest.y = 2500.0;
 ptest.sigma = sqrt(5000.0);

 Btest_create(&btest4,625,"diehard_count_the_1s",gsl_rng_name(rng));
 for(i=0;i<625;i++){
   j = i;
   btest4.y[i] = tsamples;
   btest4.x[i] = 0.0;
   /*
    * Digitize base 5, compute expected value for THIS integer i.
    */
   /* printf("%u:  ",j); */
   for(k=0;k<4;k++){
     /*
      * Take the least significant "letter" of j in range 0-4
      */
     letter = j%5;
     /*
      * multiply by the probability of getting this letter
      */
     btest4.y[i] *= pb[letter];
     /*
      * Right shift j to get next digit.
      */
     /* printf("%1u",letter); */
     j /= 5;
   }
   /* printf(" = %f\n",btest4.y[i]); */
   btest4.sigma[i] = 0.0;
 }

 Btest_create(&btest5,3125,"diehard_count_the_1s",gsl_rng_name(rng));
 for(i=0;i<3125;i++){
   j = i;
   btest5.y[i] = tsamples;
   btest5.x[i] = 0.0;
   /*
    * Digitize base 5, compute expected value for THIS integer i.
    */
   for(k=0;k<5;k++){
     /*
      * Take the least significant "letter" of j in range 0-4
      */
     letter = j%5;
     /*
      * multiply by the probability of getting this letter
      */
     btest5.y[i] *= pb[letter];
     /*
      * Right shift j to get next digit.
      */
     j /= 5;
   }
   btest5.sigma[i] = 0.0;
 }

 /*
  * Here is the test.  We cycle boffset through tsamples
  */
 boffset = 0;
 for(t=0;t<tsamples;t++){

   boffset = t%32;  /* Remember that get_bit_ntuple periodic wraps the uint */
   /*
    * Get the next five bytes and make an index5 out of them, no
    * overlap.
    */
   for(k=0;k<5;k++){
     i = gsl_rng_get(rng);
     if(verbose == D_DIEHARD_COUNT_1S_STREAM || verbose == D_ALL){
       dumpbits(&i,32);
     }
     /*
      * get next byte from the last rand we generated.
      */
     j = get_bit_ntuple(&i,1,8,boffset);
     index5 = LSHIFT5(index5,b5b[j]);
     if(verbose == D_DIEHARD_COUNT_1S_STREAM || verbose == D_ALL){
       printf("b5b[%u] = %u, index5 = %u\n",j,b5b[j],index5);
       dumpbits(&j,8);
     }
   }
   /*
    * We use modulus to throw away the sixth digit in the left-shifted
    * base 5 index value, keeping the value of the 5-digit base 5 number
    * in the range 0 to 5^5-1 or 0 to 3124 decimal.  We repeat for the
    * four digit index.  At this point we increment the counts for index4
    * and index5.  Tres simple, no?
    */
   index5 = index5%3125;
   index4 = index5%625;
   btest4.x[index4]++;
   btest5.x[index5]++;
 }
 /*
  * OK, all that is left now is to figure out the statistic.
  */
 if(verbose == D_DIEHARD_COUNT_1S_BYTE || verbose == D_ALL){
   for(i = 0;i<625;i++){
     printf("%u:  %f    %f\n",i,btest4.y[i],btest4.x[i]);
   }
   for(i = 0;i<3125;i++){
     printf("%u:  %f    %f\n",i,btest5.y[i],btest5.x[i]);
   }
 }
 Btest_eval(&btest4);
 Btest_eval(&btest5);
 if(verbose == D_DIEHARD_COUNT_1S_BYTE || verbose == D_ALL){
   printf("btest4.chisq = %f   btest5.chisq = %f\n",btest4.chisq,btest5.chisq);
 }
 ptest.x = btest5.chisq - btest4.chisq;
 Xtest_eval(&ptest);

 ks_pvalue[kspi] = ptest.pvalue;
 if(verbose == D_DIEHARD_COUNT_1S_BYTE || verbose == D_ALL){
   printf("# diehard_count_1s_byte_freq(): ks_pvalue[%u] = %10.5f\n",kspi,ks_pvalue[kspi]);
 }
 kspi++;

}

void help_diehard_count_1s_byte()
{

 printf("\n\
#==================================================================\n\
#            Diehard \"count_1s_byte\" test (modified).\n\
#     This is the COUNT-THE-1's TEST for specific bytes.        \n\
# Consider the file under test as a stream of 32-bit integers.  \n\
# From each integer, a specific byte is chosen , say the left-  \n\
# most::  bits 1 to 8. Each byte can contain from 0 to 8 1's,   \n\
# with probabilitie 1,8,28,56,70,56,28,8,1 over 256.  Now let   \n\
# the specified bytes from successive integers provide a string \n\
# of (overlapping) 5-letter words, each \"letter\" taking values  \n\
# A,B,C,D,E. The letters are determined  by the number of 1's,  \n\
# in that byte::  0,1,or 2 ---> A, 3 ---> B, 4 ---> C, 5 ---> D,\n\
# and  6,7 or 8 ---> E.  Thus we have a monkey at a typewriter  \n\
# hitting five keys with with various probabilities::  37,56,70,\n\
# 56,37 over 256. There are 5^5 possible 5-letter words, and    \n\
# from a string of 256,000 (overlapping) 5-letter words, counts \n\
# are made on the frequencies for each word. The quadratic form \n\
# in the weak inverse of the covariance matrix of the cell      \n\
# counts provides a chisquare test::  Q5-Q4, the difference of  \n\
# the naive Pearson  sums of (OBS-EXP)^2/EXP on counts for 5-   \n\
# and 4-letter cell counts.                                     \n\
# \n\
# Note: We actually cycle samples over all 0-31 bit offsets, so \n\
# that if there is a problem with any particular offset it has \n\
# a chance of being observed.  One can imagine problems with odd \n\
# offsets but not even, for example, or only with the offset 7.\n\
# tsamples and psamples can be freely varied, but you'll likely \n\
# need tsamples >> 100,000 to have enough to get a reliable kstest \n\
# result. \n\
#==================================================================\n");

}
