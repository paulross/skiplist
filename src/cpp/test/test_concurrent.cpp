//
//  test_concurrent.cpp
//  skiplist
//
//  Created by Paul Ross on 17/08/2017.
//  Copyright (c) 2017 Paul Ross. All rights reserved.
//

#include <iomanip>
#include <thread>

#include "test_print.h"
#include "test_concurrent.h"
#include "../SkipList.h"


/******************** Concurrency Tests **************************/
template<typename T>
void
insert_value(ManAHL::SkipList::HeadNode<T> *psl, const T &value) {
    psl->insert(value);
}

template<typename T>
void
insert_has_remove(ManAHL::SkipList::HeadNode<T> *psl, const T &value) {
    psl->insert(value);
    psl->has(value);
    psl->remove(value);
}

template<typename T>
void insert_has_remove_count(ManAHL::SkipList::HeadNode<T> *psl,
                             const T &value,
                             const size_t &count) {
    for (size_t i = 0; i < count; ++i) {
        insert_has_remove(psl, value);
    }
}

template<typename T>
void insert_count_has_remove_count(ManAHL::SkipList::HeadNode<T> *psl,
                                   const T &value,
                                   const size_t &count) {
    for (size_t i = 0; i < count; ++i) {
        psl->insert(value);
    }
    psl->has(value);
    for (size_t i = 0; i < count; ++i) {
        psl->remove(value);
    }
}

int test_single_thread_insert() {
#ifdef SKIPLIST_THREAD_SUPPORT
    int result = 0;
    ManAHL::SkipList::HeadNode<double> sl;

    std::thread t(insert_value<double>, &sl, 1.0);
    t.join();
    result |= sl.lacksIntegrity();
    result |= sl.size() != 1;
    return result;
#endif // SKIPLIST_THREAD_SUPPORT
    return -1; // N/A
}

int test_two_thread_insert_has_remove() {
#ifdef SKIPLIST_THREAD_SUPPORT
    int result = 0;
    ManAHL::SkipList::HeadNode<double> sl;

    std::thread t0(insert_has_remove<double>, &sl, 1.0);
    std::thread t1(insert_has_remove<double>, &sl, 2.0);
    t0.join();
    t1.join();
    result |= sl.lacksIntegrity();
    result |= sl.size() != 0;
    return result;
#endif // SKIPLIST_THREAD_SUPPORT
    return -1; // N/A
}

int test_two_thread_insert_multi_count() {
#ifdef SKIPLIST_THREAD_SUPPORT
    int result = 0;
    ManAHL::SkipList::HeadNode<double> sl;

    std::thread t0(insert_count_has_remove_count<double>, &sl, 1.0, 1024*128);
    std::thread t1(insert_count_has_remove_count<double>, &sl, 2.0, 1024*128);
    t0.join();
    t1.join();
    result |= sl.lacksIntegrity();
    result |= sl.size() != 0;
    return result;
#endif // SKIPLIST_THREAD_SUPPORT
    return -1; // N/A
}

int _test_perf_multi_threads(
#ifdef SKIPLIST_THREAD_SUPPORT
                             const char* caller_name,
                             size_t thread_count,
                             size_t skiplist_size
#else
                             const char* /* Unused */,
                             size_t /* Unused */,
                             size_t /* Unused */
#endif
                             ) {
#ifdef SKIPLIST_THREAD_SUPPORT
    int result = 0;
    ManAHL::SkipList::HeadNode<double> sl;
    std::vector<std::thread> threads;

    time_t start = clock();
    for (size_t i = 0; i < thread_count; ++i) {
        threads.push_back(std::thread(
                insert_count_has_remove_count<double>, &sl, i, skiplist_size)
        );
    }
    for (auto &t: threads) {
        t.join();
    }
    result |= sl.lacksIntegrity();
    result |= sl.size() != 0;
    double exec = 1e6 * (clock() - start) / (double) CLOCKS_PER_SEC;
    std::cout << std::setw(FUNCTION_WIDTH) << caller_name << "():";
    std::cout << " threads: " << std::setw(4) << thread_count;
    std::cout << " SkiplistSize: " << std::setw(8) << skiplist_size;
    std::cout << " time: ";
    std::cout << std::setw(12) << exec;
    std::cout << " (us)";
    std::cout << " rate " << std::setw(12);
    std::cout << thread_count * skiplist_size / (exec / 1e6) << " /s";
    std::cout << std::endl;
    return result;
#endif // SKIPLIST_THREAD_SUPPORT
    return -1; // N/A
}

const size_t SKIPLIST_FIXED_LENGTH = 1024 * 16;
// Number of threads < this number, increment is x2.
const size_t SKIPLIST_MAX_THREADS = 128;

int test_perf_multi_threads_vary_length() {
    int result = 0;

    for (size_t t = 1; t < SKIPLIST_MAX_THREADS; t *=2) {
        result |= _test_perf_multi_threads(__FUNCTION__,
                                           t,
                                           SKIPLIST_FIXED_LENGTH * 4 / t);
    }
    return result;
}

int test_perf_multi_threads_fixed_length() {
    int result = 0;

    for (size_t t = 1; t < SKIPLIST_MAX_THREADS; t *=2) {
        result |= _test_perf_multi_threads(__FUNCTION__,
                                           t,
                                           SKIPLIST_FIXED_LENGTH);
    }
    return result;
}

int _test_perf_single_thread(const char* caller_name,
                             size_t repeat_count,
                             size_t skiplist_size) {
    int result = 0;
    ManAHL::SkipList::HeadNode<double> sl;

    time_t start = clock();
    for (size_t i = 0; i < repeat_count; ++i) {
        insert_count_has_remove_count<double>(&sl, i, skiplist_size);
    }
    result |= sl.lacksIntegrity();
    result |= sl.size() != 0;
    double exec = 1e6 * (clock() - start) / (double) CLOCKS_PER_SEC;
    std::cout << std::setw(FUNCTION_WIDTH) << caller_name << "():";
    std::cout << "   count: " << std::setw(4) << repeat_count;
    std::cout << " SkiplistSize: " << std::setw(8) << skiplist_size;
    std::cout << " time: ";
    std::cout << std::setw(12) << exec;
    std::cout << " (us)";
    std::cout << " rate " << std::setw(12);
    std::cout << repeat_count * skiplist_size / (exec / 1e6) << " /s";
    std::cout << std::endl;
    return result;
}

int test_perf_single_thread_fixed_length() {
    int result = 0;

    for (size_t c = 1; c < SKIPLIST_MAX_THREADS; c *=2) {
        result |= _test_perf_single_thread(__FUNCTION__,
                                           c,
                                           SKIPLIST_FIXED_LENGTH);
    }
    return result;
}


/***************** END: Concurrency Tests ************************/

int test_concurrent_all() {
    int result = 0;
    result |= print_result("test_single_thread_insert",
                           test_single_thread_insert());
    result |= print_result("test_two_thread_insert_has_remove",
                           test_two_thread_insert_has_remove());
    result |= print_result("test_two_thread_insert_multi_count",
                           test_two_thread_insert_multi_count());
    // Performance tests are very slow if DEBUG as checking
    // integrity is very expensive for large data sets.
#ifndef DEBUG
    result |= print_result("test_perf_multi_threads_vary_length",
                           test_perf_multi_threads_vary_length());
    result |= print_result("test_perf_multi_threads_fixed_length",
                           test_perf_multi_threads_fixed_length());
    result |= print_result("test_perf_single_thread_fixed_length",
                           test_perf_single_thread_fixed_length());
#endif
    return result;
}
