//
//  test_performance.cpp
//  skiplist
//
//  Created by Paul Ross on 13/07/2016.
//  Copyright (c) 2016 Paul Ross. All rights reserved.
//

#include <iostream>
#include <iomanip>

#include "test_performance.h"
#include "test_print.h"

#include "../SkipList.h"

/******************* Performance Tests **************************
 * These should have a corresponding test_... which checks correctness.
 * The perf_... version should do no checking but aim for maximum performance
 * however assert(! sl.lacksIntegrity()); is encouraged as that will get
 * stripped out in release builds where performance is really tested.
 */

static int GLOBAL_REPEAT_COUNT = 1000 * 1000;

int perf_single_insert_remove() {
    int num = GLOBAL_REPEAT_COUNT;
    ManAHL::SkipList::HeadNode<double> sl;
    
    srand(1);
    time_t start = clock();
    for (int i = 0; i < num; ++i) {
        assert(! sl.lacksIntegrity());
        sl.insert(42.0);
        assert(! sl.lacksIntegrity());
        sl.remove(42.0);
        assert(! sl.lacksIntegrity());
    }
    double exec = 1e3 * (clock() - start) / (double) CLOCKS_PER_SEC;
    std::cout << std::setw(FUNCTION_WIDTH) << __FUNCTION__ << "(): ";
    std::cout << exec;
    std::cout << " (ms)";
    std::cout << " rate " << 1000. * num / exec << " /s";
    std::cout << std::endl;
    return 0;
}

