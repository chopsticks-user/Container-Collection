#ifndef CONTAINER_COLLECTION_SPARSE_VECTOR_HPP
#define CONTAINER_COLLECTION_SPARSE_VECTOR_HPP

#include "Base.hpp"

#include <unordered_map>
#include <vector>

namespace container {

class ISparseVector {};

template <typename DataType, typename IndexType = uint64_t> //
class SparseVector : public ISparseVector {
public:
  uint64_t size() const noexcept { return this->mData.size(); }

  auto begin() const noexcept { return this->mData.cbegin(); }
  auto end() const noexcept { return this->mData.cend(); }

  bool exists(const uint64_t &id) const noexcept {
    return this->mIDToIndex.find(id) != this->mIDToIndex.end();
  }

  DataType &at(uint64_t id) { return this->mData[this->mIDToIndex.at(id)]; }

  DataType &operator[](uint64_t id) { return this->at(id); }

  DataType &operator[](uint64_t id) const { return this->at(id); }

  void add(uint64_t id, DataType value) {
    if (!this->exists(id)) {
      this->mData.emplace_back(value);
      this->mIDToIndex[id] = this->mData.size() - 1;
      this->mIndexToID[this->mData.size() - 1] = id;
    } else {
      this->mData[this->mIDToIndex[id]] = std::move(value);
    }
  }

  void remove(uint64_t id) noexcept override {
    if (!this->exists(id)) {
      return;
    }

    auto lastElementIndex = this->mData.size() - 1;
    auto lastElementID = this->mIndexToID[lastElementIndex];
    auto removedIndex = this->mIDToIndex[id];

    this->mIndexToID[removedIndex] = lastElementID;
    this->mIDToIndex[lastElementID] = removedIndex;

    std::swap(this->mData[removedIndex], this->mData[lastElementIndex]);
    this->mIndexToID.erase(lastElementIndex);
    this->mIDToIndex.erase(id);
    this->mData.pop_back();
  }

private:
  std::unordered_map<uint64_t, uint64_t> mIDToIndex = {};
  std::unordered_map<uint64_t, uint64_t> mIndexToID = {};
  std::vector<DataType> mData = {};
};

} // namespace container

#endif // CONTAINER_COLLECTION_SPARSE_VECTOR_HPP