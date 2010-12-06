/***************************************************************************
 *   Copyright (C) 2010 by Andrey Mukhin
 *   a.mukhin77@gmail.com
 ***************************************************************************/
#include "fnv.hpp"

#include <stdio.h>
#include <iostream>
#include <string.h>

int main() {
  unsigned char buf[32];
  unsigned long long hash32 = 0, hash64 = 0;

  memset(buf, 1, 32);

  hash32 = bloom::FNVHash<unsigned>(buf, 32, hash32);
  hash64 = bloom::FNVHash(buf, 32, hash64);
  std::cout << "INIT = 0 \nhash32 = " << hash32 << "\nhash64 = " << hash64 << "\n";

  hash32 = bloom::FNVHash<unsigned>(buf, 32);
  hash64 = bloom::FNVHash<unsigned long long>(buf, 32);
  std::cout << "INIT = FNV::INIT \nhash32 = " << hash32 << "\nhash64 = " << hash64 << "\n";
  return 0;
}
