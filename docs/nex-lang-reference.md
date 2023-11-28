# Reference

## Table of Contents
- [Data Types](#data-types)
- [Variable Instantiation](#variable-instantiation)
- [Supported Operations](#supported-operations)
- [Heap Allocated Structs and Arrays](#heap-allocated-structs-and-arrays)
- [Control Flow](#control-flow)
- [Functions](#functions)
- [Pointers and Addressing](#pointers-and-addressing)
- [Char and String Literals](#char-and-string-literals)
- [Module System](#module-system)


### Data Types 
- Integer (i32): 32-bit integer.
- Boolean (bool): Boolean value (true/false).
- Character (char): Single character.
- Pointer (*): Pointer to a data type.

### Variable Instantiation
Variables can be instantiated via the `let` keyword by its name, an optionally specified type and its initial value.
If the type is not specified, it will be inferred by the expression on the right hand side.
#### Typed Specified
```rs
let var_name: i32 = 7;  
```
#### Typed Inferred
```rs
// type inferred to be i32
let my_int = 7;
// typed inferred to be char
let my_char = 'a';
```

### Supported Operations
- Boolean: Logical AND (&&), OR (||), NOT (!).
- Comparison: Equality (==), Inequality (!=), Less (<), Greater (>), Less or Equal (<=), Greater or Equal (>=).
- Arithmetic: Addition (+), Subtraction (-), Multiplication (*), Division (/), Modulus (%).
- Addressing: Address-of (&), Dereference (*).

### Heap Allocated Structs and Arrays
Structures and arrays can only be heap allocated. 
The `new` keyword is used to allocate memory on the heap and the `delete` keyword frees the memory.
#### Struct Definition
```rs
struct MyStruct {
  member_one: i32;
  member_two: char;
  // ... additional members added here
}
```
#### Struct Methods
```rs
fn init_struct(self: *MyStruct) {
  self.member_one = 0;
  self.member_two = ' ';
}
```
#### Using Structs
```rs
// type inferred to be (*MyStruct)
let my_struct = new MyStruct;
my_struct.init_struct();
// write to struct
my_struct.member_one = 5;
my_struct.member_two = 'h';
// read from struct
let var = my_struct.member_one;
// cleanup memory
delete my_struct;
```
#### Using Arrays 
```rs
// heap allocate array of 10 integers
let num_ints = 10;
let arr = new i32[num_ints];
// write to array
arr[3] = 12;
// read from array (var is now 12)
let var = arr[3];
// cleanup memory
delete arr;
```

### Control Flow
Currently only if else statements and while loops are supported.
#### Branching
```rs
if (condition) {
  ...
}
else {
  ...
}
```
#### Loops
```rs
while (condition) {
  ...
}
```

### Functions
Functions can be defined as seen below with a name, list of parameters and optional return type.
Recursion is also supported.
```rs
// function definition with single i32 parameter and return type of i32 specified after the arrow
fn factorial(n: i32) -> i32 {
    if (n == 0) {
        return 1;
    } else {
        // recursion support
        return n * factorial(n - 1);
    }
}
```

### Pointers and Addressing
- Ampersand (&) used to get pointer to variable.
- Star (*) used to dereference pointer.
```rs
let x = 10;
// & used to get pointer to variable
let ptr_to_x = &x;
// assignment, variable x is now 5
*ptr_to_x = 5; 
```

### Char and String Literals
- Char Literals: Single characters, e.g., 'a'.
- String Literals: Arrays of characters, e.g., "Hello World".
```rs
let my_char = 'a';
// type of (*char)
let my_string = "Hello World";
```

### Module System
Each file compiled is considered a module. 
Every file should begin with the mod keyword followed by the name of the module.
It can then be imported into other files.
```rs
// my_file.nl
mod my_custom_module;
fn add(a: i32, b: i32) -> i32 {
  return a + b;
}

// main.nl
mod main;
// import module from above
import my_custom_module;

fn main(x: i32, y: i32) -> i32 {
  // function from my_custom_module is now accessible
  let result = add(5, 7);
}
```
To compile both files into the project, we can include all file paths in our compilation command.
`./cnl main.nl my_file.nl`
