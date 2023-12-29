#ifndef NEZUMI_INCLUDE_NEZUMI_DYNAMIC_BITSET_HPP
#define NEZUMI_INCLUDE_NEZUMI_DYNAMIC_BITSET_HPP

#if __cplusplus >= 201402L

#include "Base.hpp"

#include <algorithm>
#include <bitset>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>

namespace nezumi {

class DynamicBitset {
public:
  DynamicBitset() noexcept = default;

  explicit DynamicBitset(uint64_t bitCount)
      : mBits{std::make_unique<uint64_t[]>(allocSize(bitCount)).release()},
        mAllocSize{allocSize(bitCount)}, mBitCount{bitCount} {
    std::memset(this->mBits, 0, this->mAllocSize);
  }

  ~DynamicBitset() { this->clear(); }

  DynamicBitset(const DynamicBitset &rhs)
      : mBits{std::make_unique<uint64_t[]>(rhs.mAllocSize).release()},
        mAllocSize{rhs.mAllocSize}, mBitCount{rhs.mBitCount} {
    std::copy(rhs.mBits, rhs.mBits + rhs.mAllocSize, this->mBits);
    this->disableUnusedBits();
  }

  DynamicBitset(DynamicBitset &&rhs) noexcept
      : mBits{std::move(rhs.mBits)}, mAllocSize{std::move(rhs.mAllocSize)},
        mBitCount{std::move(rhs.mBitCount)} {
    rhs.mBits = nullptr;
    this->disableUnusedBits();
  }

  DynamicBitset &operator=(const DynamicBitset &rhs) {
    this->mBits = std::make_unique<uint64_t[]>(rhs.mAllocSize).release();
    std::copy(rhs.mBits, rhs.mBits + rhs.mAllocSize, this->mBits);
    this->mAllocSize = rhs.mAllocSize;
    this->mBitCount = rhs.mBitCount;
    this->disableUnusedBits();
    return *this;
  }

  DynamicBitset &operator=(DynamicBitset &&rhs) noexcept {
    std::swap(this->mBits, rhs.mBits);
    std::swap(this->mAllocSize, rhs.mAllocSize);
    std::swap(this->mBitCount, rhs.mBitCount);
    this->disableUnusedBits();
    return *this;
  }

public:
  uint64_t size() const noexcept { return this->mBitCount; }

  void resize(uint64_t bitCount) {
    if (bitCount == 0) {
      this->clear();
    }

    uint64_t newAllocSize = allocSize(bitCount);
    uint64_t oldAllocSize = this->mAllocSize;

    if (newAllocSize > oldAllocSize) {
      auto pNewBits = std::make_unique<uint64_t[]>(newAllocSize);
      uint64_t *newBits = pNewBits.get();

      if (this->mBits != nullptr) {
        std::copy(this->mBits, this->mBits + oldAllocSize, newBits);
        delete[] this->mBits;
      }

      this->mBits = pNewBits.release();
    }

    this->disableUnusedBits();
    this->mBitCount = bitCount;
    this->mAllocSize = newAllocSize;
  }

  bool get(uint64_t index) const {
    if (index >= this->mBitCount) {
      throw std::out_of_range("container::DynamicBitset: index out of range");
    }
    return (this->mBits[index / 64] >> (63 - index % 64)) & 1;
  }

  void set(uint64_t index, bool value = true) {
    if (index >= this->mBitCount) {
      throw std::out_of_range("container::DynamicBitset: index out of range");
    }

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
    if (this->mBits != nullptr) {
      delete[] this->mBits;
      this->mBits = nullptr;
    }
  }

  // TODO: insert and remove a bit

public:
  bool equals(const DynamicBitset &rhs) const noexcept {
    if (rhs.mBitCount == this->mBitCount) {
      return *this == rhs;
    }

    if (rhs.mBits == nullptr || this->mBits == nullptr) {
      return true;
    }

    auto allocSize = std::min(this->mAllocSize, rhs.mAllocSize);

    if (allocSize > 1) {
      for (u64 i = 0; i < this->mAllocSize - 1; ++i) {
        if (this->mBits[i] != rhs.mBits[i]) {
          return false;
        }
      }
    }

    auto offset = std::min(this->mBitCount, rhs.mBitCount) % 64;
    if (offset == 0) {
      return true;
    }

    auto offsetMask = mask(0, offset);
    return (this->mBits[allocSize - 1] & offsetMask) ==
           (rhs.mBits[allocSize - 1] & offsetMask);
  }

