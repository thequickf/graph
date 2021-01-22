#ifndef IMPL_DIRECTED_GRAPH_HPP
#define IMPL_DIRECTED_GRAPH_HPP

#include <edge.hpp>
#include <graph.hpp>

#include <map>
#include <set>

namespace graph {

template<typename Node, typename... Types>
bool operator<(const Edge<Node, Types...>& lhe, const Edge<Node, Types...>& rhe) {
    if (lhe.from != rhe.from)
        return lhe.from < rhe.from;
    return lhe.to < rhe.to;
}

template<typename Node, typename... Types>
class DirectedGraph : Graph<Node, Types...> {
public:
    DirectedGraph() = default;
    DirectedGraph(std::initializer_list<Node> nodes) : nodes(nodes) {}

    void addEdge(Edge<Node, Types...> edge) override {
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
