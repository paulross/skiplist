//
// Created by Paul Ross on 08/04/2025.
//
#include "RollingMedian.h"

namespace OrderedStructs {
    /**
     * @brief Namespace for the C++ Rolling Median.
     */
    namespace RollingMedian {

/**
 * Returns the size of the destination array for a rolling median on an array
 * of count values with a window of win_length and a destination stride.
 *
 * @param count Number of input values.
 * @param win_length Window length.
 * @return Number of destination values.
 */
        size_t dest_count(size_t count, size_t win_length) {
            return 1 + count - win_length;
        }

/**
 * Returns the size of the destination array for a rolling median on an array
 * of count values with a window of win_length and a destination stride.
 *
 * @param count Number of input values.
 * @param win_length Window length.
 * @param dest_stride The destination stride given a 2D array.
 * @return Size of destination array.
 */
        size_t dest_size(size_t count,
                         size_t win_length,
                         size_t dest_stride) {
            return dest_count(count, win_length) * dest_stride;
        }

    } // namespace RollingMedian
} // namespace OrderedStructs
