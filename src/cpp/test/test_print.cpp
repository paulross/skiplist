/**
 * @file test_print.cpp
 * skiplist
 *
 * Concurrency Tests.
 *
 * Created by Paul Ross on 13/07/2016.
 * Copyright (c) 2016-2023 Paul Ross. All rights reserved.
 *
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
 */
//
//
//  skiplist
//
//  Created by Paul Ross on 13/07/2016.
//  Copyright (c) 2016 Paul Ross. All rights reserved.
//

#include <iostream>
#include <iomanip>

#include "test_print.h"

const int FUNCTION_WIDTH = 50;

/**
 * Print the result of a test function.
 *
 * @param fn_name Function name.
 * @param result The result of the function. If <0 this is "N/A", if 0 success else failure.
 * @return result > 0.
 */
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


