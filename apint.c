


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
  newApInt->data = (uint64_t*)malloc(sizeof(uint64_t));

  int counter = 0;
  if(hex[0] == '-') {
    newApInt->flags = 1;
    counter++;
  } else {
    newApInt->flags = 0;
  }
  while (hex[counter] == '0' && strlen(hex) > 1) {
    // help count leading zeros
      counter++;
  }
  if (counter == strlen(hex)) {
    counter--;
  }
  //printf("%d \n", counter);

  int extra = 0;
  if ((strlen(hex) - counter) % 16 != 0) {
    extra = 1;
  }
  int n = (strlen(hex) - counter) / 16 + extra;
  //printf("%d \n", n);

  int digitLeft = 16;
  for (int i = 0; i < n; i++) {
    char * temp = (char*) malloc( sizeof(char) * 17);
    if (i == n - 1) {
      digitLeft == (strlen(hex) - counter) % 16;
    }
    strncpy(temp, hex + counter, digitLeft);
    //printf("hi: %s \n", temp);
    newApInt->data[i] =  __bswap_64(strtoull(temp, NULL,  16));
    counter += 16;
  }
    


  //printf("final  %" PRIu64 "\n", __bswap_64(newApInt->data[0]));
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
  int n = (int) ap->len - 1;
  // I think largest value is stored in data[0]
  uint64_t temp = __bswap_64(ap->data[n]);
  //uint64_t temp = (ap->data[0]);
  if (temp == (uint64_t) 0) {
    return -1;
  }
  // highet bit stores the value of highest bit
  // + updates when necessary
  int highestBit = 0;
  for (int i = 0; i < 64; i++) {
    // if bit == 1
    if (temp & 1) {
      highestBit = i;
    }
    // shift 
    temp = temp >> 1;
  }
  return highestBit + (n * 64);
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
  case 15: return 'f'; break;
    default: return '\0'; break;
  }
}

char * swap_hex_values(char *hex) {
  //printf("before swap: %s\n", hex);
  int end = strlen(hex) - 1;
  int start = 0;
  if (hex[0] == '-') {
    start = 1;
    printf("neg");
  }
  while (start < end) {
    char temp = hex[start];
    hex[start++] = hex[end];
    hex[end--] = temp;
  }

  //printf("swapped: %s\n", hex);

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
  char* hex = (char*) malloc((int)ap->len * sizeof(char) * 17);
  int counter = 0;
   int remainder = 0;

   //printf("len : %d \n", ap->len);
   
   for (int i = 0; i < (int) ap->len; i++) {
    uint64_t temp = __bswap_64(ap->data[i]);
    // printf("\nin format function: ");
    //printf("before %" PRIu64 "\n", temp);
    //print_binary(temp); 
    int bool = 1;
    if (temp == (uint64_t) 0 && i < ap->len -1) {
      bool = 0;
      temp = 16;
    }

    do {
      if (bool == 0) {
	temp--;
      } else {
	// regular code
	remainder = temp % 16;
	//remainder = temp & 15;
	//printf("rem: %d\n", remainder);
	temp /= 16;
	//temp >>= 4;
      }
      hex[counter++] =  get_hex_char(remainder);
      //printf("re: %d", hex[counter - 1]);
    } while ((int) temp != 0);
    
  }
  
  
  hex = swap_hex_values(hex);
  if(ap->flags == 1) {
    char* neg = (char*) malloc((int)ap->len * sizeof(char) * 17);
    neg[0] = '-';
    //printf("this is neg\n");
    strncat(neg, hex, 16);
     free(hex);
     //printf("after : %s \n", neg);
     return neg;
    }
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
  int diff;

  if(strlen(hex_a) > strlen(hex_b)) {
    length = strlen(hex_a);
    diff = length - strlen(hex_b);
  } else {
    length = strlen(hex_b);
    diff  = length - strlen(hex_a);
  }

  //printf("length: %d ", length);
  // printf("\n");

  for (i = 0; i < length; i++) {
    // printf("a:%c + b:%c = ", hex_a[diff - i], hex_b[i]);
    int num = get_hex_num(hex_a[diff - i]) + get_hex_num(hex_b[i]) + carry;
    //printf("%d\n", num);
    
    if(num > 15) {
      carry = 1;
    } else {
      carry = 0;
    }


    hex[i] = get_hex_char(num % 16);
    //printf("try hi %c", hex[i]);
  }

  // 0000 0000 0000 0000 1

  if (carry == 1) {
    hex[i] = '1';
  }

  hex = swap_hex_values(hex);

  if(strlen(hex) > 16) {
    int extra = 0;
    if ((int)strlen(hex) % 16 != 0 ) {
      extra = 1;
      //printf("hi this shouldn't happen yet\n");
    }
    newApInt->len = (int) strlen(hex) / 16 + extra;
    //printf("so len: %d\n", newApInt->len);
  }

  /**
  for(int i = 0, n = 0; i < newApInt->len; i++, n += 16) {
    char array[16];
    strncpy(array, hex + n, 16);
    array[16] = '\0';
    
    newApInt->data[i] = __bswap_64(strtoull(array, NULL,  16));
  }
  **/

  int digitsFromEnd = strlen(hex);
  int digitsLeft = 16;
  // 16 unless otherwise stated
  
  for (int i = 0; i < newApInt->len; i++) {
    if (i < newApInt->len -1) {
      digitsFromEnd -= 16;
      digitsLeft = 16;
    } else {
      digitsLeft = strlen(hex) % 16;
      if (digitsLeft == 0) {
	digitsLeft = 16;
      }
      digitsFromEnd -= digitsLeft;
    }
    
    char array[digitsLeft];
    strncpy(array, hex + digitsFromEnd, digitsLeft);
    array[digitsLeft] = '\0';
    newApInt->data[i] = __bswap_64(strtoull(array, NULL,  16));
    //printf("swappet: %s\n", array);
  }
    
    

  free(hex);
  //newApInt->data[0] = __bswap_64(strtoull(hex, NULL,  16));
  return newApInt;
}

uint64_t unsigned_sub(const uint64_t a, const uint64_t b) {
  return a - b;
}

ApInt *apint_add(const ApInt *a, const ApInt *b) {
  ApInt * newApInt = (ApInt*)malloc(sizeof(ApInt));
  
  if (a->flags == 0 && b->flags == 0) {
    // both pos
    newApInt = unsigned_add(a, b);
  } else if (a->flags == b->flags) {
    // both neg
    newApInt = unsigned_add(apint_negate(a), apint_negate(b));
    newApInt->flags = 1;
  }
  // else if (a->data[n] < b->data[n] && a->flags == 1) {
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

  if (left->len == right->len) {
   int  n = right->len -1;
    // just compare data[n] with n being largest 
    if (left->data[n] == right->data[n] && left->flags == right->flags) {
      return 0;
    } else if (left->data[n] < right->data[n] && right->flags == 0) {
      return -1;
    } else if (right->data[n] < left->data[n] && left->flags == 0) {
      return 1;
    } else if (left->data[n] >= right->data[n] && left->flags == 1) {
      return -1;
    } else {
      return 1;
    } 

  } else if (left->len > right-> len) {
    // left len > right len
    if (left->flags == 0) {
       	// left is bigger 
       	return 1;
    }
    // right is bigger
    return -1;
  } else {
    // left len < right len
    if (right->flags == 0) {
      	// right is bigger
       	return -1;
    }
    // else left is bigger 
    return 1;
  }
}

