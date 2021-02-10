#ifndef GRAPH_TRAITS_HPP
#define GRAPH_TRAITS_HPP

namespace graph {

class GraphTrait {
 protected:
  GraphTrait() = default;
};

class EdgeTrait {
 protected:
  EdgeTrait() = default;
};

class NoConstructorTrait {
 protected:
  NoConstructorTrait() = default;
};

class Net : public GraphTrait {
 protected:
  Net() = default;
};

class Directed : public GraphTrait, NoConstructorTrait {
 protected:
  Directed() = default;
};

class Multigraph : public GraphTrait, NoConstructorTrait {
 protected:
  Multigraph() = default;
};

class HashTableBased : public GraphTrait, NoConstructorTrait {
 protected:
  HashTableBased() = default;
};

class Weighted : public EdgeTrait {
 public:
  Weighted(unsigned weight) : weight(weight) {}
  const unsigned weight;
};

}  // graph

#endif  // GRAPH_TRAITS_HPP
