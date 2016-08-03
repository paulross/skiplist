#!/bin/env python
#
# $HeadURL$

# try:
#     from ahl.pkgutils.setuptools import setup
# except ImportError:
#     print "AHL Package Utilities are not available. Please run \"easy_install ahl.pkgutils\""
#     import sys
#     sys.exit(1)

# import sys
import os
# from distutils import log
from distutils.core import setup, Extension

# from ahl.pkgutils.setuptools.extension import NumpyExtension

# from ahl.pkgutils.setuptools.command.build_ext import build_ext  # as orig_build_ext

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

cSkipList = Extension("cSkipList",
                             sources=[
                                      'cSkipList.cpp',
                                      'SkipList.cpp',
                                      ],
                             include_dirs=['.', ],
                             library_dirs=[os.getcwd(), ],
                             extra_compile_args=extra_compile_args,
                             language='c++',
                             )

setup(name='cSkipList',
      version='0.1.0',
      ext_modules=[cSkipList, ])
