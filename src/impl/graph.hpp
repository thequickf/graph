#ifndef IMPL_GRAPH_HPP
#define IMPL_GRAPH_HPP

#include <edge.hpp>
#include <graph_traits.hpp>
#include <impl/graph_traits.hpp>
#include <impl/meta_utils.hpp>

#include <map>
#include <set>
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
  using Edge = graph::Edge<Node, ConstructibleEdgeTraits...>;

public:
  Graph() = default;
  Graph(std::initializer_list<Node> node_list,
        ConstructibleGraphTraits... graph_traits) :
      nodes(std::move(node_list)),
      ConstructibleGraphTraits(std::move(graph_traits))... {
    if (contains_type<Net<Node>, GraphTraits...>::value) {
      nodes.insert(reinterpret_cast<Net<Node>*>(this)->source);
      nodes.insert(reinterpret_cast<Net<Node>*>(this)->sink);
    }
  }

  void addEdge(const Edge& edge) {
    nodes.insert(edge.from);
    nodes.insert(edge.to);
    edges[edge.from].insert(edge);
    edges[edge.to].insert(edge);
    if (!contains_type<graph::Directed, GraphTraits...>::value) {
      edges[edge.from].insert(graph::reversed(edge));
      edges[edge.to].insert(graph::reversed(edge));
    }
  }

  std::vector<Edge> inEdges(const Node& node) {
    std::vector<Edge> res;
    for (const Edge& edge : edges[node])
      if (edge.to == node)
        res.push_back(edge);
    return res;
  }

  std::vector<Edge> outEdges(const Node& node) {
    std::vector<Edge > res;
    for (const Edge& edge : edges[node])
      if (edge.from == node)
        res.push_back(edge);
    return res;
  }

  std::vector<Node> neighbors(const Node& node) {
    std::vector<Node> res;
    for (const Edge& edge : outEdges(node))
      res.push_back(edge.to);
    return res;
  }

private:
  std::map<Node, std::set<Edge> > edges;
  std::set<Node> nodes;
};

}  // graph_impl

#endif  // IMPL_GRAPH_HPP
