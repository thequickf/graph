#include <gtest/gtest.h>

#include <impl/base_utils.hpp>

#include <string>

namespace {
/*
struct NoDefaultConstructorComparable {
  NoDefaultConstructorComparable() = delete;
  NoDefaultConstructorComparable(const std::string str) : str(str) {}
  std::string str;
};

bool operator<(const NoDefaultConstructorComparable& left,
               const NoDefaultConstructorComparable& right) {
  return left.str < right.str;
}
//*/
}  // namespace

namespace graph_tests {

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
