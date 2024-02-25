#include "DynamicBitset.hpp"

#include <algorithm>
#include <bitset>
#include <cstring>
#include <iostream>

namespace nezumi {

auto DynamicBitset::operator=(const DynamicBitset &rhs) -> DynamicBitset & {
  this->mBits = std::make_unique<u64[]>(rhs.mAllocSize).release();
  std::copy(rhs.mBits, rhs.mBits + rhs.mAllocSize, this->mBits);
  this->mAllocSize = rhs.mAllocSize;
  this->mBitCount = rhs.mBitCount;
  this->disableUnusedBits();
  return *this;
}

auto DynamicBitset::operator=(DynamicBitset &&rhs) noexcept -> DynamicBitset & {
  std::swap(this->mBits, rhs.mBits);
  std::swap(this->mAllocSize, rhs.mAllocSize);
  std::swap(this->mBitCount, rhs.mBitCount);
  this->disableUnusedBits();
  return *this;
}

auto DynamicBitset::resize(u64 bitCount) -> void {
  if (bitCount == 0) {
    this->clear();
  }

  u64 newAllocSize = allocSize(bitCount);
  u64 oldAllocSize = this->mAllocSize;

  if (newAllocSize > oldAllocSize) {
    auto pNewBits = std::make_unique<u64[]>(newAllocSize);
    u64 *newBits = pNewBits.get();

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

auto DynamicBitset::set(u64 index, bool value = true) noexcept -> void {
  u64 offset = index % 64;
  u64 msk = mask(offset, offset + 1);
  if (value) {
    this->mBits[index / 64] |= msk;
  } else {
    this->mBits[index / 64] &= (~msk);
  }
}

auto DynamicBitset::equals(const DynamicBitset &rhs) const noexcept -> bool {
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

auto DynamicBitset::operator==(const DynamicBitset &rhs) const noexcept
    -> bool {
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

auto DynamicBitset::flip() noexcept -> void {
  for (u64 i = 0; i < this->mAllocSize; ++i) {
    this->mBits[i] = ~this->mBits[i];
  }
  this->disableUnusedBits();
}

auto DynamicBitset::operator~() const -> DynamicBitset {
  auto result = DynamicBitset{*this};
  result.flip();
  return result;
}

auto DynamicBitset::operator!() const noexcept -> bool {
  for (u64 i = 0; i < this->mAllocSize; ++i) {
    if (this->mBits[i]) {
      return false;
    }
  }
  return true;
}

auto DynamicBitset::operator&(const DynamicBitset &rhs) const -> DynamicBitset {
  u64 minAllocSize = std::min(this->mAllocSize, rhs.mAllocSize);
  DynamicBitset result(std::max(this->size(), rhs.size()));

  for (u64 i = 0; i < minAllocSize; ++i) {
    result.mBits[i] = rhs.mBits[i] & this->mBits[i];
  }

  return result;
}

auto DynamicBitset::operator|(const DynamicBitset &rhs) const -> DynamicBitset {
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
  for (u64 i = 0; i < minAllocSize; ++i) {
    result.mBits[i] = rhs.mBits[i] | this->mBits[i];
  }
  for (u64 i = minAllocSize; i < maxAllocSize; ++i) {
    result.mBits[i] = pLongerBitset->mBits[i];
  }

  result.disableUnusedBits();
  return result;
}

auto DynamicBitset::operator^(const DynamicBitset &rhs) const -> DynamicBitset {
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
  for (u64 i = 0; i < minAllocSize; ++i) {
    result.mBits[i] = rhs.mBits[i] ^ this->mBits[i];
  }
  for (u64 i = minAllocSize; i < maxAllocSize; ++i) {
    result.mBits[i] = pLongerBitset->mBits[i] ^ 0;
  }

  result.disableUnusedBits();
  return result;
}

auto operator<<(std::ostream &os, const DynamicBitset &bitset)
    -> std::ostream & {
  if (bitset.mAllocSize == 0) {
    return os;
  }

  if (bitset.mAllocSize > 1) {
    for (u64 i = 0; i < bitset.mAllocSize - 1; ++i) {
      os << std::bitset<64>(bitset.mBits[i]);
    }
  }

  u64 start = (bitset.mAllocSize - 1) * 64;
  while (start < bitset.mBitCount) {
    os << bitset.get(start);
    ++start;
  }
  return os;
}

} // namespace nezumi