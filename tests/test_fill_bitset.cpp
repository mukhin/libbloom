/***************************************************************************
 *   Copyright (C) 2010 by Andrey Mukhin
 *   a.mukhin77@gmail.com
 ***************************************************************************/
#include "bloom.hpp"

#include <stdio.h>
#include <iostream>
#include <string.h> // for memset

using namespace bloom;

struct UserId {
  unsigned long high;
  unsigned long low;
};

int main() {
  typedef unsigned long long BType;

  Bloom<UserId, BType> b(1000000, 0.1, FNV<BType>::INIT, &FNVHash);

  std::cout << "Bloom parameters:\n";
  std::cout << "  max_elements_number: " << b.getMaxElementsNumber() << "\n";
  std::cout << "  positive_false: " << b.getPositiveFalse() << "\n";
  std::cout << "  min_hash_f_number: " << b.getMinHashFNumber() << "\n";
  std::cout << "  bits_number: " << b.getBitsNumber() << "\n";
  std::cout << "  bytes_number: " << b.getBytesNumber() << "\n";

  unsigned char bloom_storage[b.getBytesNumber()];

  UserId uid;
  for(int i = 1; i < 1001; ++i) {
    uid.high = i % 100;
    uid.low = i % 200;
    bool f_fill = b.fillBitSet(&uid, bloom_storage, b.getBytesNumber());
    std::cout << i << ". uid(" << uid.high << "," << uid.low << "), added: " << f_fill << "\n";
    if (!f_fill)
      break;
  }

  return 0;
}
