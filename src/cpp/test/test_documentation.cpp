/**
 * @file
 *
 * Project: skiplist
 *
 * Documentation Tests.
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

#include "test_documentation.h"
#include "test_print.h"

#include "../SkipList.h"

/********************** Documentation ***************************/

/**
 * Print out the height compared with the length of the skip list
 *
 * @param level The level to go to.
 * @return Zero.
 */
static int test_doc_height_trend(size_t level) {
    std::cout << "" << __FUNCTION__ << std::endl;
    std::cout << "Comparing the Height of HeadNode with the length of the skip list:" << std::endl;
    std::cout << std::setw(4) << "i";
    std::cout << std::setw(10) << "Length";
    std::cout << std::setw(8) << "Height";
    std::cout << std::endl;
    
    srand(1);
    for (size_t i = 0; i < level; ++i) {
        OrderedStructs::SkipList::HeadNode<double> sl;
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

#ifdef INCLUDE_METHODS_THAT_USE_STREAMS

/**
 * Print out a simple DOT file on stdout.
 *
 * @return Zero.
 */
static int tests_doc_simple_dot() {
    int result = 0;
    std::stringstream ostr;
    ostr << "# " << __FUNCTION__ << std::endl;
    
    srand(1);
    OrderedStructs::SkipList::HeadNode<double> sl;
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
    sl.dotFile(ostr);
    sl.dotFileFinalise(ostr);
    std::cout << ostr.str() << std::endl;
    return result;
}

/**
 * Create a Skip List, populate it and print the DOT file to stdout.
 *
 * @return Zero on success, non-zero on failure.
 */
static int test_doc_insert() {
    int result = 0;
    int NUM = 8;
    std::stringstream ostr;
    ostr << "# " << __FUNCTION__ << std::endl;
    
    srand(1);
    OrderedStructs::SkipList::HeadNode<int> sl;
    sl.dotFile(ostr);
    for (int i = 0; i < NUM; ++i) {
        sl.insert(i);
        result |= sl.lacksIntegrity();
        sl.dotFile(ostr);
    }
    sl.dotFileFinalise(ostr);
    std::cout << ostr.str() << std::endl;
    return result;
}

/**
 * Create a Skip List, populate it, remove items and print the DOT file to stdout.
 *
 * @return Zero on success, non-zero on failure.
 */
static int test_doc_insert_remove() {
    int result = 0;
    int NUM = 4;
    std::stringstream ostr;
    ostr << "# " << __FUNCTION__ << std::endl;
    
    srand(1);
    OrderedStructs::SkipList::HeadNode<int> sl;
    sl.dotFile(ostr);
    for (int i = 0; i < NUM; ++i) {
        sl.insert(i);
        result |= sl.lacksIntegrity();
        sl.dotFile(ostr);
    }
    for (int i = 0; i < NUM; ++i) {
        sl.remove(i);
        result |= sl.lacksIntegrity();
        sl.dotFile(ostr);
    }
    sl.dotFileFinalise(ostr);
    std::cout << ostr.str() << std::endl;
    return result;
}

/**
 * Create a Skip List, repeatedly populate it and print the DOT file to stdout.
 *
 * @return Zero on success, non-zero on failure.
 */
static int test_doc_insert_remove_repeat() {
    int result = 0;
    int NUM = 4;
    int REPEAT_COUNT = 4;
    std::stringstream ostr;
    ostr << "# " << __FUNCTION__ << std::endl;
    
    srand(1);
    OrderedStructs::SkipList::HeadNode<int> sl;
    sl.dotFile(ostr);
    for (int c = 0; c < REPEAT_COUNT; ++c) {
        for (int i = 0; i < NUM; ++i) {
            sl.insert(i);
            result |= sl.lacksIntegrity();
            sl.dotFile(ostr);
        }
        for (int i = 0; i < NUM; ++i) {
            sl.remove(i);
            result |= sl.lacksIntegrity();
            sl.dotFile(ostr);
        }
    }
    sl.dotFileFinalise(ostr);
    std::cout << ostr.str() << std::endl;
    return result;
}

#endif

/******************* END: Documentation *************************/

/**
 * Run all the documentation tests.
 *
 * @return Zero on success, non-zero on failure.
 */
int test_documentation_all() {
    int result = 0;
    
    result |= print_result("test_doc_height_trend", test_doc_height_trend(20));
#ifdef INCLUDE_METHODS_THAT_USE_STREAMS
    result |= print_result("tests_doc_simple_dot", tests_doc_simple_dot());
    result |= print_result("test_doc_insert", test_doc_insert());
    result |= print_result("test_doc_insert_remove", test_doc_insert_remove());
    result |= print_result("test_doc_insert_remove_repeat",
                           test_doc_insert_remove_repeat());
#endif
    return result;
}

