#include <Container/DynamicBitset.hpp>

int main() {
  container::DynamicBitset bits;
  bits.resize(6);
  bits.set(1);
  bits.set(3);
  bits.set(5);
  container::DynamicBitset bits2;
  bits2.resize(6);
  bits2.set(0);
  bits2.set(2);
  bits2.set(4);
  bits2.set(5);
  std::cout << (bits & bits2) << std::endl;
  std::cout << (bits | bits2) << std::endl;
  std::cout << (bits ^ bits2) << std::endl;
  return 0;
}