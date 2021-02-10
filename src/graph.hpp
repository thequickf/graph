#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <edge.hpp>
#include <graph_traits.hpp>
#include <impl/graph.hpp>
#include <impl/meta_utils.hpp>

namespace graph {

template<typename Node, typename... Traits>
  requires (graph_meta::Trait<Traits> && ...)
using Graph = graph_impl::Graph<
    Node,
    graph_meta::build_graph_traits<Node, Traits...>,
    graph_meta::build_edge_traits<Node, Traits...>,
    graph_meta::constructible_graph_traits<Node, Traits...>,
    graph_meta::constructible_edge_traits<Node, Traits...>
>;

}  // graph

#endif  // GRAPH_HPP
