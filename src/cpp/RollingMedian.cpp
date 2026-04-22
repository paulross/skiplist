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

/**
 * Perform a rolling median on a vector of doubles.
 *
 * @param data Data vector.
 * @param win_length Window length.
 * @param result Result vector.
 * @return ROLLING_MEDIAN_SUCCESS on success, non-zero on failure.
 */
        RollingMedianResult rolling_median_double(
                const std::vector<double> data,
                size_t win_length,
                std::vector<double> &result,
                int pad_with_nan) {
            if (win_length == 0) {
                return ROLLING_MEDIAN_WIN_LENGTH;
            }

            result.clear();
            OrderedStructs::SkipList::HeadNode<double> sl;
            // Used for averaging two values when the window length is even.
            std::vector<double> buffer;
            for (size_t i = 0; i < data.size(); ++i) {
                sl.insert(data[i]);
                if (i >= win_length) {
                    if (win_length % 2 == 1) {
                        result.push_back(sl.at(win_length / 2));
                    } else {
                        /* Even length so average */
                        sl.at((win_length - 1) / 2, 2, buffer);
                        assert(buffer.size() == 2);
                        result.push_back(buffer[0] / 2 + buffer[1] / 2);
                    }
                    sl.remove(data[i - win_length]);
                } else if (pad_with_nan) {
                    result.push_back((double) NAN);
                }
            }
            return ROLLING_MEDIAN_SUCCESS;
        }

    } // namespace RollingMedian
} // namespace OrderedStructs
