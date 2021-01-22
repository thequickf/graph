#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <edge.hpp>

namespace graph {

template<typename Node, typename... Types>
class Graph {
public:
    virtual void addEdge(Edge <Node, Types...> edge) = 0;
};

}  // graph

#endif  // GRAPH_HPP
