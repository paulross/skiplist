//
//  test_rolling_median.cpp
//  skiplist
//
//  Created by Paul Ross on 13/07/2016.
//  Copyright (c) 2016 Paul Ross. All rights reserved.
//
#include <iostream>
#include <iomanip>

#include "test_rolling_median.h"
#include "test_print.h"

#include "TestFramework.h"

#include "RollingMedian.h"

/******************* Rolling Median Tests ****************************/

/**
 * @brief Test a simple rolling median into a dynamic array of doubles given by
 * \c OrderedStructs::RollingMedian::dest_size.
 *
 * @return Zero on success, non-zero on failure.
 */
int test_roll_med_simple() {
    const int COUNT = 20;
    const int WIN_LENGTH = 5;
    const int DEST_STRIDE = 1;
    double src[COUNT];
    double *dest = new double[OrderedStructs::RollingMedian::dest_size(COUNT,
                                                               WIN_LENGTH,
                                                               DEST_STRIDE)];
    int result = 0;
    
    for (int i = 0; i < COUNT; ++i) {
        src[i] = 2.0 * i;
    }
    // Set
    result |= OrderedStructs::RollingMedian::odd_index(src, 1, COUNT,
                                               WIN_LENGTH, dest, DEST_STRIDE);
    // Test
    for (size_t i = 0;
         i < OrderedStructs::RollingMedian::dest_size(COUNT, WIN_LENGTH, DEST_STRIDE);
         i += DEST_STRIDE) {
        //        std::cout << i << " " << dest[i] << std::endl;
        result |= dest[i] != 4. + 2.0 * i;
    }
    delete [] dest;
    return result;
}

/**
 * @brief Test a simple rolling median into a dynamic array of doubles given by
 * \c OrderedStructs::RollingMedian::dest_size. Even length window.
 *
 * @return Zero on success, non-zero on failure.
 */
int test_roll_med_even_win() {
    const int COUNT = 20;
    const int WIN_LENGTH = 4;
    const int DEST_STRIDE = 1;
    double src[COUNT];
    double *dest = new double[OrderedStructs::RollingMedian::dest_size(COUNT,
                                                               WIN_LENGTH,
                                                               DEST_STRIDE)];
    int result = 0;
    
    for (int i = 0; i < COUNT; ++i) {
        src[i] = 2.0 * i;
    }
    // Set
    result |= OrderedStructs::RollingMedian::odd_index(src, 1, COUNT,
                                               WIN_LENGTH, dest, DEST_STRIDE);
    // Test
    for (size_t i = 0;
         i < OrderedStructs::RollingMedian::dest_size(COUNT, WIN_LENGTH, DEST_STRIDE);
         i += DEST_STRIDE) {
        //        std::cout << i << " " << dest[i] << std::endl;
        result |= dest[i] != 4. + 2.0 * i;
    }
    delete [] dest;
    return result;
}

/**
 * @brief Test a simple rolling median into a dynamic array of doubles given by
 * \c OrderedStructs::RollingMedian::dest_size. Even length window.
 *
 * @return Zero on success, non-zero on failure.
 */
int test_roll_med_even_mean() {
    const int COUNT = 20;
    const int WIN_LENGTH = 4;
    const int DEST_STRIDE = 1;
    double src[COUNT];
    double *dest = new double[OrderedStructs::RollingMedian::dest_size(COUNT,
                                                               WIN_LENGTH,
                                                               DEST_STRIDE)];
    int result = 0;
    
    for (int i = 0; i < COUNT; ++i) {
        src[i] = 2.0 * i;
    }
    // Set
    result |= OrderedStructs::RollingMedian::even_odd_index(src, 1, COUNT,
                                                    WIN_LENGTH, dest, DEST_STRIDE);
    for (size_t i = 0;
         i < OrderedStructs::RollingMedian::dest_size(COUNT, WIN_LENGTH, DEST_STRIDE);
         i += DEST_STRIDE) {
        //        std::cout << i << " " << dest[i] << std::endl;
        result |= dest[i] != 3. + 2.0 * i;
    }
    delete [] dest;
    return result;
}

/**
 * @brief Test a simple rolling median into a dynamic array of doubles given by
 * \c OrderedStructs::RollingMedian::dest_size. Odd length window.
 *
 * @return Zero on success, non-zero on failure.
 */
