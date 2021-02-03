#ifndef IMPL_GRAPH_HPP
#define IMPL_GRAPH_HPP

#include <edge.hpp>
#include <graph_traits.hpp>
#include <impl/graph_traits.hpp>
#include <impl/meta_utils.hpp>

#include <map>
#include <set>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace graph_impl {

template<typename Node, typename GraphTraitList, typename EdgeTraitList,
    typename ConstructibleGraphTraitList, typename ConstructibleEdgeTraitList>
class Graph;

template<
    typename Node,
    template<typename...> typename GraphTraitList, typename... GraphTraits,
    template<typename...> typename EdgeTraitList, typename... EdgeTraits,
    template<typename...> typename ConstructibleGraphTraitList,
        typename... ConstructibleGraphTraits,
    template<typename...> typename ConstructibleEdgeTraitList,
        typename... ConstructibleEdgeTraits>
class Graph<
      Node, GraphTraitList<GraphTraits...>, EdgeTraitList<EdgeTraits...>,
      ConstructibleGraphTraitList<ConstructibleGraphTraits...>,
      ConstructibleEdgeTraitList<ConstructibleEdgeTraits...> > :
    public GraphTraits... {
  using Edge = graph::Edge<Node, EdgeTraits...>;
  using ContainerEdge = std::conditional_t<
      contains_type_v<graph::HashTableBased, GraphTraits...>,
      graph::Edge<Node, graph_impl::HashTableBasedEdge, EdgeTraits...>,
      graph::Edge<Node, graph_impl::RBTreeBasedEdge, EdgeTraits...> >;
  using EdgeSet = std::conditional_t<
      contains_type_v<graph::HashTableBased, GraphTraits...>,
      std::conditional_t<
          contains_type_v<graph::Multigraph, GraphTraits...>,
          std::unordered_multiset<ContainerEdge>,
          std::unordered_set<ContainerEdge>
      >,
      std::conditional_t<
          contains_type_v<graph::Multigraph, GraphTraits...>,
          std::multiset<ContainerEdge>,
          std::set<ContainerEdge>
      >
  >;
  using GraphMap = std::conditional_t<
      contains_type_v<graph::HashTableBased, GraphTraits...>,
      std::unordered_map<Node, EdgeSet>,
      std::map<Node, EdgeSet>
  >;
  using NodeSet = std::conditional_t<
      contains_type_v<graph::HashTableBased, GraphTraits...>,
      std::unordered_set<Node>,
      std::set<Node>
  >;

public:
  Graph() = default;
  Graph(std::initializer_list<Node> node_list,
        ConstructibleGraphTraits... graph_traits) :
      nodes(std::move(node_list)),
      ConstructibleGraphTraits(std::move(graph_traits))... {
    if (contains_type_v<Net<Node>, GraphTraits...>) {
      nodes.insert(reinterpret_cast<Net<Node>*>(this)->source);
      nodes.insert(reinterpret_cast<Net<Node>*>(this)->sink);
    }
  }

  void addEdge(const Edge& edge) {
    const ContainerEdge container_edge =
        reinterpret_cast<const ContainerEdge&>(edge);
    nodes.insert(container_edge.from);
    nodes.insert(container_edge.to);
    edges[container_edge.from].insert(container_edge);
    edges[container_edge.to].insert(container_edge);
    if (!contains_type_v<graph::Directed, GraphTraits...>) {
      edges[container_edge.from].insert(graph::reversed(container_edge));
      edges[container_edge.to].insert(graph::reversed(container_edge));
    }
  }

  std::vector<Edge> inEdges(const Node& node) {
    std::vector<Edge> res;
    for (const ContainerEdge& edge : edges[node])
      if (edge.to == node)
        res.push_back(reinterpret_cast<const Edge&>(edge));
    return res;
  }

  std::vector<Edge> outEdges(const Node& node) {
    std::vector<Edge> res;
    for (const ContainerEdge& edge : edges[node])
      if (edge.from == node)
        res.push_back(reinterpret_cast<const Edge&>(edge));
    return res;
  }

  std::vector<Node> neighbors(const Node& node) {
    NodeSet res_set;
    for (const Edge& edge : outEdges(node))
      res_set.insert(edge.to);
    std::vector<Node> res;
    res.reserve(res_set.size());
    for (const Node& node : res_set)
      res.push_back(node);
    return res;
  }

private:
  GraphMap edges;
  NodeSet nodes;
};

}  // graph_impl

#endif  // IMPL_GRAPH_HPP
