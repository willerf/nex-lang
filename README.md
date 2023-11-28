# Welcome to NexLang

### Table of Contents
- [About](https://github.com/willerf/nex-lang#about)
- [Examples](https://github.com/willerf/nex-lang#examples)
  - [Example Hello World](https://github.com/willerf/nex-lang#example-hello-world)
  - [Example Print Max](https://github.com/willerf/nex-lang#example-print-max)
  - [More Examples](https://github.com/willerf/nex-lang#more-examples)
- [Getting Started](https://github.com/willerf/nex-lang#getting-started)
  - [Install & Build](https://github.com/willerf/nex-lang#install--build)
  - [User Guide](https://github.com/willerf/nex-lang#user-guide)

### About
NexLang is a custom systems programming language focused on providing strong control over the low level fundamentals of your program while bringing a modern syntax and robust type checking. Currently, this compiler targets the MIPS assembly language instruction set and uses an emulator to run programs and unit tests. Additional architectures should be relatively straight forward to port over to and RISC-V support is currently in the works.

### Examples

#### Example Hello World
```rs
mod main;

import print;

fn main(x: i32, y: i32) -> i32 {
  println("Hello World!");
  return 0;
}
```
#### Example Print Max
```rs
mod main;

import print;
import math;

fn main(x: i32, y: i32) -> i32 {
  // type is inferred
  let z = max(x, y);
  // type can also be explicitly defined
  let w: i32 = z;
  println_num(z);
  return 0;
}
```
#### More Examples
Additional examples can be found within the examples directory.

### Getting Started

#### Install & Build
- Ensure you have [cmake](https://cmake.org/download), [rust](https://www.rust-lang.org/tools/install) and C++20 installed
  - To verify rust was installed correctly, run `rustc --version`
  - To verify cmake was installed correctly, run `cmake --version`
- Clone the repo and `cd` into the project directory
- Then run the following command(s)
```bash
mkdir build && cd build && cmake .. && make -j8
```
- To verify everything is working correctly, run `./tests/tests` to run the unit tests

#### User Guide
Within the build directly, you will find an executable named cnl. To compile your first program, simply run the executable and provide the filepath as an argument.
```bash
./cnl main.nl
```
You can also optionally specify an output path using the `-o` flag.
```bash
./cnl main.nl -o main.bin
```
The built binary can be run by using the provided emulator located in the build directory. In addition to providing a path to the compiled binary, you must also provide two integers which are supplied to the main function of the program.
```bash
./emulate main.bin 3 5
```
