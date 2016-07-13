//
//  main.cpp
//  SkipList
//
//  Created by Paul Ross on 15/11/2015.
//  Copyright (c) 2015 AHL. All rights reserved.
//
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>

//#include "SkipList.h"
//#include "RollingMedian.h"

#include "test/test_documentation.h"
#include "test/test_functional.h"
#include "test/test_performance.h"
#include "test/test_rolling_median.h"

void test_clock_resolution() {
    int count = 10;
    double average_ticks = 0;
    for (int i = 0; i < count; ++i) {
        clock_t start = clock();
        while (clock() == start) {}
        clock_t diff = clock() - start;
        average_ticks += diff;
    }
    std::cout << "Average ticks (" << count;
    std::cout << " tests) for change in clock(): " << average_ticks / count;
    std::cout << " which is every " << average_ticks / count / CLOCKS_PER_SEC;
    std::cout << " (s)" << std::endl;
    std::cout << "CLOCKS_PER_SEC: " << CLOCKS_PER_SEC << std::endl;
}

int test_all() {
    int result = 0;
    
    result |= test_functional_all();
    result |= test_rolling_median_all();
    result |= test_documentation_all();
    // Performance tests are very slow if DEBUG as checking
    // integrity is very expensive for large data sets.
#ifndef DEBUG
    result |= test_performance_all();
#endif
    return result;
}


int main(int /* argc */, const char *[] /* argv[] */) {
    int result = 0;

    std::cout << "Running skip list tests..." << std::endl;

    time_t start = clock();
    result = test_all();
    double exec = (clock() - start) / (double) CLOCKS_PER_SEC;

    std::cout << "__cplusplus: \""<< __cplusplus << "\"" << std::endl;
    std::cout << "Final result: ";
    std::cout << (result ? "FAIL" : "PASS") << std::endl;
    std::cout << "Exec time: " << exec << " (s)" << std::endl;
    test_clock_resolution();
    std::cout << "Bye, bye!\n";
    return 0;
}
