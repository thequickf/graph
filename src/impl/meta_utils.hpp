#ifndef IMPL_UTILS_HPP
#define IMPL_UTILS_HPP

#include <type_traits>

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

}  // namespace

namespace graph {
class GraphTrait;
class EdgeTrait;
}  // graph

namespace graph_impl {

template <template <typename> class Condition, typename... Ts>
using filter = typename filter_impl<Condition, Ts...>::type;

template<class Derived>
using graph_trait_condition = std::is_base_of<graph::GraphTrait, Derived>;

template<class Derived>
using edge_trait_condition = std::is_base_of<graph::EdgeTrait, Derived>;

}  // graph_impl

#endif  // IMPL_UTILS_HPP
