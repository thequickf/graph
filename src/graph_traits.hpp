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

class Net : public GraphTrait {
protected:
    Net() = default;
};

// TODO: filter away such classes from Graph constructor args
class Directed : public GraphTrait {
public:
    Directed() = default;
};

class Weighted : public EdgeTrait {
public:
    Weighted(unsigned weight) : weight(weight) {}
    const unsigned weight;
};

}  // graph

#endif  // GRAPH_TRAITS_HPP
