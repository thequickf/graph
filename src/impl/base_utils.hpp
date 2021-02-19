#ifndef IMPL_BASE_UTILS_HPP
#define IMPL_BASE_UTILS_HPP

#include <graph_traits.hpp>
#include <impl/meta_utils.hpp>

#include <climits>
#include <cstddef>
#include <map>
#include <optional>
#include <unordered_map>

namespace graph_utils {

std::size_t CombineHash(std::size_t left, std::size_t right) {
  return left ^ (right << 1 | (right >> (CHAR_BIT * sizeof(right) - 1)));
}

template<typename Type, typename... Traits>
  requires (graph_meta::CorrectDSUTraits<Traits...> &&
            graph_meta::BaseConsistent<Type, Traits...>)
class DSU {
  template <typename Key, typename Value>
  using Map = std::conditional_t<
      graph_meta::contains_type_v<graph::HashTableBased, Traits...>,
      std::unordered_map<Key, Value>,
      std::map<Key, Value>
  >;

 public:
  DSU(const std::initializer_list<Type>& sets) {
    for (const Type& set : sets) {
      Add(set);
    }
  }

  bool Add(const Type& set) {
    return parent_.emplace(set, set).second && size_.emplace(set, 1).second;
  }

  std::optional<Type> Find(const Type& u) {
    auto parent_it = parent_.find(u);
    if (parent_it == parent_.end())
      return {};
    if (TypeEqual(u, parent_it->second)) {
      return u;
    }
    return parent_it->second = *Find(parent_it->second);
  }

  bool Unite(const Type& u, const Type& v) {
    if (!Find(u) || !Find(v))
      return false;
    Type u_rep = *Find(u);
    Type v_rep = *Find(v);
    if (!TypeEqual(u_rep, v_rep)) {
      auto u_rep_size_it = size_.find(u_rep);
      auto v_rep_size_it = size_.find(v_rep);
      if (u_rep_size_it->second < v_rep_size_it->second)
        std::swap(u_rep, v_rep);
      parent_.find(v_rep)->second = u_rep;
      u_rep_size_it->second += v_rep_size_it->second;
    }
    return true;
  }

  std::optional<size_t> Size(const Type& u) {
    if (auto u_rep = Find(u))
      return size_.find(u_rep)->second;
    return {};
  }

 private:
  bool TypeEqual(const Type& lht, const Type& rht) const {
    if constexpr (graph_meta::contains_type_v<graph::HashTableBased, Traits...>)
      return lht == rht;
    else
      return !(lht < rht) && !(rht < lht);
  }

  Map<Type, Type> parent_;
  Map<Type, std::size_t> size_;
};

}  // graph_utils

#endif  // IMPL_BASE_UTILS_HPP
