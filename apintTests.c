
/*
 * Unit tests for arbitrary-precision integer data type
 *
 * These tests are by no means comprehensive.  You will need to
 * add more tests of your own!  In particular, make sure that
 * you have tests for more challenging situations, such as
 *
 * - large values
 * - adding/subtracting/comparing values with different lengths
 * - special cases (carries when adding, borrows when subtracting, etc.)
 * - etc.
 */


//1000
//1000 +
//-------
//2000

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "apint.h"
#include "tctest.h"
#include <inttypes.h>

typedef struct {
	ApInt *ap0;
	ApInt *ap1;
	ApInt *ap110660361;
	ApInt *max1;
	ApInt *minus1;
	/* TODO: add additional fields of test fixture */
        ApInt *minus110660361;
   ApInt *large;
} TestObjs;

TestObjs *setup(void);
void cleanup(TestObjs *objs);

void testCreateFromU64(TestObjs *objs);
void testHighestBitSet(TestObjs *objs);
void testCompare(TestObjs *objs);
void testFormatAsHex(TestObjs *objs);
void testAdd(TestObjs *objs);
void testSub(TestObjs *objs);
/* TODO: add more test function prototypes */

void testIsNegative(TestObjs *objs);
void testIsZero(TestObjs *objs);
void basicAdd(TestObjs *objs);
void basicSub(TestObjs *objs);
void testCreateFromHex(TestObjs *objs);

int main(int argc, char **argv) {
	TEST_INIT();

	if (argc > 1) {
		/*
		 * name of specific test case to execute was provided
		 * as a command line argument
		 */
		tctest_testname_to_execute = argv[1];
	}
	
       	TEST(testCreateFromU64);
	TEST(testHighestBitSet);
	TEST(testCompare);
	TEST(testFormatAsHex);
	TEST(testSub);
	TEST(testAdd);
	/* TODO: use TEST macro to execute more test functions */
	TEST(testIsNegative);
	TEST(testIsZero);
	TEST(basicAdd);
	TEST(basicSub);
	TEST(testCreateFromHex);
	TEST_FINI();
}

TestObjs *setup(void) {
	TestObjs *objs = malloc(sizeof(TestObjs));
	objs->ap0 = apint_create_from_u64(0UL);	
	objs->ap1 = apint_create_from_u64(1UL);
	objs->ap110660361 = apint_create_from_u64(110660361UL);
	objs->max1 = apint_create_from_u64(0xFFFFFFFFFFFFFFFFUL);
	objs->minus1 = apint_negate(objs->ap1);
	/* TODO: initialize additional members of test fixture */

	objs->minus110660361 = apint_negate(objs->ap110660361);
	objs->large = apint_create_from_hex("10000000000000000");
 
	return objs;
}

void cleanup(TestObjs *objs) {
  apint_destroy(objs->ap0);
  apint_destroy(objs->ap1);
  apint_destroy(objs->ap110660361);
  apint_destroy(objs->max1);
  apint_destroy(objs->minus1);
  apint_destroy(objs->minus110660361);
	/* TODO: destroy additional members of test fixture */
  apint_destroy(objs->large);
	free(objs);
}

void testCreateFromU64(TestObjs *objs) {
  //uint64_t temp = apint_get_bits(objs->ap1, 0);
  //printf("final " "%" PRIu64 "\n", temp);
  ASSERT(0UL == apint_get_bits(objs->ap0, 0));

  ASSERT(1UL == apint_get_bits(objs->ap1, 0));
  
  ASSERT(110660361UL == apint_get_bits(objs->ap110660361, 0));
  ASSERT(0xFFFFFFFFFFFFFFFFUL == apint_get_bits(objs->max1, 0));
  ASSERT(110660361UL == apint_get_bits(objs->ap110660361, 0));
  ASSERT(0xFFFFFFFFFFFFFFFFUL == apint_get_bits(objs->max1, 0));
  
}




void testHighestBitSet(TestObjs *objs) {
	ASSERT(-1 == apint_highest_bit_set(objs->ap0));
	ASSERT(0 == apint_highest_bit_set(objs->ap1));
	ASSERT(26 == apint_highest_bit_set(objs->ap110660361));
	ASSERT(63 == apint_highest_bit_set(objs->max1));
}

