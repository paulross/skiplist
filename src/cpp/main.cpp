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
#include <ctime>

#include "SkipList.h"
#include "RollingMedian.h"

#define DUMP_DOT_ON_FAILURE 1
const int FUNCTION_WIDTH = 40;

/******************* Functional Tests **************************/

ManAHL::SkipList::IntegrityCheck test_very_simple_insert() {
    ManAHL::SkipList::IntegrityCheck result;
    std::stringstream ostr;
    ostr << "# " << __FUNCTION__ << std::endl;
    
    srand(1);
    ManAHL::SkipList::HeadNode<double> sl;
    sl.insert(42.0);
    result = sl.lacksIntegrity();
    sl.dotFile(ostr, 0);
    sl.dotFileFinalise(ostr, 1);
    if (result && DUMP_DOT_ON_FAILURE) {
        std::cout << ostr.str() << std::endl;
    }
    return result;
}

// Need int as ManAHL::SkipList::IntegrityCheck can not use |=
int test_simple_insert() {
    int result = 0;
    std::stringstream ostr;
    ostr << "# " << __FUNCTION__ << std::endl;
    
    srand(1);
    ManAHL::SkipList::HeadNode<double> sl;
    sl.insert(42.0);
    result |= sl.lacksIntegrity();
    sl.dotFile(ostr, 0);
    sl.insert(84.0);
    result |= sl.lacksIntegrity();
    sl.dotFile(ostr, 1);
    sl.insert(21.0);
    result |= sl.lacksIntegrity();
    sl.dotFile(ostr, 2);
    sl.insert(100.0);
    result |= sl.lacksIntegrity();
    sl.dotFile(ostr, 3);
    result |= sl.lacksIntegrity();
    sl.insert(12.0);
    result |= sl.lacksIntegrity();
    sl.dotFile(ostr, 4);
    sl.dotFileFinalise(ostr, 5);
    if (result && DUMP_DOT_ON_FAILURE) {
        std::cout << ostr.str() << std::endl;
    }
    return result;
}

int test_insert_and_remove_same() {
    int result = 0;
    std::stringstream ostr;
    ostr << "# " << __FUNCTION__ << std::endl;
    
    srand(1);
    ManAHL::SkipList::HeadNode<double> sl;
    sl.insert(42.0);
    result |= sl.lacksIntegrity();
    sl.dotFile(ostr, 0);
    sl.remove(42.0);
    result |= sl.lacksIntegrity();
    sl.dotFile(ostr, 1);
    sl.dotFileFinalise(ostr, 2);
    if (result && DUMP_DOT_ON_FAILURE) {
        std::cout << ostr.str() << std::endl;
    }
    return result;
}

int test_insert_remove_multiple() {
    int result = 0;
    std::stringstream ostr;
    ostr << "# " << __FUNCTION__ << std::endl;
    
    srand(1);
    ManAHL::SkipList::HeadNode<double> sl;
    sl.insert(42.0);
    result |= sl.lacksIntegrity();
    sl.dotFile(ostr, 0);
    sl.insert(84.0);
    result |= sl.lacksIntegrity();
    sl.dotFile(ostr, 1);
    sl.remove(42.0);
    result |= sl.lacksIntegrity();
    sl.dotFile(ostr, 2);
    sl.insert(21.0);
    result |= sl.lacksIntegrity();
    sl.dotFile(ostr, 3);
    sl.remove(84.0);
    result |= sl.lacksIntegrity();
    sl.dotFile(ostr, 4);
    sl.insert(100.0);
    result |= sl.lacksIntegrity();
    sl.dotFile(ostr, 5);
    sl.insert(12.0);
    result |= sl.lacksIntegrity();
    sl.dotFile(ostr, 6);
    sl.remove(21.0);
    result |= sl.lacksIntegrity();
    sl.dotFile(ostr, 7);
    sl.remove(12.0);
    result |= sl.lacksIntegrity();
    sl.dotFile(ostr, 8);
    sl.remove(100.0);
    result |= sl.lacksIntegrity();
    sl.dotFile(ostr, 9);
    sl.dotFileFinalise(ostr, 10);
    if (result && DUMP_DOT_ON_FAILURE) {
        std::cout << ostr.str() << std::endl;
    }
    return result;
}

