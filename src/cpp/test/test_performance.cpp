/**
 * @file
 *
 * Project: skiplist
 *
 * Performance tests on a Skip List.
 *
 * These should have a corresponding test_... which checks correctness.
 * The perf_... version should do no checking but aim for maximum performance
 * however assert(! sl.lacksIntegrity()); is encouraged as that will get
 * stripped out in release builds where performance is really tested.
 *
 * Created by Paul Ross on 13/07/2016.
 *
 * Copyright (c) 2016-2023 Paul Ross. All rights reserved.
 *
 * @code
 * MIT License
 *
 * Copyright (c) 2016-2023 Paul Ross
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * @endcode
 */

#include <iostream>
#include <iomanip>

#include "RollingMedian.h"
#include "TestFramework.h"
#include "test_performance.h"
#include "test_print.h"

#include "../SkipList.h"

/** @brief The number of times to repeat a test to get an accurate performance figure. */
static int GLOBAL_REPEAT_COUNT = 1000 * 1000;

/**
 * @brief Test the performance of inserting and removing a value into a Skip List.
 *
 * This writes the performance figures to \c stdout
 *
 * @return Zero.
 */
int perf_single_insert_remove() {
    // This test is meaningless if assert is defined because of the integrity checks.
    assert(0);
    int num = GLOBAL_REPEAT_COUNT;
    OrderedStructs::SkipList::HeadNode<double> sl;

    srand(1);
    time_t start = clock();
    for (int i = 0; i < num; ++i) {
        assert(!sl.lacksIntegrity());
        sl.insert(42.0);
        assert(!sl.lacksIntegrity());
        sl.remove(42.0);
        assert(!sl.lacksIntegrity());
    }
    double exec = 1e3 * (clock() - start) / (double) CLOCKS_PER_SEC;
    std::cout << std::setw(FUNCTION_WIDTH) << __FUNCTION__ << "(): ";
    std::cout << exec;
    std::cout << " (ms)";
    std::cout << " rate " << 1000. * num / exec << " /s";
    std::cout << std::endl;
    return 0;
}

/**
 * @brief Test the performance of \c insert() to create a large Skip List with no deletion.
 *
 * This writes the performance figures to \c stdout
 *
 * @return Zero.
 */
int perf_large_skiplist_ins_only() {
    // This test is meaningless if assert is defined because of the integrity checks.
    assert(0);
    size_t SIZE = GLOBAL_REPEAT_COUNT;
    int COUNT = 1;
    OrderedStructs::SkipList::HeadNode<double> sl;

    srand(1);
    time_t start = clock();
    for (int c = 0; c < COUNT; ++c) {
        OrderedStructs::SkipList::HeadNode<double> sl;
        assert(!sl.lacksIntegrity());
        for (size_t i = 0; i < SIZE; ++i) {
            sl.insert(i);
            assert(!sl.lacksIntegrity());
        }
        assert(sl.size() == SIZE);
    }
    double exec = 1e9 * (clock() - start) / (double) CLOCKS_PER_SEC;
    std::cout << std::setw(FUNCTION_WIDTH) << __FUNCTION__ << "():";
    std::cout << " skiplist length: " << SIZE;
    std::cout << " count: " << COUNT;
    std::cout << " time: ";
    std::cout << exec;
    std::cout << " (ns)";
    std::cout << " rate " << 1e9 * COUNT * SIZE / exec << " /s";
    std::cout << std::endl;
    return 0;
}

/**
 * @brief Test the performance of \c insert() into a large skiplist then \c remove() many times.
 *
 * This writes the performance figures to \c stdout
 *
 * @return Zero.
 */
int perf_large_skiplist_ins_rem() {
    // This test is meaningless if assert is defined because of the integrity checks.
    assert(0);
    size_t SIZE = 1024 * 1024;
    int COUNT = 1;
    OrderedStructs::SkipList::HeadNode<double> sl;

    srand(1);
    time_t start = clock();
    for (int c = 0; c < COUNT; ++c) {
        OrderedStructs::SkipList::HeadNode<double> sl;
        assert(!sl.lacksIntegrity());
        for (size_t i = 0; i < SIZE; ++i) {
            sl.insert(i);
            assert(!sl.lacksIntegrity());
        }
        assert(sl.size() == SIZE);
        for (size_t i = 0; i < SIZE; ++i) {
            sl.remove(i);
            assert(!sl.lacksIntegrity());
        }
        assert(sl.size() == 0);
    }
    double exec = 1e6 * (clock() - start) / (double) CLOCKS_PER_SEC;
    std::cout << std::setw(FUNCTION_WIDTH) << __FUNCTION__ << "():";
    std::cout << " skiplist length: " << SIZE;
    std::cout << " count: " << COUNT;
    std::cout << " time: ";
    std::cout << exec;
    std::cout << " (us)";
    std::cout << " rate " << 1e6 * COUNT * SIZE / exec << " /s";
    std::cout << std::endl;
    return 0;
}

/**
 * @brief Create a large skip list then time how long it takes to insert and remove a value at the mid point.
 *
 * This writes the performance figures to \c stdout
 *
 * @return Zero.
 */
