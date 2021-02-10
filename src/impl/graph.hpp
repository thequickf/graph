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
  requires(graph_meta::BaseConsistent<Node, GraphTraits...>)
class Graph<
      Node, GraphTraitList<GraphTraits...>, EdgeTraitList<EdgeTraits...>,
      ConstructibleGraphTraitList<ConstructibleGraphTraits...>,
      ConstructibleEdgeTraitList<ConstructibleEdgeTraits...> > :
    public GraphTraits... {
  using Edge = graph::Edge<Node, EdgeTraits...>;
  using EdgeSet = std::conditional_t<
      graph_meta::contains_type_v<graph::HashTableBased, GraphTraits...>,
      std::conditional_t<
          graph_meta::contains_type_v<graph::Multigraph, GraphTraits...>,
          std::unordered_multiset<Edge>,
          std::unordered_set<Edge>
      >,
      std::conditional_t<
          graph_meta::contains_type_v<graph::Multigraph, GraphTraits...>,
          std::multiset<Edge>,
          std::set<Edge>
      >
  >;
  using EdgeMap = std::conditional_t<
      graph_meta::contains_type_v<graph::HashTableBased, GraphTraits...>,
      std::unordered_map<Node, EdgeSet>,
      std::map<Node, EdgeSet>
  >;
  using NodeSet = std::conditional_t<
      graph_meta::contains_type_v<graph::HashTableBased, GraphTraits...>,
      std::unordered_set<Node>,
      std::set<Node>
  >;

 public:
  Graph() = default;
  Graph(std::initializer_list<Node> nodes,
        ConstructibleGraphTraits... graph_traits) :
      nodes_(std::move(nodes)),
      ConstructibleGraphTraits(std::move(graph_traits))... {
    if (graph_meta::contains_type_v<Net<Node>, GraphTraits...>) {
      nodes_.insert(reinterpret_cast<Net<Node>*>(this)->source);
      nodes_.insert(reinterpret_cast<Net<Node>*>(this)->sink);
    }
  }

  void AddNode(const Node& node) {
    nodes_.insert(node);
  }

  void AddEdge(const Edge& edge) {
    nodes_.insert(edge.from);
    nodes_.insert(edge.to);
    edges_[edge.from].insert(edge);
    edges_[edge.to].insert(edge);
    if (!graph_meta::contains_type_v<graph::Directed, GraphTraits...>) {
      edges_[edge.from].insert(graph::reversed(edge));
      edges_[edge.to].insert(graph::reversed(edge));
    }
  }

  std::vector<Edge> InEdges(const Node& node) {
    nodes_.insert(node);
    std::vector<Edge> res;
    for (const Edge& edge : edges_[node])
      if (edge.to == node)
        res.push_back(edge);
    return res;
  }

  std::vector<Edge> OutEdges(const Node& node) {
    nodes_.insert(node);
    std::vector<Edge> res;
    for (const Edge& edge : edges_[node])
      if (edge.from == node)
        res.push_back(edge);
    return res;
  }

  std::vector<Node> Neighbors(const Node& node) {
    nodes_.insert(node);
    NodeSet res_set;
    for (const Edge& edge : OutEdges(node))
      res_set.insert(edge.to);
    std::vector<Node> res;
    res.reserve(res_set.size());
    for (const Node& node : res_set)
      res.push_back(node);
    return res;
  }

 private:
  EdgeMap edges_;
  NodeSet nodes_;
};

}  // graph_impl

#endif  // IMPL_GRAPH_HPP
