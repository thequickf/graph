#ifndef IMPL_GRAPH_HPP
#define IMPL_GRAPH_HPP

#include <edge.hpp>
#include <graph_traits.hpp>

#include <map>
#include <set>
#include <utility>

namespace graph_impl {

template<typename Node>
class Net : public graph::GraphTrait {
public:
    Net(Node source, Node sink) :
        source(std::move(source)), sink(std::move(sink)) {}
    Node source, sink;
};

template<typename Node, typename GraphTraitList, typename EdgeTraitList>
class Graph;

template<
    typename Node,
    template<typename...> typename GraphTraitList, typename... GraphTraits,
    template<typename...> typename EdgeTraitList, typename... EdgeTraits>
class Graph<
    Node, GraphTraitList<GraphTraits...>, EdgeTraitList<EdgeTraits...> > :
    public GraphTraits... {
public:
    Graph() = default;
    Graph(std::initializer_list<Node> node_list,
          GraphTraits... graph_traits) :
          nodes(std::move(node_list)), GraphTraits(std::move(graph_traits))... {
        if (std::is_base_of<Net<Node>, typeof(*this)>::value) {
            nodes.insert(static_cast<Net<Node>*>(this)->source);
            nodes.insert(static_cast<Net<Node>*>(this)->sink);
        }
    }

    virtual void addEdge(const graph::Edge<Node, EdgeTraits...>& edge) {
        nodes.insert(edge.from);
        nodes.insert(edge.to);
        edges[edge.from].insert(edge);
    }

private:
    std::map<Node, std::set<graph::Edge <Node, EdgeTraits...> > > edges;
    std::set<Node> nodes;
};

}  // graph_impl

#endif  // IMPL_GRAPH_HPP
