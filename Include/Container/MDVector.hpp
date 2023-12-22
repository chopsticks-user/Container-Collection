#ifndef CONTAINER_COLLECTION_MDVECTOR_HPP
#define CONTAINER_COLLECTION_MDVECTOR_HPP

#include "Base.hpp"

#include <vector>

namespace container {

template <typename DataType> //
class MDVector {
public:
private:
  std::vector<u64> mShape;
  std::vector<DataType> mData;
};

} // namespace container

#endif // CONTAINER_COLLECTION_MDVECTOR_HPP