/* For many different seeds insert 128 random values then remove them
 * in reverse order.
 */
int test_ins_rem_rand() {
    int result = 0;
    std::stringstream ostr;
    ostr << "# " << __FUNCTION__ << std::endl;
    int SEED  = 128 + 1;
    int LENGTH = 128;
    
    for (int seed = 1; seed < SEED; ++seed) {
        srand(seed);
        ManAHL::SkipList::HeadNode<unsigned int> sl;
        std::vector<unsigned int> values;
        unsigned int value;
        for (int i = 0; i < LENGTH; ++i) {
            value = rand() % LENGTH;
            values.push_back(value);
            sl.insert(value);
            result |= sl.lacksIntegrity();
            if (result) {
                sl.dotFile(ostr, 0);
                sl.dotFileFinalise(ostr, 1);
                break;
            }
        }
        sl.dotFile(ostr, 0);
        while (values.size()) {
            value = values.back();
            ostr << "# Removing value " << value << std::endl;
            sl.remove(value);
            values.pop_back();
            result |= sl.lacksIntegrity();
            if (result) {
                sl.dotFile(ostr, 0);
                sl.dotFileFinalise(ostr, 1);
                break;
            }
        }
        if (result) {
            break;
        }
    }
    if (result && DUMP_DOT_ON_FAILURE) {
        std::cout << ostr.str() << " FAIL" << std::endl;
    }
    return result;
}

int test_insert_n_numbers_same(int n, double value) {
    int result = 0;
    std::stringstream ostr;
    ostr << "# " << __FUNCTION__ << std::endl;
    
    srand(1);
    ManAHL::SkipList::HeadNode<double> sl;
    for (int i = 0; i < n; ++i) {
        sl.insert(value);
        result |= sl.lacksIntegrity();
        sl.dotFile(ostr, i);
//        assert(! sl.lacksIntegrity());
    }
    for (int i = 0; i < n; ++i) {
        sl.remove(value);
        result |= sl.lacksIntegrity();
        sl.dotFile(ostr, i + n);
    }
    
    sl.dotFileFinalise(ostr, 2 * n);
    if (result && DUMP_DOT_ON_FAILURE) {
        std::cout << ostr.str() << std::endl;
    }
    return result;
}

int test_at() {
    int result = 0;
    std::stringstream ostr;
    ostr << "# " << __FUNCTION__ << std::endl;
    
    srand(1);
    ManAHL::SkipList::HeadNode<double> sl;
    result |= sl.lacksIntegrity();
    sl.dotFile(ostr, 0);
    sl.insert(42.0);
    sl.dotFile(ostr, 1);
    result |= sl.lacksIntegrity();
    sl.insert(84.0);
    sl.dotFile(ostr, 2);
    result |= sl.lacksIntegrity();
    sl.insert(21.0);
    sl.dotFile(ostr, 3);
    result |= sl.lacksIntegrity();
    sl.insert(100.0);
    sl.dotFile(ostr, 4);
    result |= sl.lacksIntegrity();
    sl.insert(12.0);
    sl.dotFile(ostr, 5);
    result |= sl.lacksIntegrity();
    sl.dotFileFinalise(ostr, 6);
    // Test at()
    result |= sl.at(0) != 12.0;
    result |= sl.at(1) != 21.0;
    result |= sl.at(2) != 42.0;
    result |= sl.at(3) != 84.0;
    result |= sl.at(4) != 100.0;
    
    if (result && DUMP_DOT_ON_FAILURE) {
        std::cout << ostr.str() << " FAIL" << std::endl;
    }
    return result;
}