/* Insert to create a large skiplist no deletion. */
int perf_large_skiplist_ins_only() {
    size_t SIZE = GLOBAL_REPEAT_COUNT;
    int COUNT = 1;
    ManAHL::SkipList::HeadNode<double> sl;
    
    srand(1);
    time_t start = clock();
    for (int c = 0; c < COUNT; ++c) {
        ManAHL::SkipList::HeadNode<double> sl;
        assert(! sl.lacksIntegrity());
        for (size_t i = 0; i < SIZE; ++i) {
            sl.insert(i);
            assert(! sl.lacksIntegrity());
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

/* Insert into a large skiplist then remove everything and delete it
 * many times. */
int perf_large_skiplist_ins_rem() {
    size_t SIZE = 1024 * 1024;
    int COUNT = 1;
    ManAHL::SkipList::HeadNode<double> sl;
    
    srand(1);
    time_t start = clock();
    for (int c = 0; c < COUNT; ++c) {
        ManAHL::SkipList::HeadNode<double> sl;
        assert(! sl.lacksIntegrity());
        for (size_t i = 0; i < SIZE; ++i) {
            sl.insert(i);
            assert(! sl.lacksIntegrity());
        }
        assert(sl.size() == SIZE);
        for (size_t i = 0; i < SIZE; ++i) {
            sl.remove(i);
            assert(! sl.lacksIntegrity());
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

/* Create a large skip list then time how long it takes to insert and remove
 * a value at the mid point. */
int perf_single_ins_rem_middle() {
    size_t SKIPLIST_SIZE = 1024 * 1024;
    int REPEAT_COUNT = GLOBAL_REPEAT_COUNT;
    int result = 0;
    double val = SKIPLIST_SIZE / 2;
    ManAHL::SkipList::HeadNode<double> sl;
    
    srand(1);
    assert(! sl.lacksIntegrity());
    for (size_t i = 0; i < SKIPLIST_SIZE; ++i) {
        sl.insert(i);
        assert(! sl.lacksIntegrity());
    }
    time_t start = clock();
    assert(! sl.lacksIntegrity());
    for (int i = 0; i < REPEAT_COUNT; ++i) {
        sl.insert(val);
        assert(! sl.lacksIntegrity());
        sl.remove(val);
        assert(! sl.lacksIntegrity());
    }
    assert(! sl.lacksIntegrity());
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

/* Create a large skip list then time how long it takes to insert and remove
 * a value at the mid point for various length skip lists. */
int perf_single_ins_rem_middle_vary_length() {
    int REPEAT_COUNT = GLOBAL_REPEAT_COUNT;
    int result = 0;
    
    for (size_t siz = 1; siz < 1024 * 1024 + 1; siz *= 2) {
        ManAHL::SkipList::HeadNode<double> sl;
        
        srand(1);
        assert(! sl.lacksIntegrity());
        // Populate skip list
        for (size_t i = 0; i < siz + 1; ++i) {
            sl.insert(i);
            assert(! sl.lacksIntegrity());
        }
        double value = sl.size() / 2;
        time_t start = clock();
        for (int i = 0; i < REPEAT_COUNT; ++i) {
            sl.insert(value);
            assert(! sl.lacksIntegrity());
            sl.remove(value);
            assert(! sl.lacksIntegrity());
        }
        assert(! sl.lacksIntegrity());
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

/* Performance of calling at() on the middle value of a 1M long skip list. */
int perf_single_at_middle() {
    size_t SKIPLIST_SIZE = 1024 * 1024;
    int REPEAT_COUNT = GLOBAL_REPEAT_COUNT;
    int result = 0;
    double value = SKIPLIST_SIZE / 2;
    ManAHL::SkipList::HeadNode<double> sl;
    std::stringstream ostr;
    
    srand(1);
    assert(! sl.lacksIntegrity());
    for (size_t i = 0; i < SKIPLIST_SIZE; ++i) {
        sl.insert(i);
        assert(! sl.lacksIntegrity());
    }
    time_t start = clock();
    assert(! sl.lacksIntegrity());
    for (int i = 0; i < REPEAT_COUNT; ++i) {
        assert(! sl.lacksIntegrity());
        result |= sl.at(SKIPLIST_SIZE / 2) != value;
        assert(! sl.lacksIntegrity());
    }
    assert(! sl.lacksIntegrity());
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

/* Performance of calling has() on the middle value of a 1M long skip list. */
int perf_single_has_middle() {
    size_t SKIPLIST_SIZE = 1024 * 1024;
    int REPEAT_COUNT = GLOBAL_REPEAT_COUNT;
    int result = 0;
    int value;
    ManAHL::SkipList::HeadNode<double> sl;
    std::stringstream ostr;
    
    srand(1);
    assert(! sl.lacksIntegrity());
    for (size_t i = 0; i < SKIPLIST_SIZE; ++i) {
        sl.insert(i);
        assert(! sl.lacksIntegrity());
    }
    time_t start = clock();
    assert(! sl.lacksIntegrity());
    for (int i = 0; i < REPEAT_COUNT; ++i) {
        assert(! sl.lacksIntegrity());
        value = sl.has(SKIPLIST_SIZE / 2);
        result |= value != 1;
        assert(! sl.lacksIntegrity());
    }
    assert(! sl.lacksIntegrity());
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

/* Performance of calling insert(), at(), remove() on the middle value
 * of a 1M long skip list. */
int perf_single_ins_at_rem_middle() {
    size_t SKIPLIST_SIZE = 1024 * 1024;
    int REPEAT_COUNT = GLOBAL_REPEAT_COUNT;
    int result = 0;
    double value = SKIPLIST_SIZE / 2;
    ManAHL::SkipList::HeadNode<double> sl;
    std::stringstream ostr;
    
    srand(1);
    assert(! sl.lacksIntegrity());
    for (size_t i = 0; i < SKIPLIST_SIZE; ++i) {
        sl.insert(i);
        assert(! sl.lacksIntegrity());
    }
    time_t start = clock();
    assert(! sl.lacksIntegrity());
    for (int i = 0; i < REPEAT_COUNT; ++i) {
        sl.insert(value);
        assert(! sl.lacksIntegrity());
        value = sl.at(SKIPLIST_SIZE / 2);
        sl.remove(value);
        assert(! sl.lacksIntegrity());
    }
    assert(! sl.lacksIntegrity());
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

int perf_median_sliding_window() {
    srand(1);
    size_t VECTOR_LENGTH = 10000;
    size_t WINDOW_WIDTH = 100;
    std::vector<double> data;
    // Create random data
    for (size_t i = 0; i < VECTOR_LENGTH; ++i) {
        data.push_back(rand());
    }
    ManAHL::SkipList::HeadNode<double> sl;
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

/* Simulates a rolling median on 1 vector of 1000000 prices,
 * window size of 101. */
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
        ManAHL::SkipList::HeadNode<double> sl;
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

/* Simulates a rolling median on 1000 vectors of 1000 prices,
 * window size of 101. */
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
        ManAHL::SkipList::HeadNode<double> sl;
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

/* Simulates a rolling median on 8000 vectors of 8000 prices,
 * window size of 200. */
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
        ManAHL::SkipList::HeadNode<double> sl;
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

int perf_at_in_one_million() {
    size_t NUM = 1024 * 1024;
    size_t REPEAT = 1000000;
    int result = 0;
    ManAHL::SkipList::HeadNode<double> sl;
    
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

int perf_has_in_one_million_vary_length() {
    size_t NUM = 1024 * 1024;
    size_t REPEAT = 1000000;
    int result = 0;
    ManAHL::SkipList::HeadNode<double> sl;
    
    srand(1);
    for (size_t i = 0; i < NUM; ++i) {
        sl.insert(i);
    }
    for (size_t i = 1; i < NUM; i *= 2) {
        time_t start = clock();
        for (size_t j = 1; j < REPEAT; ++j) {
            result |= ! sl.has(i);
        }
        double exec = 1e9 * (clock() - start) / (double) CLOCKS_PER_SEC / REPEAT;
        std::cout << std::setw(FUNCTION_WIDTH) << __FUNCTION__ << "(): ";
        std::cout << "has(" << std::setw(8) << i << "): ";
        std::cout << std::setw(8) << exec << " ns" << std::endl;
    }
    return result;
}

/* Tests the size_of() function on a skip list of length up to 1M. */
int perf_size_of_double_vary_length() {
    size_t NUM = 1024 * 1024;
    int result = 0;
    typedef double tValue;
    
    srand(1);
    for (size_t i = 1; i <= NUM; i *= 2) {
        ManAHL::SkipList::HeadNode<tValue> sl;
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

/* Tests the size_of() function on a skip list of length up to 1M. */
int perf_size_of_char_vary_length() {
    size_t NUM = 1024 * 1024;
    int result = 0;
    typedef char tValue;
    
    srand(1);
    for (size_t i = 1; i <= NUM; i *= 2) {
        ManAHL::SkipList::HeadNode<tValue> sl;
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

/* Tests the size_of() function on a skip list of length up to 1M. */
int perf_size_of_1m() {
    size_t NUM = 1024 * 1024;
    int result = 0;
    typedef double tValue;
    
    srand(1);
    ManAHL::SkipList::HeadNode<tValue> sl;
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

int perf_index() {
    size_t NUM = 1024 * 1024;
    size_t REPEAT = 1000000;
    int result = 0;
    ManAHL::SkipList::HeadNode<double> sl;
    
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

int perf_index_vary_length() {
    size_t NUM = 1024 * 1024;
    size_t REPEAT = 1000000;
    int result = 0;
    ManAHL::SkipList::HeadNode<double> sl;
    
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

/******************* END: Performance Tests **************************/

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
    
    return result;
}

int perf_size() {
    int result = 0;
    
    result |= perf_size_of_double_vary_length();
    result |= perf_size_of_char_vary_length();
    result |= perf_size_of_1m();
    
    return result;
}

/* This function call justs tests a subset of the performance.
 * This subset is the one we are interested in when exploring the effect
 * of an unfair coin.
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

int test_performance_all() {
    int result = 0;
    
    result |= perf_skiplist();
    result |= perf_size();
//    result |= perf_skiplist_unfair_coin();
    return result;
}



