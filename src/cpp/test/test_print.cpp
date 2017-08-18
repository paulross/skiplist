//
//  test_print.cpp
//  skiplist
//
//  Created by Paul Ross on 13/07/2016.
//  Copyright (c) 2016 Paul Ross. All rights reserved.
//

#include <iostream>
#include <iomanip>

#include "test_print.h"

const int FUNCTION_WIDTH = 40;

int print_result(const char *fn_name, int result) {
    std::cout << std::setw(FUNCTION_WIDTH) << fn_name << "():";
    if (result > 1) {
        std::cout << " FAIL";
    } else if (result < 0) {
        std::cout << " N/A";
    } else {
        std::cout << " PASS";
    }
    std::cout << std::endl;
    return result > 0;
}