int test_at_fails() {
    int result = 0;
    
    srand(1);
    ManAHL::SkipList::HeadNode<double> sl;
    result |= sl.lacksIntegrity();
    sl.insert(42.0);
    result |= sl.lacksIntegrity();
    sl.insert(84.0);
    result |= sl.lacksIntegrity();
    sl.insert(21.0);
    result |= sl.lacksIntegrity();
    sl.insert(100.0);
    result |= sl.lacksIntegrity();
    sl.insert(12.0);
    result |= sl.lacksIntegrity();
    try {
        sl.at(-1);
        result |= 1;
    } catch (ManAHL::SkipList::IndexError &err) {}
    try {
        sl.at(5);
        result |= 1;
    } catch (ManAHL::SkipList::IndexError &err) {}
    try {
        sl.at(6);
        result |= 1;
    } catch (ManAHL::SkipList::IndexError &err) {}
    return result;
}

// Test at(size_t index, size_t count, T *dest)
int test_at_dest() {
    int result = 0;
    std::stringstream ostr;
    ostr << "# " << __FUNCTION__ << std::endl;
    
    srand(1);
    ManAHL::SkipList::HeadNode<double> sl;
    std::vector<double> dest;
    
    result |= sl.lacksIntegrity();
    sl.insert(4.0);
    result |= sl.lacksIntegrity();
    sl.insert(8.0);
    result |= sl.lacksIntegrity();
    sl.at(0, 2, dest);
    
    result |= dest.size() != 2;
    result |= dest[0] != 4.0;
    result |= dest[1] != 8.0;
    if (result && DUMP_DOT_ON_FAILURE) {
        std::cout << ostr.str() << " FAIL" << std::endl;
    }
    return result;
}

// Test at(size_t index, size_t count, T *dest)
int test_at_dest_fails() {
    int result = 0;
    std::stringstream ostr;
    ostr << "# " << __FUNCTION__ << std::endl;
    
    srand(1);
    ManAHL::SkipList::HeadNode<double> sl;
    std::vector<double> dest;
    
    result |= sl.lacksIntegrity();
    sl.insert(4.0);
    result |= sl.lacksIntegrity();
    sl.insert(8.0);
    result |= sl.lacksIntegrity();
    
    try {
        sl.at(0, 3, dest);
        result |= 1;
    } catch (ManAHL::SkipList::IndexError &err) {}
    try {
        sl.at(1, 2, dest);
        result |= 1;
    } catch (ManAHL::SkipList::IndexError &err) {}
    try {
        sl.at(2, 1, dest);
        result |= 1;
    } catch (ManAHL::SkipList::IndexError &err) {}
    try {
        sl.at(3, 0, dest);
        result |= 1;
    } catch (ManAHL::SkipList::IndexError &err) {}
    
    if (result && DUMP_DOT_ON_FAILURE) {
        std::cout << ostr.str() << " FAIL" << std::endl;
    }
    return result;
}

int test_has() {
    int result = 0;
    std::stringstream ostr;
    ostr << "# " << __FUNCTION__ << std::endl;
    
    srand(1);
    ManAHL::SkipList::HeadNode<double> sl;
    result |= sl.lacksIntegrity();
    sl.dotFile(ostr, 0);
    sl.insert(42.0);
    sl.dotFile(ostr, 1);
    result |= sl.lacksIntegrity();
    sl.insert(84.0);
    sl.dotFile(ostr, 2);
    result |= sl.lacksIntegrity();
    sl.insert(21.0);
    sl.dotFile(ostr, 3);
    result |= sl.lacksIntegrity();
    sl.insert(100.0);
    sl.dotFile(ostr, 4);
    result |= sl.lacksIntegrity();
    sl.insert(12.0);
    sl.dotFile(ostr, 5);
    result |= sl.lacksIntegrity();
    sl.dotFileFinalise(ostr, 6);
    // Test at()
    result |= ! sl.has(12.0);
    result |= ! sl.has(21.0);
    result |= ! sl.has(42.0);
    result |= ! sl.has(84.0);
    result |= ! sl.has(100.0);
    
    //    for (size_t i = 0; i < sl.size(); ++i) {
    //        std::cout << i << " " << sl.at(i) << std::endl;
    //    }
    if (result && DUMP_DOT_ON_FAILURE) {
        std::cout << ostr.str() << " FAIL" << std::endl;
    }
    return result;
}

