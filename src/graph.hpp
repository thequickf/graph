#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <edge.hpp>
#include <impl/graph.hpp>
#include <impl/meta_utils.hpp>

namespace graph {

class GraphTrait {
protected:
    GraphTrait() = default;
};

class EdgeTrait {
protected:
    EdgeTrait() = default;
};

// TODO: source and sink Node* type.
// Should we inherit all nodes from base Node class
// or should we apply more template woodoo?
class Net : public GraphTrait {
public:
    Net(int source, int sink) : source(source), sink(sink) {}
    int source, sink;
};

class Weighted : public EdgeTrait {
public:
    Weighted(uint32_t weight) : weight(weight) {}
    uint32_t weight;
};

// TODO: Here I want to apply
// requires ((std::is_base_of_v<graph::GraphTrait, Ts> || std::is_base_of_v<graph::EdgeTrait, Ts>) && ...)
// but I wasn't able to make it work for now
template<typename Node, typename... Traits>
using Graph = graph_impl::Graph<
    Node,
    graph_impl::filter<graph_impl::graph_trait_condition, Traits...>,
    graph_impl::filter<graph_impl::edge_trait_condition, Traits...>
>;

}  // graph

#endif  // GRAPH_HPP