int perf_single_ins_rem_middle() {
    // This test is meaningless if assert is defined because of the integrity checks.
    assert(0);
    size_t SKIPLIST_SIZE = 1024 * 1024;
    int REPEAT_COUNT = GLOBAL_REPEAT_COUNT;
    int result = 0;
    double val = SKIPLIST_SIZE / 2;
    OrderedStructs::SkipList::HeadNode<double> sl;

    srand(1);
    assert(!sl.lacksIntegrity());
    for (size_t i = 0; i < SKIPLIST_SIZE; ++i) {
        sl.insert(i);
        assert(!sl.lacksIntegrity());
    }
    time_t start = clock();
    assert(!sl.lacksIntegrity());
    for (int i = 0; i < REPEAT_COUNT; ++i) {
        sl.insert(val);
        assert(!sl.lacksIntegrity());
        sl.remove(val);
        assert(!sl.lacksIntegrity());
    }
    assert(!sl.lacksIntegrity());
    double exec = 1e6 * (clock() - start) / (double) CLOCKS_PER_SEC;
    std::cout << std::setw(FUNCTION_WIDTH) << __FUNCTION__ << "():";
    std::cout << " SkiplistSize: " << SKIPLIST_SIZE;
    std::cout << " repeat count: " << REPEAT_COUNT;
    std::cout << " time: ";
    std::cout << exec;
    std::cout << " (us)";
    std::cout << " rate " << 1e6 * REPEAT_COUNT / exec << " /s";
    std::cout << std::endl;
    return result;
}

/**
 * @brief Create a large skip list then time how long it takes to insert and remove a value at the mid point for various
 * length skip lists.
 *
 * This writes the performance figures to \c stdout
 *
 * @return Zero.
 */
int perf_single_ins_rem_middle_vary_length() {
    // This test is meaningless if assert is defined because of the integrity checks.
    assert(0);
    int REPEAT_COUNT = GLOBAL_REPEAT_COUNT;
    int result = 0;

    for (size_t siz = 1; siz < 1024 * 1024 + 1; siz *= 2) {
        OrderedStructs::SkipList::HeadNode<double> sl;

        srand(1);
        assert(!sl.lacksIntegrity());
        // Populate skip list
        for (size_t i = 0; i < siz + 1; ++i) {
            sl.insert(i);
            assert(!sl.lacksIntegrity());
        }
        double value = sl.size() / 2;
        time_t start = clock();
        for (int i = 0; i < REPEAT_COUNT; ++i) {
            sl.insert(value);
            assert(!sl.lacksIntegrity());
            sl.remove(value);
            assert(!sl.lacksIntegrity());
        }
        assert(!sl.lacksIntegrity());
        double exec = 1e6 * (clock() - start) / (double) CLOCKS_PER_SEC;
        std::cout << std::setw(FUNCTION_WIDTH) << __FUNCTION__ << "():";
        std::cout << " SkiplistSize: " << std::setw(10) << sl.size();
        std::cout << " repeat count: " << REPEAT_COUNT;
        std::cout << " time: ";
        std::cout << std::setw(10) << exec / 1e3;
        std::cout << " (ms)";
        std::cout << " rate ";
        std::cout << std::setw(12) << 1e6 * REPEAT_COUNT / exec << " /s";
        std::cout << std::endl;
    }
    return result;
}

/**
 * @brief Performance of calling \c at() on the middle value of a 1M long skip list.
 *
 * This writes the performance figures to \c stdout
 *
 * @return Zero.
 */
int perf_single_at_middle() {
    // This test is meaningless if assert is defined because of the integrity checks.
    assert(0);
    size_t SKIPLIST_SIZE = 1024 * 1024;
    int REPEAT_COUNT = GLOBAL_REPEAT_COUNT;
    int result = 0;
    double value = SKIPLIST_SIZE / 2;
    OrderedStructs::SkipList::HeadNode<double> sl;
    std::stringstream ostr;

    srand(1);
    assert(!sl.lacksIntegrity());
    for (size_t i = 0; i < SKIPLIST_SIZE; ++i) {
        sl.insert(i);
        assert(!sl.lacksIntegrity());
    }
    time_t start = clock();
    assert(!sl.lacksIntegrity());
    for (int i = 0; i < REPEAT_COUNT; ++i) {
        assert(!sl.lacksIntegrity());
        result |= sl.at(SKIPLIST_SIZE / 2) != value;
        assert(!sl.lacksIntegrity());
    }
    assert(!sl.lacksIntegrity());
    double exec = 1e6 * (clock() - start) / (double) CLOCKS_PER_SEC;
    std::cout << std::setw(FUNCTION_WIDTH) << __FUNCTION__ << "():";
    std::cout << " SkiplistSize: " << SKIPLIST_SIZE;
    std::cout << " repeat count: " << REPEAT_COUNT;
    std::cout << " time: ";
    std::cout << exec;
    std::cout << " (us)";
    std::cout << " rate " << 1e6 * REPEAT_COUNT / exec << " /s";
    std::cout << std::endl;
    return result;
}

/**
 * @brief Performance of calling \c has() on the middle value of a 1M long skip list.
 *
 * This writes the performance figures to \c stdout
 *
 * @return Zero.
 */
