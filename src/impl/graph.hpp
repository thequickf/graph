#ifndef IMPL_GRAPH_HPP
#define IMPL_GRAPH_HPP

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

template<typename Node,
         typename GraphTraitList,
         typename EdgeTraitList,
         typename ConstructibleGraphTraitList,
         typename ConstructibleEdgeTraitList>
class Graph;

template<typename Node,
         typename... GraphTraits,
         typename... EdgeTraits,
         typename... ConstructibleGraphTraits,
         typename... ConstructibleEdgeTraits>
  requires (graph_meta::BaseConsistent<Node, GraphTraits...>)
class Graph<Node,
            graph_meta::list<GraphTraits...>,
            graph_meta::list<EdgeTraits...>,
            graph_meta::list<ConstructibleGraphTraits...>,
            graph_meta::list<ConstructibleEdgeTraits...> > :
    public GraphTraits... {
 public:
  using EdgeType = Edge<Node,
                        graph_meta::list<EdgeTraits...>,
                        graph_meta::list<ConstructibleEdgeTraits...> >;
 private:
  using EdgeSet = std::conditional_t<
      graph_meta::contains_type_v<graph::HashTableBased, GraphTraits...>,
      std::conditional_t<
          graph_meta::contains_type_v<graph::Multigraph, GraphTraits...>,
          std::unordered_multiset<EdgeType>,
          std::unordered_set<EdgeType>
      >,
      std::conditional_t<
          graph_meta::contains_type_v<graph::Multigraph, GraphTraits...>,
          std::multiset<EdgeType>,
          std::set<EdgeType>
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
      AddNode(reinterpret_cast<Net<Node>*>(this)->source);
      AddNode(reinterpret_cast<Net<Node>*>(this)->sink);
    }
  }

  void AddNode(const Node& node) {
    nodes_.insert(node);
  }

  void AddEdge(const EdgeType& edge) {
    AddNode(edge.from);
    AddNode(edge.to);
    edges_[edge.from].insert(edge);
    edges_[edge.to].insert(edge);
    if (!graph_meta::contains_type_v<graph::Directed, GraphTraits...>) {
      edges_[edge.from].insert(edge.ReversedCopy());
      edges_[edge.to].insert(edge.ReversedCopy());
    }
  }

  void AddEdge(Node from, Node to, ConstructibleEdgeTraits... edge_traits) {
    return AddEdge({std::move(from), std::move(to), std::move(edge_traits)...});
  }

  std::vector<EdgeType> InEdges(const Node& node) {
    AddNode(node);
    std::vector<EdgeType> res;
    for (const EdgeType& edge : edges_[node])
      if (edge.to == node)
        res.push_back(edge);
    return res;
  }

  std::vector<EdgeType> OutEdges(const Node& node) {
    AddNode(node);
    std::vector<EdgeType> res;
    for (const EdgeType& edge : edges_[node])
      if (edge.from == node)
        res.push_back(edge);
    return res;
  }

  std::vector<Node> Neighbors(const Node& node) {
    AddNode(node);
    NodeSet res_set;
    for (const EdgeType& edge : OutEdges(node))
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
