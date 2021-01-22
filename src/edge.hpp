#ifndef EDGE_HPP
#define EDGE_HPP

namespace graph {

template<typename Node, typename... Types>
class Edge : public Types... {
public:
    Edge(const Node& from, const Node& to, Types... args) : from(from), to(to), Types(args)... {}

    const Node from;
    const Node to;
};

}  // graph

#endif  // EDGE_H