int test_has_not() {
    int result = 0;
    
    srand(1);
    ManAHL::SkipList::HeadNode<double> sl;
    result |= sl.has(-1);
    result |= sl.lacksIntegrity();
    sl.insert(42.0);
    result |= sl.lacksIntegrity();
    sl.insert(84.0);
    result |= sl.lacksIntegrity();
    sl.insert(21.0);
    result |= sl.lacksIntegrity();
    sl.insert(100.0);
    result |= sl.lacksIntegrity();
    sl.insert(12.0);
    result |= sl.lacksIntegrity();
    result |= sl.has(-1);
    return result;
}

int test_remove_fails() {
    int result = 0;
    ManAHL::SkipList::HeadNode<double> sl;
    
    srand(1);
    result |= sl.lacksIntegrity();
    sl.insert(42.0);
    try {
        sl.remove(84.0);
        result |= 1;
    } catch (ManAHL::SkipList::ValueError &err) {}
    try {
        sl.remove(21.0);
        result |= 1;
    } catch (ManAHL::SkipList::ValueError &err) {}
    return result;
}

int test_at_large() {
    int result = 0;
    std::stringstream ostr;
    ostr << "# " << __FUNCTION__ << std::endl;
    int SEED  = 128 + 1;
    int LENGTH = 128;
    
    for (int seed = 1; seed < SEED; ++seed) {
        srand(seed);
        ManAHL::SkipList::HeadNode<int> sl;
        for (int i = 0; i < LENGTH; ++i) {
            sl.insert(i * 2);
            result |= sl.lacksIntegrity();
        }
        for (int i = 0; i < LENGTH; ++i) {
            int val = sl.at(i);
            result |= val != i * 2;
        }
    }
    if (result && DUMP_DOT_ON_FAILURE) {
        std::cout << ostr.str() << " FAIL" << std::endl;
    }
    return result;
}

int test_ins_at_rem_with_srand() {
    int result = 0;
    std::stringstream ostr;
    ostr << "# " << __FUNCTION__ << std::endl;
    int SEED  = 128 + 1;
    int LENGTH = 128;
    
    for (int seed = 1; seed < SEED; ++seed) {
        srand(seed);
        ManAHL::SkipList::HeadNode<int> sl;
        for (int i = 0; i < LENGTH; ++i) {
            sl.insert(i * 2);
            result |= sl.lacksIntegrity();
        }
        for (int i = 0; i < LENGTH; ++i) {
            result |= sl.at(i) != i * 2;
            result |= sl.lacksIntegrity();
        }
        for (int i = 0; i < LENGTH; ++i) {
            sl.remove(i * 2);
            result |= sl.lacksIntegrity();
        }
    }
    if (result && DUMP_DOT_ON_FAILURE) {
        std::cout << ostr.str() << " FAIL" << std::endl;
    }
    return result;
}

/* Functional tests that correspond to performance tests. */
int test_single_insert_remove() {
    int num = 1000 * 1000;
    int result = 0;
    ManAHL::SkipList::HeadNode<double> sl;
    
    srand(1);
    result |= sl.lacksIntegrity();
    for (int i = 0; i < num; ++i) {
        sl.insert(42.0);
        result |= sl.lacksIntegrity();
        sl.remove(42.0);
        result |= sl.lacksIntegrity();
    }
    return result;
}

