/***************************************************************************
 *   Copyright (C) 2010 by Andrey Mukhin
 *   a.mukhin77@gmail.com
 ***************************************************************************/
#ifndef INCLUDE_FNV_HPP
#define INCLUDE_FNV_HPP

#include <sys/types.h> // for size_t

/** FNV hash function params
@see http://en.wikipedia.org/wiki/Fowler_Noll_Vo_hash */
namespace bloom {
  template <typename T> struct FNV {};

  /** FNV params for the 32-bit hash function */
  template<> struct FNV<unsigned> {
    typedef unsigned FNVT;
    static FNVT const PRIME = 16777619U;
    static FNVT const INIT = 2166136261U;
    static FNVT const MAX_BIT = 0xffffffffU;
  };

  /** FNV params for the 64-bit hash function */
  template<> struct FNV<unsigned long long> {
    typedef unsigned long long FNVT;
    static FNVT const PRIME = 1099511628211ULL;
    static FNVT const INIT = 14695981039346656037ULL;
    static FNVT const MAX_BIT = 0xffffffffffffffffULL;
  };

  /** FNV hash function
  @see http://en.wikipedia.org/wiki/Fowler_Noll_Vo_hash */
  template<typename T> T FNVHash(unsigned const char *buf,
    size_t len, T hval = FNV<T>::INIT) {
    unsigned const char *bp = buf; // start of buffer
    unsigned const char *be = bp + len; // beyond end of buffer
    while (bp < be) { // FNV-1 hash each octet in the buffer
      hval *= FNV<T>::PRIME; // multiply by the FNV magic prime mod 2^32
      hval ^= static_cast<T>(*bp++); // xor the bottom with the current octet
    }
    return hval; // return our new hash value
  }
}

#endif // INCLUDE_FNV_HPP