/* Performance of calling has() on the middle value of a 1M long skip list. */
int perf_single_has_middle() {
    // This test is meaningless if assert is defined because of the integrity checks.
    assert(0);
    size_t SKIPLIST_SIZE = 1024 * 1024;
    int REPEAT_COUNT = GLOBAL_REPEAT_COUNT;
    int result = 0;
    int value;
    OrderedStructs::SkipList::HeadNode<double> sl;
    std::stringstream ostr;

    srand(1);
    assert(!sl.lacksIntegrity());
    for (size_t i = 0; i < SKIPLIST_SIZE; ++i) {
        sl.insert(i);
        assert(!sl.lacksIntegrity());
    }
    time_t start = clock();
    assert(!sl.lacksIntegrity());
    for (int i = 0; i < REPEAT_COUNT; ++i) {
        assert(!sl.lacksIntegrity());
        value = sl.has(SKIPLIST_SIZE / 2);
        result |= value != 1;
        assert(!sl.lacksIntegrity());
    }
    assert(!sl.lacksIntegrity());
    double exec = 1e6 * (clock() - start) / (double) CLOCKS_PER_SEC;
    std::cout << std::setw(FUNCTION_WIDTH) << __FUNCTION__ << "():";
    std::cout << " SkiplistSize: " << SKIPLIST_SIZE;
    std::cout << " repeat count: " << REPEAT_COUNT;
    std::cout << " time: ";
    std::cout << exec;
    std::cout << " (us)";
    std::cout << " rate " << 1e6 * REPEAT_COUNT / exec << " /s";
    std::cout << std::endl;
    return result;
}

/**
 * @brief Performance of calling \c insert(), \c at(), \c remove() on the middle value of a 1M long skip list.
 *
 * This writes the performance figures to \c stdout
 *
 * @return Zero.
 */
int perf_single_ins_at_rem_middle() {
    // This test is meaningless if assert is defined because of the integrity checks.
    assert(0);
    size_t SKIPLIST_SIZE = 1024 * 1024;
    int REPEAT_COUNT = GLOBAL_REPEAT_COUNT;
    int result = 0;
    double value = SKIPLIST_SIZE / 2;
    OrderedStructs::SkipList::HeadNode<double> sl;
    std::stringstream ostr;

    srand(1);
    assert(!sl.lacksIntegrity());
    for (size_t i = 0; i < SKIPLIST_SIZE; ++i) {
        sl.insert(i);
        assert(!sl.lacksIntegrity());
    }
    time_t start = clock();
    assert(!sl.lacksIntegrity());
    for (int i = 0; i < REPEAT_COUNT; ++i) {
        sl.insert(value);
        assert(!sl.lacksIntegrity());
        value = sl.at(SKIPLIST_SIZE / 2);
        sl.remove(value);
        assert(!sl.lacksIntegrity());
    }
    assert(!sl.lacksIntegrity());
    double exec = 1e6 * (clock() - start) / (double) CLOCKS_PER_SEC;
    std::cout << std::setw(FUNCTION_WIDTH) << __FUNCTION__ << "():";
    std::cout << " SkiplistSize: " << SKIPLIST_SIZE;
    std::cout << " repeat count: " << REPEAT_COUNT;
    std::cout << " time: ";
    std::cout << exec;
    std::cout << " (us)";
    std::cout << " rate " << 1e6 * REPEAT_COUNT / exec << " /s";
    std::cout << std::endl;
    return result;
}

/**
 * @brief Performance of calling a rolling median of 100 values in a 10,000 long skip list.
 *
 * This requires an \c insert(), \c at() and \c remove() on the middle value.
 *
 * This writes the performance figures to \c stdout
 *
 * @return Zero.
 */
int perf_median_sliding_window() {
    srand(1);
    size_t VECTOR_LENGTH = 10000;
    size_t WINDOW_WIDTH = 100;
    std::vector<double> data;
    // Create random data
    for (size_t i = 0; i < VECTOR_LENGTH; ++i) {
        data.push_back(rand());
    }
    OrderedStructs::SkipList::HeadNode<double> sl;
    time_t start = clock();
    std::vector<double> medians;
    for (size_t i = 0; i < VECTOR_LENGTH; ++i) {
        sl.insert(data[i]);
        if (i > WINDOW_WIDTH) {
            medians.push_back(sl.at(WINDOW_WIDTH / 2));
            sl.remove(data[i - WINDOW_WIDTH]);
        }
    }
    double exec = 1e6 * (clock() - start) / (double) CLOCKS_PER_SEC;
    std::cout << std::setw(FUNCTION_WIDTH) << __FUNCTION__ << "():";
    std::cout << " vector length: " << VECTOR_LENGTH;
    std::cout << " window width: " << WINDOW_WIDTH;
    std::cout << " time: ";
    std::cout << exec;
    std::cout << " (us)";
    std::cout << std::endl;
    return 0;
}

/**
 * @brief Simulates a rolling median on single vector of 1000000 prices, window size of 101.
 *
 * This requires an \c insert(), \c at() and \c remove() on the middle value.
 *
 * This writes the performance figures to \c stdout
 *
 * @return Zero.
 */
