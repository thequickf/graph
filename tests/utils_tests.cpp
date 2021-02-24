#include <gtest/gtest.h>

#include <graph_traits.hpp>
#include <impl/base_utils.hpp>

#include <functional>
#include <string>

namespace {

struct NoDefaultConstructorComparable {
  NoDefaultConstructorComparable() = delete;
  NoDefaultConstructorComparable(std::string str) : str(std::move(str)) {}
  std::string str;
};

bool operator<(const NoDefaultConstructorComparable& left,
               const NoDefaultConstructorComparable& right) {
  return left.str < right.str;
}

struct NoDefaultConstructorHashable {
  NoDefaultConstructorHashable() = delete;
  NoDefaultConstructorHashable(std::string str) : str(std::move(str)) {}
  std::string str;
};

bool operator==(const NoDefaultConstructorHashable& left,
                const NoDefaultConstructorHashable& right) {
  return left.str == right.str;
}

}  // namespace

namespace std {

template<>
struct hash<NoDefaultConstructorHashable> {
  std::size_t operator()(
      const NoDefaultConstructorHashable& str) const noexcept {
    return std::hash<std::string>{}(str.str);
  }
};

}  // std

namespace graph_tests {

template <typename T>
class DsuStruct : public ::testing::Test {};

TYPED_TEST_SUITE_P(DsuStruct);

TYPED_TEST_P(DsuStruct, DsuConstructability) {
  const std::string s1 = "first";
  const std::string s2 = "second";
  const std::string s3 = "third";
  const std::string s4 = "fourth";
  const std::string su = "unknown";
  TypeParam dsu({s1, s2, s3});
  EXPECT_FALSE(dsu.Add(s1));
  EXPECT_TRUE(dsu.Add(s4));
  EXPECT_TRUE(dsu.Unite(s1, s2));
  EXPECT_FALSE(dsu.Unite(s1, s2));
  EXPECT_FALSE(dsu.Size(su));
  EXPECT_FALSE(dsu.Unite(s1, su));
  EXPECT_EQ(*dsu.Size(s1), 2);
}

REGISTER_TYPED_TEST_SUITE_P(DsuStruct, DsuConstructability);

using DsuSetTypes = testing::Types<
    graph_utils::Dsu<std::string>,
    graph_utils::Dsu<std::string, graph::HashTableBased>,
    graph_utils::Dsu<NoDefaultConstructorComparable>,
    graph_utils::Dsu<NoDefaultConstructorHashable, graph::HashTableBased>
>;

INSTANTIATE_TYPED_TEST_SUITE_P(DsuConstructability, DsuStruct, DsuSetTypes);

TEST(DsuUtil, IntDsuValidity) {
  graph_utils::Dsu<int> dsu({1, 2, 3, 4, 5});
  EXPECT_TRUE(dsu.Add(6));
  EXPECT_FALSE(dsu.Add(6));
  EXPECT_EQ(*dsu.Find(6), 6);
  EXPECT_FALSE(dsu.Find(7));
  EXPECT_TRUE(dsu.Unite(1, 2));
  EXPECT_FALSE(dsu.Unite(1, 2));
  EXPECT_FALSE(dsu.Unite(1, 7));
  EXPECT_TRUE(dsu.Unite(2, 3));
  EXPECT_TRUE(dsu.Unite(4, 5));
  EXPECT_TRUE(dsu.Unite(5, 6));
  EXPECT_FALSE(dsu.Unite(4, 6));
  EXPECT_LE(*dsu.Find(1), *dsu.Find(4));
  EXPECT_EQ(*dsu.Find(1), *dsu.Find(3));
  EXPECT_EQ(*dsu.Find(5), *dsu.Find(6));
  EXPECT_NE(*dsu.Find(3), *dsu.Find(4));
  EXPECT_EQ(*dsu.Size(1), 3);
  EXPECT_EQ(*dsu.Size(4), 3);
  EXPECT_TRUE(dsu.Unite(2, 5));
  EXPECT_EQ(*dsu.Find(3), *dsu.Find(4));
  EXPECT_EQ(*dsu.Find(1), *dsu.Find(6));
  EXPECT_EQ(*dsu.Size(3), 6);
  EXPECT_EQ(*dsu.Size(6), 6);
}

}  // graph_tests
