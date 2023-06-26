/*
 *     bitpack.c
 *     by Theo Dayton and Thomson Brand, 10/27/2021
 *     Machine Arithmetic
 *
 *     Contains implementations for the bitpack family functions
 *     which bitpacks data into words to save space
 */

#include <bitpack.h>
#include <assert.h>
#include <stdlib.h>

Except_T Bitpack_Overflow = { "Overflow packing bits" };

/*
changes a value at a specified location in a packed unsigned integer. Expects
that width is less than 64 and that width and lsb added together are less than
64, and returns the updated word integer.
*/
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb,
                      uint64_t value)
{
  assert(width <= 64 && width + lsb <= 64);
  uint64_t maxwidth = 1;
  for(unsigned i = 0; i < width; i++){
    maxwidth = maxwidth * 2;
  }
  if(value >= maxwidth){
    RAISE(Bitpack_Overflow);
  }
  value = value << lsb;

  uint64_t mask = ~(maxwidth << lsb);

  word = (word & mask) | value;

  return word;
}

/*
changes a value at a specified location in a packed signed integer. Expects
that width is less than 64 and that width and lsb added together are less than
64, and returns the updated word integer.
*/
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,
                      int64_t value)
{
  assert(width <= 64 && (width + lsb) <= 64);

  if (!Bitpack_fitss(value, width)) {
          RAISE(Bitpack_Overflow);
          assert(0);
  }


  uint64_t left = (1 << (64 - width - lsb)) - 1;

  left = left << (width + lsb);
  uint64_t right = (1 << lsb) - 1;
  uint64_t mask = left + right;

  uint64_t value_field = value;
  value_field = value_field << (64 - width);
  value_field = value_field >> (64 - width - lsb);

  return (word & mask) | value_field;
}

/*
returns true if the value n can be fit into an unsigned integer with width bits,
false otherwise. Width must be less than or equal to 64.
*/
bool Bitpack_fitsu(uint64_t n, unsigned width)
{
  assert(width <= 64);

  int width_bits = 1;
  for(unsigned i = 0; i < width; i++){
    width_bits = width_bits * 2;
  }
  width_bits = width_bits - 1;
  return (n <= (uint64_t)width_bits);
}

/*
returns true if the signed value n can be fit into an integer with width bits,
false otherwise. Width must be less than or equal to 64.
*/
bool Bitpack_fitss(int64_t n, unsigned width)
{
  assert(width <= 64);

  int width_bits = 1;
  for(unsigned i = 0; i < width - 1; i++){
    width_bits = width_bits * 2;
  }
  return(n >= (int64_t)(width_bits * -1) && n <= (int64_t)(width_bits - 1));

}
/*
Gets a value packed into an unsigned integer. Expects that width plus lsb is
less than 64, and returns the value as an unsigned integer.
*/
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
  assert(width <= 64 && width + lsb <= 64);

  uint64_t mask = 1;
  for(unsigned i = 0; i < width; i++){
    mask = mask * 2;
  }
  mask = mask - 1;
  mask = mask << lsb;

  return (word & mask) >> lsb;
}

/*
Gets a value packed into a signed integer. Expects that width plus lsb is
less than 64, and returns the value as a signed integer.
*/
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
  int64_t to_return = Bitpack_getu(word, width, lsb);

  if(to_return >> (width - 1) == 1){ //if negative, do negative things

    uint64_t maxneg = (uint64_t)1 << width;
    uint64_t to_subtract = to_return & (~maxneg);
    return (maxneg - to_subtract) * -1;
  }
  //otherwise just send it on over
  return to_return;
}