int perf_1m_median_values() {
    srand(1);
    size_t NUMBER_VECTORS = 1;
    size_t VECTOR_LENGTH = 1000 * 1000;
    size_t WINDOW_WIDTH = 101;
    std::vector<double> data;
    // Create random data
    for (size_t i = 0; i < VECTOR_LENGTH; ++i) {
        data.push_back(rand());
    }
    time_t start = clock();
    for (size_t equity = 0; equity < NUMBER_VECTORS; ++equity) {
        OrderedStructs::SkipList::HeadNode<double> sl;
        std::vector<double> medians;
        for (size_t i = 0; i < VECTOR_LENGTH; ++i) {
            sl.insert(data[i]);
            if (i > WINDOW_WIDTH) {
                medians.push_back(sl.at(WINDOW_WIDTH / 2));
                sl.remove(data[i - WINDOW_WIDTH]);
            }
        }
    }
    double exec = 1e6 * (clock() - start) / (double) CLOCKS_PER_SEC;
    std::cout << std::setw(FUNCTION_WIDTH) << __FUNCTION__ << "():";
    std::cout << " vectors: " << std::setw(6) << NUMBER_VECTORS;
    std::cout << " of length: " << std::setw(8) << VECTOR_LENGTH;
//    std::cout << " window width: " << std::setw(4) << WINDOW_WIDTH;
    std::cout << " time: ";
    std::cout << exec / 1e6;
    std::cout << " (s)";
    std::cout << std::endl;
    return 0;
}

/**
 * @brief Simulates a rolling median on 1000 vectors of 1000 prices, window size of 101.
 *
 * This requires an \c insert(), \c at() and \c remove() on the middle value.
 *
 * This writes the performance figures to \c stdout
 *
 * @return Zero.
 */
int perf_1m_medians_1000_vectors() {
    srand(1);
    size_t NUMBER_VECTORS = 1000;
    size_t VECTOR_LENGTH = 1000;
    size_t WINDOW_WIDTH = 101;
    std::vector<double> data;
    // Create random data
    for (size_t i = 0; i < VECTOR_LENGTH; ++i) {
        data.push_back(rand());
    }
    time_t start = clock();
    for (size_t equity = 0; equity < NUMBER_VECTORS; ++equity) {
        OrderedStructs::SkipList::HeadNode<double> sl;
        std::vector<double> medians;
        for (size_t i = 0; i < VECTOR_LENGTH; ++i) {
            sl.insert(data[i]);
            if (i > WINDOW_WIDTH) {
                medians.push_back(sl.at(WINDOW_WIDTH / 2));
                sl.remove(data[i - WINDOW_WIDTH]);
            }
        }
    }
    double exec = 1e6 * (clock() - start) / (double) CLOCKS_PER_SEC;
    std::cout << std::setw(FUNCTION_WIDTH) << __FUNCTION__ << "():";
    std::cout << " vectors: " << std::setw(6) << NUMBER_VECTORS;
    std::cout << " of length: " << std::setw(8) << VECTOR_LENGTH;
    std::cout << " window width: " << std::setw(4) << WINDOW_WIDTH;
    std::cout << " time: ";
    std::cout << exec / 1e6;
    std::cout << " (s)";
    std::cout << std::endl;
    return 0;
}

/**
 * @brief Simulates a rolling median on 8000 vectors of 8000 prices, window size of 200.
 *
 * This requires an \c insert(), \c at() and \c remove() on the middle value.
 *
 * This writes the performance figures to \c stdout
 *
 * @return Zero.
 */
int perf_simulate_real_use() {
    srand(1);
    size_t NUMBER_VECTORS = 8000;
    size_t VECTOR_LENGTH = 8000;
    size_t WINDOW_WIDTH = 200;
    std::vector<double> data;
    // Create random data
    for (size_t i = 0; i < VECTOR_LENGTH; ++i) {
        data.push_back(rand());
    }
    time_t start = clock();
    for (size_t equity = 0; equity < NUMBER_VECTORS; ++equity) {
        OrderedStructs::SkipList::HeadNode<double> sl;
        std::vector<double> medians;
        for (size_t i = 0; i < VECTOR_LENGTH; ++i) {
            sl.insert(data[i]);
            if (i > WINDOW_WIDTH) {
                medians.push_back(sl.at(WINDOW_WIDTH / 2));
                sl.remove(data[i - WINDOW_WIDTH]);
            }
        }
    }
    double exec = 1e6 * (clock() - start) / (double) CLOCKS_PER_SEC;
    std::cout << std::setw(FUNCTION_WIDTH) << __FUNCTION__ << "():";
    std::cout << " vectors: " << std::setw(6) << NUMBER_VECTORS;
    std::cout << " of length: " << std::setw(8) << VECTOR_LENGTH;
    std::cout << " window width: " << std::setw(4) << WINDOW_WIDTH;
    std::cout << " time: ";
    std::cout << exec / 1e6;
    std::cout << " (s)";
    std::cout << std::endl;
    return 0;
}

/**
 * @brief Performance of \c at() in various positions on a vector 1m long.
 *
 * This writes the performance figures to \c stdout
 *
 * @return Zero.
 */
int perf_at_in_one_million() {
    size_t NUM = 1024 * 1024;
    size_t REPEAT = 1000000;
    int result = 0;
    OrderedStructs::SkipList::HeadNode<double> sl;

    srand(1);
    for (size_t i = 0; i < NUM; ++i) {
        sl.insert(i);
    }
    for (size_t i = 1; i < NUM; i *= 2) {
        time_t start = clock();
        for (size_t j = 1; j < REPEAT; ++j) {
            result |= sl.at(i) != i;
        }
        double exec = 1e9 * (clock() - start) / (double) CLOCKS_PER_SEC / REPEAT;
        std::cout << std::setw(FUNCTION_WIDTH) << __FUNCTION__ << "(): ";
        std::cout << "at(" << std::setw(8) << i << "): ";
        std::cout << std::setw(8) << exec << " ns";
        std::cout << " ticks " << clock() - start << std::endl;
    }
    return result;
}

