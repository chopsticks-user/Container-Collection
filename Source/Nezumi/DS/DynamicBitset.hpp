#ifndef NEZUMI_SOURCE_NEZUMI_DS_DYNAMIC_BITSET_HPP
#define NEZUMI_SOURCE_NEZUMI_DS_DYNAMIC_BITSET_HPP

#include "Core/Core.hpp"

namespace nezumi {

class DynamicBitset {
public:
  DynamicBitset() noexcept = default;

  explicit DynamicBitset(u64 bitCount)
      : mBits{std::make_unique<u64[]>(allocSize(bitCount)).release()},
        mAllocSize{allocSize(bitCount)}, mBitCount{bitCount} {}

  ~DynamicBitset() { this->clear(); }

  DynamicBitset(const DynamicBitset &rhs)
      : mBits{std::make_unique<u64[]>(rhs.mAllocSize).release()},
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

  auto operator=(const DynamicBitset &rhs) -> DynamicBitset &;

  DynamicBitset &operator=(DynamicBitset &&rhs) noexcept;

public:
  auto operator==(const DynamicBitset &rhs) const noexcept -> bool;

  auto operator!=(const DynamicBitset &rhs) const noexcept -> bool {
    return !(*this == rhs);
  }

  auto operator~() const -> DynamicBitset;

  auto operator!() const noexcept -> bool;

  // TODO: Bitwise-assignment operators

  DynamicBitset operator&(const DynamicBitset &rhs) const;

  DynamicBitset operator|(const DynamicBitset &rhs) const;

  DynamicBitset operator^(const DynamicBitset &rhs) const;

  friend std::ostream &operator<<(std::ostream &os,
                                  const DynamicBitset &bitset);

public:
  // TODO: insert and remove a bit

  // TODO: Flip i-th bit

  // TODO: Set a value to all bits

  auto size() const noexcept -> u64 { return this->mBitCount; }

  auto resize(u64 bitCount) -> void;

  auto get(u64 index) const noexcept -> bool {
    return (this->mBits[index / 64] >> (63 - index % 64)) & 1;
  }

  auto set(u64 index, bool value = true) noexcept -> void;

  void clear() noexcept {
    this->mBitCount = 0;
    this->mAllocSize = 0;
    if (this->mBits != nullptr) {
      delete[] this->mBits;
      this->mBits = nullptr;
    }
  }

  auto equals(const DynamicBitset &rhs) const noexcept -> bool;

  auto flip() noexcept -> void;

private:
  static u64 mask(u64 start, u64 end) noexcept {
    return (u64(~0) << (64 - end)) ^ (start ? (u64(~0) << (64 - start)) : 0);
  }

  static u64 allocSize(u64 bitCount) noexcept {
    return (bitCount / 64) + (bitCount % 64 ? 1 : 0);
  }

  void disableUnusedBits() noexcept {
    if (this->mBits != nullptr) {
      this->mBits[this->mAllocSize - 1] &= mask(0, this->mBitCount % 64);
    }
  }

public:
  u64 *mBits = nullptr;
  u64 mAllocSize = 0;
  u64 mBitCount = 0;
};

} // namespace nezumi

#endif // NEZUMI_SOURCE_NEZUMI_DS_DYNAMIC_BITSET_HPP