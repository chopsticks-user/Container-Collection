#ifndef CONTAINER_COLLECTION_DYNAMIC_BITSET_HPP
#define CONTAINER_COLLECTION_DYNAMIC_BITSET_HPP

#include "Base.hpp"

#include <algorithm>
#include <bitset>
#include <cstdint>
#include <iostream>
#include <memory>

namespace container {

class DynamicBitset {
public:
  DynamicBitset() noexcept = default;

  explicit DynamicBitset(uint64_t bitCount)
      : mBits{std::make_unique<uint64_t[]>(allocSize(bitCount)).release()},
        mAllocSize{allocSize(bitCount)}, mBitCount{bitCount} {}

  ~DynamicBitset() { this->clear(); }

  DynamicBitset(const DynamicBitset &rhs)
      : mBits{std::make_unique<uint64_t[]>(rhs.mAllocSize).release()},
        mAllocSize{rhs.mAllocSize}, mBitCount{rhs.mBitCount} {
    std::copy(rhs.mBits, rhs.mBits + rhs.mAllocSize, this->mBits);
  }

  DynamicBitset(DynamicBitset &&rhs) noexcept
      : mBits{std::move(rhs.mBits)}, mAllocSize{std::move(rhs.mAllocSize)},
        mBitCount{std::move(rhs.mBitCount)} {
    rhs.mBits = nullptr;
  }

  DynamicBitset &operator=(const DynamicBitset &rhs) {
    this->mBits = std::make_unique<uint64_t[]>(rhs.mAllocSize).release();
    std::copy(rhs.mBits, rhs.mBits + rhs.mAllocSize, this->mBits);
    this->mAllocSize = rhs.mAllocSize;
    this->mBitCount = rhs.mBitCount;
    return *this;
  }

  DynamicBitset &operator=(DynamicBitset &&rhs) noexcept {
    std::swap(this->mBits, rhs.mBits);
    std::swap(this->mAllocSize, rhs.mAllocSize);
    std::swap(this->mBitCount, rhs.mBitCount);
    return *this;
  }

public:
  bool operator==(const DynamicBitset &rhs) const noexcept {
    if (rhs.mBitCount != this->mBitCount) {
      return false;
    }

    for (u64 i = 0; i < this->mAllocSize; ++i) {
      if (this->mBits[i] != rhs.mBits[i]) {
        return false;
      }
    }

    return true;
  }

  bool operator!=(const DynamicBitset &rhs) const noexcept {
    return !(*this == rhs);
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
    while (start < bitset.mBitCount) {
      os << bitset.get(start);
      ++start;
    }
    return os;
  }

public:
  uint64_t size() const noexcept { return this->mBitCount; }

  void resize(uint64_t bitCount) {
    uint64_t newAllocSize = allocSize(bitCount);
    uint64_t oldAllocSize = this->mAllocSize;
    if (newAllocSize > oldAllocSize) {
      uint64_t *newBits = std::make_unique<uint64_t[]>(newAllocSize).release();
      if (this->mBits == nullptr) {
        this->mBits = newBits;
      } else {
        std::copy(this->mBits, this->mBits + oldAllocSize, newBits);
      }
    }
    this->mBitCount = bitCount;
    this->mAllocSize = newAllocSize;
  }

  bool get(uint64_t index) const noexcept {
    return (this->mBits[index / 64] >> (63 - index % 64)) & 1;
  }

  void set(uint64_t index, bool value = true) noexcept {
    uint64_t offset = index % 64;
    uint64_t msk = mask(offset, offset + 1);
    if (value) {
      this->mBits[index / 64] |= msk;
    } else {
      this->mBits[index / 64] &= (~msk);
    }
  }

  void clear() noexcept {
    this->mBitCount = 0;
    this->mAllocSize = 0;
    delete[] this->mBits;
  }

  // TODO: insert and remove a bit

private:
  static uint64_t mask(uint64_t start, uint64_t end) {
    return (uint64_t(~0) << (64 - end)) ^
           (start ? (uint64_t(~0) << (64 - start)) : 0);
  }

  static uint64_t allocSize(uint64_t bitCount) {
    return (bitCount / 64) + (bitCount % 64 ? 1 : 0);
  }

public:
  uint64_t *mBits = nullptr;
  uint64_t mAllocSize = 0;
  uint64_t mBitCount = 0;
};

} // namespace container

#endif // CONTAINER_COLLECTION_DYNAMIC_BITSET_HPP