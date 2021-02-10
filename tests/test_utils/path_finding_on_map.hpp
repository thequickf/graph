#ifndef TEST_UTILS_PATH_FINDING_ON_MAP
#define TEST_UTILS_PATH_FINDING_ON_MAP

#include <gtest/gtest.h>

#include <string>
#include <vector>

namespace graph_tests {

struct PointOnMap {
  PointOnMap() : i(-1), j(-1), type(-1) {}
  PointOnMap(size_t i, size_t j, char type) : i(i), j(j), type(type) {}
  friend bool operator==(const PointOnMap& lhp, const PointOnMap& rhp) {
    return lhp.i == rhp.i && lhp.j == rhp.j;
  }
  friend bool operator<(const PointOnMap& lhp, const PointOnMap& rhp) {
    if (lhp.i != rhp.i)
      return lhp.i < rhp.i;
    return lhp.j < rhp.j;
  }
  bool IsValid() const {
    return *this != PointOnMap();
  }
  size_t i, j;
  char type;
};

struct PathFindingOnMapCase {
  PathFindingOnMapCase(const std::vector<std::string>& map,
                       size_t shortest_length) :
      map(map), shortest_length(shortest_length) {}
  const std::vector<std::string> map;
  const size_t shortest_length;
};

const PathFindingOnMapCase path_finding_on_map_cases[] ={
  {
    {
      "#####",
      "#S..#",
      "#...#",
      "#..F#",
      "#####"
    },
    4
  },
  {
    {
      "S...#",
      "...#.",
      "..#..",
      ".#...",
      "#...F"
    },
    0
  },
  {
    {
      "######",
      "#S...#",
      "#..#.#",
      "#.#..#",
      "#...F#",
      "######"
    },
    6
  },
  {
    {
      "###.###.##",
      "#S.......#",
      "########.#",
      ".......#.#",
      ".##.####.#",
      "..#.#.#..#",
      "..#.#.#..#",
      ".##...#..#",
      ".#F##....#",
      "....######"
    },
    36
  },
  {
    {
      "........#.S",
      ".#....###..",
      ".#....#...#",
      ".#....###..",
      "..#......#.",
      "...#####...",
      "#......####",
      ".#.........",
      ".F########.",
      "..........."
    },
    51
  },
  {
    {
      "........#.S",
      ".#....###..",
      ".#....#...#",
      ".#....#.#..",
      ".........#.",
      "...#####...",
      "#......####",
      ".#.........",
      ".F##.#####.",
      "..........."
    },
    23
  },
  {
    {
      "........#.F",
      ".#....###..",
      ".#....#...#",
      ".#....#.#..",
      ".........#.",
      "...#####...",
      "#......####",
      ".#.........",
      ".S##.#####.",
      "..........."
    },
    23
  },
  {
    {
      "......#.#....#....#......#........#.....#....#.#.F",
      "S#........#.....#....#.......#..#....#....#......."
    },
    64
  }
};

class PathFindingOnMap :
    public testing::TestWithParam<PathFindingOnMapCase> {
 public:
  void SetUp() override {
    size_t start_cnt = 0;
    size_t finish_cnt = 0;
    const std::vector<std::string>& map = GetParam().map;
    for (size_t i = 0; i < map.size(); i++) {
      if (i != 0)
        ASSERT_EQ(map[i].size(), map[i-1].size());
      for (size_t j = 0; j < map[i].size(); j++) {
        ASSERT_TRUE(IsValidType(map[i][j]));
        if (map[i][j] == 'S')
          start_cnt++;
        if (map[i][j] == 'F')
          finish_cnt++;
        if (map[i][j] == '#')
          continue;
        const PointOnMap u = {i, j, map[i][j]};
        graph_.AddNode(u);
        const std::vector<PointOnMap> potential_neighbors = GetNeighbors(u);
        for (const PointOnMap& potential_neighbor : potential_neighbors)
          if (potential_neighbor.IsValid())
            graph_.AddEdge({u, potential_neighbor});
      }
    }
    ASSERT_EQ(start_cnt, 1);
    ASSERT_EQ(finish_cnt, 1);
  }

  void TearDown() override {
    ASSERT_EQ(GetShortestLength(), answer_.size());
    if (GetShortestLength() == 0)
      return;

    const std::vector<std::string>& map = GetMap();
    const PointOnMap start = GetStart();
    const PointOnMap finish = GetFinish();

    size_t step = 0;
    PointOnMap current = start;
    for (; current != finish && step < answer_.size(); step++) {
      size_t& i = current.i;
      size_t& j = current.j;
      ASSERT_TRUE(i >= 0 && i < map.size() && j >= 0 && i < map[i].size());
      ASSERT_NE(map[i][j], '#');
      switch (answer_[step]) {
        case 'L':
          j--;
          break;
        case 'U':
          i--;
          break;
        case 'R':
          j++;
          break;
        case 'D':
          i++;
          break;
        default:
          ASSERT_TRUE(false);
          break;
      }
    }
    ASSERT_EQ(step, answer_.size());
    ASSERT_EQ(current, finish);
  }

 protected:
  const std::vector<std::string>& GetMap() const {
    return GetParam().map;
  }

  std::size_t GetShortestLength() const {
    return GetParam().shortest_length;
  }

  PointOnMap GetStart() const {
    return FindPointOnMap('S');
  }

  PointOnMap GetFinish() const {
    return FindPointOnMap('F');
  }

  PointOnMap GetLeft(const PointOnMap& u) const {
    const std::vector<std::string>& map = GetMap();
    if (u.j > 0)
      if (map[u.i][u.j - 1] != '#')
        return {u.i, u.j - 1, map[u.i][u.j - 1]};
    return {};
  }

  PointOnMap GetUp(const PointOnMap& u) const {
    const std::vector<std::string>& map = GetMap();
    if (u.i > 0)
      if (map[u.i - 1][u.j] != '#')
        return {u.i - 1, u.j, map[u.i - 1][u.j]};
    return {};
  }

  PointOnMap GetRight(const PointOnMap& u) const {
    const std::vector<std::string>& map = GetMap();
    if (u.j < map[u.i].size() - 1)
      if (map[u.i][u.j + 1] != '#')
        return {u.i, u.j + 1, map[u.i][u.j + 1]};
    return {};
  }

  PointOnMap GetDown(const PointOnMap& u) const {
    const std::vector<std::string>& map = GetMap();
    if (u.i < map.size() - 1)
      if (map[u.i + 1][u.j] != '#')
        return {u.i + 1, u.j, map[u.i + 1][u.j]};
    return {};
  }

  std::vector<PointOnMap> GetNeighbors(const PointOnMap& u) const {
    return { GetLeft(u), GetUp(u), GetRight(u), GetDown(u) };
  }

  static bool IsValidType(char type) {
    return type == 'S' || type == 'F' || type == '.' || type == '#';
  }

  std::string answer_;
  graph::Graph<PointOnMap> graph_;

 private:
  PointOnMap FindPointOnMap(char type) const {
    const std::vector<std::string>& map = GetMap();
    for (size_t i = 0; i < map.size(); i++)
      for (size_t j = 0; j < map[i].size(); j++)
        if (map[i][j] == type)
          return {i, j, map[i][j]};
    return {};
  }
};

INSTANTIATE_TEST_SUITE_P(PathFidning,
                         PathFindingOnMap,
                         testing::ValuesIn(path_finding_on_map_cases));

}  // graph_tests

#endif  // TEST_UTILS_PATH_FINDING_ON_MAP
