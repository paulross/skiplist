/**
 * @file
 *
 * Project: skiplist
 *
 * Basic functional tests on a Skip List. Most of these are with a \c SkipList<double>
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
#include "test_print.h"
#include "test_functional.h"

#include <functional> // For comparison function

#include "../SkipList.h"

/******************* Functional Tests **************************/

/**
 * @brief Test the integrity of a Skip List after a single insert.
 *
 * @return Zero on success, non-zero on failure.
 */
int test_very_simple_insert() {
    int result = 0;
    
    srand(1);
    OrderedStructs::SkipList::HeadNode<double> sl;
    sl.insert(42.0);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    return result;
}

// Need int as OrderedStructs::SkipList::IntegrityCheck can not use |=
/**
 * @brief Test the integrity of a Skip List after a multiple inserts.
 *
 * @return Zero on success, non-zero on failure.
 */
int test_simple_insert() {
    int result = 0;
    std::stringstream ostr;
    ostr << "# " << __FUNCTION__ << std::endl;
    
    srand(1);
    OrderedStructs::SkipList::HeadNode<double> sl;
    sl.insert(42.0);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    sl.insert(84.0);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    sl.insert(21.0);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    sl.insert(100.0);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    sl.insert(12.0);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    return result;
}

/**
 * @brief Test the integrity of a Skip List after a single insert and remove.
 *
 * @return Zero on success, non-zero on failure.
 */
int test_insert_and_remove_same() {
    int result = 0;
    std::stringstream ostr;
    ostr << "# " << __FUNCTION__ << std::endl;
    
    srand(1);
    OrderedStructs::SkipList::HeadNode<double> sl;
    sl.insert(42.0);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    result |= sl.remove(42.0) != 42.0;
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    return result;
}

/**
 * @brief Test the integrity of a Skip List after a multiple inserts and removes.
 *
 * @return Zero on success, non-zero on failure.
 */
int test_insert_remove_multiple() {
    int result = 0;
    std::stringstream ostr;
    ostr << "# " << __FUNCTION__ << std::endl;
    
    srand(1);
    OrderedStructs::SkipList::HeadNode<double> sl;
    sl.insert(42.0);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    sl.insert(84.0);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    result |= sl.remove(42.0) != 42.0;
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    sl.insert(21.0);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    result |= sl.remove(84.0) != 84.0;
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    sl.insert(100.0);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    sl.insert(12.0);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    result |= sl.remove(21.0) != 21.0;
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    result |= sl.remove(12.0) != 12.0;
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    result |= sl.remove(100.0) != 100.0;
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    return result;
}

/**
 * @brief For many different seeds insert 128 random values then remove them
 * in reverse order.
 *
 * Test the integrity of the Skip List throughout.
 *
 * @return Zero on success, non-zero on failure.
 */
