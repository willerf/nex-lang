# NexLang Library

## Table of Contents


## Math Module
Returns the maximum of two integers
```rs
fn max(x: i32, y: i32) -> i32
```
Returns the minimum of two integers
```rs
fn min(x: i32, y: i32) -> i32
```
Returns the absolute value of an integer
```rs
fn abs(x: i32) -> i32
```
Calculates the power of a base integer raised to an exponent
```rs
fn pow(base: i32, exponent: i32) -> i32
```
Calculates the factorial of an integer
```rs
fn factorial(n: i32) -> i32
```
Calculates the greatest common divisor of two integers
```rs
fn gcd(a: i32, b: i32) -> i32
```
Calculates the least common multiple of two integers
```rs
fn lcm(a: i32, b: i32) -> i32
```
Estimates the square root of an integer
```rs
fn sqrt(n: i32) -> i32
```
Determines if an integer is prime
```rs
fn is_prime(n: i32) -> bool
```
Calculates the nth number in the Fibonacci sequence
```rs
fn fibonacci(n: i32) -> i32
```

## Print Module
Prints a string from a pointer to characters
```rs
fn print(word: *char)
```
Prints a newline character
```rs
fn println()
```
Prints a string from a pointer to characters and then a newline
```rs
fn println(word: *char)
```
Prints a single character
```rs
fn print(letter: char)
```
Prints a single character and then a newline
```rs
fn println(letter: char)
```
Prints "true" if the condition is true, "false" otherwise
```rs
fn print(cond: bool)
```
Prints "true" if the condition is true, "false" otherwise, followed by a newline
```rs
fn println(cond: bool)
```
Prints an integer
```rs
fn print(num: i32)
```
Prints an integer followed by a newline
```rs
fn println(num: i32)
```
Prints an array of integers
```rs
fn print(arr: *i32)
```
Prints an array of integers followed by a newline
```rs
fn println(arr: *i32)
```

## String Module
Creates an empty string
```rs
fn String() -> *String
```
Creates a string from a given character array
```rs
fn String(word: *char) -> *String
```
Destructs a string, freeing its resources
```rs
fn destruct(self: *String)
```
Reallocates the string's internal buffer to a new capacity
```rs
fn realloc(self: *String, new_capacity: i32, default: char)
```
Resizes the string to a new size, filling with a default character if necessary
```rs
fn resize(self: *String, size: i32, default: char)
```
Reduces the capacity of the string to fit its size
```rs
fn shrink_to_fit(self: *String)
```
Appends a character to the end of the string
```rs
fn push_back(self: *String, elem: char)
```
Removes and returns the last character of the string
```rs
fn pop_back(self: *String) -> char
```
Returns the first character of the string
```rs
fn front(self: *String) -> char
```
Returns the last character of the string
```rs
fn back(self: *String) -> char
```
Returns a pointer to the character at a specific index
```rs
fn at(self: *String, index: i32) -> *char
```
Returns the character at a specific index
```rs
fn get(self: *String, index: i32) -> char
```
Sets the value of a character at a specific index
```rs
fn set(self: *String, index: i32, value: char)
```
Returns the size of the string
```rs
fn size(self: *String) -> i32
```
Checks if the string is empty
```rs
fn empty(self: *String) -> bool
```
Prints the string
```rs
fn print(self: *String)
```
Prints the string followed by a newline
```rs
fn println(self: *String)
```
Converts the string to a null-terminated character array
```rs
fn c_str(self: *String) -> *char
```

## List Module
Creates an empty list of integers
```rs
fn ListI32() -> *ListI32
```
Destructs a list, freeing its resources
```rs
fn destruct(self: *ListI32)
```
Reallocates the list's internal buffer to a new capacity
```rs
fn realloc(self: *ListI32, new_capacity: i32, default: i32)
```
Resizes the list to a new size, filling with a default value if necessary
```rs
fn resize(self: *ListI32, size: i32, default: i32)
```
Reduces the capacity of the list to fit its size
```rs
fn shrink_to_fit(self: *ListI32)
```
Appends an integer to the end of the list
```rs
fn push_back(self: *ListI32, elem: i32)
```
Removes and returns the last integer of the list
```rs
fn pop_back(self: *ListI32) -> i32
```
Returns the first integer of the list
```rs
fn front(self: *ListI32) -> i32
```
Returns the last integer of the list
```rs
fn back(self: *ListI32) -> i32
```
Returns a pointer to the integer at a specific index
```rs
fn at(self: *ListI32, index: i32) -> *i32
```
Returns the integer at a specific index
```rs
fn get(self: *ListI32, index: i32) -> i32
```
Sets the value of an integer at a specific index
```rs
fn set(self: *ListI32, index: i32, value: i32)
```
Returns the size of the list
```rs
fn size(self: *ListI32) -> i32
```
Checks if the list is empty
```rs
fn empty(self: *ListI32) -> bool
```
Prints the list
```rs
fn print(self: *ListI32)
```
Prints the list followed by a newline
```rs
fn println(self: *ListI32)
```

