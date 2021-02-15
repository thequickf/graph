#ifndef IMPL_EDGE_HPP
#define IMPL_EDGE_HPP

#include <impl/meta_utils.hpp>

#include <climits>
#include <functional>
#include <utility>

namespace graph_impl {

template<typename Node,
         typename EdgeTraitList,
         typename ConstructibleEdgeTraitList>
class Edge;

template<typename Node,
         typename... Traits,
         typename... ConstructibleTraits>
class Edge<Node,
           graph_meta::list<Traits...>,
           graph_meta::list<ConstructibleTraits...> > : public Traits... {
 public:
  Edge(Node from, Node to, ConstructibleTraits... args) :
      from(std::move(from)),
      to(std::move(to)),
      ConstructibleTraits(std::move(args))... {}
  Edge(const Edge& other) : from(other.from), to(other.to), Traits(other)... {}
  Edge(const Edge& other, Node from, Node to) :
      from(std::move(from)), to(std::move(to)), Traits(other)... {}

  Edge<Node,
       graph_meta::list<Traits...>,
       graph_meta::list<ConstructibleTraits...> > ReversedCopy() const {
    return Edge<Node,
                graph_meta::list<Traits...>,
                graph_meta::list<ConstructibleTraits...> >(*this, to, from);
  }

  const Node from;
  const Node to;
};

template<typename Node, typename... Types>
bool operator<(const Edge<Node, Types...>& lhe,
               const Edge<Node, Types...>& rhe) {
  if (lhe.from < rhe.from || rhe.from < lhe.from)
    return lhe.from < rhe.from;
  return lhe.to < rhe.to;
}

template<typename Node, typename... Types>
bool operator==(const Edge<Node, Types...>& lhe,
                const Edge<Node, Types...>& rhe) {
  return lhe.from == rhe.from && lhe.to == rhe.to;
}

}  // graph_impl

namespace std {

template<typename Node, typename... Types>
struct hash<graph_impl::Edge<Node, Types...> > {
  std::size_t operator()(
      const graph_impl::Edge<Node, Types...>& edge) const noexcept {
    const std::size_t h1 = std::hash<Node>{}(edge.from);
    const std::size_t h2 = std::hash<Node>{}(edge.to);
    return h1 ^ (h2 << 1 | (h2 >> (CHAR_BIT * sizeof(h2) - 1)));
  }
};

}  // std

#endif  // IMPL_EDGE_HPP