int test_single_ins_rem_middle() {
    int NUM = 1000; // * 1000;
    size_t SIZE = 1000;//24;
    int result = 0;
    double val = SIZE / 2;
    ManAHL::SkipList::HeadNode<double> sl;
    
    srand(1);
    result |= sl.lacksIntegrity();
    for (size_t i = 0; i < SIZE; ++i) {
        sl.insert(i);
        result |= sl.lacksIntegrity();
    }
    result |= sl.lacksIntegrity();
    for (int i = 0; i < NUM; ++i) {
        sl.insert(val);
        result = sl.lacksIntegrity();
        sl.remove(val);
        result |= sl.lacksIntegrity();
    }
    return result;
}

/* Insert 1m doubles for memory measurement. */
int test_insert_one_million() {
    int num = 1024 * 1024;
    int result = 0;
    ManAHL::SkipList::HeadNode<double> sl;
    
    srand(1);
    std::string in;
//    std::cout << "Ready? ";
//    std::cin >> in;
    result |= sl.lacksIntegrity();
    for (int i = 0; i < num; ++i) {
        sl.insert(42.0);
    }
//    std::cout << "Complete...";
//    std::cin >> in;
    return result;
}

/***************** END: Functional Tests ************************/

/******************* Rolling Median Tests ****************************/

int test_roll_med_simple() {
    const int COUNT = 20;
    const int WIN_LENGTH = 5;
    const int DEST_STRIDE = 1;
    double src[COUNT];
    double *dest = new double[ManAHL::RollingMedian::dest_size(COUNT,
            WIN_LENGTH,
            DEST_STRIDE)];
    int result = 0;
    
    for (int i = 0; i < COUNT; ++i) {
        src[i] = 2.0 * i;
    }
    // Set
    result |= ManAHL::RollingMedian::odd_index(src, 1, COUNT,
                                               WIN_LENGTH, dest, DEST_STRIDE);
    // Test
    for (size_t i = 0;
         i < ManAHL::RollingMedian::dest_size(COUNT, WIN_LENGTH, DEST_STRIDE);
         i += DEST_STRIDE) {
        //        std::cout << i << " " << dest[i] << std::endl;
        result |= dest[i] != 4. + 2.0 * i;
    }
    delete [] dest;
    return result;
}

int test_roll_med_even_win() {
    const int COUNT = 20;
    const int WIN_LENGTH = 4;
    const int DEST_STRIDE = 1;
    double src[COUNT];
    double *dest = new double[ManAHL::RollingMedian::dest_size(COUNT,
                                                               WIN_LENGTH,
                                                               DEST_STRIDE)];
    int result = 0;
    
    for (int i = 0; i < COUNT; ++i) {
        src[i] = 2.0 * i;
    }
    // Set
    result |= ManAHL::RollingMedian::odd_index(src, 1, COUNT,
                                               WIN_LENGTH, dest, DEST_STRIDE);
    // Test
    for (size_t i = 0;
         i < ManAHL::RollingMedian::dest_size(COUNT, WIN_LENGTH, DEST_STRIDE);
         i += DEST_STRIDE) {
        //        std::cout << i << " " << dest[i] << std::endl;
        result |= dest[i] != 4. + 2.0 * i;
    }
    delete [] dest;
    return result;
}

int test_roll_med_even_mean() {
    const int COUNT = 20;
    const int WIN_LENGTH = 4;
    const int DEST_STRIDE = 1;
    double src[COUNT];
    double *dest = new double[ManAHL::RollingMedian::dest_size(COUNT,
                                                               WIN_LENGTH,
                                                               DEST_STRIDE)];
    int result = 0;
    
    for (int i = 0; i < COUNT; ++i) {
        src[i] = 2.0 * i;
    }
    // Set
    result |= ManAHL::RollingMedian::even_odd_index(src, 1, COUNT,
                                                    WIN_LENGTH, dest, DEST_STRIDE);
    for (size_t i = 0;
         i < ManAHL::RollingMedian::dest_size(COUNT, WIN_LENGTH, DEST_STRIDE);
         i += DEST_STRIDE) {
        //        std::cout << i << " " << dest[i] << std::endl;
        result |= dest[i] != 3. + 2.0 * i;
    }
    delete [] dest;
    return result;
}

