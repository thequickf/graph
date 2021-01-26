#ifndef IMPL_META_UTILS_HPP
#define IMPL_META_UTILS_HPP

#include <graph_traits.hpp>

#include <type_traits>

namespace graph_impl {

template<typename Node>
class Net;

}  // graph_impl

namespace {

template <typename...>
struct list;

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

template <typename, typename...>
struct replace_abstract_traits_impl;

template <typename Node>
struct replace_abstract_traits_impl<Node> {
    using type = list<>;
};

template <typename Node, typename Head, typename... Tail>
struct replace_abstract_traits_impl<Node, Head, Tail...> {
    using type = typename list_concat_impl<
                    std::conditional_t<
                        std::is_same_v<Head, graph::Net>,
                        list<graph_impl::Net<Node> >,
                        list<Head>
                    >,
                    typename replace_abstract_traits_impl<Node, Tail...>::type
                 >::type;
};

template<typename Node, typename TraitList>
struct replace_abstract_traits;

template<
    typename Node,
    template<typename...> typename TraitList, typename... Traits>
struct replace_abstract_traits<Node, TraitList<Traits...> > {
    using type = replace_abstract_traits_impl<Node, Traits...>::type;
};

template<class Derived>
using graph_trait_condition = std::is_base_of<graph::GraphTrait, Derived>;

template<class Derived>
using edge_trait_condition = std::is_base_of<graph::EdgeTrait, Derived>;

}  // namespace

namespace graph_impl {

template<typename Node, typename... Traits>
using build_graph_traits =
    replace_abstract_traits<
        Node, filter<graph_trait_condition, Traits...> >::type;

template<typename Node, typename... Traits>
using build_edge_traits =
    replace_abstract_traits<
        Node, filter<edge_trait_condition, Traits...> >::type;

template<typename T>
concept Trait =
    std::is_base_of_v<graph::GraphTrait, T> or
    std::is_base_of_v<graph::EdgeTrait, T>;

}  // graph_impl

#endif  // IMPL_UTILS_HPP
