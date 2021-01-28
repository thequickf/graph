#ifndef IMPL_GRAPH_HPP
#define IMPL_GRAPH_HPP

#include <edge.hpp>
#include <graph_traits.hpp>

#include <map>
#include <set>
#include <utility>
#include <vector>

namespace graph_impl {

template<typename Node>
class Net : public graph::GraphTrait {
public:
    Net(Node source, Node sink) :
        source(std::move(source)), sink(std::move(sink)) {}
    const Node source, sink;
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
    using Edge = graph::Edge<Node, EdgeTraits...>;

public:
    Graph() = default;
    Graph(std::initializer_list<Node> node_list,
          GraphTraits... graph_traits) :
          nodes(std::move(node_list)), GraphTraits(std::move(graph_traits))... {
        if (std::is_base_of_v<Net<Node>, typeof(*this)>) {
            nodes.insert(static_cast<Net<Node>*>(this)->source);
            nodes.insert(static_cast<Net<Node>*>(this)->sink);
        }
    }

    void addEdge(const Edge& edge) {
        nodes.insert(edge.from);
        nodes.insert(edge.to);
        edges[edge.from].insert(edge);
        edges[edge.to].insert(edge);
        if (!std::is_base_of_v<graph::Directed, typeof(*this)>) {
            edges[edge.from].insert(graph::reversed(edge));
            edges[edge.to].insert(graph::reversed(edge));
        }
    }

    std::vector<Edge> inEdges(Node node) {
        std::vector<Edge> res;
        for (const Edge& edge : edges[node])
            if (edge.to == node)
                res.push_back(edge);
        return res;
    }

    std::vector<Edge> outEdges(Node node) {
        std::vector<Edge > res;
        for (const Edge& edge : edges[node])
            if (edge.from == node)
                res.push_back(edge);
        return res;
    }

    std::vector<Node> neighbors(Node node) {
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
