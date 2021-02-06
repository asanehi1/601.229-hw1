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
// this was included to help test code + print
//#include <inttypes.h>
#include <stdio.h>

ApInt *apint_create_from_u64(uint64_t val) {
  ApInt * newApInt = (ApInt*)malloc(sizeof(ApInt));
  newApInt->len = 1;
  newApInt->flags = 0;
  //printf("before %" PRIu64 "\n", val);
  
  newApInt->data = (uint64_t*)malloc(sizeof(uint64_t));
 
  newApInt->data[0] = __bswap_64(val);

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

/* Deallocates the memory used by the ApInt instance pointed-to by the ap parameter */
void apint_destroy(ApInt *ap) {
  free(ap->data);
  free(ap);
}

/* Returns 1 if the ApInt instance whose address is passed is numerically equal to 0
 returns 0 if the ApInt has a non-zero value (positive or negative */
int apint_is_zero(const ApInt *ap) {
  if (ap->data[0] != 0){
    return 0;
  }
  return 1;
}

/* Returns 1 if the ApInt instance whose address is passed is negative (numerically less than 0)
 returns 0 otherwise (if the ApInt is non-negative) */
int apint_is_negative(const ApInt *ap) {
  if (ap->flags == 1) {
    return 1;
  }
  return 0;
}

uint64_t apint_get_bits(const ApInt *ap, unsigned n) {
  // ignore sign
  uint64_t temp = __bswap_64(ap->data[n]);
  //uint64_t temp = (ap->data[n]);
  return  temp;
}

/* return which digit has highest bit */
int apint_highest_bit_set(const ApInt *ap) {
  uint64_t temp = __bswap_64(ap->data[0]);
  //uint64_t temp = (ap->data[0]);
  if (temp == 0) {
    return -1;
  }
  // highet bit stores the value of highest bit
  // + updates when necessary
  int highestBit = 0;
  for (int i = 0; i < (int) ap->len * 64; i++) {
    // if bit == 1
    if (temp & 1) {
      highestBit = i;
    }
    // shift 
    temp = temp >> 1;
  }
  return highestBit;
}

char get_hex_char(int num) {
  switch(num) {
    case 0: return '0'; break;
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

void print_binary(uint64_t w) {
  for (int i = 0; i < 1 * 64; i++)
  {
    if(w & 1) {
      printf("1");
    } else {
      printf("0");
    }

    w>>=1;
  }

  printf("\n");
}

char *apint_format_as_hex(const ApInt *ap) {
  char* hex = (char*)malloc(sizeof(char) * 17);
  int result = 0;
  uint64_t temp = ap->data[0];

  //print_binary(temp);

  temp = __bswap_64(temp);

  //print_binary(temp);


  for(int i = 0, j = 0; i < ap->len * 64; i+=4, j++)  {
    result = temp & 0xf;

    printf("res %d: %d\n", i, result);

    hex[j] =  get_hex_char(result);

    temp >>= 4;

  }

  printf("\n %s\n", hex);

  int hex_length = strlen(hex) - 1;
  int l = strlen(hex);

  for (int i = 0; i < l; i+=2)
  {
    char s = hex[hex_length];
    hex[hex_length] = hex[i];
    hex[i] = s;
    hex_length-=2;
  }
  

  printf("\n %s", hex);

  return hex;
}

ApInt *apint_negate(const ApInt *ap) {
  ApInt *newApInt = (ApInt*)malloc(sizeof(ApInt));
  newApInt->len = ap->len;
  newApInt->data = (uint64_t*)malloc(sizeof(uint64_t));
  newApInt->data[0] = ap->data[0];
  if (ap->data[0] == 0) {
    return newApInt;
  } else if(ap->flags == 0) {
    newApInt->flags = 1;
  } else {
    newApInt->flags = 0;
  }
  // apint_destroy(ap);
  return newApInt;
}

uint64_t unsigned_add(const uint64_t a, const uint64_t b) {
  return a + b;
}

uint64_t unsigned_sub(const uint64_t a, const uint64_t b) {
  return a - b;
}

ApInt *apint_add(const ApInt *a, const ApInt *b) {
  uint64_t result;

  ApInt * newApInt = (ApInt*)malloc(sizeof(ApInt));
  newApInt->data = (uint64_t*)malloc(sizeof(uint64_t));
  newApInt->len = 1;
  
  if (a->flags == 0 && b->flags == 0) {
    // both positive 
    result = unsigned_add(a->data[0], b->data[0]);
    newApInt->flags = 0;
  } else if (a -> flags ==  b->flags) {
    // both are negative 
    result = unsigned_add(a->data[0], b->data[0]);
    newApInt->flags = 1;
  } else if (a->data[0] < b->data[0] && a->flags == 1) {
    // a < b and a is negative and b is positive
    result = unsigned_sub(b->data[0], a->data[0]);
    newApInt->flags = 0;
  }  else if (a->data[0] < b->data[0]) {
    // a < b and a is positive while b is neg
    result = unsigned_sub(b->data[0], a->data[0]);
    newApInt->flags = 1;
  } else if (a->data[0] > b->data[0] && a->flags == 1) {
     // a > b and a is neg  while b is pos
    result = unsigned_sub(a->data[0], b->data[0]);
    newApInt->flags = 1;
  } else {
    // a > b and a is pos  while b is neg
    result = unsigned_sub(a->data[0], b->data[0]);
    newApInt->flags = 0;
  } 
  

  newApInt->data[0] = result;
  return newApInt;
}

ApInt *apint_sub(const ApInt *a, const ApInt *b) {
   uint64_t result;

  ApInt * newApInt = (ApInt*)malloc(sizeof(ApInt));
  newApInt->data = (uint64_t*)malloc(sizeof(uint64_t));
  newApInt->len = 1;
  
  if (a->data[0] > b->data[0] && a->flags == b->flags) {
    //a > b  
    result = unsigned_sub(a->data[0], b->data[0]);
    // if both positive, sign = positive
    // if both negative, sign = negative
    newApInt->flags = a->flags;
  } else if (a->data[0] > b->data[0]) {
    // a > b and signs r opposite 
    result = unsigned_add(a->data[0], b->data[0]);
    newApInt->flags = a->flags;
  } else if (a->data[0] < b->data[0] && a->flags == b->flags) {
    //a < b  
    result = unsigned_sub(b->data[0], a->data[0]);
    // opposite sign
    newApInt->flags = (a->flags + 1) % 2;
  } else if (a->data[0] < b->data[0]) {
    // a > b and signs r opposite 
    result = unsigned_add(a->data[0], b->data[0]);
    newApInt->flags = a->flags;
  } else if (apint_compare(a, b) == 0) {
    result = 0;
    newApInt->flags = 0;
  } else {
    result = a->data[0] * 2;
    newApInt->flags = a->flags;
  }
  

  newApInt->data[0] = result;
  return newApInt;
}

/* compare left and right 
 * return 0 if same 
 * return 1 if left > right 
 * return -1 if left < right
 */
int apint_compare(const ApInt *left, const ApInt *right) {
  if (left->data[0] == right->data[0] && left->flags == right->flags) {
    return 0;
  } else if (left->data[0] < right->data[0] && right->flags == 0) {
    return -1;
  } else if (right->data[0] < left->data[0] && left->flags == 0) {
    return 1;
  } else if (left->data[0] >= right->data[0] && left->flags == 1) {
    return -1;
  } else {
    return 1;
  }
}