/******************* END: Rolling Median Tests ************************/

/********************** Documentation ***************************/

/* Print out the height compared with the lenght of the skip list
 */
int doc_height_trend(size_t level) {
    std::cout << "" << __FUNCTION__ << std::endl;
    std::cout << "Comparing the Height of HeadNode with the length of the skip list:" << std::endl;
    std::cout << std::setw(4) << "i";
    std::cout << std::setw(10) << "Length";
    std::cout << std::setw(8) << "Height";
    std::cout << std::endl;
    
    srand(1);
    for (size_t i = 0; i < level; ++i) {
        ManAHL::SkipList::HeadNode<double> sl;
        int num = 1 << i;
        for (int j = 0; j < num; ++j) {
            sl.insert(42.0);
        }
        std::cout << std::setw(4) << i;
        std::cout << std::setw(10) << num;
        std::cout << std::setw(8) << sl.height();
        std::cout << std::endl;
    }
    return 0;
}

int doc_simple_dot() {
    int result = 0;
    std::stringstream ostr;
    ostr << "# " << __FUNCTION__ << std::endl;
    
    srand(1);
    ManAHL::SkipList::HeadNode<double> sl;
    sl.insert(42.0);
    result |= sl.lacksIntegrity();
    sl.insert(84.0);
    result |= sl.lacksIntegrity();
    sl.insert(21.0);
    result |= sl.lacksIntegrity();
    sl.insert(100.0);
    result |= sl.lacksIntegrity();
    sl.insert(12.0);
    result |= sl.lacksIntegrity();
    sl.dotFile(ostr, 0);
    sl.dotFileFinalise(ostr, 1);
    std::cout << ostr.str() << std::endl;
    return result;
}

int doc_insert() {
    int result = 0;
    int NUM = 8;
    int dot_count = 0;
    std::stringstream ostr;
    ostr << "# " << __FUNCTION__ << std::endl;
    
    srand(1);
    ManAHL::SkipList::HeadNode<int> sl;
    sl.dotFile(ostr, dot_count++);
    for (int i = 0; i < NUM; ++i) {
        sl.insert(i);
        result |= sl.lacksIntegrity();
        sl.dotFile(ostr, dot_count++);
    }
    sl.dotFileFinalise(ostr, dot_count);
    std::cout << ostr.str() << std::endl;
    return result;
}

int doc_insert_remove() {
    int result = 0;
    int NUM = 4;
    int dot_count = 0;
    std::stringstream ostr;
    ostr << "# " << __FUNCTION__ << std::endl;
    
    srand(1);
    ManAHL::SkipList::HeadNode<int> sl;
    sl.dotFile(ostr, dot_count++);
    for (int i = 0; i < NUM; ++i) {
        sl.insert(i);
        result |= sl.lacksIntegrity();
        sl.dotFile(ostr, dot_count++);
    }
    for (int i = 0; i < NUM; ++i) {
        sl.remove(i);
        result |= sl.lacksIntegrity();
        sl.dotFile(ostr, dot_count++);
    }
    sl.dotFileFinalise(ostr, dot_count);
    std::cout << ostr.str() << std::endl;
    return result;
}