int perf_roll_med_odd_index() {
    const int COUNT = 1000000;
    const int WIN_LENGTH = 101;
    const int DEST_STRIDE = 1;
    double src[COUNT];
    double *dest = new double[OrderedStructs::RollingMedian::dest_size(COUNT,
                                                               WIN_LENGTH,
                                                               DEST_STRIDE)];
    int result = 0;
    
    for (int i = 0; i < COUNT; ++i) {
        src[i] = 2.0 * i;
    }
    srand(1);
    time_t start = clock();
    result |= OrderedStructs::RollingMedian::odd_index(src, 1, COUNT,
                                               WIN_LENGTH, dest, DEST_STRIDE);
    double exec = 1e6 * (clock() - start) / (double) CLOCKS_PER_SEC;
    std::cout << std::setw(FUNCTION_WIDTH) << __FUNCTION__ << "():";
    std::cout << " vectors length: " << std::setw(8) << COUNT;
    std::cout << " window width: " << std::setw(6) << WIN_LENGTH;
    std::cout << " time: ";
    std::cout << exec / 1e6;
    std::cout << " (s)";
    std::cout << std::endl;

    delete [] dest;
    return result;
}

/**
 * @brief Test the performance of a simple rolling median into a dynamic array of doubles given by
 * \c OrderedStructs::RollingMedian::dest_size. Odd length window.
 *
 * @return Zero on success, non-zero on failure.
 */
int perf_roll_med_odd_index_wins(TestResultS &test_results) {
    const size_t COUNT = 1000000;
    const int REPEAT = 5;
    const int DEST_STRIDE = 1;
    double *src = new double[COUNT];
    int result = 0;

    std::ostringstream title;
    title << __FUNCTION__ << "[" << COUNT << "]";
    TestResult test_result(title.str());
//    std::cout << "Running: " << title.str() << std::endl;

    // Create source data
    for (size_t i = 0; i < COUNT; ++i) {
        src[i] = 2.0 * i;
    }
    // Loop over this data for various window sizes from 1 to 524288
    for (size_t win = 1; win < COUNT; win *= 2) {
        for (int i = 0; i < REPEAT; ++i) {
            size_t dest_size = OrderedStructs::RollingMedian::dest_size(COUNT, win, DEST_STRIDE);
            double *dest = new double[dest_size];
            srand(1);
//            time_t start = clock();
            ExecClock exec_clock;
            result |= OrderedStructs::RollingMedian::odd_index(src, 1, COUNT,
                                                               win, dest, DEST_STRIDE);
//            double exec = 1e6 * (clock() - start) / (double) CLOCKS_PER_SEC;
            double exec_time = exec_clock.seconds();
//            std::cout << std::setw(FUNCTION_WIDTH) << __FUNCTION__ << "():";
//            std::cout << " vectors length: " << std::setw(8) << COUNT;
//            std::cout << " window width: " << std::setw(6) << win;
//            std::cout << " time: ";
//            std::cout << exec_time * 1e3;
//            std::cout << " (ms)";
//            std::cout << std::endl;
            delete[] dest;
            test_result.execTimeAdd(0, exec_time, 1, win);
        }
    }
    delete [] src;
    test_results.push_back(test_result);
    return result;
}

/**
 * @brief Test the performance of a simple rolling median using std::vector<double>.
 * Even length window.
 *
 * @return Zero on success, non-zero on failure.
 */
int perf_roll_med_vector_style_even_win_length(TestResultS &test_results) {
    const size_t COUNT = 1000000;
    const int REPEAT = 5;
    std::vector<double> src;
    std::vector<double> dest;
    int result = 0;

    std::ostringstream title;
    title << __FUNCTION__ << "[" << COUNT << "]";
    TestResult test_result(title.str());
//    std::cout << "Running: " << title.str() << std::endl;

    // Create source data
    for (size_t i = 0; i < COUNT; ++i) {
        src.push_back(2.0 * i);
    }
    // Loop over this data for various window sizes from 2 to 524288
    for (size_t win = 2; win < COUNT; win *= 2) {
        for (int i = 0; i < REPEAT; ++i) {
            srand(1);
            //        time_t start = clock();
            ExecClock exec_clock;
            result |= OrderedStructs::RollingMedian::rolling_median(src, win, dest);
            //        double exec = 1e6 * (clock() - start) / (double) CLOCKS_PER_SEC;
            double exec_time = exec_clock.seconds();
//            std::cout << std::setw(FUNCTION_WIDTH) << __FUNCTION__ << "():";
//            std::cout << " vectors length: " << std::setw(8) << COUNT;
//            std::cout << " window width: " << std::setw(6) << win;
//            std::cout << " time: ";
//            std::cout << exec_time * 1e3;
//            std::cout << " (ms)";
//            std::cout << std::endl;
            test_result.execTimeAdd(0, exec_time, 1, win);
        }
    }
    test_results.push_back(test_result);
    return result;
}

/**
 * @brief Test the performance of a simple rolling median using std::vector<double>.
 * Odd length window.
 *
 * @return Zero on success, non-zero on failure.
 */
