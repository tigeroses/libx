# Overview

## Timer

Timer is a tool for timing code blocks.

`include <libx/Timer.hpp>`

```cpp
Timer t1("s");
// do some work
std::cout<<"elapsed time(s): "<<t1.toc()<<std::endl;
```

## String

`include <libx/String.hpp>`

### split string

Split a string into a list of tokens by delimiter.

```cpp
// method 1
std::vector<std::string> v;
libx::split("a:bc:def", ':', v);

// method 2
std::string a;
int b;
double c;
if (libx::split("abc 123 45.678", " ", a, b, c))
    // process
```

### join string

Concatenate a list of tokens by delimiter.

```cpp
// 1. vector<> as input
vector<int> vec(1,2,3);
cout<<libx::join(vec, ' ');

// 2. initializer_list<> as input
cout<<libx::join({"a", "bc", "def"}, "\t");

// 3. variadic template
string out;
int i = 1;
float f = 2.55;
string s = "libx";
libx::join('-', out, i, f, s);
```

### trim characters

Remove substring from the front or back of string.

```cpp
// 1. trim whitespace
string res = libx::trimWhitespace(" a ");

// 2. trim specific characters
auto toTrimAB = [](const char c) { return (c == 'a' || c == 'b'); };
string res;
res = libx::ltrim("abcbad", toTrimAB);
res = libx::rtrim("abcbad", toTrimAB);
res = libx::trim("abcbad", toTrimAB);
```

### check prefix and suffix

Check if a string starts or ends with another string.

```cpp
if (libx::startswith("abcdefg", "abc"))
    // do work

if (libx::endswith(filename, ".txt"))
    // do work
else if (libx::endswith(filename, ".csv"))
    // do work
```

## thread pool

ThreadPool is a thread pool for executing parallel tasks.

`include <libx/ThreadPool.hpp>`

```cpp
libx::ThreadPool thpool(20);

vector<future<int>> results;
for (int i = 0; i < 40; ++i)
    results.emplace_back(thpool.commit(func, params...));

for (auto& result : results)
    int num = result.get();
```

## converter

`include <libx/Conv.hpp>`

### string to numeric

Convert objects from std::string to numeric types.

```cpp
int a = libx::to<int>("123");
```

### numeric to string

Convert objects from numeric types to std::string.

```cpp
std::string s = libx::toStr(123);
```

## system

`include <libx/System.hpp>`

### subprocess

Executing shell script using popen() and pipe.

```cpp
std::string cmd("ls -l 2>&1");
std::string out;
int rtn = libx::subprocess(cmd, out);
```

### compare last write time of two files

```cpp
ofstream(f1.c_str()).put('a');
std::this_thread::sleep_for(std::chrono::milliseconds(100));
ofstream(f2.c_str()).put('b');

if (libx::compareFileTime(f1, f2))
{
    cout<<"make sense, f1 older than f2";
}
else
{
    cout<<"some thing wrong";
}
```

### generate random string

Generate random string include "0-9A-Za-z"

```cpp
int len = 10;
string s2 = libx::genRandomString(len);
```

### get system memory

Get system maximum memory by read system file '/proc/meminfo'

```cpp
int systemMem = libx::getSystemMemory();
```

### get process memory

Get process memory with pid.

```cpp
double      vm, pm;
std::string unit("KB"); // support "KB" "MB" "GB"
int         pid = 1;  // init process, it always exists
libx::getProcessMemory(vm, pm, unit, pid);
```

Get self(current process) memory by wrapping "getProcessMemory()"

```cpp
double usedMem = libx::getSelfMemory();
```

## file IO

### text file

`include <libx/File.hpp>`

#### read text file

Read file line by line, and process each line using the user-specified lambda function

```cpp
vector<string> records;
auto readLine = [&](string& line) {
    records.push_back(line);
};
readFile(filename, readLine, "#", 1);
```

#### write text file

Write data to file, there are two ways:

```cpp
// 1 input string, seperated by '\n'
writeFile("a b c\nd e f\n", filename);

// 2 vector of string or numeric types
// the second value 'ture' means append to file
vector<string> inputsString{"a b c", "d e f"};
writeFile(inputsString, filename);
vector<int> inputsInt{1, 200};
writeFile(inputsInt, filename, true);
```

### gz file

It is same to text file for reading and writing gz file, just 
use readGZFile() and writeGZFile()
