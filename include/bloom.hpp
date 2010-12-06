/***************************************************************************
 *   Copyright (C) 2010 by Andrey Mukhin
 *   a.mukhin77@gmail.com
 ***************************************************************************/
#ifndef INCLUDE_BLOOM_HPP
#define INCLUDE_BLOOM_HPP

#include "fnv.hpp"

#include <math.h>
#include <climits> // for CHAR_BIT

namespace bloom {
  /** Bloom filter realization @see http://en.wikipedia.org/wiki/Bloom_filter
  @param T type of handling object (must be C++ POD type)
  @param HASHT type of the hash function return-value */
  template<typename T, typename HASHT> class Bloom {
    /** Max number of the keeping elements */
    unsigned long max_elements_number;
    /** Probability of positive false in percent (can have got value between 0.0 and 100.0) */
    double positive_false;
    /** Min hash functions number for the concrete max_elements_number and positieve_false.
      This value should be calculated by program but it can be defined by user */
    unsigned long min_hash_f_number;
    /** Number of bits for the max_elements_number (will been calculated) */
    unsigned long  bits_number;
    /** Min bytes number for the bits_number storing */
    unsigned long bytes_number;
    /** Initialization vector for the hash function (can be 0) */
    HASHT iv;
    /** Pointer to the hash function */
    HASHT (*p_hash_f)(unsigned const char *buf, size_t len, HASHT iv);

    public:
      /** Constructor
      @param max_elements_number max number of the keeping elements
      @param positive_false positive false in percent
      @param iv initialization vector for the hash function (can be 0)
      @param p_hash_f pointer to the hash function*/
      Bloom(unsigned long max_elements_number, double positive_false, HASHT iv,
        HASHT (*p_hash_f)(unsigned const char *buf, size_t len, HASHT iv)):
        max_elements_number(max_elements_number),
        positive_false(positive_false),
        iv(iv),
        p_hash_f(p_hash_f) {
        bits_number = calculateBitsNumber(max_elements_number, positive_false);
        bytes_number = calcBytesNumber(bits_number);
        min_hash_f_number = calcMinHashFNumber(max_elements_number, bits_number);
      }

      /** Constructor
      @param max_elements_number max number of the storing elements
      @param positive_false positive false in percent
      @param min_hash_f_number min hash functions number
      @param iv initialization vector for the hash function (can be 0)
      @param p_hash_f pointer to the hash function*/
      Bloom(unsigned long max_elements_number, double positive_false, unsigned long min_hash_f_number, HASHT iv,
        HASHT (*p_hash_f)(unsigned const char *buf, size_t len, HASHT iv)):
        max_elements_number(max_elements_number),
        positive_false(positive_false),
        min_hash_f_number(min_hash_f_number),
        iv(iv),
        p_hash_f(p_hash_f) {
        bits_number = calculateBitsNumber(max_elements_number, positive_false);
        bytes_number = calcBytesNumber(bits_number);
      }

      /** Copy-Constructor */
      Bloom(const Bloom& b) {
        max_elements_number = b.max_elements_number;
        positive_false = b.positive_false;
        min_hash_f_number = b.min_hash_f_number;
        iv = b.iv;
        p_hash_f = b.p_hash_f;
        bits_number = b.bits_number;
        bytes_number = b.bytes_number;
      }

      /** Destructor */
      virtual ~Bloom() {}

      /** Fill bits in the char sequence for the some element
      @param element pointer to the element
      @param bit_set char sequence which store bits
      @param bit_set_size size of char sequence
      @return true if the element is new */
      bool fillBitSet(T* element, unsigned char* bit_set, size_t bit_set_size) {
        HASHT hash = 0, bitmask = 0, bitslot = 0;
        bool collision = true;
        for (HASHT i = iv; i < (min_hash_f_number + iv); ++i) {
          hash = (*p_hash_f)(reinterpret_cast<unsigned const char*>(element), sizeof(T), i);
          hash %= bit_set_size;
          bitmask = 1 << (hash % CHAR_BIT);
          bitslot = hash / CHAR_BIT;
          if (!(bit_set[bitslot] & bitmask)) {
            collision = false;
            bit_set[bitslot] |= bitmask;
          }
        }
        return !collision;
      }

      /** Find bits in the char sequence for the some element
      @param element pointer to the element
      @param bit_set char sequence which store bits
      @param bit_set_size size of char sequence
      @return true if all bits are present */
      bool isElementPresent(T* element, unsigned char* bit_set, size_t bit_set_size) {
        HASHT hash = 0, bitmask = 0, bitslot = 0;
        bool collision = true;
        for (HASHT i = iv; i < (min_hash_f_number + iv); ++i) {
          hash = (*p_hash_f)(reinterpret_cast<unsigned const char*>(element), sizeof(T), i);
          hash %= bit_set_size;
          bitmask = 1 << (hash % CHAR_BIT);
          bitslot = hash / CHAR_BIT;
          if (!(bit_set[bitslot] & bitmask)) {
            collision = false;
            break;
          }
        }
        return collision;
      }

      /** Calculate the number of hash functions
      @param max_elements_number max number of the storing elements
      @param bits_number number of bits in the sequence
      @return number of functions or 0*/
      static unsigned long calcMinHashFNumber(unsigned long max_elements_number, unsigned long bits_number) {
        if (bits_number > 0) {
          return static_cast<unsigned long>(static_cast<double>(bits_number) /
            (static_cast<double>(max_elements_number)) * log(2.));
        }
        return 0;
      }

      /** Calculate the number of bits
      @param max_elements_number max number of the storing elements
      @param positive_false probability of positive false in percent
      @return number of bits or 0 */
      static unsigned long calculateBitsNumber(unsigned long max_elements_number, double positive_false) {
        if (0. < positive_false && positive_false < 100.) {
          return static_cast<unsigned long>(- static_cast<double>(max_elements_number) *
            log(positive_false / 100.) / pow(log(2.), 2.0));
        }
        return 0;
      }

      /** Calculate bytes number
      @param bits_number number of bits in the sequence
      @return number of bytes */
      static unsigned long calcBytesNumber(unsigned long  bits_number) {
        unsigned long bytes_number = bits_number / CHAR_BIT;
        if ((bits_number % CHAR_BIT) > 0) {
          ++bytes_number;
        }
        return bytes_number;
      }

      /** Return max_elements_number */
      unsigned long getMaxElementsNumber() {return max_elements_number;}
      /** Return positive_false */
      double getPositiveFalse() {return positive_false;}
      /** Return min_hash_f_number */
      unsigned long getMinHashFNumber() {return min_hash_f_number;}
      /** Return bits_number */
      unsigned long getBitsNumber() {return bits_number;}
      /** Return bytes_number */
      unsigned long getBytesNumber() {return bytes_number;}
  };
}

#endif // INCLUDE_BLOOM_HPP