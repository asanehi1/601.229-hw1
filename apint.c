


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


/** Returns a pointer to an ApInt instance whose value is specified by the val parameter, which is a 64-bit unsigned value **/ 
ApInt *apint_create_from_u64(uint64_t val) {
  ApInt * newApInt = (ApInt*)malloc(sizeof(ApInt));
  newApInt->len = 1;
  newApInt->flags = 0; 
  newApInt->data = (uint64_t*)malloc(sizeof(uint64_t));
  newApInt->data[0] = (val);
  return newApInt;
}

/** Returns a pointer to an ApInt instance whose value is specified by the hex parameter, which is an arbitrary sequence of hexadecimal (base 16) digits. **/
ApInt *apint_create_from_hex(const char *hex) {
  ApInt * newApInt = (ApInt*)malloc(sizeof(ApInt));
  int counter = 0;
  
  if(hex[0] == '-') {
    newApInt->flags = 1;
    counter++;
    // if neg
  } else {
    newApInt->flags = 0;
  }

  // if only neg such as hex = '-' 
  if (hex[0] == '-' && strlen(hex) == 1) {
    newApInt->data = (uint64_t*) malloc(sizeof(uint64_t));
    newApInt->data[0] = 0;
    newApInt->len = 1;
    return newApInt;
  }
  
  while (hex[counter] == '0' && strlen(hex) > 1) {
    // help count leading zeros
    counter++;
  }

  if (counter == (int) strlen(hex)) {
    //if all zeros (so 00000000 turns into 0)
    counter--;
    newApInt->flags = 0;
  }

  // make new hex without leading zeros and '-'
  char *newHex = (char*) calloc(strlen(hex) - counter,  sizeof(char) + 1);
  strncpy(newHex, hex + counter, strlen(hex) - counter);
  
  int is_remainder = 0;
  if (strlen(newHex) % 16 != 0) {
    is_remainder = 1;
  }

  int n = strlen(newHex) / 16 + is_remainder;
  // n is ap->len value
  
  newApInt->data = (uint64_t*) malloc(n * sizeof(uint64_t));
  newApInt->len = n;

  int digitsFromLeft = 0;
  int digitLeft = strlen(newHex);
  if (n > 1) {
    digitsFromLeft = strlen(newHex) - 16;
    digitLeft = 16;
  }

  for (int i = 0; i < n; i++) {
    // this happens when i = n - 1 usually
    if (digitsFromLeft < 0) {
      digitsFromLeft = 0;
      digitLeft = strlen(newHex) % 16;
    }
      
    char * temp = (char*) calloc(17,  sizeof(char));
    strncpy(temp, newHex + digitsFromLeft, digitLeft);
    // strtoull changes hex into uint
    newApInt->data[i] = (strtoull(temp, NULL,  16));
    digitsFromLeft -=16;
    free(temp);
  }
    
  free(newHex);
  return newApInt;
}
  

