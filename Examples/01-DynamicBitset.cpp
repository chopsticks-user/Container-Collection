#include <Nezumi/DynamicBitset.hpp>

#include <bitset>

int main() {
  nezumi::DynamicBitset bits1;
  bits1.resize(66);
  bits1.set(1);
  bits1.set(3);
  bits1.set(5);
  std::cout << bits1 << '\n';
  bits1 = ~bits1;
  std::cout << bits1 << '\n';
  bits1.resize(7);
  std::cout << bits1 << '\n';
  bits1.resize(66);
  std::cout << bits1 << '\n';

  nezumi::DynamicBitset bits2;
  bits2.resize(66);
  bits2.set(0);
  bits2.set(2);
  bits2.set(4);
  bits2.set(5);

  std::cout << '\n' << bits1 << '\n';
  std::cout << bits2 << '\n';

  auto bits3 = bits1 & bits2;
  auto bits4 = bits1 | bits2;
  auto bits5 = bits1 ^ bits2;

  std::cout << '\n' << bits3 << '\n';
  std::cout << bits4 << '\n';
  std::cout << bits5 << '\n';

  std::cout << '\n' << bits3.equals(bits4) << '\n'; // false
  std::cout << bits4.equals(bits3) << '\n';         // false
  std::cout << '\n' << bits4.equals(bits5) << '\n'; // false
  std::cout << bits5.equals(bits4) << '\n';         // false
  std::cout << '\n' << bits3.equals(bits5) << '\n'; // false
  std::cout << bits5.equals(bits3) << '\n';         // false

  bits3.resize(5);
  std::cout << '\n' << bits3 << '\n';

  std::cout << '\n' << bits3.equals(bits4) << '\n'; // true
  std::cout << bits4.equals(bits3) << '\n';         // true
  std::cout << '\n' << bits3.equals(bits5) << '\n'; // false
  std::cout << bits5.equals(bits3) << '\n';         // false

  bits3.resize(0);

  std::cout << '\n' << bits3.equals(bits4) << '\n'; // true
  std::cout << bits4.equals(bits3) << '\n';         // true
  std::cout << '\n' << bits3.equals(bits5) << '\n'; // true
  std::cout << bits5.equals(bits3) << '\n';         // true

  return 0;
}