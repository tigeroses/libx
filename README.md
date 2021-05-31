# libx

A library for code reuse in c++ for myself

# Environment

os: centos 7

librarys:

| name    | version | function           |
| ------- | ------- | ------------------ |
| cmake   | 3.17.2  | build the software |
| gcc     | 9.1.0   | build cpp code     |
| doctest | 2.3.7   | unittest           |

## Compile

```sh
# set environments
$ export CC=/path/to/gcc-9.1.0/bin/gcc
$ export CXX=/path/to/gcc-9.1.0/bin/g++
$ export CPLUS_INCLUDE_PATH=/path/to/doctest/include:$CPLUS_INCLUDE_PATH
# compile with cmake
$ mkdir -p build && cd build
$ cmake .. -DINSTALL_PATH=/path/to/source/bin
$ make -j 8
```

## Test

```sh
$./bin/unittest
[doctest] doctest version is "2.3.7"
[doctest] run with "--help" for options
===============================================================================
[doctest] test cases:      1 |      1 passed |      0 failed |      0 skipped
[doctest] assertions:      3 |      3 passed |      0 failed |
[doctest] Status: SUCCESS!
```

## Usage

In the source code:

```cpp
#include <libx/Timer.h>
```