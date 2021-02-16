#ifndef IMPL_BASE_UTILS_HPP
#define IMPL_BASE_UTILS_HPP

#include <cstddef>
#include <climits>

namespace graph_utils {

std::size_t CombineHash(std::size_t left, std::size_t right) {
    return left ^ (right << 1 | (right >> (CHAR_BIT * sizeof(right) - 1)));
}

}  // graph_utils

#endif  // IMPL_BASE_UTILS_HPP
