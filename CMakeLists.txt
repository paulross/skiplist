cmake_minimum_required(VERSION 3.13)
project(SkipList)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_C_FLAGS "-std=c99")

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    message("debug build")
    add_compile_definitions(DEBUG)
ELSE()
    message("release build")
ENDIF(CMAKE_BUILD_TYPE MATCHES Debug)

# To deal with:
# ld: warning: ignoring file /usr/local/Frameworks/Python.framework/Versions/3.11/lib/libpython3.11.dylib, building for macOS-arm64 but attempting to link with file built for macOS-x86_64
set(CMAKE_OSX_ARCHITECTURES "x86_64")

add_compile_options(
        "-Wall" "-Wpedantic" "-Wextra" "-fexceptions"
        "$<$<CONFIG:DEBUG>:-O0;-g3;-ggdb>"
)

link_directories(
        /Library/Frameworks/Python.framework/Versions/3.11/lib
)

add_executable(
        SkipList

        src/cpp/HeadNode.h
        src/cpp/IntegrityEnums.h
        src/cpp/main.cpp
        src/cpp/Node.h
        src/cpp/NodeRefs.h
        src/cpp/RollingMedian.h
        src/cpp/SkipList.cpp
        src/cpp/SkipList.h
        # Test code
        src/cpp/test/test_concurrent.cpp
        src/cpp/test/test_concurrent.h
        src/cpp/test/test_documentation.cpp
        src/cpp/test/test_documentation.h
        src/cpp/test/test_functional.cpp
        src/cpp/test/test_functional.h
        src/cpp/test/test_performance.cpp
        src/cpp/test/test_performance.h
        src/cpp/test/test_print.cpp
        src/cpp/test/test_print.h
        src/cpp/test/test_rolling_median.cpp
        src/cpp/test/test_rolling_median.h
        # Python interface
        src/cpy/cmpPyObject.cpp
        src/cpy/cmpPyObject.h
        src/cpy/cOrderedStructs.cpp
        src/cpy/cOrderedStructs.h
        src/cpy/cSkipList.cpp
        src/cpy/cSkipList.h
        src/cpy/OrderedStructs.cpp
        src/cpy/OrderedStructs.h
)

include_directories(
        src/cpp
        src/cpp/test
        src/cpy
)

FIND_PACKAGE(PythonLibs 3.11 REQUIRED)
IF(PYTHONLIBS_FOUND)
    message(status "    Python libraries: ${PYTHON_LIBRARIES}")
    message(status " Python include dirs: ${PYTHON_INCLUDE_DIRS}")
    message(status "      Python version: ${PYTHONLIBS_VERSION_STRING}")
    message(status "      Python library: ${PYTHON_LIBRARY}")
    message(status "  Python include dir: ${PYTHON_INCLUDE_DIR}")
    message(status " Python include dirs: ${PYTHON_INCLUDE_DIRs}")
    INCLUDE_DIRECTORIES("${PYTHON_INCLUDE_DIRS}")
ELSE()
    MESSAGE(FATAL_ERROR "Unable to find Python libraries.")
ENDIF()


target_link_libraries(SkipList ${PYTHON_LIBRARIES})

target_compile_options(SkipList PRIVATE -Wall -Wextra -Wno-c99-extensions -pedantic)# -Werror)
