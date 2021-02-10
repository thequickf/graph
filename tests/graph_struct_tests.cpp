#include <gtest/gtest.h>

#include <graph.hpp>
#include <test_utils/path_finding_on_map.hpp>

#include <algorithm>
#include <map>
#include <queue>
#include <set>
#include <string>

namespace {

struct BFSPathPoint {
  BFSPathPoint() : point(), type(-1) {}
  BFSPathPoint(graph_tests::PointOnMap point, char type) :
      point(point), type(type) {}
  graph_tests::PointOnMap point;
  char type;
};
  
}  // namespace

namespace graph_tests {

TEST(GraphStruct, SimpleIntGraph) {
  graph::Graph<int>g({1, 2, 3});
  g.addEdge({1, 2});
  g.addEdge({2, 3});
  g.addEdge({3, 1});
  auto n1 = g.neighbors(1);
  std::sort(n1.begin(), n1.end());
  EXPECT_EQ(n1.size(), 2);
  EXPECT_EQ(n1[0], 2);
  EXPECT_EQ(n1[1], 3);
}

TEST(GraphStruct, DirectedIntGraph) {
  graph::Graph<int, graph::Directed>g({1, 2, 3});
  g.addEdge({1, 2});
  g.addEdge({2, 3});
  g.addEdge({3, 1});
  auto n1 = g.neighbors(1);
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
    for (const PointOnMap& v : graph.neighbors(u)) {
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
      answer.push_back(bfs_path[u].type);
      u = bfs_path[u].point;
    } while (u != finish);
  }
}

}  // graph_tests