/**
 * @brief Performance of \c has() in various positions on a vector 1m long.
 *
 * This writes the performance figures to \c stdout
 *
 * @return Zero.
 */
int perf_has_in_one_million_vary_length() {
    size_t NUM = 1024 * 1024;
    size_t REPEAT = 1000000;
    int result = 0;
    OrderedStructs::SkipList::HeadNode<double> sl;

    srand(1);
    for (size_t i = 0; i < NUM; ++i) {
        sl.insert(i);
    }
    for (size_t i = 1; i < NUM; i *= 2) {
        time_t start = clock();
        for (size_t j = 1; j < REPEAT; ++j) {
            result |= !sl.has(i);
        }
        double exec = 1e9 * (clock() - start) / (double) CLOCKS_PER_SEC / REPEAT;
        std::cout << std::setw(FUNCTION_WIDTH) << __FUNCTION__ << "(): ";
        std::cout << "has(" << std::setw(8) << i << "): ";
        std::cout << std::setw(8) << exec << " ns" << std::endl;
    }
    return result;
}

/**
 * @brief Tests the performance of \c size_of() function on a skip list of doubles length up to 1M.
 *
 * This writes the performance figures to \c stdout
 *
 * @return Zero.
 */
int perf_size_of_double_vary_length() {
    size_t NUM = 1024 * 1024;
    int result = 0;
    typedef double tValue;

    srand(1);
    for (size_t i = 1; i <= NUM; i *= 2) {
        OrderedStructs::SkipList::HeadNode<tValue> sl;
        for (size_t j = 0; j < i; ++j) {
            sl.insert(j);
        }
        size_t size_of = sl.size_of();
        std::cout << std::setw(FUNCTION_WIDTH) << __FUNCTION__ << "(): ";
        std::cout << "size_of(" << std::setw(8) << i << "): ";
        std::cout << std::setw(8) << size_of << " bytes";
        std::cout << " ratio: ";
        std::cout << std::setw(8) << std::setprecision(4);
        std::cout << 1.0 * size_of / i;
        std::cout << " /sizeof(T): ";
        std::cout << std::setw(8) << std::setprecision(4);
        std::cout << 1.0 * size_of / (i * sizeof(tValue));
        std::cout << std::endl;
    }
    return result;
}

/**
 * @brief Tests the performance of \c size_of() function on a skip list of chars length up to 1M.
 *
 * This writes the performance figures to \c stdout
 *
 * @return Zero.
 */
int perf_size_of_char_vary_length() {
    size_t NUM = 1024 * 1024;
    int result = 0;
    typedef char tValue;

    srand(1);
    for (size_t i = 1; i <= NUM; i *= 2) {
        OrderedStructs::SkipList::HeadNode<tValue> sl;
        for (size_t j = 0; j < i; ++j) {
            sl.insert(j & 0xFF);
        }
        size_t size_of = sl.size_of();
        std::cout << std::setw(FUNCTION_WIDTH) << __FUNCTION__ << "(): ";
        std::cout << "size_of(" << std::setw(8) << i << "): ";
        std::cout << std::setw(8) << size_of << " bytes";
        std::cout << " ratio: ";
        std::cout << std::setw(8) << std::setprecision(4);
        std::cout << 1.0 * size_of / i;
        std::cout << " /sizeof(T): ";
        std::cout << std::setw(8) << std::setprecision(4);
        std::cout << 1.0 * size_of / (i * sizeof(tValue));
        std::cout << std::endl;
    }
    return result;
}

/**
 * @brief Tests the \c size_of() function on a skip list of length 1M.
 *
 * This writes the performance figures to \c stdout
 *
 * @return Zero.
 */
int perf_size_of_1m() {
    size_t NUM = 1024 * 1024;
    int result = 0;
    typedef double tValue;

    srand(1);
    OrderedStructs::SkipList::HeadNode<tValue> sl;
    for (size_t j = 0; j < NUM; ++j) {
        sl.insert(j);
    }
    size_t size_of = sl.size_of();
    std::cout << std::setw(FUNCTION_WIDTH) << __FUNCTION__ << "(): ";
    std::cout << "size_of(" << std::setw(8) << NUM << "): ";
    std::cout << std::setw(8) << size_of << " bytes";
    std::cout << " ratio: ";
    std::cout << std::setw(8) << std::setprecision(4);
    std::cout << 1.0 * size_of / NUM;
    std::cout << " /sizeof(T): ";
    std::cout << std::setw(8) << std::setprecision(4);
    std::cout << 1.0 * size_of / (NUM * sizeof(tValue));
    std::cout << std::endl;
    return result;
}

/**
 * @brief Tests the \c index() function on a skip list of length 1M.
 *
 * This writes the performance figures to \c stdout
 *
 * @return Zero.
 */
int perf_index() {
    size_t NUM = 1024 * 1024;
    size_t REPEAT = 1000000;
    int result = 0;
    OrderedStructs::SkipList::HeadNode<double> sl;

    srand(1);
    for (size_t i = 0; i < NUM; ++i) {
        sl.insert(i);
    }
    time_t start = clock();
    for (size_t j = 1; j < REPEAT; ++j) {
        result |= sl.index(NUM / 2) != NUM / 2;
    }
    double exec = 1e9 * (clock() - start) / (double) CLOCKS_PER_SEC / REPEAT;
    std::cout << std::setw(FUNCTION_WIDTH) << __FUNCTION__ << "(): ";
    std::cout << std::setw(8) << exec << " ns" << std::endl;
    return result;
}