/* Deallocates the memory used by the ApInt instance pointed-to by the ap parameter */
void apint_destroy(ApInt *ap) {
  //for (int i = 0; i < ap->len; i++) {
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

/** return uint64_t values at that n **/
uint64_t apint_get_bits(const ApInt *ap, unsigned n) {
  // ignore sign
  uint64_t temp = (ap->data[n]);
  return  temp;
}

/* return which digit has highest bit */
int apint_highest_bit_set(const ApInt *ap) {
  int n = (int) ap->len - 1;
  // smallest  value is stored in data[0]
  uint64_t temp = (ap->data[n]);
  if (temp == (uint64_t) 0 && n == 0) {
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

/** swap hex values **/
char * swap_hex_values(char *hex) {
  int end = strlen(hex) - 1;
  int start = 0;
  if (hex[0] == '-') {
    start = 1;
  }
  while (start < end) {
    char temp = hex[start];
    hex[start++] = hex[end];
    hex[end--] = temp;
  }

  return hex;
}

/**  Returns a pointer to a dynamically-allocated C character string containing the hexadecimal (base 16) digits of the representation of the ApInt instance pointed to by ap **/
char *apint_format_as_hex(const ApInt *ap) {
  char* hex = (char*) calloc((int) ap->len,  sizeof(char) * 17 + 1);
  int counter = 0;
  int remainder = 0;

  if (ap->flags == 1) {
    hex[0] = '-';
    counter++;
  }
   
  for (int i = 0; i < (int) ap->len; i++) {

    uint64_t temp  = (ap->data[i]);
    remainder = 0;
    int countForPadding = 0;

    do {
      remainder = temp % 16;
      temp /= 16;
      countForPadding++;	
      hex[counter++] =  get_hex_char(remainder);
    } while ((int) temp != 0);  
    
    while (countForPadding < 16 && i != (int) ap->len - 1) {
      hex[counter++] = '0';
      countForPadding++;
    }
  }
  
  hex = swap_hex_values(hex);
  return hex;
}
/** Returns a new ApInt instance with the opposite sign from the one passed as the parameter. As a special case, if the ApInt passed is numerically equal to zero, then an ApInt instance representing an identical (zero) value should be returned. **/
ApInt *apint_negate(const ApInt *ap) {
  ApInt *newApInt = (ApInt*)malloc(sizeof(ApInt));
  newApInt->len = ap->len;
  newApInt->data = (uint64_t*)malloc((int) ap->len * sizeof(uint64_t));

  for (int i = 0; i < (int) ap->len; i++) {
    newApInt->data[i] = ap->data[i];
  }

  if (ap->data[0] == 0 && ap->len == 1) {
    return newApInt;
  } else if(ap->flags == 1) {
    newApInt->flags = 0;
  } else {
    newApInt->flags = 1;
  }

  //apint_destroy(ap);
  return newApInt;
}

/*returns hex number*/
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

/*returns 1 if theres an overflow and 0 if not*/
int add_overflow(uint64_t temp1, uint64_t temp2, int carry) {
  if((temp1 + temp2 + carry) < temp1 || (temp1 + temp2 + carry) < temp2) {
    return 1;
  } else if (temp1 == UINT64_MAX && temp2 == UINT64_MAX && carry == 1) {
    return 1;
  } else {
    return 0;
  }
}

/*returns the highest ApInt length*/
int get_length(const ApInt* a, const ApInt* b) {
  if(a->len > b->len) {
    return (int)a->len;
  } else {
    return (int)b->len;
  }
}

/** calculations for add / sub functions **/
ApInt* unsigned_calc(const ApInt* a, const ApInt* b, char c) {
  int carryIn = 0, i = 0, carryOut = 0;
  int length = get_length(a,b);

  ApInt * ap = (ApInt*)malloc(sizeof(ApInt));
  uint64_t* tempData = (uint64_t*)malloc(sizeof(uint64_t) * (length + 1));

  for(; i < length; i++) {
    uint64_t temp1;
    uint64_t temp2;

    //a has no more values to add
    if((int)a->len < i + 1) {
      temp1 = 0UL;
      temp2 = (b->data[i]);
    } else if ((int)b->len < i + 1) {
      //b has no more values to add
      temp2 = 0UL;
      temp1 = (a->data[i]);
    } else {
      //a and b both still have values to add
      temp1 = (a->data[i]);
      temp2 = (b->data[i]);
    }

    if(c == '+') {
      carryOut = add_overflow(temp1, temp2, carryIn); 
      tempData[i]= temp1 + temp2 + carryIn;     
    } else {
      carryOut = sub_underflow(temp1, temp2, carryIn);
      tempData[i] = temp1 - carryIn - temp2;
    } 
    carryIn = carryOut; 
  }

  if (carryIn > 0) {
    //there was an overflow
    tempData[i++] = carryIn;
  }

  ap->len = i;
  ap->data = (uint64_t*)malloc(sizeof(uint64_t) * i);

  for (int j = 0; j < (int)ap->len; j++) {
    ap->data[j] = (tempData[j]);
  }
  
  free(tempData);
  return ap;
}

//checks underflow for subtraction
int sub_underflow(uint64_t temp1, uint64_t temp2, int carry) {
  if(temp1 < temp2 + carry) {
    return 1;
  } else {
    return 0;
  }
}

/** Computes the sum a plus b, and returns a pointer to an ApInt instance representing the sum. Note that either or both of the operand values could be negative **/
ApInt *apint_add(const ApInt *a, const ApInt *b) {
  ApInt * newApInt;
  int len = get_length(a, b) - 1;

  if (a->flags == 0 && b->flags == 0) {
    // both positive 
    newApInt = unsigned_calc(a, b, '+');
    newApInt->flags = 0;
  } 
  else if (a -> flags ==  b->flags) {
    // both are negative 
    newApInt = unsigned_calc(a, b, '+');
    newApInt->flags = 1;
  } else if (a->len > b-> len && a->flags == 0) {
    // a > b  (a is pos and b is neg )
    newApInt = unsigned_calc(a, b, '-');
    newApInt->flags = 0;
  } else if (b -> len > a->len && b->flags == 1) {
    // |b| > |a| but a > b  (b is negative)
    newApInt = unsigned_calc(b, a, '-');
    newApInt->flags = 1;

  } else if (a->len > b->len && a->flags == 1) {
      // |a| > |b| but b > a
    newApInt = unsigned_calc(a, b, '-');
    newApInt->flags = 1;

   } else if (b->len > a->len && a->flags == 0) {
    // |b| > |a| but a > b since a is pos b is neg
    newApInt = unsigned_calc(b, a, '-');
    newApInt->flags = 0;
  } else if (b->len > a->len && a->flags == 1) {
    newApInt = unsigned_calc(b, a, '-');
    newApInt->flags = 0;
  } else if (a->data[len] < b->data[len] && a->flags == 1) {
    // a < b and a is negative and b is positive
    newApInt = unsigned_calc(b, a, '-');
    newApInt->flags = 0;
  }  else if (a->data[len] < b->data[len]) {
    // a < b and a is positive while b is neg
    newApInt = unsigned_calc(b, a, '-');
    newApInt->flags = 1;
  } else if (a->data[len] > b->data[len] && a->flags == 1) {
    // a > b and a is neg  while b is pos
    newApInt = unsigned_calc(a, b, '-');
    newApInt->flags = 1;
  } else {
    // a > b and a is pos  while b is neg
    newApInt = unsigned_calc(a, b, '-');
    newApInt->flags = 0;
  } 
  
  return newApInt;
}

/** Computes the difference a minus b, and returns a pointer to an ApInt instance representing the difference. Note that either or both of the operand values could be negative **/
ApInt *apint_sub(const ApInt *a, const ApInt *b) {
  ApInt * newApInt;
  ApInt * newB = apint_negate(b);
  newApInt = apint_add(a,newB);
  apint_destroy(newB);
  
  return newApInt;
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

