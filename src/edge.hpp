#ifndef EDGE_HPP
#define EDGE_HPP

#include <impl/graph_traits.hpp>

#include <functional>
#include <utility>

namespace graph {

template<typename Node, typename... Types>
class Edge : public Types... {
public:
  Edge(Node from, Node to, Types... args) :
      from(std::move(from)), to(std::move(to)), Types(std::move(args))... {}
  Edge(const Edge& other) : from(other.from), to(other.to), Types(other)... {}
  Edge(const Edge& other, Node from, Node to) :
      from(std::move(from)), to(std::move(to)), Types(other)... {}

  const Node from;
  const Node to;
};

template<typename Node, typename... Types>
Edge<Node, Types...> reversed(const Edge<Node, Types...>& edge) {
  return Edge<Node, Types...>(edge, edge.to, edge.from);
}

template<typename Node, typename... Types>
bool operator<(const Edge<Node, graph_impl::RBTreeBasedEdge, Types...>& lhe,
               const Edge<Node, graph_impl::RBTreeBasedEdge, Types...>& rhe) {
  if (lhe.from < rhe.from || rhe.from < lhe.from)
    return lhe.from < rhe.from;
  return lhe.to < rhe.to;
}

template<typename Node, typename... Types>
bool operator==(
      const Edge<Node, graph_impl::HashTableBasedEdge, Types...>& lhe,
      const Edge<Node, graph_impl::HashTableBasedEdge, Types...>& rhe) {
  return lhe.from == rhe.from && lhe.to == rhe.to;
}

}  // graph

namespace std {

template<typename Node, typename... Types>
struct hash<graph::Edge<Node, graph_impl::HashTableBasedEdge, Types...> > {
  std::size_t operator()(
      const graph::Edge<Node, graph_impl::HashTableBasedEdge, Types...>& edge
  ) const noexcept {
    const std::size_t h1 = std::hash<Node>{}(edge.from);
    const std::size_t h2 = std::hash<Node>{}(edge.to);
    return h1 ^ (h2 << 1);
  }
};

}  // std

#endif  // EDGE_H
