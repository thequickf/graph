#ifndef IMPL_GRAPH_TRAITS_HPP
#define IMPL_GRAPH_TRAITS_HPP

#include <graph_traits.hpp>

#include <utility>

namespace graph_impl {

template<typename Node>
class Net : public graph::GraphTrait {
public:
  Net(Node source, Node sink) :
      source(std::move(source)), sink(std::move(sink)) {}
  const Node source, sink;
};

class RBTreeBasedEdge : public graph::EdgeTrait, graph::NoConstructorTrait {
protected:
  RBTreeBasedEdge() = default;
};

class HashTableBasedEdge : public graph::EdgeTrait, graph::NoConstructorTrait {
protected:
  HashTableBasedEdge() = default;
};

}  // graph_impl

#endif  // IMPL_GRAPH_TRAITS_HPP