/**
 * @brief Tests the \c size_of() function on a skip lists of variable length up to 1M doubles.
 *
 * This writes the performance figures to \c stdout
 *
 * @return Zero.
 */
int perf_index_vary_length() {
    size_t NUM = 1024 * 1024;
    size_t REPEAT = 1000000;
    int result = 0;
    OrderedStructs::SkipList::HeadNode<double> sl;

    srand(1);
    for (size_t i = 0; i < NUM; ++i) {
        sl.insert(i);
    }
    for (size_t i = 1; i < NUM; i *= 2) {
        time_t start = clock();
        for (size_t j = 1; j < REPEAT; ++j) {
            result |= sl.index(i) != i;
        }
        double exec = 1e9 * (clock() - start) / (double) CLOCKS_PER_SEC / REPEAT;
        std::cout << std::setw(FUNCTION_WIDTH) << __FUNCTION__ << "(): ";
        std::cout << "index(" << std::setw(8) << i << "): ";
        std::cout << std::setw(8) << exec << " ns" << std::endl;
    }
    return result;
}

int perf_test_double_insert_remove_value(std::string function, size_t test_count, size_t repeat, size_t sl_length,
                                         double value, TestResultS &test_results) {
    std::ostringstream title;
    title << function << "[" << sl_length << "]";
    TestResult test_result(title.str());

    int result = 0;

    for (size_t i = 0; i < repeat; ++i) {
        // Create and populate a SkipList of sl_length doubles
        OrderedStructs::SkipList::HeadNode<double> sl;
        for (size_t i = 0; i < sl_length; ++i) {
            sl.insert(i);
        }
        ExecClock exec_clock;
        for (size_t j = 0; j < test_count; ++j) {
            sl.insert(value);
            sl.remove(value);
        }
        double exec_time = exec_clock.seconds();
        if (i == 0) {
            std::cout << function << "[" << sl_length << "] Sample time/op = " << 1e9 * exec_time / test_count << "(ns)"
                      << std::endl;
        }
        test_result.execTimeAdd(0, exec_time, test_count, sl_length);
    }
    test_results.push_back(test_result);
    return result;
}

int perf_test_double_insert_remove_value_begin(size_t test_count, size_t repeat, TestResultS &test_results) {
    std::cout << "Running test: " << __FUNCTION__ << std::endl;
    size_t sl_length = 2;
    int result = 0;
    while (sl_length <= 1 << 20) {
        result |= perf_test_double_insert_remove_value(__FUNCTION__, test_count, repeat, sl_length, 0.0, test_results);
        sl_length *= 2;
    }
    return result;
}

int perf_test_double_insert_remove_value_mid(size_t test_count, size_t repeat, TestResultS &test_results) {
    std::cout << "Running test: " << __FUNCTION__ << std::endl;
    size_t sl_length = 2;
    int result = 0;
    while (sl_length <= 1 << 20) {
        result |= perf_test_double_insert_remove_value(__FUNCTION__, test_count, repeat, sl_length, sl_length / 2.0,
                                                       test_results);
        sl_length *= 2;
    }
    return result;
}

int perf_test_double_insert_remove_value_end(size_t test_count, size_t repeat, TestResultS &test_results) {
    std::cout << "Running test: " << __FUNCTION__ << std::endl;
    size_t sl_length = 2;
    int result = 0;
    while (sl_length <= 1 << 20) {
        result |= perf_test_double_insert_remove_value(__FUNCTION__, test_count, repeat, sl_length, sl_length,
                                                       test_results);
        sl_length *= 2;
    }
    return result;
}

int perf_test_double_at_1m(size_t test_count, size_t repeat, size_t index, TestResultS &test_results) {
    std::ostringstream title;
    title << __FUNCTION__ << "[" << index << "]";
    TestResult test_result(title.str());

    int result = 0;

    for (size_t i = 0; i < repeat; ++i) {
        // Create and populate a SkipList of sl_length doubles
        OrderedStructs::SkipList::HeadNode<double> sl;
        for (size_t i = 0; i <= 1 << 20; ++i) {
            sl.insert(i);
        }
        ExecClock exec_clock;
        for (size_t j = 0; j < test_count; ++j) {
            sl.at(index);
        }
        double exec_time = exec_clock.seconds();
        if (i == 0) {
            std::cout << __FUNCTION__ << "[" << index << "] Sample time/op = " << 1e9 * exec_time / test_count << "(ns)"
                      << std::endl;
        }
        test_result.execTimeAdd(0, exec_time, test_count, index);
    }
    test_results.push_back(test_result);
    return result;
}

int perf_test_double_at_1m_all(size_t test_count, size_t repeat, TestResultS &test_results) {
    std::cout << "Running test: " << __FUNCTION__ << std::endl;
    size_t index_value = 1;
    int result = 0;
    while (index_value <= 1 << 20) {
        result |= perf_test_double_at_1m(test_count, repeat, index_value, test_results);
        index_value *= 2;
    }
    return result;
}

