#ifndef IMPL_META_UTILS_HPP
#define IMPL_META_UTILS_HPP

#include <graph_traits.hpp>
#include <impl/graph_traits.hpp>

#include <concepts>
#include <type_traits>

namespace graph_meta {

template <typename...>
struct list;

template<typename Node>
using abstract_trait_replacements = list<
    list<graph::Net, graph_impl::Net<Node> >
>;

template <typename, typename> struct list_concat_impl;

template <typename... LHTs, typename... RHTs>
struct list_concat_impl<list<LHTs...>, list<RHTs...>> {
  using type = list<LHTs..., RHTs...>;
};

template <template <typename> class, typename...>
struct filter_impl;

template <template <typename> class Condition>
struct filter_impl<Condition> {
  using type = list<>;
};

template <template <typename> class Condition, typename Head, typename... Tail>
struct filter_impl<Condition, Head, Tail...> {
  using type = typename list_concat_impl<
          std::conditional_t<
              Condition<Head>::value,
              list<Head>,
              list<>
          >,
          typename filter_impl<Condition, Tail...>::type
      >::type;
};

template <template <typename> class Condition, typename... Ts>
using filter = typename filter_impl<Condition, Ts...>::type;

template<template <typename> class, typename>
struct filter_list_impl;

template<template <typename> class Condition,
    template <typename...> typename List, typename... Ts>
struct filter_list_impl<Condition, List<Ts...> > {
  using type = filter<Condition, Ts...>;
};

template<template <typename> class Condition, typename List>
using filter_list = filter_list_impl<Condition, List>::type;

template <typename, typename, typename...>
struct replace_impl;

template <typename SourceT, typename TargetT>
struct replace_impl<SourceT, TargetT> {
  using type = list<>;
};

template <typename SourceT, typename TargetT, typename Head, typename... Tail>
struct replace_impl<SourceT, TargetT, Head, Tail...> {
  using type = typename list_concat_impl<
          std::conditional_t<
              std::is_same_v<Head, SourceT>,
              list<TargetT>,
              list<Head>
          >,
          typename replace_impl<SourceT, TargetT, Tail...>::type
      >::type;
};

template<typename, typename, typename>
struct replace;

template<
    typename SourceT, typename TargetT,
    template<typename...> typename List, typename... Ts>
struct replace<SourceT, TargetT, List<Ts...> > {
  using type = replace_impl<SourceT, TargetT, Ts...>::type;
};

template<typename SourceT, typename TargetT, typename List>
using replace_t = replace<SourceT, TargetT, List>::type;

template <typename, typename...>
struct replace_all_impl;

template <typename TraitList>
struct replace_all_impl<TraitList> {
  using type = TraitList;
};

template <
    typename TraitList, 
    template<typename...> typename List, typename SourceT, typename TargetT,
    typename... Tail>
struct replace_all_impl<TraitList, List<SourceT, TargetT>, Tail...> {
  using type =
      replace_all_impl<replace_t<SourceT, TargetT, TraitList>, Tail...>::type;
};

template<typename, typename>
struct replace_all;

template<
    typename TraitList,
    template<typename...> typename ReplacementList, typename... Replacements>
struct replace_all<TraitList, ReplacementList<Replacements...> > {
  using type = replace_all_impl<TraitList, Replacements...>::type;
};

template<typename, typename, typename...>
struct contains_type_impl;

template<typename Result,  typename T>
struct contains_type_impl<Result, T> {
  using value = Result;
};

template<typename Result,  typename T, typename Head, typename... Tail>
struct contains_type_impl<Result,  T, Head, Tail...> {
  using value = contains_type_impl<
      std::disjunction<Result, std::is_same<T, Head> >, T, Tail...>::value;
};

template<typename Node, typename TraitList>
using replace_abstract_traits =
    replace_all<TraitList, abstract_trait_replacements<Node> >::type;

template<class Derived>
using graph_trait_condition = std::is_base_of<graph::GraphTrait, Derived>;

template<class Derived>
using edge_trait_condition = std::is_base_of<graph::EdgeTrait, Derived>;

template<class Derived>
using constructible_trait_condition =
    std::conditional_t<
        std::is_base_of_v<graph::NoConstructorTrait, Derived>,
        std::false_type,
        std::true_type>;

template<typename T, typename... Ts>
using contains_type = contains_type_impl<std::false_type, T, Ts...>::value;

template <typename T, typename... Ts>
inline constexpr bool contains_type_v = contains_type<T, Ts...>::value;

template<typename Node, typename... Traits>
using build_graph_traits =
    replace_abstract_traits<
        Node, filter<graph_trait_condition, Traits...> >;

template<typename Node, typename... Traits>
using build_edge_traits =
    replace_abstract_traits<
        Node, filter<edge_trait_condition, Traits...> >;

template<typename Node, typename... Traits>
using constructible_graph_traits =
    filter_list<constructible_trait_condition,
        build_graph_traits<Node, Traits...> >;

template<typename Node, typename... Traits>
using constructible_edge_traits =
    filter_list<constructible_trait_condition,
        build_edge_traits<Node, Traits...> >;

template<typename T>
concept Trait =
    std::is_base_of_v<graph::GraphTrait, T> ||
    std::is_base_of_v<graph::EdgeTrait, T>;

template<typename T>
concept Hashable = requires(T a, T b) {
  { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
  { a == b } -> std::convertible_to<bool>;
};

template<typename T>
concept Comparable =
    requires(const std::remove_reference_t<T>& a,
             const std::remove_reference_t<T>& b) {
  { a < b } -> std::convertible_to<bool>;
};

template<typename Node, typename... GraphTraits>
concept HashBaseConsistent =
    Hashable<Node> && contains_type_v<graph::HashTableBased, GraphTraits...>;

template<typename Node, typename... GraphTraits>
concept RBTreeBaseConsistent =
    Comparable<Node> && !contains_type_v<graph::HashTableBased, GraphTraits...>;

template<typename Node, typename... GraphTraits>
concept BaseConsistent =
    HashBaseConsistent<Node, GraphTraits...> ||
    RBTreeBaseConsistent<Node, GraphTraits...>;

}  // graph_meta

#endif  // IMPL_UTILS_HPP