int test_ins_rem_rand() {
    int result = 0;
    std::stringstream ostr;
    ostr << "# " << __FUNCTION__ << std::endl;
    int SEED  = 128 + 1;
    int LENGTH = 128;
    
    for (int seed = 1; seed < SEED; ++seed) {
        srand(seed);
        OrderedStructs::SkipList::HeadNode<unsigned int> sl;
        std::vector<unsigned int> values;
        unsigned int value;
        for (int i = 0; i < LENGTH; ++i) {
            value = rand() % LENGTH;
            values.push_back(value);
            sl.insert(value);
            result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
            if (result) {
                break;
            }
        }
        while (values.size()) {
            value = values.back();
            ostr << "# Removing value " << value << std::endl;
            result |= sl.remove(value) != value;
            values.pop_back();
            result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
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

/**
 * @brief Test the integrity of a Skip List after multiple insert then multiple removes.
 *
 * @return Zero on success, non-zero on failure.
 */
int test_insert_n_numbers_same(int n, double value) {
    int result = 0;
    
    srand(1);
    OrderedStructs::SkipList::HeadNode<double> sl;
    for (int i = 0; i < n; ++i) {
        sl.insert(value);
        result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    }
    for (int i = 0; i < n; ++i) {
        result |= sl.remove(value) != value;
        result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    }
    return result;
}

/**
 * @brief Test \c .at() and the integrity of a Skip List after multiple inserts.
 *
 * @return Zero on success, non-zero on failure.
 */
int test_at() {
    int result = 0;
    std::stringstream ostr;
    ostr << "# " << __FUNCTION__ << std::endl;
    
    srand(1);
    OrderedStructs::SkipList::HeadNode<double> sl;
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    sl.insert(42.0);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    sl.insert(84.0);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    sl.insert(21.0);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    sl.insert(100.0);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    sl.insert(12.0);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    // Test at()
    result |= sl.at(0) != 12.0;
    result |= sl.at(1) != 21.0;
    result |= sl.at(2) != 42.0;
    result |= sl.at(3) != 84.0;
    result |= sl.at(4) != 100.0;
    return result;
}

/**
 * @brief Test \c .at() \b fails by raising a \c OrderedStructs::SkipList::IndexError
 *
 * Also test the integrity of a Skip List after multiple inserts.
 *
 * @return Zero on success, non-zero on failure.
 */
int test_at_fails() {
    int result = 0;
    
    srand(1);
    OrderedStructs::SkipList::HeadNode<double> sl;
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    sl.insert(42.0);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    sl.insert(84.0);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    sl.insert(21.0);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    sl.insert(100.0);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    sl.insert(12.0);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    try {
        sl.at(-1);
        result |= 1;
    } catch (OrderedStructs::SkipList::IndexError &err) {}
    try {
        sl.at(5);
        result |= 1;
    } catch (OrderedStructs::SkipList::IndexError &err) {}
    try {
        sl.at(6);
        result |= 1;
    } catch (OrderedStructs::SkipList::IndexError &err) {}
    return result;
}

/**
 * @brief Test <tt>at(size_t index, size_t count, T *dest)</tt>.
 *
 * Also tests the integrity of a Skip List after multiple inserts.
 *
 * @return Zero on success, non-zero on failure.
 */
int test_at_dest() {
    int result = 0;
    
    srand(1);
    OrderedStructs::SkipList::HeadNode<double> sl;
    std::vector<double> dest;
    
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    sl.insert(4.0);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    sl.insert(8.0);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    sl.at(0, 2, dest);
    
    result |= dest.size() != 2;
    result |= dest[0] != 4.0;
    result |= dest[1] != 8.0;
    return result;
}

/**
 * @brief Test <tt>at(size_t index, size_t count, T *dest)</tt> \b fails throwing a \c OrderedStructs::SkipList::IndexError
 *
 * Also tests the integrity of a Skip List after multiple inserts.
 *
 * @return Zero on success, non-zero on failure.
 */
int test_at_dest_fails() {
    int result = 0;
    
    srand(1);
    OrderedStructs::SkipList::HeadNode<double> sl;
    std::vector<double> dest;
    
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    sl.insert(4.0);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    sl.insert(8.0);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    
    try {
        sl.at(0, 3, dest);
        result |= 1;
    } catch (OrderedStructs::SkipList::IndexError &err) {}
    try {
        sl.at(1, 2, dest);
        result |= 1;
    } catch (OrderedStructs::SkipList::IndexError &err) {}
    try {
        sl.at(2, 1, dest);
        result |= 1;
    } catch (OrderedStructs::SkipList::IndexError &err) {}
    try {
        sl.at(3, 0, dest);
        result |= 1;
    } catch (OrderedStructs::SkipList::IndexError &err) {}
    return result;
}

/**
 * @brief Test \c .has() and the integrity of a Skip List after multiple inserts.
 *
 * @return Zero on success, non-zero on failure.
 */
int test_has() {
    int result = 0;
    std::stringstream ostr;
    ostr << "# " << __FUNCTION__ << std::endl;
    
    srand(1);
    OrderedStructs::SkipList::HeadNode<double> sl;
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    sl.insert(42.0);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    sl.insert(84.0);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    sl.insert(21.0);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    sl.insert(100.0);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    sl.insert(12.0);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    // Test at()
    result |= ! sl.has(12.0);
    result |= ! sl.has(21.0);
    result |= ! sl.has(42.0);
    result |= ! sl.has(84.0);
    result |= ! sl.has(100.0);
    return result;
}

/**
 * @brief Test \c .has() \b fails.
 *
 * Also tests the integrity of a Skip List after multiple inserts.
 *
 * @return Zero on success, non-zero on failure.
 */
int test_has_not() {
    int result = 0;
    
    srand(1);
    OrderedStructs::SkipList::HeadNode<double> sl;
    result |= sl.has(-1);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    sl.insert(42.0);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    sl.insert(84.0);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    sl.insert(21.0);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    sl.insert(100.0);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    sl.insert(12.0);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    result |= sl.has(-1);
    return result;
}

/**
 * @brief Test \c .remove() \b fails throwing a \c OrderedStructs::SkipList::ValueError
 *
 * Also tests the integrity of a Skip List after multiple inserts.
 *
 * @return Zero on success, non-zero on failure.
 */
int test_remove_fails() {
    int result = 0;
    OrderedStructs::SkipList::HeadNode<double> sl;
    
    srand(1);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    sl.insert(42.0);
    try {
        sl.remove(84.0);
        result |= 1;
    } catch (OrderedStructs::SkipList::ValueError &err) {}
    try {
        sl.remove(21.0);
        result |= 1;
    } catch (OrderedStructs::SkipList::ValueError &err) {}
    return result;
}

/**
 * @brief Test \c .at() on a larger Skip List with different seeds.
 *
 * Also tests the integrity of a Skip List after multiple inserts.
 *
 * @return Zero on success, non-zero on failure.
 */
int test_at_large() {
    int result = 0;
    int SEED  = 128 + 1;
    int LENGTH = 128;
    
    for (int seed = 1; seed < SEED; ++seed) {
        srand(seed);
        OrderedStructs::SkipList::HeadNode<int> sl;
        for (int i = 0; i < LENGTH; ++i) {
            sl.insert(i * 2);
            result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
        }
        for (int i = 0; i < LENGTH; ++i) {
            int val = sl.at(i);
            result |= val != i * 2;
        }
    }
    return result;
}

/**
 * @brief Test \c .insert() \c .at() \c .remove() on a larger Skip List with different seeds.
 *
 * Also tests the integrity of a Skip List after each call.
 *
 * @return Zero on success, non-zero on failure.
 */
int test_ins_at_rem_with_srand() {
    int result = 0;
    int SEED  = 128 + 1;
    int LENGTH = 128;
    
    for (int seed = 1; seed < SEED; ++seed) {
        srand(seed);
        OrderedStructs::SkipList::HeadNode<int> sl;
        for (int i = 0; i < LENGTH; ++i) {
            sl.insert(i * 2);
            result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
        }
        for (int i = 0; i < LENGTH; ++i) {
            result |= sl.at(i) != i * 2;
            result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
        }
        for (int i = 0; i < LENGTH; ++i) {
            result |= sl.remove(i * 2.0) != i * 2.0;
            result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
        }
        result |= sl.size() != 0;
    }
    return result;
}

/**
 * @brief Test \c .insert() \c .remove() on a large Skip List.
 *
 * Also tests the integrity of a Skip List after each call.
 *
 * @return Zero on success, non-zero on failure.
 */
int test_single_insert_remove() {
    int num = 1000 * 1000;
    int result = 0;
    OrderedStructs::SkipList::HeadNode<double> sl;

    srand(1);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    for (int i = 0; i < num; ++i) {
        sl.insert(42.0);
        result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
        result |= sl.remove(42.0) != 42.0;
        result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
        result |= sl.size() != 0;
    }
    result |= sl.size() != 0;
    return result;
}

/**
 * @brief Test \c .insert() \c .remove() in the middle of a medium sized Skip List.
 *
 * Also tests the integrity of a Skip List after each call.
 *
 * @return Zero on success, non-zero on failure.
 */
int test_single_ins_rem_middle() {
    int NUM = 1000; // * 1000;
    size_t SIZE = 1000;//24;
    int result = 0;
    double val = SIZE / 2;
    OrderedStructs::SkipList::HeadNode<double> sl;
    
    srand(1);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    for (size_t i = 0; i < SIZE; ++i) {
        sl.insert(i);
        result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
        result |= sl.size() != i + 1;
    }
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    result |= sl.size() != SIZE;
    for (int i = 0; i < NUM; ++i) {
        sl.insert(val);
        result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
        result |= sl.remove(val) != val;
        result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    }
    return result;
}

/**
 * @brief Tests insert 1m duplicate doubles for memory measurement.
 *
 * Also tests the integrity of a Skip List after each call.
 *
 * @return Zero on success, non-zero on failure.
 */
int test_insert_one_million() {
    size_t num = 1024 * 1024;
    int result = 0;
    OrderedStructs::SkipList::HeadNode<double> sl;
    
    srand(1);
    std::string in;
    //    std::cout << "Ready? ";
    //    std::cin >> in;
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    for (size_t i = 0; i < num; ++i) {
        sl.insert(42.0);
        result |= sl.size() != i + 1;
    }
    result |= sl.size() != num;
    //    std::cout << "Complete...";
    //    std::cin >> in;
    return result;
}

/**
 * @brief Tests inserting a NaN double throws a \c OrderedStructs::SkipList::FailedComparison
 *
 * @return Zero on success, non-zero on failure.
 */
int test_insert_nan_throws() {
    int result = 0;
    OrderedStructs::SkipList::HeadNode<double> sl;
    
    srand(1);
    std::string in;
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    try {
        sl.insert(std::numeric_limits<double>::quiet_NaN());
        result |= 1;
    } catch (OrderedStructs::SkipList::FailedComparison &err) {}
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    return result;
}

/**
 * @brief Tests basic use of \c .index() on a small Skip List.
 *
 * @return Zero on success, non-zero on failure.
 */
int test_index_basic_7_node() {
    size_t NUM = 8;
    int result = 0;
    OrderedStructs::SkipList::HeadNode<size_t> sl;
    
    srand(1);
    std::string in;
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    for (size_t i = 0; i < NUM; ++i) {
        sl.insert(i);
    }
    for (size_t i = 0; i < NUM; ++i) {
        result |= sl.index(i) != i;
    }
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    return result;
}

/**
 * @brief Tests \c .index() throws a \c OrderedStructs::SkipList::ValueError with a non-existent value.
 *
 * @return Zero on success, non-zero on failure.
 */
int test_index_throws() {
    int NUM = 8;
    int result = 0;
    OrderedStructs::SkipList::HeadNode<int> sl;
    
    srand(1);
    std::string in;
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    for (int i = 0; i < NUM; ++i) {
        sl.insert(i);
    }
    size_t idx = 0;
    try {
        idx = sl.index(-1);
        result |= 1;
    } catch (OrderedStructs::SkipList::ValueError &err) {}
    try {
        idx = sl.index(NUM);
        result |= 1;
    } catch (OrderedStructs::SkipList::ValueError &err) {}
    result |= idx;
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    return result;
}

/* Use of index() on a large number of integers. */
/**
 * @brief Tests \c .index() throws a \c OrderedStructs::SkipList::ValueError with a non-existent value.
 *
 * Medium sized Skip List.
 *
 * @return Zero on success, non-zero on failure.
 */
int test_index_large() {
    size_t NUM = 1024 * 128;
    int result = 0;
    OrderedStructs::SkipList::HeadNode<size_t> sl;
    
    srand(1);
    std::string in;
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    for (size_t i = 0; i < NUM; ++i) {
        sl.insert(i);
    }
    for (size_t i = 0; i < NUM; ++i) {
        result |= sl.index(i) != i;
    }
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    return result;
}

/******* Functional Tests with compare() specified **************/

/** @brief Creates a comparison function that return the inverse of \c std::less to create a decreasing Skip List. */
template <typename T>
struct reversed {
    bool operator()(const T &a, const T &b) const {
        return b < a;
    }
};

/**
 * @brief Test a Skip List with a comparison function that gives decreasing order.
 *
 * @return Zero on success, non-zero on failure.
 */
int test_reversed_simple_insert() {
    int result = 0;
    std::stringstream ostr;
    ostr << "# " << __FUNCTION__ << std::endl;
    
    srand(1);
    OrderedStructs::SkipList::HeadNode<double, reversed<double>> sl;
    sl.insert(42.0);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    sl.insert(84.0);
    result |= sl.lacksIntegrity() != OrderedStructs::SkipList::INTEGRITY_SUCCESS;
    result |= sl.at(0) != 84.0;
    result |= sl.index(84.0) != 0;
    result |= sl.at(1) != 42.0;
    result |= sl.index(42.0) != 1;
    return result;
}

/******* END: Functional Tests with compare() specified **********/

/***************** END: Functional Tests ************************/

/**
 * @brief All functional tests.
 *
 * @return Zero on success, non-zero on failure.
 */
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
    result |= print_result("test_index_basic_7_node",
                           test_index_basic_7_node());
    result |= print_result("test_index_throws", test_index_throws());
    result |= print_result("test_index_large", test_index_large());
    // Tests of reversed skiplists
    result |= print_result("test_reversed_simple_insert", test_reversed_simple_insert());
    return result;
}
