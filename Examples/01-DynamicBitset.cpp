#include <Container/DynamicBitset.hpp>

#include <bitset>

int main() {
  container::DynamicBitset bits1;
  bits1.resize(66);
  bits1.set(1);
  bits1.set(3);
  bits1.set(5);
  bits1.set(23);
  // bits1.set(65);
  container::DynamicBitset bits2;
  bits2.resize(66);
  bits2.set(0);
  bits2.set(2);
  bits2.set(4);
  bits2.set(5);
  bits2.set(62);
  std::cout << (bits1 & bits2) << std::endl;
  std::cout << (bits1 | bits2) << std::endl;
  std::cout << (bits1 ^ bits2) << std::endl;

  std::bitset<66> b1, b2;
  b1[65 - 1] = 1;
  b1[65 - 3] = 1;
  b1[65 - 5] = 1;
  b1[65 - 23] = 1;
  // b1[65 - 65] = 1;

  b2[65 - 0] = 1;
  b2[65 - 2] = 1;
  b2[65 - 4] = 1;
  b2[65 - 5] = 1;
  b2[65 - 62] = 1;

  std::cout << (b1 & b2) << std::endl;
  std::cout << (b1 | b2) << std::endl;
  std::cout << (b1 ^ b2) << std::endl;

  std::cout << (b1 ^ b2)[0] << std::endl;

#if __cplusplus >= 202002L
  std::cout << "C++20\n";
#elif __cplusplus >= 201703L
  std::cout << "C++17\n";
#elif __cplusplus >= 201402L
  std::cout << "C++14\n";
#endif

  return 0;
}