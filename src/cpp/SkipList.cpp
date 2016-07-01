//
//  SkipList.cpp
//  SkipList
//
//  Created by Paul Ross on 19/12/2015.
//  Copyright (c) 2015 AHL. All rights reserved.
//

#include <cstdlib>
#include "SkipList.h"

namespace ManAHL {
namespace SkipList {

bool tossCoin() {
    // A fair coin is / 2.
    // For a 25% coin (lower nodes) use rand() < RAND_MAX / 4
    // For a 75% coin (taller nodes) use rand() < RAND_MAX  - RAND_MAX / 4
    return rand() < RAND_MAX / 2;
}

void seedRand(unsigned seed) {
    srand(seed);
}

// This throws an IndexError when the index value >= size.
// If possible the error will have an informative message.
void _throw_exceeds_size(size_t size) {
#ifdef INCLUDE_METHODS_THAT_USE_STREAMS
    std::ostringstream oss;
    oss << "Index out of range 0 <= index < " << size;
    std::string err_msg = oss.str();
#else
    std::string err_msg = "Index out of range.";
#endif
    throw IndexError(err_msg);
}

} // namespace SkipList
} // namespace ManAHL
