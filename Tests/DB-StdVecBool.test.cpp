#include "Container/DynamicBitset.hpp"

#include <algorithm>
#include <gtest/gtest.h>
#include <vector>

using container::DynamicBitset;
typedef std::vector<bool> StdDynamicBitset;

static constexpr int64_t nSamples = 10000;
static constexpr uint64_t maxSize = 1000000;

TEST(File, Info) {
  std::cerr << "\nRunning tests provided in " << __FILE__ << "\n\n";
}

TEST(SizeInit, BenchmarkDynamicBitset) {
  int64_t iSample = nSamples * 10;
  while (iSample--) {
    DynamicBitset bitset(maxSize);
  }
}

TEST(SizeInit, BenchmarkStdVecBool) {
  int64_t iSample = nSamples * 10;
  while (iSample--) {
    StdDynamicBitset stdBitset(maxSize);
  }
}

TEST(SizeInit, CompareSizes) {
  std::srand(static_cast<uint32_t>(std::time(nullptr)));

  int64_t iSample = nSamples;
  while (iSample--) {
    uint64_t size = 1 + std::rand() / ((RAND_MAX + 1u) / maxSize);

    StdDynamicBitset stdBitset(size);
    DynamicBitset bitset(size);

    EXPECT_EQ(bitset.size(), stdBitset.size());
  }

  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

TEST(Resize, BenchmarkDynamicBitset) {
  int64_t iSample = nSamples;
  while (iSample--) {
    DynamicBitset bitset(1000);

    uint64_t newSize = 1 + std::rand() / ((RAND_MAX + 1u) / maxSize);
    bitset.resize(newSize);
  }
}

TEST(Resize, BenchmarkStdVecBool) {
  int64_t iSample = nSamples;
  while (iSample--) {
    StdDynamicBitset stdBitset(1000);

    uint64_t newSize = 1 + std::rand() / ((RAND_MAX + 1u) / maxSize);
    stdBitset.resize(newSize);
  }
}

TEST(Resize, CompareSizes) {
  std::srand(static_cast<uint32_t>(std::time(nullptr)));

  int64_t iSample = nSamples;
  while (iSample--) {
    uint64_t size = 1 + std::rand() / ((RAND_MAX + 1u) / maxSize);

    StdDynamicBitset stdBitset(size);
    DynamicBitset bitset(size);

    uint64_t newSize = 1 + std::rand() / ((RAND_MAX + 1u) / maxSize);
    stdBitset.resize(newSize);
    bitset.resize(newSize);
    EXPECT_EQ(bitset.size(), stdBitset.size());
  }

  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

constexpr uint64_t opTestBitCounts = 100000;
constexpr uint64_t opTestStaticBitCounts = 20000;
constexpr int64_t opTestNSamples = 10000;

typedef std::bitset<opTestStaticBitCounts> StdBitset;

TEST(OpAnd, BenchmarkDynamicBitset) {
  DynamicBitset bitset1(opTestBitCounts);
  DynamicBitset bitset2(opTestBitCounts);

  auto iSample = opTestNSamples;
  while (iSample--) {
    [[maybe_unused]] auto bitsetResult = bitset1 & bitset2;
  }
}

TEST(OpAnd, BenchmarkStdVecU64) {
  std::vector<uint64_t> bitset1(opTestBitCounts / 64);
  std::vector<uint64_t> bitset2(opTestBitCounts / 64);

  auto iSample = opTestNSamples;
  while (iSample--) {
    std::vector<uint64_t> bitsetResult(opTestBitCounts);
    for (uint64_t i = 0; i < opTestBitCounts; ++i) {
      bitsetResult[i] = bitset1[i] & bitset2[2];
    }
  }
}

TEST(OpAnd, CompareResults) {
  // uint64_t opTestBitCounts = 128;
  StdBitset stdBitset1, stdBitset2;
  DynamicBitset bitset1(opTestStaticBitCounts), bitset2(opTestStaticBitCounts);

  auto iSample = 50;
  // auto iSample = 1;
  while (iSample--) {
    int nRands = 1 + std::rand() / ((RAND_MAX + 1u) / 100);
    while (nRands--) {
      uint64_t index1 =
          std::rand() / ((RAND_MAX + 1u) / opTestStaticBitCounts - 1);
      uint64_t index2 =
          std::rand() / ((RAND_MAX + 1u) / opTestStaticBitCounts - 1);

      stdBitset1[opTestStaticBitCounts - index1 - 1] = 1;
      stdBitset2[opTestStaticBitCounts - index2 - 1] = 1;
      bitset1.set(index1);
      bitset2.set(index2);
    }

    stdBitset1[0] = 1;
    stdBitset2[opTestStaticBitCounts - 1] = 1;
    bitset1.set(opTestStaticBitCounts - 1);
    bitset2.set(0);

    StdBitset stdBitsetResult = stdBitset1 & stdBitset2;
    DynamicBitset bitsetResult = bitset1 & bitset2;

    for (uint64_t i = 0; i < opTestStaticBitCounts; ++i) {
      EXPECT_EQ(
          static_cast<bool>(stdBitsetResult[opTestStaticBitCounts - 1 - i]),
          bitsetResult.get(i));
    }

    stdBitsetResult = stdBitset2 & stdBitset1;
    bitsetResult = bitset2 & bitset1;
    for (uint64_t i = 0; i < opTestStaticBitCounts; ++i) {
      EXPECT_EQ(
          static_cast<bool>(stdBitsetResult[opTestStaticBitCounts - 1 - i]),
          bitsetResult.get(i));
    }
  }
}

TEST(OpOr, BenchmarkDynamicBitset) {
  DynamicBitset bitset1(opTestBitCounts);
  DynamicBitset bitset2(opTestBitCounts);

  auto iSample = opTestNSamples;
  while (iSample--) {
    [[maybe_unused]] auto bitsetResult = bitset1 | bitset2;
  }
}

TEST(OpOr, BenchmarkStdVecU64) {
  std::vector<uint64_t> bitset1(opTestBitCounts / 64);
  std::vector<uint64_t> bitset2(opTestBitCounts / 64);

  auto iSample = opTestNSamples;
  while (iSample--) {
    std::vector<uint64_t> bitsetResult(opTestBitCounts);
    for (uint64_t i = 0; i < opTestBitCounts; ++i) {
      bitsetResult[i] = bitset1[i] | bitset2[2];
    }
  }
}

TEST(OpOr, CompareResults) {
  // uint64_t opTestBitCounts = 128;
  StdBitset stdBitset1, stdBitset2;
  DynamicBitset bitset1(opTestStaticBitCounts), bitset2(opTestStaticBitCounts);

  auto iSample = 50;
  // auto iSample = 1;
  while (iSample--) {
    int nRands = 1 + std::rand() / ((RAND_MAX + 1u) / 100);
    while (nRands--) {
      uint64_t index1 =
          std::rand() / ((RAND_MAX + 1u) / opTestStaticBitCounts - 1);
      uint64_t index2 =
          std::rand() / ((RAND_MAX + 1u) / opTestStaticBitCounts - 1);

      stdBitset1[opTestStaticBitCounts - index1 - 1] = 1;
      stdBitset2[opTestStaticBitCounts - index2 - 1] = 1;
      bitset1.set(index1);
      bitset2.set(index2);
    }

    stdBitset1[0] = 1;
    stdBitset2[opTestStaticBitCounts - 1] = 1;
    bitset1.set(opTestStaticBitCounts - 1);
    bitset2.set(0);

    StdBitset stdBitsetResult = stdBitset1 | stdBitset2;
    DynamicBitset bitsetResult = bitset1 | bitset2;

    for (uint64_t i = 0; i < opTestStaticBitCounts; ++i) {
      EXPECT_EQ(
          static_cast<bool>(stdBitsetResult[opTestStaticBitCounts - 1 - i]),
          bitsetResult.get(i));
    }

    stdBitsetResult = stdBitset2 | stdBitset1;
    bitsetResult = bitset2 | bitset1;
    for (uint64_t i = 0; i < opTestStaticBitCounts; ++i) {
      EXPECT_EQ(
          static_cast<bool>(stdBitsetResult[opTestStaticBitCounts - 1 - i]),
          bitsetResult.get(i));
    }
  }
}

TEST(OpXor, BenchmarkDynamicBitset) {
  DynamicBitset bitset1(opTestBitCounts);
  DynamicBitset bitset2(opTestBitCounts);

  auto iSample = opTestNSamples;
  while (iSample--) {
    [[maybe_unused]] auto bitsetResult = bitset1 ^ bitset2;
  }
}

TEST(OpXor, BenchmarkStdVecU64) {
  std::vector<uint64_t> bitset1(opTestBitCounts / 64);
  std::vector<uint64_t> bitset2(opTestBitCounts / 64);

  auto iSample = opTestNSamples;
  while (iSample--) {
    std::vector<uint64_t> bitsetResult(opTestBitCounts);
    for (uint64_t i = 0; i < opTestBitCounts; ++i) {
      bitsetResult[i] = bitset1[i] ^ bitset2[2];
    }
  }
}

TEST(OpXor, CompareResults) {
  // uint64_t opTestBitCounts = 128;
  StdBitset stdBitset1, stdBitset2;
  DynamicBitset bitset1(opTestStaticBitCounts), bitset2(opTestStaticBitCounts);

  auto iSample = 50;
  // auto iSample = 1;
  while (iSample--) {
    int nRands = 1 + std::rand() / ((RAND_MAX + 1u) / 100);
    while (nRands--) {
      uint64_t index1 =
          std::rand() / ((RAND_MAX + 1u) / opTestStaticBitCounts - 1);
      uint64_t index2 =
          std::rand() / ((RAND_MAX + 1u) / opTestStaticBitCounts - 1);

      stdBitset1[opTestStaticBitCounts - index1 - 1] = 1;
      stdBitset2[opTestStaticBitCounts - index2 - 1] = 1;
      bitset1.set(index1);
      bitset2.set(index2);
    }

    stdBitset1[0] = 1;
    stdBitset2[opTestStaticBitCounts - 1] = 1;
    bitset1.set(opTestStaticBitCounts - 1);
    bitset2.set(0);

    StdBitset stdBitsetResult = stdBitset1 ^ stdBitset2;
    DynamicBitset bitsetResult = bitset1 ^ bitset2;

    for (uint64_t i = 0; i < opTestStaticBitCounts; ++i) {
      EXPECT_EQ(
          static_cast<bool>(stdBitsetResult[opTestStaticBitCounts - 1 - i]),
          bitsetResult.get(i));
    }

    stdBitsetResult = stdBitset2 ^ stdBitset1;
    bitsetResult = bitset2 ^ bitset1;
    for (uint64_t i = 0; i < opTestStaticBitCounts; ++i) {
      EXPECT_EQ(
          static_cast<bool>(stdBitsetResult[opTestStaticBitCounts - 1 - i]),
          bitsetResult.get(i));
    }

    // std::cerr << stdBitset1 << '\n';
    // std::cerr << stdBitset2 << "\n";
    // std::cerr << stdBitsetResult << "\n\n";
    // std::cerr << bitset1 << '\n';
    // std::cerr << bitset2 << '\n';
    // std::cerr << bitsetResult << "\n\n";
  }
}
