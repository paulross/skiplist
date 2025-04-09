/**
 * @file
 *
 * Project: skiplist
 *
 * Concurrency Tests.
 *
 * Created by Paul Ross on 17/08/2017.
 *
 * Copyright (c) 2017-2023 Paul Ross. All rights reserved.
 *
 * @code
 * MIT License
 *
 * Copyright (c) 2017-2023 Paul Ross
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

#include <iomanip>
#include <thread>

#include "SkipList.h"
#include "TestFramework.h"
#include "test_print.h"
#include "test_concurrent.h"


/**
 * Insert a value into a Skip List.
 *
 * @tparam T Type of values in the Skip List.
 * @param psl Pointer to the Skip List.
 * @param value Value to insert.
 */
template<typename T>
static void
insert_value(OrderedStructs::SkipList::HeadNode<T> *psl, const T &value) {
    psl->insert(value);
}

/**
 * Insert a value into a Skip List, check it is there then remove it.
 *
 * @tparam T Type of values in the Skip List.
 * @param psl Pointer to the Skip List.
 * @param value Value to insert.
 */
template<typename T>
static void
insert_has_remove(OrderedStructs::SkipList::HeadNode<T> *psl, const T &value) {
    psl->insert(value);
    psl->has(value);
    psl->remove(value);
}

/**
 * Insert a value into a Skip List, check it is there then remove it.
 * Repeat this @c count times.
 * This can be used to simulate a rolling median in a single or multi threaded environment.
 *
 * @tparam T Type of values in the Skip List.
 * @param psl Pointer to the Skip List.
 * @param value Value to insert.
 * @param count Number of times to repeat the insert/add/remove.
 */
template<typename T>
static void
insert_has_remove_count(OrderedStructs::SkipList::HeadNode<T> *psl,
                        const T &value,
                        const size_t &count) {
//    std::cout << __FUNCTION__ << " Value: " << value << " thread ID: " << std::this_thread::get_id() << std::endl;
    for (size_t i = 0; i < count; ++i) {
        insert_has_remove(psl, value);
    }
//    std::cout << __FUNCTION__ << " Value: " << value << " thread ID: " << std::this_thread::get_id() << " DONE"
//              << std::endl;
}

/**
 * Insert a value into a Skip List @c count times, check it is there then remove it @c count times.
 *
 * @tparam T Type of values in the Skip List.
 * @param psl Pointer to the Skip List.
 * @param value Value to insert.
 * @param count Number of times to repeat the insert/remove.
 */
