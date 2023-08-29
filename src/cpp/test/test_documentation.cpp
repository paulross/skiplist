//
//  test_documentation.cpp
//  skiplist
//
//  Created by Paul Ross on 13/07/2016.
//  Copyright (c) 2016 Paul Ross. All rights reserved.
//
#include <iostream>
#include <iomanip>

#include "test_documentation.h"
#include "test_print.h"

#include "../SkipList.h"

/********************** Documentation ***************************/

/* Print out the height compared with the length of the skip list
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

int doc_simple_dot() {
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

int doc_insert() {
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

int doc_insert_remove() {
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

int doc_insert_remove_repeat() {
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

int test_documentation_all() {
    int result = 0;
    
    result |= print_result("doc_height_trend", doc_height_trend(20));
#ifdef INCLUDE_METHODS_THAT_USE_STREAMS
    result |= print_result("doc_simple_dot", doc_simple_dot());
    result |= print_result("doc_insert", doc_insert());
    result |= print_result("doc_insert_remove", doc_insert_remove());
    result |= print_result("doc_insert_remove_repeat",
                           doc_insert_remove_repeat());
#endif
    return result;
}

