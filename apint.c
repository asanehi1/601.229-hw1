/*
 * CSF Assignment 1
 * Arbitrary-precision integer data type
 * Function implementations
 */

#include "apint.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <byteswap.h>
#include <inttypes.h>
#include <stdio.h>

ApInt *apint_create_from_u64(uint64_t val) {
  /* TODO: implement */
  // needs help! Values get overwritten 
  ApInt * newApInt = malloc(sizeof(ApInt));
  newApInt->len = 1;
  newApInt->flags = 0;
  printf("before %" PRIu64 "\n", val);
  // bswap makes val (in big endian form) into little endian form for data
  val = (__bswap_64(val));
  newApInt->data = (uint64_t *)&val;
  printf("after %" PRIu64 "\n", val);
  return newApInt;
}

ApInt *apint_create_from_hex(const char *hex) {
	/* TODO: implement */
  uint64_t temp = strtoull(hex, NULL,  64);
	return NULL;
}

void apint_destroy(ApInt *ap) {
	/* TODO: implement */
  free(ap);
}

int apint_is_zero(const ApInt *ap) {
	/* TODO: implement */
	assert(0);
	return 0;
}

int apint_is_negative(const ApInt *ap) {
	/* TODO: implement */
	assert(0);
	return 0;
}

uint64_t apint_get_bits(const ApInt *ap, unsigned n) {
	/* TODO: implement */
  // ignore sign
  // add code for n
  uint64_t temp = *(ap->data);
  return  __bswap_64(temp);
}

int apint_highest_bit_set(const ApInt *ap) {
	/* TODO: implement */
	assert(0);
	return -1;
}

char *apint_format_as_hex(const ApInt *ap) {
	/* TODO: implement */
	assert(0);
	return NULL;
}

ApInt *apint_negate(const ApInt *ap) {
	/* TODO: implement */
	assert(0);
	return NULL;
}

ApInt *apint_add(const ApInt *a, const ApInt *b) {
	/* TODO: implement */
	assert(0);
	return NULL;
}


ApInt *apint_sub(const ApInt *a, const ApInt *b) {
	/* TODO: implement */
	assert(0);
	return NULL;
}

int apint_compare(const ApInt *left, const ApInt *right) {
	/* TODO: implement */
	assert(0);
	return 0;
}
