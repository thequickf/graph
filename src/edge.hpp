#ifndef EDGE_HPP
#define EDGE_HPP

namespace graph {

template<typename Node, typename... Types>
class Edge : public Types... {
public:
    Edge(const Node& from, const Node& to, const Types&... args) : from(from), to(to), Types(args)... {}

    const Node from;
    const Node to;
};

template<typename Node, typename... Types>
bool operator<(const Edge<Node, Types...>& lhe, const Edge<Node, Types...>& rhe) {
    if (lhe.from != rhe.from)
        return lhe.from < rhe.from;
    return lhe.to < rhe.to;
}

}  // graph

#endif  // EDGE_H