void testCompare(TestObjs *objs) {
	/* 1 > 0 */
	ASSERT(apint_compare(objs->ap1, objs->ap0) > 0);
	/* 0 < 1 */
	ASSERT(apint_compare(objs->ap0, objs->ap1) < 0);
	/* 110660361 > 0 */
	ASSERT(apint_compare(objs->ap110660361, objs->ap0) > 0);
	/* 110660361 > 1 */
	ASSERT(apint_compare(objs->ap110660361, objs->ap1) > 0);
	/* 0 < 110660361 */
	ASSERT(apint_compare(objs->ap0, objs->ap110660361) < 0);
	/* 1 < 110660361 */
	ASSERT(apint_compare(objs->ap1, objs->ap110660361) < 0);
	/* -1 < 1 */
	ASSERT(apint_compare(objs->minus1, objs->ap1) < 0);
	/* 1 > -1 */
	ASSERT(apint_compare(objs->ap1, objs->minus1) > 0);
	/* -110660361 < -1 */
      	ASSERT(apint_compare(objs->minus110660361, objs->minus1) < 0);
	/* -1 > -110660361 */
       	ASSERT(apint_compare(objs->minus1, objs->minus110660361) > 0);
	
	// compare large numbers
	ApInt *sum = apint_add(objs->max1, objs->ap1);
	ASSERT(apint_compare(sum, objs->ap0) > 0);
	  
	
}