int perf_test_double_has_1m(size_t test_count, size_t repeat, size_t value, TestResultS &test_results) {
    std::ostringstream title;
    title << __FUNCTION__ << "[" << value << "]";
    TestResult test_result(title.str());
    double double_value = value;

    int result = 0;

    for (size_t i = 0; i < repeat; ++i) {
        // Create and populate a SkipList of sl_length doubles
        OrderedStructs::SkipList::HeadNode<double> sl;
        for (size_t i = 0; i <= 1 << 20; ++i) {
            sl.insert(i);
        }
        ExecClock exec_clock;
        for (size_t j = 0; j < test_count; ++j) {
            sl.has(double_value);
        }
        double exec_time = exec_clock.seconds();
        if (i == 0) {
            std::cout << __FUNCTION__ << "[" << value << "] Sample time/op = " << 1e9 * exec_time / test_count << "(ns)"
                      << std::endl;
        }
        test_result.execTimeAdd(0, exec_time, test_count, value);
    }
    test_results.push_back(test_result);
    return result;
}

int perf_test_double_has_1m_all(size_t test_count, size_t repeat, TestResultS &test_results) {
    std::cout << "Running test: " << __FUNCTION__ << std::endl;
    size_t index_value = 1;
    int result = 0;
    while (index_value <= 1 << 20) {
        result |= perf_test_double_has_1m(test_count, repeat, index_value, test_results);
        index_value *= 2;
    }
    return result;
}

int perf_test_double_index_1m(size_t test_count, size_t repeat, size_t value, TestResultS &test_results) {
    std::ostringstream title;
    title << __FUNCTION__ << "[" << value << "]";
    TestResult test_result(title.str());
    double double_value = value;

    int result = 0;

    for (size_t i = 0; i < repeat; ++i) {
        // Create and populate a SkipList of sl_length doubles
        OrderedStructs::SkipList::HeadNode<double> sl;
        for (size_t i = 0; i <= 1 << 20; ++i) {
            sl.insert(i);
        }
        ExecClock exec_clock;
        for (size_t j = 0; j < test_count; ++j) {
            sl.index(double_value);
        }
        double exec_time = exec_clock.seconds();
        if (i == 0) {
            std::cout << __FUNCTION__ << "[" << value << "] Sample time/op = " << 1e9 * exec_time / test_count << "(ns)"
                      << std::endl;
        }
        test_result.execTimeAdd(0, exec_time, test_count, value);
    }
    test_results.push_back(test_result);
    return result;
}

int perf_test_double_index_1m_all(size_t test_count, size_t repeat, TestResultS &test_results) {
    std::cout << "Running test: " << __FUNCTION__ << std::endl;
    size_t index_value = 1;
    int result = 0;
    while (index_value <= 1 << 20) {
        result |= perf_test_double_index_1m(test_count, repeat, index_value, test_results);
        index_value *= 2;
    }
    return result;
}

// Tests evaluating a rolling median on 1m doubles with different window lengths.
int perf_roll_med_by_win_size(size_t repeat, TestResultS &test_results) {
    int result = 0;
    const int DEST_STRIDE = 1;
    const size_t ARRAY_SIZE = 1 << 20;
    // Create and populate an array of 1m random doubles
    double *src = new double[ARRAY_SIZE];
    for (size_t i = 0; i < ARRAY_SIZE; ++i) {
        src[i] = rand();
    }
    for (size_t win_length = 1; win_length < 1 << 20; win_length *= 2) {
        std::ostringstream title;
        title << __FUNCTION__ << "[" << win_length << "]";
        TestResult test_result(title.str());

        size_t dest_size = OrderedStructs::RollingMedian::dest_size(ARRAY_SIZE, win_length, DEST_STRIDE);
        double *dest = new double[dest_size];
        for (size_t r = 0; r < repeat; ++r) {
            ExecClock exec_clock;
            result |= OrderedStructs::RollingMedian::even_odd_index(src, 1, ARRAY_SIZE,
                                                                    win_length, dest, DEST_STRIDE);
            double exec_time = exec_clock.seconds();
            if (r == 0) {
                std::cout << __FUNCTION__ << "[" << win_length << "] Sample time/op = "
                          << exec_time / 1 << "(s)"
                          << std::endl;
            }
            test_result.execTimeAdd(0, exec_time, 1, win_length);
        }
        delete[] dest;
        test_results.push_back(test_result);
    }
    delete[] src;
    return result;
}

int perf_test_node_height_growth(size_t repeat, TestResultS &test_results) {
    int result = 0;

    for (size_t sl_length = 1; sl_length < 1 << 20; sl_length *= 2) {
        std::ostringstream title;
        title << __FUNCTION__ << "[" << sl_length << "]";
        TestResult test_result(title.str());
        for (size_t i = 0; i < repeat; ++i) {
            // Create and populate a SkipList of sl_length doubles
            OrderedStructs::SkipList::HeadNode<double> sl;
            for (size_t i = 0; i <= sl_length; ++i) {
                sl.insert(i);
            }
            if (i == 0) {
                std::cout << __FUNCTION__ << "[" << sl_length << "] Sample height = " << sl.height() << std::endl;
            }
            test_result.execTimeAdd(0, sl.height(), repeat, sl_length);
        }
        test_results.push_back(test_result);
    }
    return result;
}

