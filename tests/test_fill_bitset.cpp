/***************************************************************************
 *   Copyright (C) 2010 by Andrey Mukhin
 *   a.mukhin77@gmail.com
 ***************************************************************************/
#include "bloom.hpp"

#include <stdio.h>
#include <iostream>
#include <string.h> // for memset

using namespace bloom;

typedef unsigned long long BType;

struct UserId {
  unsigned long high;
  unsigned long low;
};

void printBloomParams(const Bloom<UserId, BType> &b) {
  std::cout << "Bloom parameters:\n";
  std::cout << "  max_elements_number: " << b.getMaxElementsNumber() << "\n";
  std::cout << "  positive_false: " << b.getPositiveFalse() << "\n";
  std::cout << "  min_hash_f_number: " << b.getMinHashFNumber() << "\n";
  std::cout << "  bits_number: " << b.getBitsNumber() << "\n";
  std::cout << "  bytes_number: " << b.getBytesNumber() << "\n";
  std::cout << "  bit_set: 0x" << std::hex << reinterpret_cast<unsigned long>(b.getBitStorage().bit_set) << "\n";
  std::cout << "  bit_set_size: " << std::dec << b.getBitStorage().bit_set_size << "\n";
}

int main() {

  Bloom<UserId, BType> b(1000000, 0.1, FNV<BType>::INIT, &FNVHash);
  unsigned char bloom_storage[b.getBytesNumber()];
  memset(bloom_storage, 0, b.getBytesNumber());

  b.setBitStorage(bloom_storage, b.getBytesNumber());
  printBloomParams(b);

  UserId uid;
  for(int i = 1; i < 1001; ++i) {
    uid.high = i % 100;
    uid.low = i % 200;
    bool f_fill = b.fillBitSet(&uid);
    std::cout << i << ". uid(" << uid.high << "," << uid.low << "), added: " << f_fill << "\n";
    if (!f_fill)
      break;
  }

  return 0;
}
