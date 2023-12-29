#ifndef NEZUMI_INCLUDE_NEZUMI_MDVECTOR_HPP
#define NEZUMI_INCLUDE_NEZUMI_MDVECTOR_HPP

#if __cplusplus >= 201402L

#include "Base.hpp"

#include <vector>

namespace nezumi {

template <typename DataType> //
class MDVector {
public:
private:
  std::vector<u64> mShape;
  std::vector<DataType> mData;
};

} // namespace nezumi

#else  // C++11 or older
static_assert(false, "Nezumi library requires C++14 or newer");
#endif // C++14 or newer

#endif // NEZUMI_INCLUDE_NEZUMI_MDVECTOR_HPP