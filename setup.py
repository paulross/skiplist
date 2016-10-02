#!/bin/env python
import os
from distutils.core import setup, Extension

# Makefile:
# CFLAGS = -Wall -Wextra
# CFLAGS_RELEASE = $(CFLAGS) -O3 -DNDEBUG
# CFLAGS_DEBUG = $(CFLAGS) -g3 -O0 -DDEBUG=1
extra_compile_args = ['-Wall', '-Wextra', '-Werror', '-Wfatal-errors', ]
DEBUG = False

if DEBUG:
    extra_compile_args.extend(['-g3', '-O0', '-DDEBUG=1'])
else:
    extra_compile_args.extend(['-O3', '-DNDEBUG'])

orderedstructs = Extension("orderedstructs",
                             sources=[
                                      'src/cpp/cOrderedStructs.cpp',
                                      'src/cpp/OrderedStructs.cpp',
                                      'src/cpp/cSkipList.cpp',
                                      'src/cpp/cSortedMap.cpp',
                                      'src/cpp/SkipList.cpp',
                                      ],
                             include_dirs=['.', ],
                             library_dirs=[os.getcwd(), ],
                             extra_compile_args=extra_compile_args,
                             language='c++',
                             )

setup(name='orderedstructs',
      version='0.2.0',
      ext_modules=[orderedstructs, ])
