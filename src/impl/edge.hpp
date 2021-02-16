#ifndef IMPL_EDGE_HPP
#define IMPL_EDGE_HPP

#include <impl/base_utils.hpp>
#include <impl/meta_utils.hpp>

#include <functional>
#include <tuple>
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
  using ThisType = Edge<Node,
                        graph_meta::list<Traits...>,
                        graph_meta::list<ConstructibleTraits...> >;
 public:
  Edge(Node from, Node to, ConstructibleTraits... args) :
      from(std::move(from)),
      to(std::move(to)),
      ConstructibleTraits(std::move(args))... {}
  Edge(const ThisType& other) : from(other.from), to(other.to), Traits(other)... {}
  Edge(const ThisType& other, Node from, Node to) :
      from(std::move(from)), to(std::move(to)), Traits(other)... {}

  ThisType ReversedCopy() const {
    return ThisType(*this, to, from);
  }

  const Node from;
  const Node to;
};

template<typename Node, typename... Types>
bool operator<(const Edge<Node, Types...>& lhe,
               const Edge<Node, Types...>& rhe) {
  return std::tie(lhe.from, lhe.to) < std::tie(rhe.from, rhe.to);
}

template<typename Node, typename... Types>
bool operator==(const Edge<Node, Types...>& lhe,
                const Edge<Node, Types...>& rhe) {
  return std::tie(lhe.from, lhe.to) == std::tie(rhe.from, rhe.to);
}

}  // graph_impl

namespace std {

template<typename Node, typename... Types>
struct hash<graph_impl::Edge<Node, Types...> > {
  std::size_t operator()(
      const graph_impl::Edge<Node, Types...>& edge) const noexcept {
    return graph_utils::CombineHash(
        std::hash<Node>{}(edge.from), std::hash<Node>{}(edge.to));
  }
};

}  // std

#endif  // IMPL_EDGE_HPP
