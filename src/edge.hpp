#ifndef EDGE_HPP
#define EDGE_HPP

#include <utility>

namespace graph {

template<typename Node, typename... Types>
class Edge : public Types... {
public:
    Edge(Node from, Node to, Types... args) :
        from(std::move(from)), to(std::move(to)), Types(std::move(args))... {}

    const Node from;
    const Node to;
};

template<typename Node, typename... Types>
bool operator<(const Edge<Node, Types...>& lhe, 
               const Edge<Node, Types...>& rhe) {
    if (lhe.from != rhe.from)
        return lhe.from < rhe.from;
    return lhe.to < rhe.to;
}

}  // graph

#endif  // EDGE_H
