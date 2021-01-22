#ifndef IMPL_DIRECTED_GRAPH_HPP
#define IMPL_DIRECTED_GRAPH_HPP

#include <edge.hpp>
#include <graph.hpp>

#include <map>
#include <set>

namespace graph {

template<typename Node, typename... Types>
class DirectedGraph : public Graph<Node, Types...> {
public:
    DirectedGraph() = default;
    DirectedGraph(std::initializer_list<Node> nodes) : nodes(nodes) {}

    void addEdge(const Edge<Node, Types...>& edge) override {
        nodes.insert(edge.from);
        nodes.insert(edge.to);
        edges[edge.from].insert(edge);
    }

private:
    std::map<Node, std::set<Edge <Node, Types...> > > edges;
    std::set<Node> nodes;
};

}  // graph

#endif  // IMPL_DIRECTED_GRAPH_HPP