void testFormatAsHex(TestObjs *objs) {
        ApInt *ap;
	char *s;

        ASSERT(0 == strcmp("0", (s = apint_format_as_hex(objs->ap0))));
	free(s);

	ASSERT(0 == strcmp("1", (s = apint_format_as_hex(objs->ap1))));
	free(s);

	ASSERT(0 == strcmp("-1", (s = apint_format_as_hex(objs->minus1))));
	free(s);

	ASSERT(0 == strcmp("6988b09", (s = apint_format_as_hex(objs->ap110660361))));
	free(s);

	ASSERT(0 == strcmp("ffffffffffffffff", (s = apint_format_as_hex(objs->max1))));
	free(s);

	ASSERT(0 == strcmp("-6988b09", (s = apint_format_as_hex(objs->minus110660361))));
	free(s);

	ap = apint_negate(objs->max1);
	ASSERT(ap->flags == 1);
	ASSERT(0 == strcmp("-ffffffffffffffff", (s = apint_format_as_hex(ap))));
	apint_destroy(ap);
	free(s);

	ASSERT(0 == strcmp("10000000000000000", (s = apint_format_as_hex(objs->large))));
	free(s);
	

	ap = apint_create_from_hex("64000000000000000000000000000000000000000000000000000");
	ASSERT(0 == strcmp("64000000000000000000000000000000000000000000000000000", (s = apint_format_as_hex(ap))));
	free(s);
	apint_destroy(ap);

	ap = apint_negate(objs->large);
	ASSERT(ap->flags == 1);
	ASSERT(0 == strcmp("-10000000000000000", (s = apint_format_as_hex(ap))));
	apint_destroy(ap);
	free(s);

       	ap = apint_create_from_hex("FFFFFFFFFFFFFFFF");
	ASSERT(0 == strcmp("ffffffffffffffff", (s = apint_format_as_hex(ap))));
	apint_destroy(ap);
	free(s);

       	ap = apint_create_from_hex("-00000006988b09");
	ASSERT(0 == strcmp("-6988b09", (s = apint_format_as_hex(ap))));
	ASSERT(ap->flags == 1);
	apint_destroy(ap);
       	free(s);


	ap = apint_create_from_hex("-64000000000000000000000000000000000000000000000000000");
	ASSERT(0 == strcmp("-64000000000000000000000000000000000000000000000000000", (s = apint_format_as_hex(ap))));
	ASSERT(ap->flags == 1);
	apint_destroy(ap);
       	free(s);


       	ap = apint_create_from_hex("-000000000");
	ASSERT(0 == strcmp("0", (s = apint_format_as_hex(ap))));
	ASSERT(ap->flags == 0);
	apint_destroy(ap);
	free(s);

	

	ap = apint_create_from_hex("239AC7936764D88B15");
	ASSERT(0 == strcmp("239ac7936764d88b15", (s = apint_format_as_hex(ap))));
	apint_destroy(ap);
	free(s);

	ap = apint_create_from_hex("21F487FC8F46A89303B1180");
	ASSERT(0 == strcmp("21f487fc8f46a89303b1180", (s = apint_format_as_hex(ap))));
	apint_destroy(ap);
	free(s);

        ap = apint_create_from_hex("-143D26FFE70C2F2164BF284F73180");
	ASSERT(0 == strcmp("-143d26ffe70c2f2164bf284f73180", (s = apint_format_as_hex(ap))));
	apint_destroy(ap);
	free(s);

       	ap = apint_create_from_hex("B8124E58E0A58721DE1775C437389361010F6DA");
	ASSERT(0 == strcmp("b8124e58e0a58721de1775c437389361010f6da", (s = apint_format_as_hex(ap))));
	apint_destroy(ap);
	free(s);

       	ap = apint_create_from_hex("1044D1A3CC031703BC53AF74CDC4718C3FB76620891BD1A468A9753D57A8862B41F4B1");
	ASSERT(0 == strcmp("1044d1a3cc031703bc53af74cdc4718c3fb76620891bd1a468a9753d57a8862b41f4b1", (s = apint_format_as_hex(ap))));
	apint_destroy(ap);
	free(s);


	ap = apint_create_from_hex("1044D1A3CC031703BC53AF74CDC4718C3FB76620891BD1A468A9753D57A8862B41F4B1");
	ASSERT(0 == strcmp("1044d1a3cc031703bc53af74cdc4718c3fb76620891bd1a468a9753d57a8862b41f4b1", (s = apint_format_as_hex(ap))));
	apint_destroy(ap);
	free(s);
	

	ap = apint_create_from_hex("f000000000000000f");
	ASSERT(0 == strcmp("f000000000000000f", (s = apint_format_as_hex(ap))));
	apint_destroy(ap);
	free(s);

	ap = apint_create_from_hex("-1044D1A3CC031703BC53AF74CDC4718C3FB76620891BD1A468A9753D57A8862B41F4B1");
	ASSERT(0 == strcmp("-1044d1a3cc031703bc53af74cdc4718c3fb76620891bd1a468a9753d57a8862b41f4b1", (s = apint_format_as_hex(ap))));
	apint_destroy(ap);
	free(s);

	ap = apint_create_from_hex("2734e16f26eefd72103de3c6c0036a66998e72fc6116af0775279240b3c4bec7b768e855b317c748384219f05fb70ae1b13c960f117c00a1dd1ed77ad9dc1988677f658975f576507c57b6da57c686b1d34db524e621fdeef71cea1b5479b2cbf24ea39797433d57a8862b41f4b1");
	ASSERT(0 == strcmp("2734e16f26eefd72103de3c6c0036a66998e72fc6116af0775279240b3c4bec7b768e855b317c748384219f05fb70ae1b13c960f117c00a1dd1ed77ad9dc1988677f658975f576507c57b6da57c686b1d34db524e621fdeef71cea1b5479b2cbf24ea39797433d57a8862b41f4b1", (s = apint_format_as_hex(ap))));
	apint_destroy(ap);
	free(s);

	ap = apint_create_from_hex("-2734e16f26eefd72103de3c6c0036a66998e72fc6116af0775279240b3c4bec7b768e855b317c748384219f05fb70ae1b13c960f117c00a1dd1ed77ad9dc1988677f658975f576507c57b6da57c686b1d34db524e621fdeef71cea1b5479b2cbf24ea39797433d57a8862b41f4b1");
	ASSERT(0 == strcmp("-2734e16f26eefd72103de3c6c0036a66998e72fc6116af0775279240b3c4bec7b768e855b317c748384219f05fb70ae1b13c960f117c00a1dd1ed77ad9dc1988677f658975f576507c57b6da57c686b1d34db524e621fdeef71cea1b5479b2cbf24ea39797433d57a8862b41f4b1", (s = apint_format_as_hex(ap))));
	apint_destroy(ap);
	free(s);

	ap = apint_create_from_hex("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff00000000000000000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
	ASSERT(0 == strcmp("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff00000000000000000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff", (s = apint_format_as_hex(ap))));
	apint_destroy(ap);
	free(s);

	ap = apint_create_from_hex("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff00fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
	ASSERT(0 == strcmp("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff00fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff", (s = apint_format_as_hex(ap))));
	apint_destroy(ap);
	free(s);

	ap = apint_create_from_hex("49265742CB3BD3BD5B070E3DD8607F7DD3062EC589EB7F501B8CD526FEAC23E01F0C967D656E4A8DE");
	ASSERT(0 == strcmp("49265742cb3bd3bd5b070e3dd8607f7dd3062ec589eb7f501b8cd526feac23e01f0c967d656e4a8de", (s = apint_format_as_hex(ap))));
	apint_destroy(ap);
	free(s);

	ap = apint_create_from_hex("583B57AE9ED6E3947C3381F506D5459404901477738AF0000000000000000");
	ASSERT(0 == strcmp("583b57ae9ed6e3947c3381f506d5459404901477738af0000000000000000", (s = apint_format_as_hex(ap))));
	apint_destroy(ap);
	free(s);

	 ap = apint_create_from_hex("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
	ASSERT(0 == strcmp("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff", (s = apint_format_as_hex(ap))));
	apint_destroy(ap);
	free(s);

	 ap = apint_create_from_hex("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
	ASSERT(0 == strcmp("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff", (s = apint_format_as_hex(ap))));
	apint_destroy(ap);
	free(s);
	
}



void testAdd(TestObjs *objs) {
    ApInt *sum, *a, *b;
    char *s;
    
    /* 0 + 0 = 0 */
    sum = apint_add(objs->ap0, objs->ap0);
    ASSERT(0 == strcmp("0", (s = apint_format_as_hex(sum))));
    apint_destroy(sum);
    free(s);

    /* 1 + 0 = 1 */
    sum = apint_add(objs->ap1, objs->ap0);
    ASSERT(0 == strcmp("1", (s = apint_format_as_hex(sum))));
    apint_destroy(sum);
    free(s);

    /* 1 + 1 = 2 */
    sum = apint_add(objs->ap1, objs->ap1);
    ASSERT(0 == strcmp("2", (s = apint_format_as_hex(sum))));
    apint_destroy(sum);
    free(s);
    

    // /* 110660361 + 1 = 110660362 */
    sum = apint_add(objs->ap110660361, objs->ap1);
    ASSERT(0 == strcmp("6988b0a", (s = apint_format_as_hex(sum))));
    apint_destroy(sum);
    free(s);

    /* FFFFFFFFFFFFFFFF + 0 */
    sum = apint_add(objs->max1, objs->ap0);
    //printf("%s \n", apint_format_as_hex(sum));
    ASSERT(0 == strcmp("ffffffffffffffff", (s = apint_format_as_hex(sum))));
    apint_destroy(sum);
    free(s);
    

    /* FFFFFFFFFFFFFFFF + 1 = 10000000000000000 */
    sum = apint_add(objs->max1, objs->ap1);
    ASSERT(0 == strcmp("10000000000000000", (s = apint_format_as_hex(sum))));
    //  printf("data[0]:" PRIu64 "\n", sum->data[0]);
    //  printf("data[1]:" PRIu64 "\n", sum->data[1]);
    
    apint_destroy(sum);
    free(s);

    /*- 110660361 +- 1 = -110660362 */
    sum = apint_add(objs->minus110660361, objs->minus1);
    ASSERT(0 == strcmp("-6988b0a", (s = apint_format_as_hex(sum))));
    apint_destroy(sum);
    free(s);

    sum = apint_add(objs->large, objs->large);
    ASSERT(0 == strcmp("20000000000000000", (s = apint_format_as_hex(sum))));
    apint_destroy(sum);
    free(s);

    a = apint_create_from_hex("ffffffffffffffffffffffffffffffff");
    b = apint_create_from_hex("1");
    sum = apint_add(a, b);
    s = apint_format_as_hex(sum);
    printf("string: %s\n", s);
    ASSERT(0 == strcmp("100000000000000000000000000000000", s));
    apint_destroy(sum);
    apint_destroy(b);
    apint_destroy(a);
    free(s);

    // a = apint_create_from_hex("              193abd5f2ba62791063bad");
    // b = apint_create_from_hex("189073badc26db28ba910a628b7db2a8d390");
    // sum = apint_add(a, b);
    // s = apint_format_as_hex(sum);
    // printf("string: %s\n", s);
    // ASSERT(0 == strcmp("3d", s));
    // apint_destroy(sum);
    // apint_destroy(b);
    // apint_destroy(a);
    // free(s);

    a = apint_create_from_hex("000000000000000000000000");
    b = apint_create_from_hex("0000000000000000");
    sum = apint_add(a, b);
    s = apint_format_as_hex(sum);
    printf("string: %s\n", s);
    ASSERT(0 == strcmp("0", s));
    apint_destroy(sum);
    apint_destroy(b);
    apint_destroy(a);
    free(s);

    a = apint_create_from_hex("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
    b = apint_create_from_hex("1");
    sum = apint_add(a, b);
    s = apint_format_as_hex(sum);
    printf("string: %s\n", s);
    ASSERT(0 == strcmp("10000000000000000000000000000000000000000000000000000000000000000", s));
    apint_destroy(sum);
    apint_destroy(b);
    apint_destroy(a);
    free(s);

    // a = apint_create_from_hex("-ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
    // b = apint_create_from_hex("1");
    // sum = apint_add(a, b);
    // s = apint_format_as_hex(sum);
    // printf("string: %s\n", s);
    // ASSERT(0 == strcmp("10000000000000000000000000000000000000000000000000000000000000000", s));
    // apint_destroy(sum);
    // apint_destroy(b);
    // apint_destroy(a);
    // free(s);

    a = apint_create_from_hex("fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffb");
    b = apint_create_from_hex("4");
    sum = apint_add(a, b);
    s = apint_format_as_hex(sum);
    printf("string: %s\n", s);
    ASSERT(0 == strcmp("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff", s));
    apint_destroy(sum);
    apint_destroy(b);
    apint_destroy(a);
    free(s);

}



void testSub(TestObjs *objs) {
	ApInt *a, *b, *diff;
	char *s;

	/* subtracting 1 from ffffffffffffffff is fffffffffffffffe */
	diff = apint_sub(objs->max1, objs->ap1);
	ASSERT(0 == strcmp("fffffffffffffffe", (s = apint_format_as_hex(diff))));
	apint_destroy(diff);
	free(s);

	/* subtracting 0 from 1 is 1 */
	diff = apint_sub(objs->ap1, objs->ap0);
	ASSERT(0 == strcmp("1", (s = apint_format_as_hex(diff))));
	ASSERT(0 == apint_compare(diff, objs->ap1));
	apint_destroy(diff);
	free(s);

	/* subtracting 1 from 1 is 0 */
	diff = apint_sub(objs->ap1, objs->ap1);
	ASSERT(0 == strcmp("0", (s = apint_format_as_hex(diff))));
	ASSERT(0 == apint_compare(diff, objs->ap0));
	apint_destroy(diff);
	free(s);

	/* subtracting 1 from 0 is -1 */
	diff = apint_sub(objs->ap0, objs->ap1);
	ASSERT(0 == (strcmp("-1", (s = apint_format_as_hex(diff)))));
	ASSERT(0 == apint_compare(diff, objs->minus1));
	apint_destroy(diff);
	free(s);

	/* test involving larger values */
	a = apint_create_from_hex("7e35207519b6b06429378631ca460905c19537644f31dc50114e9dc90bb4e4ebc43cfebe6b86d");
	b = apint_create_from_hex("9fa0fb165441ade7cb8b17c3ab3653465e09e8078e09631ec8f6fe3a5b301dc");
	diff = apint_sub(a, b);
	ASSERT(0 == strcmp("7e35207519b6afc4883c6fdd8898213a367d73b918de95f20766963b0251c622cd3ec4633b691",
		(s = apint_format_as_hex(diff))));
	apint_destroy(diff);
	apint_destroy(b);
	apint_destroy(a);
	free(s);

	/* test involving larger values (with a negative difference) */
	a = apint_create_from_hex("9fa0fb165441ade7cb8b17c3ab3653465e09e8078e09631ec8f6fe3a5b301dc");
	b = apint_create_from_hex("7e35207519b6b06429378631ca460905c19537644f31dc50114e9dc90bb4e4ebc43cfebe6b86d");
	diff = apint_sub(a, b);
	ASSERT(0 == strcmp("-7e35207519b6afc4883c6fdd8898213a367d73b918de95f20766963b0251c622cd3ec4633b691",
		(s = apint_format_as_hex(diff))));
	apint_destroy(diff);
	apint_destroy(b);
	apint_destroy(a);
	free(s);
}


void testIsZero(TestObjs *objs) {
  ASSERT(1 == apint_is_zero(objs->ap0));
  ASSERT(0 == apint_is_zero(objs->ap1));
  
}

void testIsNegative(TestObjs *objs) {
  ASSERT (0 == apint_is_negative(objs->ap0));
  ASSERT (0 == apint_is_negative(objs->ap1));
  ASSERT (1 == apint_is_negative(objs->minus1));
  
}
  
  
void basicAdd(TestObjs *objs) {
  	ApInt *sum;

	/* 0 + 0 = 0 */
	sum = apint_add(objs->ap0, objs->ap0);
	ASSERT(0 == apint_get_bits(sum, 0));
	ASSERT(sum->flags == 0);
	apint_destroy(sum);
        

	/* 1 + 0 = 1 */
       	sum = apint_add(objs->ap1, objs->ap0);
	ASSERT(1 ==  apint_get_bits(sum, 0));
	ASSERT(sum->flags == 0);
	apint_destroy(sum);


	/* 1 + 1 = 2 */
	sum = apint_add(objs->ap1, objs->ap1);
	ASSERT(2 == apint_get_bits(sum, 0));
	ASSERT(sum->flags == 0);
	apint_destroy(sum);
	

	/* -1 + 1 = 0 */
	printf("hello\n");
	sum = apint_add(objs->minus1, objs->ap1);
	ASSERT(0 == apint_get_bits(sum, 0));
	ASSERT(sum->flags == 0);
	apint_destroy(sum);

	/* 1 + -1 = 0 */
	sum = apint_add(objs->ap1, objs->minus1);
	ASSERT(0 == apint_get_bits(sum, 0));
	ASSERT(sum->flags == 0);
	apint_destroy(sum);

	/* -1 + -1 = -2 */
	sum = apint_add(objs->minus1, objs->minus1);
	ASSERT(2 == apint_get_bits(sum, 0));
	ASSERT(sum->flags == 1);
	apint_destroy(sum);

	/* -110660361 + -1 = -110660362 */
	sum = apint_add(objs->minus110660361, objs->minus1);
	ASSERT(110660362 == apint_get_bits(sum, 0));
	ASSERT(sum->flags == 1);
	apint_destroy(sum);

	/* 110660361 + -1 = 110660360 */
	sum = apint_add(objs->ap110660361, objs->minus1);
	ASSERT(110660360 == apint_get_bits(sum, 0));
	ASSERT(sum->flags == 0);
	apint_destroy(sum);

       	/* -110660361 + 0  = -110660361 */
	sum = apint_add(objs->minus110660361, objs->ap0);
	ASSERT(110660361 == apint_get_bits(sum, 0));
	ASSERT(sum->flags == 1);
	apint_destroy(sum);

	// sum = apint_add(objs->max1, objs->ap1);
	// ASSERT(1 == apint_get_bits(sum, 0));
	// ASSERT(sum->flags == 1);
	// apint_destroy(sum);

}

void basicSub(TestObjs *objs) {
  	ApInt *sum;

	/* 0 - 0 = 0 */
	sum = apint_sub(objs->ap0, objs->ap0);
	ASSERT(0 == apint_get_bits(sum, 0));
	ASSERT(sum->flags == 0);
	apint_destroy(sum);
        

	/* 1 - 0 = 1 */
       	sum = apint_sub(objs->ap1, objs->ap0);
	ASSERT(1 ==  apint_get_bits(sum, 0));
	ASSERT(sum->flags == 0);
	apint_destroy(sum);


	/* 1 - 1 = 0 */
	sum = apint_sub(objs->ap1, objs->ap1);
	ASSERT(0 == apint_get_bits(sum, 0));
	ASSERT(sum->flags == 0);
	apint_destroy(sum);
	

	/* -1 - 1 = -2 */
	sum = apint_sub(objs->minus1, objs->ap1);
	ASSERT(2 == apint_get_bits(sum, 0));
	ASSERT(sum->flags == 1);
	apint_destroy(sum);

	/* 1 - -1 = 2 */
	sum = apint_sub(objs->ap1, objs->minus1);
	ASSERT(2 == apint_get_bits(sum, 0));
	ASSERT(sum->flags == 0);
	apint_destroy(sum);

	/* -1 - -1 = 0 */
	sum = apint_sub(objs->minus1, objs->minus1);
	ASSERT(0 == apint_get_bits(sum, 0));
	ASSERT(sum->flags == 0);
	apint_destroy(sum);

	/* -110660361 - -1 = -110660360 */
	sum = apint_sub(objs->minus110660361, objs->minus1);
	ASSERT(110660360 == apint_get_bits(sum, 0));
	ASSERT(sum->flags == 1);
	apint_destroy(sum);

	/* 110660361 - -1 = 110660362 */
	sum = apint_sub(objs->ap110660361, objs->minus1);
	ASSERT(110660362 == apint_get_bits(sum, 0));
	ASSERT(sum->flags == 0);
	apint_destroy(sum);

       	/* -110660361 - 0  = -110660361 */
	sum = apint_sub(objs->minus110660361, objs->ap0);
	ASSERT(110660361 == apint_get_bits(sum, 0));
	ASSERT(sum->flags == 1);
	apint_destroy(sum);
	
}

void testCreateFromHex (TestObjs *objs) {
	ApInt* ap;

	ap = apint_create_from_hex("0");
	ASSERT(0UL == apint_get_bits(ap, 0));
	apint_destroy(ap);

	ap = apint_create_from_hex("00000000000");
	ASSERT(0UL == apint_get_bits(ap, 0));
	apint_destroy(ap);

	 ap = apint_create_from_hex("-00000000000");
	ASSERT(0UL == apint_get_bits(ap, 0));
	ASSERT(ap->flags == 0);
	apint_destroy(ap);

        ap = apint_create_from_hex("-");
	ASSERT(0UL == apint_get_bits(ap, 0));
	apint_destroy(ap);
	

	ap = apint_create_from_hex("-1");
	ASSERT(1UL == apint_get_bits(ap, 0));
	ASSERT(ap->flags == 1);
	apint_destroy(ap);
	
	ap = apint_create_from_hex("6988b09");
	ASSERT(110660361UL == apint_get_bits(ap, 0));
	apint_destroy(ap);

	ap = apint_create_from_hex("ffffffffffffffff");
	ASSERT(0xFFFFFFFFFFFFFFFFUL == apint_get_bits(ap, 0));
         apint_destroy(ap);

	ap = apint_create_from_hex("FFFFFFFFFFFFFFFF");
	ASSERT(0xFFFFFFFFFFFFFFFFUL == apint_get_bits(ap, 0));
        apint_destroy(ap);


	ap = apint_create_from_hex("-000000000000000000000000000000000000000000000000000000000000000000ef2345abde789878");
	//17231693203723819128 =  ef2345abde789878 hex
	ASSERT(17231693203723819128 == (ap->data[0]));
	apint_destroy(ap);

	ap = apint_create_from_hex("-00000000006988b09");
	ASSERT(110660361UL == apint_get_bits(ap, 0));
	ASSERT(ap->flags == 1);
        apint_destroy(ap);

	ap = apint_negate(objs->large);
	ASSERT(ap->flags == 1);
	ASSERT(0000000000000000 == ap->data[0]);
	ASSERT(1 == (ap->data[1]));
	apint_destroy(ap);
	
	ap = apint_create_from_hex("-10000000000000000");
	ASSERT(0000000000000000 == ap->data[0]);
	ASSERT(1 == (ap->data[1]));
	ASSERT(ap->flags == 1);
	apint_destroy(ap);

	ap = apint_create_from_hex("640000000000000000");
       	ASSERT(0000000000000000 == ap->data[0]);
	// 100 = 64 hex
	ASSERT(100 == (ap->data[1]));	
	apint_destroy(ap);

	ap = apint_create_from_hex("64000000000000000000000000000000000000000000000000000");
	ASSERT(0000000000000000 == ap->data[0]);
	ASSERT(0000000000000000 == ap->data[1]);
	ASSERT(0000000000000000 == ap->data[2]);
	// 18020995579248640(little endian) = 409600 (big endian)  = 64000 hex
	ASSERT(409600 == ap->data[3]);
	apint_destroy(ap);

	

}
