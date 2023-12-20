#ifndef CONTAINER_SOURCE_DYNAMIC_BITSET_HPP
#define CONTAINER_SOURCE_DYNAMIC_BITSET_HPP

#include <algorithm>
#include <bitset>
#include <cstdint>
#include <cstring>
#include <iostream>

namespace container {

class DynamicBitset {
public:
  DynamicBitset() = default;
  ~DynamicBitset() { this->clear(); }

  DynamicBitset(const DynamicBitset &rhs)
      : mBits{new uint64_t[rhs.mAllocSize]}, mAllocSize{rhs.mAllocSize},
        mSize{rhs.mSize} {
    std::copy(rhs.mBits, rhs.mBits + rhs.mAllocSize, this->mBits);
  }

  DynamicBitset(DynamicBitset &&rhs) noexcept
      : mBits{std::move(rhs.mBits)}, mAllocSize{std::move(rhs.mAllocSize)},
        mSize{std::move(rhs.mSize)} {
    rhs.mBits = nullptr;
  }

  DynamicBitset &operator=(const DynamicBitset &rhs) {
    this->mBits = new uint64_t[rhs.mAllocSize];
    std::copy(rhs.mBits, rhs.mBits + rhs.mAllocSize, this->mBits);
    this->mAllocSize = rhs.mAllocSize;
    this->mSize = rhs.mSize;
    return *this;
  }

  DynamicBitset &operator=(DynamicBitset &&rhs) noexcept {
    this->mBits = std::move(rhs.mBits);
    this->mAllocSize = std::move(rhs.mAllocSize);
    this->mSize = std::move(rhs.mSize);
    rhs.mBits = nullptr;
    return *this;
  }

  uint64_t size() const noexcept { return this->mSize; }

  void resize(uint64_t newSize) {
    uint64_t newAllocSize = newSize / 64 + newSize % 64 ? 1 : 0;
    uint64_t oldAllocSize = this->mSize / 64;
    if (newAllocSize > oldAllocSize) {
      uint64_t *newBits = new uint64_t[newAllocSize];
      if (this->mBits == nullptr) {
        std::memset(newBits, 0, newAllocSize);
        this->mBits = newBits;
      } else {
        std::copy(this->mBits, this->mBits + oldAllocSize, newBits);
      }
    }
    this->mSize = newSize;
    this->mAllocSize = newAllocSize;
  }

  bool get(uint64_t index) const noexcept {
    return (this->mBits[index / 64] >> (63 - index % 64)) & 1;
  }

  void set(uint64_t index, bool value = true) noexcept {
    index %= 64;
    uint64_t msk = mask(index, index + 1);
    if (value) {
      this->mBits[index / 64] |= msk;
    } else {
      this->mBits[index / 64] &= msk;
    }
  }

  void clear() noexcept {
    this->mSize = 0;
    this->mAllocSize = 0;
    delete[] this->mBits;
  }

  friend std::ostream &operator<<(std::ostream &os,
                                  const DynamicBitset &bitset) {
    if (bitset.mAllocSize == 0) {
      return os;
    }

    if (bitset.mAllocSize > 1) {
      for (uint64_t i = 0; i < bitset.mAllocSize - 1; ++i) {
        os << std::bitset<64>(bitset.mBits[i]);
      }
    }

    uint64_t start = (bitset.mAllocSize - 1) * 64;
    while (start < bitset.mSize) {
      os << bitset.get(start);
      ++start;
    }
    return os;
  }

  DynamicBitset operator~() const {
    DynamicBitset result = *this;
    for (uint64_t i = 0; i < result.mAllocSize; ++i) {
      result.mBits[i] = ~result.mBits[i];
    }
    return result;
  }

  bool operator!() const noexcept {
    for (uint64_t i = 0; i < this->mAllocSize; ++i) {
      if (this->mBits[i]) {
        return false;
      }
    }
    return true;
  }

  DynamicBitset operator&(DynamicBitset rhs) const {
    for (uint64_t i = 0; i < rhs.mAllocSize; ++i) {
      rhs.mBits[i] &= this->mBits[i];
    }
    return rhs;
  }

  DynamicBitset operator|(DynamicBitset rhs) const {
    for (uint64_t i = 0; i < rhs.mAllocSize; ++i) {
      rhs.mBits[i] |= this->mBits[i];
    }
    return rhs;
  }

  DynamicBitset operator^(DynamicBitset rhs) const {
    for (uint64_t i = 0; i < rhs.mAllocSize; ++i) {
      rhs.mBits[i] ^= this->mBits[i];
    }
    return rhs;
  }

private:
  static uint64_t mask(uint64_t start, uint64_t end) {
    return (uint64_t(~0) << (64 - end)) ^
           (start ? (uint64_t(~0) << (64 - start)) : 0);
  }

private:
  uint64_t *mBits = nullptr;
  uint64_t mAllocSize = 0;
  uint64_t mSize = 0;
};

} // namespace container

#endif // CONTAINER_SOURCE_DYNAMIC_BITSET_HPP