int perf_test_string_insert_remove_value(std::string function, size_t test_count, size_t repeat, size_t sl_length,
                                         size_t str_length, size_t index_for_value, TestResultS &test_results) {
    std::ostringstream title;
    title << function << "[" << sl_length << "]";
    TestResult test_result(title.str());

    int result = 0;
    std::string value;

    for (size_t i = 0; i < repeat; ++i) {
        // Create and populate a SkipList of sl_length strings
        OrderedStructs::SkipList::HeadNode<std::string> sl;
        for (size_t i = 0; i < sl_length; ++i) {
            if (i == index_for_value) {
                value = unique_string(str_length);
            } else {
                sl.insert(unique_string(str_length));
            }
        }
        ExecClock exec_clock;
        for (size_t j = 0; j < test_count; ++j) {
            sl.insert(value);
            sl.remove(value);
        }
        double exec_time = exec_clock.seconds();
        if (i == 0) {
            std::cout << function << "[" << sl_length << "] Sample time/op = " << 1e9 * exec_time / test_count << "(ns)"
                      << std::endl;
        }
        test_result.execTimeAdd(0, exec_time, test_count, sl_length);
    }
    test_results.push_back(test_result);
    return result;
}

int perf_test_string_insert_remove_value_begin(size_t test_count, size_t repeat, TestResultS &test_results) {
    std::cout << "Running test: " << __FUNCTION__ << std::endl;
    size_t sl_length = 2;
    int result = 0;
    while (sl_length <= 1 << 10) {
        result |= perf_test_string_insert_remove_value(__FUNCTION__, test_count, repeat, sl_length, 1024, 0, test_results);
        sl_length *= 2;
    }
    return result;
}

int perf_test_string_insert_remove_value_mid(size_t test_count, size_t repeat, TestResultS &test_results) {
    std::cout << "Running test: " << __FUNCTION__ << std::endl;
    size_t sl_length = 2;
    int result = 0;
    while (sl_length <= 1 << 10) {
        result |= perf_test_string_insert_remove_value(__FUNCTION__, test_count, repeat, sl_length, 1024, sl_length / 2, test_results);
        sl_length *= 2;
    }
    return result;
}

int perf_test_string_insert_remove_value_end(size_t test_count, size_t repeat, TestResultS &test_results) {
    std::cout << "Running test: " << __FUNCTION__ << std::endl;
    size_t sl_length = 2;
    int result = 0;
    while (sl_length <= 1 << 10) {
        result |= perf_test_string_insert_remove_value(__FUNCTION__, test_count, repeat, sl_length, sl_length, sl_length - 1, test_results);
        sl_length *= 2;
    }
    return result;
}


/******************* END: Performance Tests **************************/

/**
 * @brief Runs all the basic performance tests.
 *
 * @return Zero on success, non-zero on failure.
 */
int perf_skiplist() {
    int result = 0;

    result |= perf_single_insert_remove();
    result |= perf_large_skiplist_ins_only();
    result |= perf_large_skiplist_ins_rem();
    result |= perf_single_ins_rem_middle();
    result |= perf_single_ins_rem_middle_vary_length();
    result |= perf_single_at_middle();
    result |= perf_single_has_middle();
    result |= perf_single_ins_at_rem_middle();
    result |= perf_median_sliding_window();
    result |= perf_1m_median_values();
    result |= perf_1m_medians_1000_vectors();
    result |= perf_simulate_real_use();
    result |= perf_at_in_one_million();
    result |= perf_has_in_one_million_vary_length();
    result |= perf_index();
    result |= perf_index_vary_length();

    // Multiple statistical tests
    TestResultS perf_test_results;
    result |= perf_test_double_insert_remove_value_begin(100, 10, perf_test_results);
    result |= perf_test_double_insert_remove_value_mid(100, 10, perf_test_results);
    result |= perf_test_double_insert_remove_value_end(100, 10, perf_test_results);
    result |= perf_test_double_at_1m_all(10, 5, perf_test_results);
    result |= perf_test_double_has_1m_all(10, 5, perf_test_results);
    result |= perf_test_double_index_1m_all(10, 5, perf_test_results);
    result |= perf_roll_med_by_win_size(10, perf_test_results);
    result |= perf_test_node_height_growth(20, perf_test_results);

    result |= perf_test_string_insert_remove_value_begin(100, 10, perf_test_results);
    result |= perf_test_string_insert_remove_value_mid(100, 10, perf_test_results);
    result |= perf_test_string_insert_remove_value_end(100, 10, perf_test_results);

    perf_test_results.dump_header(std::cout);
    perf_test_results.dump_tests(std::cout);
    perf_test_results.dump_tail(std::cout);

    return result;
}

/**
 * @brief Runs all the basic size tests.
 *
 * @return Zero on success, non-zero on failure.
 */
int perf_size() {
    int result = 0;

    result |= perf_size_of_double_vary_length();
    result |= perf_size_of_char_vary_length();
    result |= perf_size_of_1m();

    return result;
}

/**
 * @brief This function call just tests a subset of the performance.
 * This subset is the one we are interested in when exploring the effect
 * of an unfair coin.
 *
 * @return Zero on success, non-zero on failure.
 */
int perf_skiplist_unfair_coin() {
    int result = 0;

    result |= perf_single_at_middle();
    result |= perf_single_has_middle();
    result |= perf_single_ins_at_rem_middle();
    result |= perf_1m_median_values();
    result |= perf_size_of_1m();

    return result;
}

/**
 * @brief Runs all the performance tests.
 *
 * @return Zero on success, non-zero on failure.
 */
int test_performance_all() {
    int result = 0;

    result |= perf_skiplist();
#if 1
    result |= perf_size();
    result |= perf_skiplist_unfair_coin();
#endif
    return result;
}
