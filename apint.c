


/*
 * CSF Assignment 1
 * Arbitrary-precision integer data type
 * Function implementations
 */

#include "apint.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <ctype.h>
#include <byteswap.h>
// this was included to help test code + print
#include <inttypes.h>
#include <stdio.h>

ApInt *apint_create_from_u64(uint64_t val) {
  ApInt * newApInt = (ApInt*)malloc(sizeof(ApInt));
  newApInt->len = 1;
  newApInt->flags = 0; 
  newApInt->data = (uint64_t*)malloc(sizeof(uint64_t));
  newApInt->data[0] = __bswap_64(val);
  return newApInt;
}

ApInt *apint_create_from_hex(const char *hex) {
	ApInt * newApInt = (ApInt*)malloc(sizeof(ApInt));
  newApInt->len = 1;
  newApInt->data = (uint64_t*)malloc(sizeof(uint64_t));
  newApInt->data[0] = __bswap_64(strtoull(hex, NULL,  16));
  //print_binary(newApInt->data[0]);

  if(hex[0] == '-') {
    newApInt->flags = 1;
  } else {
    newApInt->flags = 0;
  }

  return newApInt;
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

char * swap_hex_values(char *hex) {
  //printf("before swap: %s\n", hex);
  int end = strlen(hex) - 1;
  int start = 0;
  while (start < end) {
    char temp = hex[start];
    hex[start++] = hex[end];
    hex[end--] = temp;
  }

  printf("swapped: %s\n", hex);

  return hex;
}

void print_binary(uint64_t w) {
  printf("binary: ");
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
  char* hex = (char*) malloc(sizeof(char) * 17);
  
  for (int i = 0; i < (int) ap->len; i++) {
    uint64_t temp = __bswap_64(ap->data[i]);
    printf("\nin format function: ");
    print_binary(temp);
    int remainder = 0;
    int counter = 0;

    if(ap->flags == 1 && i == 0) {
      hex[counter++] = '-';
    }

    do {
      remainder = temp % 16;
      //remainder = temp & 15;
      //printf("rem: %d\n", remainder);
      temp /= 16;
      //temp >>= 4;
      hex[counter++] =  get_hex_char(remainder);
    } while ((int) temp != 0);
    
  }
  
  printf("before swap: %s \n", hex);
  return swap_hex_values(hex);
  
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

int get_hex_num(char c) {
  switch(tolower(c)) {
  case '0': return 0; break;
  case '1': return 1; break;
  case '2': return 2; break;
  case '3': return 3; break;
  case '4': return 4; break;
  case '5': return 5; break;
  case '6': return 6; break;
  case '7': return 7; break;
  case '8': return 8; break;
  case '9': return 9; break;
  case 'a': return 10; break;
  case 'b': return 11; break;
  case 'c': return 12; break;
  case 'd': return 13; break;
  case 'e': return 14; break;
  case 'f': return 15; break;
  default: return 0; break;
  }
}

ApInt* unsigned_add(const ApInt *a, const ApInt *b) {
  ApInt * newApInt = (ApInt*)malloc(sizeof(ApInt));
  newApInt->data = (uint64_t*)malloc(sizeof(uint64_t));
  newApInt->len = 1;
  newApInt->flags = 0;

  char* hex_a = apint_format_as_hex(a);
  char* hex_b = apint_format_as_hex(b);
  char* hex = (char*)malloc(sizeof(char) * 17);

  int length, carry = 0, i;

  if(strlen(hex_a) > strlen(hex_b)) {
    length = strlen(hex_a);
  } else {
    length = strlen(hex_b);
  }

  printf("\n");

  for (i = 0; i < length; i++) {
    printf("a:%c + b:%d = ", hex_a[i], hex_b[i]);
    int num = get_hex_num(hex_a[i]) + get_hex_num(hex_b[i]) + carry;
    printf("%d\n", num);

    if(num > 15) {
      carry = 1;
    } else {
      carry = 0;
    }

    hex[i] = get_hex_char(num % 16);
  }

  // 0000 0000 0000 0000 1

  if (carry == 1) {
    hex[i] = '1';
  }

  hex = swap_hex_values(hex);

  if(strlen(hex) > 15) {
    newApInt->len = (int)strlen(hex) / 8;
  }

  for(int i = 0, n=0; i < newApInt->len; i++, n+=8) {
    char array[16];
    strncpy(array, hex + n, 8);
    newApInt->data[i] = (strtoull(array, NULL,  16));
  }

  free(hex);
  return newApInt;
}

uint64_t unsigned_sub(const uint64_t a, const uint64_t b) {
  return a - b;
}

ApInt *apint_add_n(const ApInt *a, const ApInt *b, const int n, const int overflow) {
  uint64_t result;

  ApInt * newApInt;
  
  if (a->flags == 0 && b->flags == 0) {
    // both positive 
   // is_overflow(a->data[n], b->data[n]);
    
    //result = unsigned_add(a->data[n], b->data[n]);
    newApInt = unsigned_add(a, b);

    //printf("add result  %" PRIu64 "\n", __bswap_64(result));
    //print_binary(result);
    newApInt->flags = 0;
  } 
  // else if (a -> flags ==  b->flags) {
  //   // both are negative 
  //   //result = unsigned_add(a->data[n], b->data[n]);
  //   result = unsigned_add(a, b);
  //   newApInt->flags = 1;
  // } else if (a->data[n] < b->data[n] && a->flags == 1) {
  //   // a < b and a is negative and b is positive
  //   result = unsigned_sub(b->data[n], a->data[n]);
  //   newApInt->flags = 0;
  // }  else if (a->data[n] < b->data[n]) {
  //   // a < b and a is positive while b is neg
  //   result = unsigned_sub(b->data[n], a->data[n]);
  //   newApInt->flags = 1;
  // } else if (a->data[n] > b->data[n] && a->flags == 1) {
  //    // a > b and a is neg  while b is pos
  //   result = unsigned_sub(a->data[n], b->data[n]);
  //   newApInt->flags = 1;
  // } else {
  //   // a > b and a is pos  while b is neg
  //   result = unsigned_sub(a->data[n], b->data[n]);
  //   newApInt->flags = 0;
  // } 


  

  // if (overflow == 1) {
  //   result ++;
  //   printf("overflow new result: ");
  //   printf("%" PRIu64 "\n", __bswap_64(result));
  //   print_binary(result);
  // }
   
  //  if (result < a->data[n] && a->flags == b->flags) {
  //     newApInt->len ++;
  //     printf("big number warning \n");
  //  } 
     

  // newApInt->data[n] = result;
  // return newApInt;

  // if overflows then ap->len ++
  // can't overflow with subtraction for now?
 
}


ApInt *apint_add(const ApInt *a, const ApInt *b) {
  ApInt* newApInt;
  if (a->flags == 0 && b->flags == 0) {
    // both positive 
   // is_overflow(a->data[n], b->data[n]);
    
    //result = unsigned_add(a->data[n], b->data[n]);
    newApInt = unsigned_add(a, b);

    //printf("add result  %" PRIu64 "\n", __bswap_64(result));
    //print_binary(result);
    
  } 

  //return apint_add_n(a, b, 0, 0);
  
  // uint64_t result;
  // int length;

  // ApInt * newApInt = (ApInt*)malloc(sizeof(ApInt));
  // newApInt->data = (uint64_t*)malloc(sizeof(uint64_t));

  // // find largest n
  // if (a->len >= b->len) {
  //   length = a->len;
  // } else {
  //   length = b->len;
  // }
  // int overflow = 0;
  // int tempLength = 1;

  // for (int i = 0; i < length; i++) {
  //   ApInt * temp = apint_add_n(a, b, i, overflow);
  //   newApInt->data[i] = temp->data[i];
  //   newApInt->flags = temp->flags; 
     
  //   if ((int) temp->len > tempLength) {
  //     overflow = 1;
  //     length++;
  //     tempLength = temp->len;
  //     //newApInt->data[x] = apint_add_n(a, b, x, overflow)->data[x];
  //   } else {
  //     overflow = 0;
  //   }
  // }
  // //newApInt->data[0] = apint_add_n(a, b, 0, 0)->data[0];
  
  // newApInt->len = tempLength;
  
  return newApInt;

 
}


ApInt *apint_sub(const ApInt *a, const ApInt *b) {
  //  uint64_t result;

  // ApInt * newApInt = (ApInt*)malloc(sizeof(ApInt));
  // newApInt->data = (uint64_t*)malloc(sizeof(uint64_t));
  // newApInt->len = 1;
  
  // if (a->data[0] > b->data[0] && a->flags == b->flags) {
  //   //a > b  
  //   result = unsigned_sub(a->data[0], b->data[0]);
  //   // if both positive, sign = positive
  //   // if both negative, sign = negative
  //   newApInt->flags = a->flags;
  // } else if (a->data[0] > b->data[0]) {
  //   // a > b and signs r opposite 
  //   result = unsigned_add(a->data[0], b->data[0]);
  //   newApInt->flags = a->flags;
  // } else if (a->data[0] < b->data[0] && a->flags == b->flags) {
  //   //a < b  
  //   result = unsigned_sub(b->data[0], a->data[0]);
  //   // opposite sign
  //   newApInt->flags = (a->flags + 1) % 2;
  // } else if (a->data[0] < b->data[0]) {
  //   // a > b and signs r opposite 
  //   result = unsigned_add(a->data[0], b->data[0]);
  //   newApInt->flags = a->flags;
  // } else if (apint_compare(a, b) == 0) {
  //   result = 0;
  //   newApInt->flags = 0;
  // } else {
  //   result = a->data[0] * 2;
  //   newApInt->flags = a->flags;
  // }
  

  // newApInt->data[0] = result;
  // return newApInt;
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
