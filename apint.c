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
  ApInt * newApInt = malloc(sizeof(ApInt*));
  newApInt->len = 1;
  newApInt->flags = 0;
  // printf("before %" PRIu64 "\n", val);
  // bswap makes val (in big endian form) into little endian form for data
  // val = (__bswap_64(val));
  newApInt->data = (uint64_t * )val;
  //printf("after %" PRIu64 "\n", val);
  return newApInt;
}

ApInt *apint_create_from_hex(const char *hex) {
	/* TODO: implement */
  uint64_t temp = strtoull(hex, NULL,  64);
  //printf("hex: %s",hex);
  //printf("uint64_t temp: %lu", temp);

  return apint_create_from_u64(temp);
}

void apint_destroy(ApInt *ap) {
	/* TODO: implement */
  free(ap);
}

/* Returns 1 if the ApInt instance whose address is passed is numerically equal to 0, and returns 0 if the ApInt has a non-zero value (positive or negative */
int apint_is_zero(const ApInt *ap) {
  if (ap->data != 0){
    return 0;
  }
  return 1;
}

/* Returns 1 if the ApInt instance whose address is passed is negative (numerically less than 0), and returns 0 otherwise (if the ApInt is non-negative) */
int apint_is_negative(const ApInt *ap) {
  if (ap->data < 0) {
    return 1;
  }
  return 0;
}

uint64_t apint_get_bits(const ApInt *ap, unsigned n) {
	/* TODO: implement */
  // ignore sign
  // add code for n
  uint64_t temp = (uint64_t) (ap->data);
  //temp = __bswap_64(temp);
  return  temp;
}

int apint_highest_bit_set(const ApInt *ap) {
	/* TODO: implement */
  uint64_t temp = (uint64_t) ap->data;
  if (temp == 0) {
    return -1;
  }
  int highestBit = 0;
  for (int i = 0; i < (int) ap->len * 64; i++) {
    if (temp & 0x01) {
      highestBit = i;
    }
    temp = temp >> 1;
  }
  return highestBit;
}

char *apint_format_as_hex(const ApInt *ap) {
	/* TODO: implement */
	assert(0);
	return NULL;
}

ApInt *apint_negate(const ApInt *ap) {
  ApInt *newApInt = malloc(sizeof(ApInt));
  if (ap->data == 0) {
    newApInt->data = 0;
    return newApInt;
  } else if(ap->flags == 0) {
    newApInt->flags = 1;
  } else {
    newApInt->flags = 0;
  }
  return newApInt;
}

ApInt *apint_add(const ApInt *a, const ApInt *b) {
  /* TODO: implement */
  uint64_t temp = (uint64_t)a->data;
  uint64_t temp1 = (uint64_t)b->data;
  uint64_t result = temp + temp1;
  printf("a %" PRIu64 "\n", temp);
  printf("b %" PRIu64 "\n", temp1);
  printf("a+b %" PRIu64 "\n", result);
  ApInt * newApInt = malloc(sizeof(ApInt));
  newApInt->len = 1;
  newApInt->flags = 0;
  newApInt->data = (uint64_t*)result;
  assert(0);
  return newApInt;
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
