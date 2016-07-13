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
    if (result) {
        std::cout << " FAIL";
    } else {
        std::cout << " PASS";
    }
    std::cout << std::endl;
    return result;
}


