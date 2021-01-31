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
  if (ap->flags == 1) {
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

char get_hex_char(int num) {
  switch(num) {
  case 1: return '1'; break;
  case 2: return '2'; break;
  case 3: return '3'; break;
  case 4: return '4'; break;
  case 5: return '5'; break;
  case 6: return '6'; break;
  case 7: return '7'; break;
  case 8: return '8'; break;
  case 9: return '9'; break;
  case 10: return 'a'; break;
  case 11: return 'b'; break;
  case 12: return 'c'; break;
  case 13: return 'd'; break;
  case 14: return 'e'; break;
  default: return 'f'; break;
  }
}

int apint_to_hex_calculations(int pos) {
  switch(pos) {
  case 0: return 1; break;
  case 1: return 2; break;
  case 2: return 4; break;
  default: return 8; break;
  }
}

char *apint_format_as_hex(const ApInt *ap) {
	/* TODO: implement */

  printf("f1\n");
  char* hex = NULL;
  printf("f2\n");
  uint64_t temp = (uint64_t*)ap->data;
  int j = 0;
  int sum = 0;
  for (int i = 0, k = 0; i < (int) ap->len * 64; i++) {
    printf("f3 bit index: %d \n",i );
    printf("before sum: %d \n", sum);
    if(j < 4) {
      printf("f4\n");
      if(temp == 1) {
	printf("f5\n");
	sum =sum + apint_to_hex_calculations(j);
	printf("new sum: %d \n", sum);
      }
      printf("f6\n");

      if (j == 3) {
	printf("f7\n");
	printf("sum: %d \n", sum);
	//if(sum > 9) {
	  printf("f8\n");
	  hex[k++] = get_hex_char(sum);
	  printf("f9\n");
	 
	  //}
	//hex[k++] = sum;
	printf("f10\n");

	j = 0; sum = 0;
      } else { j++;}
      
    }
    printf("f11\n");
    if (i + 1 == (int) ap->len*64) {
      printf("f12\n");
      //if(sum > 9) {
	printf("f13\n");
	hex[k++] = get_hex_char(sum);
	printf("f14\n");
	
	//}
      //      hex[k] = sum;
      // printf("f15\n");
      
    }
    temp = temp >> 1;
    printf("f16a");
  }
  
  
  return hex;
}

ApInt *apint_negate(const ApInt *ap) {
  ApInt *newApInt = malloc(sizeof(ApInt));
  newApInt->len = ap->len;
  newApInt->data = ap->data;
  if (ap->data == 0) {
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

  //for testing
  printf("a %" PRIu64 "\n", temp);
  printf("b %" PRIu64 "\n", temp1);
  printf("a+b %" PRIu64 "\n", result);

  ApInt * newApInt = malloc(sizeof(ApInt));
  newApInt->len = 1;
  newApInt->flags = 0;
  newApInt->data = (uint64_t*)result;
  return newApInt;
}


ApInt *apint_sub(const ApInt *a, const ApInt *b) {
	/* TODO: implement */
  uint64_t temp = (uint64_t)a->data;
  uint64_t temp1 = (uint64_t)b->data;
  uint64_t result = temp - temp1;

  //for testing
  printf("a %" PRIu64 "\n", temp);
  printf("b %" PRIu64 "\n", temp1);
  printf("a-b %" PRIu64 "\n", result);

  ApInt * newApInt = malloc(sizeof(ApInt));
  newApInt->len = 1;
  newApInt->flags = 0;
  newApInt->data = (uint64_t*)result;
  return newApInt;
}

int apint_compare(const ApInt *left, const ApInt *right) {
	/* TODO: implement */
  if (left->data == right->data && left->flags == right->flags) {
    return 0;
  } else if (left->data < right->data && right->flags == 0) {
    return -1;
  } else if (right->data < left->data && left->flags == 0) {
    return 1;
  } else if (left->data >= right->data && left->flags == 1) {
    return -1;
  } else {
    return 1;
  }
}
