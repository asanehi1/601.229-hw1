


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
  newApInt->data[0] = (val);
  return newApInt;
}

ApInt *apint_create_from_hex(const char *hex) {

  ApInt * newApInt = (ApInt*)malloc(sizeof(ApInt));

  int counter = 0;
  if(hex[0] == '-') {
    newApInt->flags = 1;
    //printf("this is neg \n");
    counter++;
  } else {
    newApInt->flags = 0;
  }

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
    // if its just 0
    counter--;
    newApInt->flags = 0;
  }
  //printf("%d \n", counter);

  int extra = 0;
  if ((strlen(hex) - counter) % 16 != 0) {
    extra = 1;
  }
  int n = (strlen(hex) - counter) / 16 + extra;
  //printf("length: %d \n", n);

   newApInt->data = (uint64_t*) malloc(n * sizeof(uint64_t));
   newApInt->len = n;

   int num = strlen(hex) - counter;

  int digitLeft = 16;
  //printf("num: %d \n ", num);
  int countDigits;
  if (n > 1) {
    countDigits = 16;
  } else {
    countDigits = num % 16;
  }
  int digitsFromLeft = num - countDigits + counter;

  //char *tempH = (char *) calloc(1, sizeof(hex));
  //tempH = swap_hex_values(hex);
  for (int i = 0; i < n; i++) {
    char * temp = (char*) calloc(17,  sizeof(char));
 
    if (i == n - 1) {
      digitsFromLeft = counter % 17;
      digitLeft =  num % 16 + 16 * (1- 1 * extra);
      //digitLeft = num % 16 - counter ;
      //digitLeft = (strlen(hex)- counter) % 16;
      //counter = strlen(hex) - digitLeft;
    }
    strncpy(temp, hex + digitsFromLeft, digitLeft);
    //printf("digitsFromLeft %d \n", digitsFromLeft);
    //printf("digitLeft %d \n", digitLeft);
    //printf("string: %s \n", hex);
    //printf("hi: %s \n", temp);
    newApInt->data[i] = (strtoull(temp, NULL,  16));
    //counter += 16;
    digitsFromLeft -= 16;
    free(temp);
  }
    

  //printf("\n");
  //printf("final  %" PRIu64 "\n", (newApInt->data[0]));
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

uint64_t apint_get_bits(const ApInt *ap, unsigned n) {
  // ignore sign
  uint64_t temp = (ap->data[n]);
  //printf("value: %" PRIu64, temp);
  //uint64_t temp = (ap->data[n]);
  return  temp;
}

