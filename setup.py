#!/bin/env python
import os

from setuptools import setup, find_packages
from distutils.core import Extension

# Makefile:
# CFLAGS = -Wall -Wextra
# CFLAGS_RELEASE = $(CFLAGS) -O2 -DNDEBUG
# CFLAGS_DEBUG = $(CFLAGS) -g3 -O0 -DDEBUG=1
extra_compile_args = [
    '-Wall',
    '-Wextra',
    '-Werror',
    '-Wfatal-errors',
    # Some internal Python library code does not like this.
    '-Wno-c++11-compat-deprecated-writable-strings',
    '-std=c++11',
    '-Isrc/cpp',
    # We implement mutex with Python's thread locking so we don't want the
    # overhead of C++'s thread locking as well.
    '-USKIPLIST_THREAD_SUPPORT',
    '-Wno-stdlibcxx-not-found',
]

DEBUG = False

if DEBUG:
    extra_compile_args.extend(['-g3', '-O0', '-DDEBUG=1'])
else:
    extra_compile_args.extend(['-g3', '-O2', '-DNDEBUG'])

orderedstructs = Extension(
    "orderedstructs",
    sources=[
        'src/cpy/cOrderedStructs.cpp',
        'src/cpy/OrderedStructs.cpp',
        'src/cpy/cSkipList.cpp',
        'src/cpy/cmpPyObject.cpp',
        'src/cpp/SkipList.cpp',
    ],
    include_dirs=[
        '.',
        '/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1',
    ],
    library_dirs=[os.getcwd(), ],
    extra_compile_args=extra_compile_args,
    extra_link_args=['-lc++'],
    language='c++11',
)

with open('README.md') as readme_file:
    readme = readme_file.read()

with open('HISTORY.md') as history_file:
    history = history_file.read()

requirements = [
]

setup_requirements = [
    'pytest-runner',
]

test_requirements = [
    'pytest',
    'hypothesis',
]

setup(
    name='orderedstructs',
    version='0.3.2',
    ext_modules=[orderedstructs, ],
    description="Contains a variety of ordered structures, in particular a SkipList.",
    long_description=readme + '\n\n' + history,
    long_description_content_type = 'text/markdown',
    author="Paul Ross",
    author_email='apaulross@gmail.com',
    url='https://github.com/paulross/skiplist',
#     packages=find_packages('src'),
    license="MIT License",
    keywords=['orderedstructs', 'SkipList'],
    classifiers=[
        'Development Status :: 2 - Pre-Alpha',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: MIT License',
        'Natural Language :: English',
        "Programming Language :: Python :: 2",
        'Programming Language :: Python :: 2.7',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.3',
        'Programming Language :: Python :: 3.4',
        'Programming Language :: Python :: 3.5',
        'Programming Language :: Python :: 3.6',
        'Programming Language :: Python :: 3.7',
        'Programming Language :: Python :: 3.8',
        'Programming Language :: Python :: 3.9',
    ],
    test_suite='tests',
    tests_require=test_requirements,
    setup_requires=setup_requirements,
)
