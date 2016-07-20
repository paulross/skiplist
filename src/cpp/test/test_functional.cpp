//
//  test_functional.cpp
//  skiplist
//
//  Created by Paul Ross on 13/07/2016.
//  Copyright (c) 2016 Paul Ross. All rights reserved.
//
#include "test_print.h"
#include "test_functional.h"

#include "../SkipList.h"

/******************* Functional Tests **************************/

ManAHL::SkipList::IntegrityCheck test_very_simple_insert() {
    ManAHL::SkipList::IntegrityCheck result;
    
    srand(1);
    ManAHL::SkipList::HeadNode<double> sl;
    sl.insert(42.0);
    result = sl.lacksIntegrity();
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
    sl.insert(84.0);
    result |= sl.lacksIntegrity();
    sl.insert(21.0);
    result |= sl.lacksIntegrity();
    sl.insert(100.0);
    result |= sl.lacksIntegrity();
    result |= sl.lacksIntegrity();
    sl.insert(12.0);
    result |= sl.lacksIntegrity();
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
    sl.remove(42.0);
    result |= sl.lacksIntegrity();
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
    sl.insert(84.0);
    result |= sl.lacksIntegrity();
    sl.remove(42.0);
    result |= sl.lacksIntegrity();
    sl.insert(21.0);
    result |= sl.lacksIntegrity();
    sl.remove(84.0);
    result |= sl.lacksIntegrity();
    sl.insert(100.0);
    result |= sl.lacksIntegrity();
    sl.insert(12.0);
    result |= sl.lacksIntegrity();
    sl.remove(21.0);
    result |= sl.lacksIntegrity();
    sl.remove(12.0);
    result |= sl.lacksIntegrity();
    sl.remove(100.0);
    result |= sl.lacksIntegrity();
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
                break;
            }
        }
        while (values.size()) {
            value = values.back();
            ostr << "# Removing value " << value << std::endl;
            sl.remove(value);
            values.pop_back();
            result |= sl.lacksIntegrity();
            if (result) {
                break;
            }
        }
        if (result) {
            break;
        }
    }
    if (result) {
        std::cout << ostr.str() << " FAIL" << std::endl;
    }
    return result;
}

int test_insert_n_numbers_same(int n, double value) {
    int result = 0;
    
    srand(1);
    ManAHL::SkipList::HeadNode<double> sl;
    for (int i = 0; i < n; ++i) {
        sl.insert(value);
        result |= sl.lacksIntegrity();
    }
    for (int i = 0; i < n; ++i) {
        sl.remove(value);
        result |= sl.lacksIntegrity();
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
    // Test at()
    result |= sl.at(0) != 12.0;
    result |= sl.at(1) != 21.0;
    result |= sl.at(2) != 42.0;
    result |= sl.at(3) != 84.0;
    result |= sl.at(4) != 100.0;
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
    return result;
}

// Test at(size_t index, size_t count, T *dest)
int test_at_dest_fails() {
    int result = 0;
    
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
    return result;
}

int test_has() {
    int result = 0;
    std::stringstream ostr;
    ostr << "# " << __FUNCTION__ << std::endl;
    
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
    // Test at()
    result |= ! sl.has(12.0);
    result |= ! sl.has(21.0);
    result |= ! sl.has(42.0);
    result |= ! sl.has(84.0);
    result |= ! sl.has(100.0);
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
    return result;
}

int test_ins_at_rem_with_srand() {
    int result = 0;
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

/* Insert 1m doubles for memory measurement. */
int test_insert_nan_throws() {
    int result = 0;
    ManAHL::SkipList::HeadNode<double> sl;
    
    srand(1);
    std::string in;
    result |= sl.lacksIntegrity();
    try {
        sl.insert(std::numeric_limits<double>::quiet_NaN());
        result |= 1;
    } catch (ManAHL::SkipList::FailedComparison &err) {}
    result |= sl.lacksIntegrity();
    return result;
}

/* Test basic use of index() */
int test_index_basic_7_node() {
    size_t NUM = 8;
    int result = 0;
    ManAHL::SkipList::HeadNode<size_t> sl;
    
    srand(1);
    std::string in;
    result |= sl.lacksIntegrity();
    for (size_t i = 0; i < NUM; ++i) {
        sl.insert(i);
    }
//    size_t idx;
//    for (int i = 0; i < NUM; ++i) {
//        try {
//            idx = sl.index(i);
//            std::cout << "i = " << i << " sl.index(i) = " << idx << std::endl;
//        } catch (ManAHL::SkipList::ValueError &err) {
//            std::cout << "i = " << i << " FAILED" << std::endl;
//        }
//    }
    for (size_t i = 0; i < NUM; ++i) {
        result |= sl.index(i) != i;
    }
    result |= sl.lacksIntegrity();
    return result;
}

/* Test index() throws with out of range values. */
int test_index_throws() {
    int NUM = 8;
    int result = 0;
    ManAHL::SkipList::HeadNode<int> sl;
    
    srand(1);
    std::string in;
    result |= sl.lacksIntegrity();
    for (int i = 0; i < NUM; ++i) {
        sl.insert(i);
    }
    size_t idx = 0;
    try {
        idx = sl.index(-1);
        result |= 1;
    } catch (ManAHL::SkipList::ValueError &err) {}
    try {
        idx = sl.index(NUM);
        result |= 1;
    } catch (ManAHL::SkipList::ValueError &err) {}
    result |= idx;
    result |= sl.lacksIntegrity();
    return result;
}

/* Use of index() on a large number of integers. */
int test_index_large() {
    size_t NUM = 1024 * 128;
    int result = 0;
    ManAHL::SkipList::HeadNode<size_t> sl;
    
    srand(1);
    std::string in;
    result |= sl.lacksIntegrity();
    for (size_t i = 0; i < NUM; ++i) {
        sl.insert(i);
    }
    for (size_t i = 0; i < NUM; ++i) {
        result |= sl.index(i) != i;
    }
    result |= sl.lacksIntegrity();
    return result;
}

/***************** END: Functional Tests ************************/

int test_functional_all() {
    int result = 0;
    
    result |= print_result("test_very_simple_insert",
                           test_very_simple_insert());
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
    result |= print_result("test_insert_nan_throws", test_insert_nan_throws());
    result |= print_result("test_index_basic_7_node()",
                           test_index_basic_7_node());
    result |= print_result("test_index_throws", test_index_throws());
    result |= print_result("test_index_large", test_index_large());
    return result;
}