template<typename T>
static void
insert_count_has_remove_count(OrderedStructs::SkipList::HeadNode<T> *psl,
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

/**
 * Functional test of a single insert of a value with a single thread.
 *
 * @return -1 if compiled without thread support. Otherwise 0 on success, non-zero on failure.
 */
static int test_single_thread_insert() {
#ifdef SKIPLIST_THREAD_SUPPORT
    int result = 0;
    OrderedStructs::SkipList::HeadNode<double> sl;

    std::thread t(insert_value<double>, &sl, 1.0);
    t.join();
    result |= sl.lacksIntegrity();
    result |= sl.size() != 1;
    return result;
#endif // SKIPLIST_THREAD_SUPPORT
    return -1; // N/A
}

/**
 * Functional test of a single insert/has/remove with a single Skip List with two threads.
 *
 * @return -1 if compiled without thread support. Otherwise 0 on success, non-zero on failure.
 */
static int test_two_thread_insert_has_remove() {
#ifdef SKIPLIST_THREAD_SUPPORT
    int result = 0;
    OrderedStructs::SkipList::HeadNode<double> sl;

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

/**
 * Functional test of a multiple insert/has/remove with a single Skip List with two threads.
 *
 * @return -1 if compiled without thread support. Otherwise 0 on success, non-zero on failure.
 */
static int test_two_thread_insert_count_has_remove_count() {
#ifdef SKIPLIST_THREAD_SUPPORT
    int result = 0;
    OrderedStructs::SkipList::HeadNode<double> sl;

    std::thread t0(insert_count_has_remove_count<double>, &sl, 1.0, 1024 * 128);
    std::thread t1(insert_count_has_remove_count<double>, &sl, 2.0, 1024 * 128);
    t0.join();
    t1.join();
    result |= sl.lacksIntegrity();
    result |= sl.size() != 0;
    return result;
#endif // SKIPLIST_THREAD_SUPPORT
    return -1; // N/A
}

/// Size of the test Skip List.
const size_t SKIPLIST_FIXED_LENGTH = 1024 * 16;
/// Number of threads < this number, increment is x2.
const size_t SKIPLIST_MAX_THREADS = 128 * 2;

/**
 * Create a @c OrderedStructs::SkipList::HeadNode<double> then create the given number of threads each of which calls
 * insert_count_has_remove_count() with the given count.
 *
 * This then reports the total time taken in us and the rate which is <tt>thread_count * count / total time</tt>
 *
 * Example output where the count is fixed at @ref SKIPLIST_FIXED_LENGTH :
 *
 * @code
    test_perf_insert_count_has_remove_count_multi_threads_fixed_length(): threads:    1 SkiplistSize:    16384 time:         8673 (us) rate    1.889e+06 /s
    test_perf_insert_count_has_remove_count_multi_threads_fixed_length(): threads:    2 SkiplistSize:    16384 time:        71462 (us) rate    4.585e+05 /s
    test_perf_insert_count_has_remove_count_multi_threads_fixed_length(): threads:    4 SkiplistSize:    16384 time:       687167 (us) rate    9.537e+04 /s
    test_perf_insert_count_has_remove_count_multi_threads_fixed_length(): threads:    8 SkiplistSize:    16384 time:      2328498 (us) rate    5.629e+04 /s
    test_perf_insert_count_has_remove_count_multi_threads_fixed_length(): threads:   16 SkiplistSize:    16384 time:      2729010 (us) rate    9.606e+04 /s
    test_perf_insert_count_has_remove_count_multi_threads_fixed_length(): threads:   32 SkiplistSize:    16384 time:     10644947 (us) rate    4.925e+04 /s
    test_perf_insert_count_has_remove_count_multi_threads_fixed_length(): threads:   64 SkiplistSize:    16384 time:     15568317 (us) rate    6.735e+04 /s
 * @endcode
 *
 * This shows that whilst the total time increases strongly with the number of threads the rate/thread approaches an
 * asymptotic value, although one that is significantly lower than the single threaded value.
 *
 * @param caller_name Name of the caller function, the test name.
 * @param thread_count Number of threads to create.
 * @param count Number of insertions into the Skip List.
 * @return -1 if compiled without thread support. Otherwise 0 on success, non-zero on failure.
 */
static int _test_perf_insert_count_has_remove_count_multi_threads(
#ifdef SKIPLIST_THREAD_SUPPORT
        const char *caller_name,
        size_t thread_count,
        size_t count
#else
        const char* /* Unused */,
        size_t /* Unused */,
        size_t /* Unused */
#endif
) {
#ifdef SKIPLIST_THREAD_SUPPORT
    int result = 0;
    OrderedStructs::SkipList::HeadNode<double> sl;
    std::vector<std::thread> threads;

    time_t start = clock();
    for (size_t i = 0; i < thread_count; ++i) {
        threads.push_back(std::thread(insert_count_has_remove_count<double>, &sl, i, count));
    }
    for (auto &t: threads) {
        t.join();
    }
    double exec = 1e6 * (clock() - start) / (double) CLOCKS_PER_SEC;
    uint32_t exec_us = exec + 0.5;
    result |= sl.lacksIntegrity();
    result |= sl.size() != 0;
    std::cout << std::setw(FUNCTION_WIDTH) << caller_name << "():";
    std::cout << " threads: " << std::setw(4) << thread_count;
    std::cout << " SkiplistSize: " << std::setw(8) << count;
    std::cout << " time: ";
    std::cout << std::setw(12) << exec_us;
    std::cout << " (us)";
    std::cout << " rate " << std::setw(12);
    std::cout << thread_count * count / (exec / 1e6) << " /s";
    std::cout << std::endl;
    return result;
#endif // SKIPLIST_THREAD_SUPPORT
    return -1; // N/A
}

#ifndef DEBUG

/**
 * Test insert/at/remove on a single Skip List in a multi-threaded environment.
 * Number of threads is from 1 to < SKIPLIST_MAX_THREADS
 *
 * The repeat count is adjusted for the number of threads to be:
 * @code SKIPLIST_FIXED_LENGTH * 4 / number_of_threads @endcode
 *
 * This calls _test_perf_insert_count_has_remove_count_multi_threads() that does most of the work.
 *
 * @return 0 on success, non-zero on failure.
 */
static int test_perf_insert_count_has_remove_count_multi_threads_vary_length() {
    int result = 0;

    for (size_t t = 1; t < SKIPLIST_MAX_THREADS; t *= 2) {
        result |= _test_perf_insert_count_has_remove_count_multi_threads(__FUNCTION__,
                                                                         t,
                                                                         SKIPLIST_FIXED_LENGTH / t);
    }
    return result;
}

#endif

/**
 * Test insert/at/remove on a single Skip List in a multi-threaded environment.
 * Number of threads is from 1 to < SKIPLIST_MAX_THREADS
 *
 * The repeat count is constant at SKIPLIST_FIXED_LENGTH so, inevitably the more threads the longer the execution time.
 *
 * This calls _test_perf_insert_count_has_remove_count_multi_threads() that does most of the work.
 *
 * @return 0 on success, non-zero on failure.
 */
int test_perf_insert_count_has_remove_count_multi_threads_fixed_length() {
    int result = 0;

    for (size_t t = 1; t < SKIPLIST_MAX_THREADS; t *= 2) {
        result |= _test_perf_insert_count_has_remove_count_multi_threads(__FUNCTION__,
                                                                         t,
                                                                         SKIPLIST_FIXED_LENGTH);
    }
    return result;
}

/**
 * Test insert/at/remove on a single Skip List in a single-threaded environment.
 *
 * The repeat count is constant at SKIPLIST_FIXED_LENGTH.
 *
 * This is comparable with test_perf_single_thread_fixed_length()
 *
 * @return 0 on success, non-zero on failure.
 */
int _test_perf_insert_count_has_remove_count_single_thread(const char *caller_name,
                                                           size_t repeat_count,
                                                           size_t count) {
    int result = 0;
    OrderedStructs::SkipList::HeadNode<double> sl;

    time_t start = clock();
    for (size_t i = 0; i < repeat_count; ++i) {
        insert_count_has_remove_count<double>(&sl, i, count);
    }
    double exec = 1e6 * (clock() - start) / (double) CLOCKS_PER_SEC;
    uint32_t exec_us = exec + 0.5;
    result |= sl.lacksIntegrity();
    result |= sl.size() != 0;
    std::cout << std::setw(FUNCTION_WIDTH) << caller_name << "():";
    std::cout << "   count: " << std::setw(4) << repeat_count;
    std::cout << " SkiplistSize: " << std::setw(8) << count;
    std::cout << " time: ";
    std::cout << std::setw(12) << exec_us;
    std::cout << " (us)";
    std::cout << " rate " << std::setw(12);
    std::cout << repeat_count * count / (exec / 1e6) << " /s";
    std::cout << std::endl;
    return result;
}

/**
 * Test insert/at/remove on a single Skip List in a single-threaded environment.
 *
 * The repeat count is constant at SKIPLIST_FIXED_LENGTH .
 *
 * This calls _test_perf_insert_count_has_remove_count_single_thread() that does most of the work.
 *
 * This is comparable with test_perf_insert_count_has_remove_count_multi_threads_fixed_length()
 *
 * @return 0 on success, non-zero on failure.
 */
int test_perf_single_thread_fixed_length() {
    int result = 0;

    for (size_t c = 1; c < SKIPLIST_MAX_THREADS; c *= 2) {
        result |= _test_perf_insert_count_has_remove_count_single_thread(__FUNCTION__,
                                                                         c,
                                                                         SKIPLIST_FIXED_LENGTH);
    }
    return result;
}

int test_perf_sim_rolling_median_single_thread() {
    int result = 0;
    OrderedStructs::SkipList::HeadNode<double> sl;
    size_t skip_list_size = 1000 * 1000;
    size_t repeat_count = 10000;
    size_t thread_count = 1;
    for (size_t i = 0; i < skip_list_size; ++i) {
        sl.insert(i);
    }

    time_t start = clock();
    insert_has_remove_count<double>(&sl, skip_list_size / 2.0, repeat_count);
    double exec_s = (clock() - start) / (double) CLOCKS_PER_SEC;
    uint32_t exec_us = 1e6 * exec_s + 0.5;

    result |= sl.lacksIntegrity();
    result |= sl.size() != skip_list_size;
    std::cout << std::setw(FUNCTION_WIDTH) << __FUNCTION__ << "():";
    std::cout << " threads: " << std::setw(4) << thread_count;
    std::cout << " SkiplistSize: " << std::setw(8) << skip_list_size;
    std::cout << " repeat_count: " << std::setw(8) << repeat_count;
    std::cout << " total time: ";
    std::cout << std::setw(12) << exec_us;
    std::cout << " (us)";
    std::cout << " per op: " << std::setw(12);
    std::cout << exec_s * 1e6 / (thread_count * repeat_count) << " (us)";
    std::cout << " rate " << std::setw(12);
    std::cout << thread_count * repeat_count / exec_s << " /s";
    std::cout << std::endl;
    return result;
}

#ifndef DEBUG

static int _test_perf_sim_rolling_median_multi_thread(size_t thread_count) {
#ifdef SKIPLIST_THREAD_SUPPORT
    int result = 0;
    // Create and load a Skip List
    OrderedStructs::SkipList::HeadNode<double> sl;
    size_t skip_list_size = 1000 * 1000;
    size_t repeat_count = 10000;
    for (size_t i = 0; i < skip_list_size; ++i) {
        sl.insert(i);
    }
    // Set up the threads
    std::vector<std::thread> threads;

    time_t start = clock();
    for (size_t i = 0; i < thread_count; ++i) {
        threads.push_back(std::thread(insert_has_remove_count<double>, &sl, skip_list_size / 2.0, repeat_count));
    }
    for (auto &t: threads) {
        t.join();
    }
    double exec_s = (clock() - start) / (double) CLOCKS_PER_SEC;
    uint32_t exec_us = 1e6 * exec_s + 0.5;
    result |= sl.lacksIntegrity();
    result |= sl.size() != skip_list_size;
    std::cout << std::setw(FUNCTION_WIDTH) << __FUNCTION__ << "():";
    std::cout << " threads: " << std::setw(4) << thread_count;
    std::cout << " SkiplistSize: " << std::setw(8) << skip_list_size;
    std::cout << " repeat_count: " << std::setw(8) << repeat_count;
    std::cout << " total time: ";
    std::cout << std::setw(12) << exec_us;
    std::cout << " (us)";
    std::cout << " per op: " << std::setw(12);
    std::cout << exec_s * 1e6 / (thread_count * repeat_count) << " (us)";
    std::cout << " rate " << std::setw(12);
    std::cout << thread_count * repeat_count / exec_s << " /s";
    std::cout << std::endl;
    return result;
#endif // SKIPLIST_THREAD_SUPPORT
    return -1; // N/A
}

#endif

#ifndef DEBUG

static int test_perf_sim_rolling_median_multi_thread() {
#ifdef SKIPLIST_THREAD_SUPPORT
    int result = 0;
    for (size_t num_threads = 1; num_threads < SKIPLIST_MAX_THREADS; num_threads *= 2) {
        result |= _test_perf_sim_rolling_median_multi_thread(num_threads);
    }
    return result;
#endif // SKIPLIST_THREAD_SUPPORT
    return -1; // N/A
}

#endif

static int test_perf_sim_rolling_median_multi_thread_multi_no_thread(
        size_t test_count, size_t repeat, TestResultS &test_results
) {
    int result = 0;

    std::ostringstream title;
    title << __FUNCTION__ << "[" << 0 << "]";
    TestResult test_result(title.str());

    for (size_t r = 0; r < repeat; ++r) {
        // Create and populate a SkipList of 1m doubles.
        OrderedStructs::SkipList::HeadNode<double> sl;
        size_t skip_list_size = 1000 * 1000;
        for (size_t i = 0; i < skip_list_size; ++i) {
            sl.insert(i);
        }
        // Set up the threads
        ExecClock exec_clock;
        insert_has_remove_count<double>(&sl, skip_list_size / 2.0, test_count);
        double exec_time = exec_clock.seconds();
        if (r == 0) {
            std::cout << __FUNCTION__ << "[" << 0 << "] Sample time/op = " << 1e6 * exec_time / test_count
                      << "(us)" << std::endl;
        }
        test_result.execTimeAdd(0, exec_time, test_count, 0);
//        result |= sl.lacksIntegrity();
        result |= sl.size() != skip_list_size;
    }
    test_results.push_back(test_result);
    return result;
}

// These tests accumulate statistics in a TestResultS object
#ifndef DEBUG

static int _test_perf_sim_rolling_median_multi_thread_multi(
        std::string function, size_t thread_count, size_t test_count, size_t repeat, TestResultS &test_results
) {
#ifdef SKIPLIST_THREAD_SUPPORT
    int result = 0;

    std::ostringstream title;
    title << function << "[" << thread_count << "]";
    TestResult test_result(title.str());

    for (size_t r = 0; r < repeat; ++r) {
        // Create and populate a SkipList of 1m doubles.
        OrderedStructs::SkipList::HeadNode<double> sl;
        size_t skip_list_size = 1000 * 1000;
        for (size_t i = 0; i < skip_list_size; ++i) {
            sl.insert(i);
        }
        // Set up the threads
        std::vector<std::thread> threads;
        ExecClock exec_clock;
        for (size_t t = 0; t < thread_count; ++t) {
            // Value is mid point of SkipList plus thread_count so the value is unique to the thread.
            threads.push_back(
                    std::thread(insert_has_remove_count<double>, &sl, skip_list_size / 2.0 + thread_count, test_count)
            );
//            std::cout << "Repeat " << r << " created thread ID: " << threads[threads.size() - 1].get_id() << std::endl;
        }
        for (auto &t: threads) {
//            std::cout << "Repeat " << r << " joining thread ID: " << t.get_id() << std::endl;
            t.join();
        }
        double exec_time = exec_clock.seconds();
        if (r == 0) {
            std::cout << function << "[" << thread_count << "] Sample time/op = "
                      << 1e6 * exec_time / test_count / thread_count
                      << "(us)" << std::endl;
        }
        test_result.execTimeAdd(0, exec_time, test_count * thread_count, thread_count);
//        result |= sl.lacksIntegrity();
        result |= sl.size() != skip_list_size;
    }
    test_results.push_back(test_result);
    return result;
#endif // SKIPLIST_THREAD_SUPPORT
    return -1; // N/A
}

#endif

#ifndef DEBUG

static int test_perf_sim_rolling_median_multi_thread_multi(
        size_t test_count, size_t repeat, TestResultS &test_results
) {
#ifdef SKIPLIST_THREAD_SUPPORT
    int result = 0;
//    for (size_t num_threads = 2; num_threads < 4; num_threads *= 2) {
    for (size_t num_threads = 1; num_threads < SKIPLIST_MAX_THREADS; num_threads *= 2) {
        result |= _test_perf_sim_rolling_median_multi_thread_multi(
                __FUNCTION__, num_threads, test_count, repeat, test_results
        );
    }
    return result;
#endif // SKIPLIST_THREAD_SUPPORT
    return -1; // N/A
}

#endif

/***************** END: Concurrency Tests ************************/

/**
 * Run all concurrency tests.
 *
 * @return 0 on success, non-zero on failure.
 */
int test_concurrent_all() {
    int result = 0;
#if 1
    result |= print_result("test_single_thread_insert",
                           test_single_thread_insert());
    result |= print_result("test_two_thread_insert_has_remove",
                           test_two_thread_insert_has_remove());
    result |= print_result("test_two_thread_insert_count_has_remove_count",
                           test_two_thread_insert_count_has_remove_count());
#endif
    // Performance tests are very slow if DEBUG as checking
    // integrity is very expensive for large data sets.
#ifndef DEBUG
#if 1
    result |= print_result("test_perf_insert_count_has_remove_count_multi_threads_vary_length",
                           test_perf_insert_count_has_remove_count_multi_threads_vary_length());
    result |= print_result("test_perf_insert_count_has_remove_count_multi_threads_fixed_length",
                           test_perf_insert_count_has_remove_count_multi_threads_fixed_length());
    result |= print_result("test_perf_single_thread_fixed_length",
                           test_perf_single_thread_fixed_length());
    result |= print_result("test_perf_sim_rolling_median_single_thread",
                           test_perf_sim_rolling_median_single_thread());
    result |= print_result("test_perf_sim_rolling_median_multi_thread",
                           test_perf_sim_rolling_median_multi_thread());
#endif
#endif // DEBUG
#ifndef DEBUG
    // Multiple statistical tests
    TestResultS perf_test_results;
    result |= test_perf_sim_rolling_median_multi_thread_multi_no_thread(20, 10, perf_test_results);
    result |= test_perf_sim_rolling_median_multi_thread_multi(20, 10, perf_test_results);
    perf_test_results.dump_header(std::cout);
    perf_test_results.dump_tests(std::cout);
    perf_test_results.dump_tail(std::cout);

#endif // DEBUG
    return result;
}
