# libx

自用c++库,包含标准库尚未实现或较难使用的一些功能,如字符串分割,计时,调用外部脚本等.

A library for code reuse in c++ for myself.

## Environment

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

## Coverage

Before generating code coverage report, you need install **lcov-1.5**, the 
lower version do not fit gcc-9.1.0.

The following commands will generate a directory named *libx_coverage*, open 
*index.html* to get coverage html report.

```sh
$ cd /path/to/build
$ lcov --rc lcov_branch_coverage=1 -d . -c -o info_tmp
$ lcov --rc lcov_branch_coverage=1 -e info_tmp "*libx/include*" -o info
$ genhtml --rc lcov_branch_coverage=1 -o libx_coverage info
$ rm info_tmp info
```

## Usage

In the source code:

```cpp
#include <libx/Timer.hpp>
```

## Code style

Use clang-format to control the code style.

```sh
sh script/check_style.sh
```

## Reference

* [threadpool](https://github.com/lzpong/threadpool)
* [String](https://github.com/facebook/folly)