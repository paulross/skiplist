//
// Created by Paul Ross on 08/04/2025.
//
#include "RollingMedian.h"

namespace OrderedStructs {
    /**
     * @brief Namespace for the C++ Rolling Median.
     */
    namespace RollingMedian {

        size_t dest_count(size_t count, size_t win_length) {
            return 1 + count - win_length;
        }

        size_t dest_size(size_t count,
                         size_t win_length,
                         size_t dest_stride) {
            return dest_count(count, win_length) * dest_stride;
        }

    } // namespace RollingMedian
} // namespace OrderedStructs