int perf_roll_med_vector_style_odd_win_length(TestResultS &test_results) {
    const size_t COUNT = 1000000;
    const int REPEAT = 5;
    std::vector<double> src;
    std::vector<double> dest;
    int result = 0;

    std::ostringstream title;
    title << __FUNCTION__ << "[" << COUNT << "]";
    TestResult test_result(title.str());
//    std::cout << "Running: " << title.str() << std::endl;

    // Create source data
    for (size_t i = 0; i < COUNT; ++i) {
        src.push_back(2.0 * i);
    }
    // Loop over this data for various window sizes from 2 to 524288
    for (size_t win = 2; win < COUNT; win *= 2) {
        for (int i = 0; i < REPEAT; ++i) {
            srand(1);
//            time_t start = clock();
            ExecClock exec_clock;
            result |= OrderedStructs::RollingMedian::rolling_median(src, win + 1, dest);
//            double exec = 1e6 * (clock() - start) / (double) CLOCKS_PER_SEC;
            double exec_time = exec_clock.seconds();
//            std::cout << std::setw(FUNCTION_WIDTH) << __FUNCTION__ << "():";
//            std::cout << " vectors length: " << std::setw(8) << COUNT;
//            std::cout << " window width: " << std::setw(6) << win;
//            std::cout << " time: ";
//            std::cout << exec_time * 1e3;
//            std::cout << " (ms)";
//            std::cout << std::endl;
            test_result.execTimeAdd(0, exec_time, 1, win);
        }
    }
    test_results.push_back(test_result);
    return result;
}

/**
 * @brief Test the performance of a simple rolling median using std::vector<std::string>.
 * Even length window.
 *
 * @return Zero on success, non-zero on failure.
 */
int perf_roll_med_vector_style_even_win_length_string(TestResultS &test_results) {
    const size_t COUNT = 1000000 / 10;
    int REPEAT = 5;
    std::vector<std::string> src;
    std::vector<std::string> dest;
    int result = 0;

    std::ostringstream title;
    title << __FUNCTION__ << "[" << COUNT << "]";
    TestResult test_result(title.str());
//    std::cout << "Running: " << title.str() << std::endl;

    // Create source data
    for (size_t i = 0; i < COUNT; ++i) {
        src.push_back(random_string(1024));
    }
    // Loop over this data for various window sizes from 2 to 524288
    for (size_t win = 2; win < COUNT; win *= 2) {
        for (int i = 0; i < REPEAT; ++i) {
            srand(1);
//        time_t start = clock();
            ExecClock exec_clock;
            result |= OrderedStructs::RollingMedian::rolling_median_lower_bound(src, win, dest);
//        double exec = 1e6 * (clock() - start) / (double) CLOCKS_PER_SEC;
            double exec_time = exec_clock.seconds();
//            std::cout << std::setw(FUNCTION_WIDTH) << __FUNCTION__ << "():";
//            std::cout << " vectors length: " << std::setw(8) << COUNT;
//            std::cout << " window width: " << std::setw(6) << win;
//            std::cout << " time: ";
//            std::cout << exec_time * 1e3;
//            std::cout << " (ms)";
//            std::cout << std::endl;
            test_result.execTimeAdd(0, exec_time, 1, win);
        }
    }
    test_results.push_back(test_result);
    return result;
}

/******************* END: Rolling Median Tests ************************/

/**
 * @brief Test all rolling median tests.
 *
 * @return Zero on success, non-zero on failure.
 */
int test_rolling_median_all() {
    int result = 0;

    TestResultS perf_test_results;
    
    result |= print_result("test_roll_med_simple", test_roll_med_simple());
    result |= print_result("test_roll_med_even_win", test_roll_med_even_win());
    result |= print_result("test_roll_med_even_mean", test_roll_med_even_mean());
    // Performance tests are very slow if DEBUG as checking
    // integrity is very expensive for large data sets.
#ifndef DEBUG
    result |= print_result("perf_roll_med_odd_index", perf_roll_med_odd_index());
    result |= print_result(
            "perf_roll_med_odd_index_wins",
            perf_roll_med_odd_index_wins(perf_test_results)
            );
    result |= print_result(
            "perf_roll_med_vector_style_even_win_length",
            perf_roll_med_vector_style_even_win_length(perf_test_results)
            );
    result |= print_result(
            "perf_roll_med_vector_style_odd_win_length",
            perf_roll_med_vector_style_odd_win_length(perf_test_results)
            );
    result |= print_result(
            "perf_roll_med_vector_style_even_win_length_string",
            perf_roll_med_vector_style_even_win_length_string(perf_test_results)
            );
#endif
    perf_test_results.dump_header(std::cout);
    perf_test_results.dump_tests(std::cout);
    perf_test_results.dump_tail(std::cout);

    return result;
}
