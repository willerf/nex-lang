# Welcome to NexLang

### Table of Contents
- [About](https://github.com/willerf/nex-lang#about)
- [Examples](https://github.com/willerf/nex-lang#examples)
  - [Example Print Implementation](https://github.com/willerf/nex-lang#example-print-implementation)
  - [Example Power Implementation](https://github.com/willerf/nex-lang#example-power-implementation)
- [Getting Started](https://github.com/willerf/nex-lang#getting-started)
  - [Install & Build](https://github.com/willerf/nex-lang#install--build)
  - [User Guide](https://github.com/willerf/nex-lang#user-guide)

### About
NexLang is a custom systems programming language focused on providing strong control over the low level fundamentals of your program while bringing a modern syntax and robust type checking. Currently, this compiler targets the MIPS assembly language instruction set and uses an emulator to run programs and unit tests. Additional architectures should be relatively straight forward to port over to and RISC-V support is currently in the works.

### Examples

#### Example Print Implementation
```rs
fn printchar(letter: char) {
    let print_addr: *char = (0 - 65524) as (*char);
    (*print_addr) = letter;
}

fn print(word: *char) {
    while ((*word) != (0 as char)) {
        printchar(*word);
        word = ((word as i32) + 4) as (*char);
    }
}

fn println(word: *char) {
    print(word);
    printchar(10 as char);
}

fn main(x: i32, y: i32) -> i32 {
    println("Hello World!");
    return 0;
}
```
#### Example Power Implementation
```rs
fn power(base: i32, exponent: i32) -> i32 {
    if (exponent == 1) {
        return base;
    }
    else {
        return base * power(base, exponent - 1);
    }
}

fn main(x: i32, y: i32) -> i32 {
    return power(x, y);
}
```

### Getting Started

#### Install & Build
- Ensure you have cmake, rust and C++20 installed
- Then run the following commands
  - `mkdir build`
  - `cd build`
  - `cmake ..`
  - `make -j8`
- Or just copy and paste the following command
```bash
mkdir build && cd build && cmake .. && make -j8
```
- To verify everything is working correctly, run `./tests/tests` to run the unit tests.

#### User Guide
Within the build directly, you will find an executable named cnl. To compile your first program, simply run the executable and provide the filepath as an argument.
```bash
./cnl main.nl
```
You can also optionally specify an output path using the `-o` flag.
```bash
./cnl main.nl -o main.bin
```
The emulator is currently located within the dependencies folder. Either move the executable elsewhere or it can be run from the build directory as seen below. In addition to providing a path to the compiled binary, you must also provide two integers which are supplied to the main function of the program.
```bash
./_deps/rust_emulator-src/target/release/mips_emulator main.bin 3 5
```
