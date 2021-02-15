#include <gtest/gtest.h>

#include <graph.hpp>
#include <test_utils/path_finding_on_map.hpp>

#include <algorithm>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <utility>

namespace {

struct BFSPathPoint {
  BFSPathPoint() : point(), type(-1) {}
  BFSPathPoint(graph_tests::PointOnMap point, char type) :
      point(std::move(point)), type(type) {}
  graph_tests::PointOnMap point;
  char type;
};
  
}  // namespace

namespace graph_tests {

template <typename T>
class GraphStruct : public ::testing::Test {};

TYPED_TEST_SUITE_P(GraphStruct);

TYPED_TEST_P(GraphStruct, IntGraphConstructability) {
  TypeParam g({1, 2, 3});
  g.AddNode(4);
  g.AddNode(4);
  g.AddEdge({1, 2});
  g.AddEdge(2, 3);
  g.AddEdge({3, 1});
  g.AddEdge(3, 1);
  g.Neighbors(1);
  g.InEdges(1);
  g.OutEdges(1);
}

REGISTER_TYPED_TEST_SUITE_P(GraphStruct, IntGraphConstructability);

using SimpleIntGraphTypes = testing::Types<
    graph::Graph<int>,
    graph::Graph<int, graph::HashTableBased>,
    graph::Graph<int, graph::Directed>,
    graph::Graph<int, graph::HashTableBased, graph::Directed>,
    graph::Graph<int, graph::HashTableBased, graph::Multigraph>,
    graph::Graph<int, graph::Multigraph, graph::Directed>,
    graph::Graph<
        int, graph::HashTableBased, graph::Multigraph, graph::Directed>
>;

INSTANTIATE_TYPED_TEST_SUITE_P(
    Constructability, GraphStruct, SimpleIntGraphTypes);

TEST(SimpleTest, IntGraph) {
  graph::Graph<int>g({1, 2, 3});
  g.AddEdge({1, 2});
  g.AddEdge({2, 3});
  g.AddEdge({3, 1});
  auto n1 = g.Neighbors(1);
  std::sort(n1.begin(), n1.end());
  EXPECT_EQ(n1.size(), 2);
  EXPECT_EQ(n1[0], 2);
  EXPECT_EQ(n1[1], 3);
}

TEST(SimpleTest, DirectedIntGraph) {
  graph::Graph<int, graph::Directed>g({1, 2, 3});
  g.AddEdge({1, 2});
  g.AddEdge({2, 3});
  g.AddEdge({3, 1});
  auto n1 = g.Neighbors(1);
  std::sort(n1.begin(), n1.end());
  EXPECT_EQ(n1.size(), 1);
  EXPECT_EQ(n1[0], 2);
}

TEST_P(PathFindingOnMap, ManualBFS) {
  const PointOnMap finish = GetFinish();
  std::queue<PointOnMap> q;
  q.push(finish);
  std::set<PointOnMap> visited;
  std::map<PointOnMap, BFSPathPoint> bfs_path;
  while (!q.empty()) {
    const PointOnMap u = q.front();
    q.pop();
    visited.insert(u);
    for (const PointOnMap& v : graph_.Neighbors(u)) {
      if (visited.find(v) == visited.end()) {
        q.push(v);
        if (u.i < v.i)
          bfs_path[v] = {u, 'U'};
        else if (u.i > v.i)
          bfs_path[v] = {u, 'D'};
        else if (u.j < v.j)
          bfs_path[v] = {u, 'L'};
        else if (u.j > v.j)
          bfs_path[v] = {u, 'R'};
      }
    }
  }
  const PointOnMap start = GetStart();
  if (visited.find(start) != visited.end()) {
    PointOnMap u = start;
    do {
      answer_.push_back(bfs_path[u].type);
      u = bfs_path[u].point;
    } while (u != finish);
  }
}

}  // graph_tests