  bool operator==(const DynamicBitset &rhs) const noexcept {
    if (rhs.mBitCount != this->mBitCount) {
      return false;
    }

    if (rhs.mBits == nullptr || this->mBits == nullptr) {
      return rhs.mBits == nullptr && this->mBits == nullptr;
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

  // TODO: Flip i-th bit

  void flip() noexcept {
    for (uint64_t i = 0; i < this->mAllocSize; ++i) {
      this->mBits[i] = ~this->mBits[i];
    }
    this->disableUnusedBits();
  }

  // TODO: Set a value to all bits

  DynamicBitset operator~() const {
    DynamicBitset result = *this;
    result.flip();
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

  // TODO: Bitwise-assignment operators

  DynamicBitset operator&(const DynamicBitset &rhs) const {
    u64 minAllocSize = std::min(this->mAllocSize, rhs.mAllocSize);
    DynamicBitset result(std::max(this->size(), rhs.size()));

    for (uint64_t i = 0; i < minAllocSize; ++i) {
      result.mBits[i] = rhs.mBits[i] & this->mBits[i];
    }

    return result;
  }

  DynamicBitset operator|(const DynamicBitset &rhs) const {
    const DynamicBitset *pLongerBitset = this;
    u64 minAllocSize = this->mAllocSize;
    u64 maxAllocSize = minAllocSize;

    if (this->mAllocSize < rhs.mAllocSize) {
      pLongerBitset = &rhs;
      maxAllocSize = rhs.mAllocSize;
    } else {
      minAllocSize = rhs.mAllocSize;
    }

    DynamicBitset result(std::max(this->size(), rhs.size()));
    for (uint64_t i = 0; i < minAllocSize; ++i) {
      result.mBits[i] = rhs.mBits[i] | this->mBits[i];
    }
    for (uint64_t i = minAllocSize; i < maxAllocSize; ++i) {
      result.mBits[i] = pLongerBitset->mBits[i];
    }

    result.disableUnusedBits();
    return result;
  }

  DynamicBitset operator^(const DynamicBitset &rhs) const {
    const DynamicBitset *pLongerBitset = this;
    u64 minAllocSize = this->mAllocSize;
    u64 maxAllocSize = minAllocSize;

    if (this->mAllocSize < rhs.mAllocSize) {
      pLongerBitset = &rhs;
      maxAllocSize = rhs.mAllocSize;
    } else {
      minAllocSize = rhs.mAllocSize;
    }

    DynamicBitset result(std::max(this->size(), rhs.size()));
    for (uint64_t i = 0; i < minAllocSize; ++i) {
      result.mBits[i] = rhs.mBits[i] ^ this->mBits[i];
    }
    for (uint64_t i = minAllocSize; i < maxAllocSize; ++i) {
      result.mBits[i] = pLongerBitset->mBits[i] ^ 0;
    }

    result.disableUnusedBits();
    return result;
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

private:
  static uint64_t mask(uint64_t start, uint64_t end) noexcept {
    return (uint64_t(~0) << (64 - end)) ^
           (start ? (uint64_t(~0) << (64 - start)) : 0);
  }

  static uint64_t allocSize(uint64_t bitCount) noexcept {
    return (bitCount / 64) + (bitCount % 64 ? 1 : 0);
  }

  void disableUnusedBits() noexcept {
    if (this->mBits != nullptr) {
      this->mBits[this->mAllocSize - 1] &= mask(0, this->mBitCount % 64);
    }
  }

public:
  uint64_t *mBits = nullptr;
  uint64_t mAllocSize = 0;
  uint64_t mBitCount = 0;
};

} // namespace nezumi

#else  // C++11 or older
static_assert(false, "Nezumi library requires C++14 or newer");
#endif // C++14 or newer

#endif // NEZUMI_INCLUDE_NEZUMI_DYNAMIC_BITSET_HPP