int doc_insert_remove_repeat() {
    int result = 0;
    int NUM = 4;
    int REPEAT_COUNT = 4;
    int dot_count = 0;
    std::stringstream ostr;
    ostr << "# " << __FUNCTION__ << std::endl;
    
    srand(1);
    ManAHL::SkipList::HeadNode<int> sl;
    sl.dotFile(ostr, dot_count++);
    for (int c = 0; c < REPEAT_COUNT; ++c) {
        for (int i = 0; i < NUM; ++i) {
            sl.insert(i);
            result |= sl.lacksIntegrity();
            sl.dotFile(ostr, dot_count++);
        }
        for (int i = 0; i < NUM; ++i) {
            sl.remove(i);
            result |= sl.lacksIntegrity();
            sl.dotFile(ostr, dot_count++);
        }
    }
    sl.dotFileFinalise(ostr, dot_count);
    std::cout << ostr.str() << std::endl;
    return result;
}

/******************* END: Documentation *************************/

/******************* Performance Tests **************************
 * These should have a corresponding test_... which checks correctness.
 * The perf_... version should do no checking but aim for maximum performance
 * however assert(! sl.lacksIntegrity()); is encouraged as that will get
 * stripped out in release builds where performance is really tested.
 */

int perf_single_insert_remove() {
    int num = 1000 * 1000;
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
    size_t SIZE = 1000 * 1000;
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
 * a vaue at the mod point. */
int perf_single_ins_rem_middle() {
    size_t SKIPLIST_SIZE = 1024 * 1024;
    int REPEAT_COUNT = 1000 * 1000;
    int result = 0;
    double val = SKIPLIST_SIZE / 2;
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

/* Performance of calling at() on the middle value of a 1M long skip list. */
int perf_single_at_middle() {
    size_t SKIPLIST_SIZE = 1024 * 1024;
    int REPEAT_COUNT = 1000 * 1000;
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
    int REPEAT_COUNT = 1000 * 1000;
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
        result |= value != 0;
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
    int REPEAT_COUNT = 1000 * 1000;
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
    std::cout << " window width: " << std::setw(4) << WINDOW_WIDTH;
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
        std::cout << std::setw(8) << exec << " ns" << std::endl;
    }
    return result;
}

int perf_has_in_one_million() {
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

int perf_roll_med_odd_index() {
    const int COUNT = 1000000;
    const int WIN_LENGTH = 101;
    const int DEST_STRIDE = 1;
    double src[COUNT];
    double *dest = new double[ManAHL::RollingMedian::dest_size(COUNT,
                                                               WIN_LENGTH,
                                                               DEST_STRIDE)];
    int result = 0;
    
    for (int i = 0; i < COUNT; ++i) {
        src[i] = 2.0 * i;
    }
    srand(1);
    time_t start = clock();
    result |= ManAHL::RollingMedian::odd_index(src, 1, COUNT,
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

int perf_roll_med_odd_index_wins() {
    const size_t COUNT = 1000000;
    const int DEST_STRIDE = 1;
    double *src = new double[COUNT];
    int result = 0;
    
    // Create source data
    for (size_t i = 0; i < COUNT; ++i) {
        src[i] = 2.0 * i;
    }
    // Loop over this data for various window sizes from 1 to 524288
    for (size_t win = 1; win < COUNT; win *= 2) {
        size_t dest_size = ManAHL::RollingMedian::dest_size(COUNT, win, DEST_STRIDE);
        double *dest = new double[dest_size];
        srand(1);
        time_t start = clock();
        result |= ManAHL::RollingMedian::odd_index(src, 1, COUNT,
                                                   win, dest, DEST_STRIDE);
        double exec = 1e6 * (clock() - start) / (double) CLOCKS_PER_SEC;
        std::cout << std::setw(FUNCTION_WIDTH) << __FUNCTION__ << "():";
        std::cout << " vectors length: " << std::setw(8) << COUNT;
        std::cout << " window width: " << std::setw(6) << win;
        std::cout << " time: ";
        std::cout << exec / 1e3;
        std::cout << " (ms)";
        std::cout << std::endl;
        delete [] dest;
    }
    delete [] src;
    return result;
}

/* Tests the size_of() function on a skip list of length up to 1M. */
int perf_size_of() {
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

/******************* END: Performance Tests **************************/

int print_result(const char *fn_name, int result) {
    std::cout << std::setw(FUNCTION_WIDTH) << fn_name << "():";
    if (result) {
        std::cout << " FAIL";
    } else {
        std::cout << " PASS";
    }
    std::cout << std::endl;
    return result;
}


int test_all() {
    int result = 0;

    result |= print_result("test_very_simple_insert", test_very_simple_insert());
    result |= print_result("test_simple_insert", test_simple_insert());
    result |= print_result("test_insert_and_remove_same",
                           test_insert_and_remove_same());
    result |= print_result("test_insert_remove_multiple",
                           test_insert_remove_multiple());
    result |= print_result("test_ins_rem_rand", test_ins_rem_rand());
    result |= print_result("test_insert_n_numbers_same",
                           test_insert_n_numbers_same(32, 42.0));
    result |= print_result("test_at", test_at());
    result |= print_result("test_at_fails", test_at_fails());
    result |= print_result("test_at_dest", test_at_dest());
    result |= print_result("test_at_dest_fails", test_at_dest_fails());
    result |= print_result("test_has", test_has());
    result |= print_result("test_has_not", test_has_not());
    result |= print_result("test_remove_fails", test_remove_fails());
    result |= print_result("test_at_large", test_at_large());
    result |= print_result("test_ins_at_rem_with_srand",
                           test_ins_at_rem_with_srand());
    result |= print_result("test_single_insert_remove",
                           test_single_insert_remove());
    result |= print_result("test_single_ins_rem_middle",
                           test_single_ins_rem_middle());
    result |= print_result("test_insert_one_million",
                           test_insert_one_million());
    return result;
}

int doc_all() {
    int result = 0;
    
    result |= print_result("doc_height_trend", doc_height_trend(20));
    result |= print_result("doc_simple_dot", doc_simple_dot());
    result |= print_result("doc_insert", doc_insert());
    result |= print_result("doc_insert_remove", doc_insert_remove());
    result |= print_result("doc_insert_remove_repeat",
                           doc_insert_remove_repeat());
    return result;
}

int perf_all() {
    int result = 0;
    
    result |= perf_single_insert_remove();
    result |= perf_large_skiplist_ins_only();
    result |= perf_large_skiplist_ins_rem();
    result |= perf_single_ins_rem_middle();
    result |= perf_single_at_middle();
    result |= perf_single_has_middle();
    result |= perf_single_ins_at_rem_middle();
    result |= perf_median_sliding_window();
    result |= perf_1m_median_values();
    result |= perf_1m_medians_1000_vectors();
    result |= perf_simulate_real_use();
    result |= perf_at_in_one_million();
    result |= perf_has_in_one_million();

    result |= perf_roll_med_odd_index();
    result |= perf_roll_med_odd_index_wins();
    
    result |= perf_size_of();
    
    return result;
}

int test_rolling_median_all() {
    int result = 0;
    
    result |= print_result("test_roll_med_simple", test_roll_med_simple());
    result |= print_result("test_roll_med_even_win", test_roll_med_even_win());
    result |= print_result("test_roll_med_even_mean", test_roll_med_even_mean());
    return result;
}


int main(int /* argc */, const char *[] /* argv[] */) {
    int result = 0;
    time_t start = clock();
    
    result |= test_all();
    result |= test_rolling_median_all();
    result |= doc_all();
    // Performance tests are very slow if DEBUG as checking
    // integrity is very expensive for large data sets.
#ifndef DEBUG
    result |= perf_all();
#endif
    double exec = 1e6 * (clock() - start) / (double) CLOCKS_PER_SEC;
    std::cout << "Final result: ";
    std::cout << (result ? "FAIL" : "PASS") << std::endl;
    std::cout << "Exec time: " << exec / 1e6 << " (s)" << std::endl;
    std::cout << "Bye, bye!\n";
    return 0;
}