/* return which digit has highest bit */
int apint_highest_bit_set(const ApInt *ap) {
  int n = (int) ap->len - 1;
  // I think largest value is stored in data[0]
  uint64_t temp = (ap->data[n]);
  //uint64_t temp = (ap->data[0]);
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

char * swap_hex_values(char *hex) {
  //printf("before swap: %s\n", hex);
  int end = strlen(hex) - 1;
  int start = 0;
  if (hex[0] == '-') {
    start = 1;
    //printf("neg");
  }
  while (start < end) {
    char temp = hex[start];
    hex[start++] = hex[end];
    hex[end--] = temp;
  }

  //printf("swapped: %s\n", hex);

  return hex;
}

void print_binary(ApInt* w) {
  for(int j = 0; j < w->len; j++) {
    printf("Index %d: ", j);
    for (int i = 0; i < 1 * 64; i++)
    {
      if(w->data[j] & 1) {
        printf("1");
      } else {
        printf("0");
      }

      w->data[j]>>=1;
    }
    printf("\n");
  }

  printf("\n");
}

char *apint_format_as_hex(const ApInt *ap) {
  char* hex = (char*) calloc((int) ap->len,  sizeof(char) * 17 + 1);
  int counter = 0;
  int remainder = 0;

  printf("\nIn hex function\n");
  //print_binary(ap);

  if (ap->flags == 1) {
    hex[0] = '-';
    counter++;
  }
   printf("len : %d \n", ap->len);
   
   for (int i = 0; i < (int) ap->len; i++) {
     //printf("%d\n", i);
    uint64_t temp  = (ap->data[i]);
    remainder = 0;
    // printf("\nin format function: ");
    //printf("\nbefore hex %" PRIu64 "\n", temp);
    //print_binary(temp); 
    int bool = 1;
    if (temp == (uint64_t) 0 && i < (int) ap->len -1) {
      bool = 0;
      temp = 16;
      printf("bool is trueee \n");
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
      if (counter == 123 || counter == 124 || counter == 125 || counter == 126) {
	printf("re: %d, temp: %ld \n", remainder, temp);
      }
      hex[counter++] =  get_hex_char(remainder);
      printf("#: %d, hex: %c \n", counter - 1, hex[counter - 1]);
    } while ((int) temp != 0);
    //free(temp);
  }
  
  
  hex = swap_hex_values(hex);
  printf("hex: %s\n", hex);

  return hex;
}

ApInt *apint_negate(const ApInt *ap) {
  ApInt *newApInt = (ApInt*)malloc(sizeof(ApInt));
  newApInt->len = ap->len;
  newApInt->data = (uint64_t*)malloc((int) ap->len * sizeof(uint64_t));
  for (int i = 0; i < ap->len; i++) {
    newApInt->data[i] = ap->data[i];
  }

  if (ap->data[0] == 0 && ap->len == 1) {
    return newApInt;
  } else if(ap->flags == 1) {
    newApInt->flags = 0;
  } else {
    newApInt->flags = 1;
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

int add_overflow(uint64_t temp1, uint64_t temp2, int carry) {
  if((temp1 + temp2 + carry) < temp1 || (temp1 + temp2 + carry) < temp2) {
    printf("\nOverflow\n");
    return 1;
  } else {
    return 0;
  }
}

int get_length(const ApInt* a, const ApInt* b) {
  if(a->len > b->len) {
    return (int)a->len;
  } else {
    return (int)b->len;
  }
}

ApInt* unsigned_add(const ApInt* a, const ApInt* b, char c) {
  int carryIn = 0, i = 0, carryOut = 0, borrow;
  int length = get_length(a,b);

  ApInt * ap = (ApInt*)malloc(sizeof(ApInt));
  uint64_t* tempData = (uint64_t*)malloc(sizeof(uint64_t) * length);
  printf("\nlength: %d\n", length);

  for(; i < length; i++) {
    uint64_t temp1 = (a->data[i]);
    uint64_t temp2 = (b->data[i]);

    printf("" "%" PRIu64 , (temp1) );
    if(c == '+') {
      carryOut = add_overflow(temp1, temp2, carryIn); //carryIn = 0 carryOut = 1  carryIn = 1 carryOut = 1
      tempData[i]= (temp1 + temp2 + carryIn);          //carryIn = 0               carryIn = 1
      carryIn = carryOut;                             //carryIn = 1               carryIn = 1
      printf(" + " "%" PRIu64 , temp2 );
    } else {
      tempData[i]= temp1 - temp2;
      printf(" - " "%" PRIu64 , temp2 );
    }
    printf(" = " "%" PRIu64 "\n",tempData[i] ); 
  }

  //2 0000 0000 0000 0000

  if (carryIn > 0) {
    tempData[i++] = carryIn;
  }

  ap->len = i;
  ap->data = (uint64_t*)malloc(sizeof(uint64_t) * i);
  //ap->data = (uint64_t*)realloc(tempData, sizeof(uint64_t) * i);
  printf("Final length: %d\n", i);

  for (int j = 0; j < (int)ap->len; j++) {
    ap->data[j] = (tempData[j]);
    //printf("\nResult final: " "%" PRIu64 "\n", ap->data[j] );
  }

 //print_binary(ap);

  //printf("\nResult final: " "%" PRIu64 "\n", ap->data[0] );
  
  free(tempData);
  return ap;
}

int sub_overflow(uint64_t temp1, uint64_t temp2, int carry) {
  if(temp1) {
    printf("\nOverflow\n");
    return 1;
  } else {
    return 0;
  }
}

ApInt *apint_add(const ApInt *a, const ApInt *b) {
  ApInt * newApInt;
  int len = get_length(a, b) - 1;
  //newApInt->data = (uint64_t*)malloc(sizeof(uint64_t));
  
  if (a->flags == 0 && b->flags == 0) {
    // both positive 
    newApInt = unsigned_add(a, b, '+');
    //print_binary(result);
    //printf("result: " "%" PRIu64 "\n", newApInt->data[0]);
    newApInt->flags = 0;
  } 
  else if (a -> flags ==  b->flags) {
    // both are negative 
    //result = unsigned_add(a->data[0], b->data[0]);
    newApInt = unsigned_add(a, b, '+');
    newApInt->flags = 1;
  } else if (a->len > b-> len && a->flags == 0) {
    // a > b  (a is pos and b is neg )
    newApInt = unsigned_add(a, b, '-');
    newApInt->flags = 0;
  } else if (b -> len > a->len && b->flags == 1) {
    // |b| > |a| but a > b  (b is negative)
    newApInt = unsigned_add(b, a, '-');
    newApInt->flags = 1;

  } else if (a->len > b->len && a->flags == 1) {
      // |a| > |b| but b > a
    newApInt = unsigned_add(a, b, '-');
    newApInt->flags = 1;

   } else if (b->len > a->len && a->flags == 0) {
    // |b| > |a| but a > b since a is pos b is neg
    newApInt = unsigned_add(b, a, '-');
    newApInt->flags = 0;
    
  } else if (a->data[len] < b->data[len] && a->flags == 1) {
    // a < b and a is negative and b is positive
    //result = unsigned_sub(b->data[0], a->data[0]);
    newApInt = unsigned_add(b, a, '-');
    newApInt->flags = 0;
  }  else if (a->data[len] < b->data[len]) {
    // a < b and a is positive while b is neg
    //result = unsigned_sub(b->data[0], a->data[0]);
    newApInt = unsigned_add(b, a, '-');
    newApInt->flags = 1;
  } else if (a->data[len] > b->data[len] && a->flags == 1) {
     // a > b and a is neg  while b is pos
    //result = unsigned_sub(a->data[0], b->data[0]);
    newApInt = unsigned_add(a, b, '-');
    newApInt->flags = 1;
  } else {
    // a > b and a is pos  while b is neg
    //result = unsigned_sub(a->data[0], b->data[0]);
    newApInt = unsigned_add(a, b, '-');
    newApInt->flags = 0;
  } 
  

  // newApInt->data[0] = result;
  return newApInt;
}

ApInt *apint_sub(const ApInt *a, const ApInt *b) {
   ApInt * newApInt;
  if (a->data[0] > b->data[0] && a->flags == b->flags) {
    //a > b  
    //result = unsigned_sub(a->data[0], b->data[0]);
    newApInt = unsigned_add(a, b, '-');
    // if both positive, sign = positive
    // if both negative, sign = negative
    newApInt->flags = a->flags;
  } else if (a->data[0] > b->data[0]) {
    // a > b and signs r opposite 
    //result = unsigned_add(a->data[0], b->data[0]);
    newApInt = unsigned_add(a, b, '+');
    newApInt->flags = a->flags;
  } else if (a->data[0] < b->data[0] && a->flags == b->flags) {
    //a < b  
    //result = unsigned_sub(b->data[0], a->data[0]);
    newApInt = unsigned_add(b, a, '-');
    // opposite sign
    newApInt->flags = (a->flags + 1) % 2;
  } else if (a->data[0] < b->data[0]) {
    // a > b and signs r opposite 
    //result = unsigned_add(a->data[0], b->data[0]);
    newApInt = unsigned_add(a, b, '-');
    newApInt->flags = a->flags;
  } else if (apint_compare(a, b) == 0) {
    //result = 0;
    newApInt = (ApInt*)malloc(sizeof(ApInt));
    newApInt->data = (uint64_t*)malloc(sizeof(uint64_t));
    newApInt->data[0] = 0; 
    newApInt->flags = 0;
    newApInt->len = 1;
  } else {
    //result = a->data[0] * 2;
    newApInt = (ApInt*)malloc(sizeof(ApInt));
    newApInt->data = (uint64_t*)malloc(sizeof(uint64_t));
    newApInt->data[0] = a->data[0] * 2;
    newApInt->flags = a->flags;
    newApInt->len = 1;
  }
  

  // newApInt->data[0] = result;
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

