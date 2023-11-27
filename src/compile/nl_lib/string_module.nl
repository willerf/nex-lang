R"(
mod string;

import print;
import math;

struct String {
    data: *char;
    size: i32;
    capacity: i32;
}

fn String() -> *String {
    let list = new String;
    list.data = new char[1];
    list.size = 0;
    list.capacity = 1;
    return list;
}

fn String(word: *char) -> *String {
    let str = String();
    let i = 0;
    while (word[i] != 0 as char) {
        str.push_back(word[i]);
        i = i + 1;
    }
    return str;
}

fn destruct(self: *String) {
    delete self.data;
    delete self;
}

fn realloc(self: *String, new_capacity: i32, default: char) {
    let new_data = new char[new_capacity];
    let i = 0;
    while (i < new_capacity) {
        if (i < self.size) {
            new_data[i] = self.data[i];
        }
        else {
            new_data[i] = default;
        }
        i = i + 1;
    }
    delete self.data;
    self.data = new_data;
    self.capacity = new_capacity;
}

fn resize(self: *String, size: i32, default: char) {
    if (self.capacity != size) {
        self.realloc(size, default);
    }
    self.size = size;
}

fn shrink_to_fit(self: *String) {
    if (self.capacity != self.size) {
        self.realloc(self.size, ' ');
    }
}

fn push_back(self: *String, elem: char) {
    if (self.size == self.capacity) {
        let new_capacity = self.capacity * 2;
        self.realloc(new_capacity, ' ');
    }
    self.data[self.size] = elem;
    self.size = self.size + 1;
}

fn pop_back(self: *String) -> char {
    let result = self.data[self.size - 1];
    self.size = self.size - 1;
    return result;
}

fn front(self: *String) -> char {
    return self.data[0];
}

fn back(self: *String) -> char {
    return self.data[self.size - 1];
}

fn at(self: *String, index: i32) -> *char {
    return (self.data as i32 + 4 * index) as *char;
}

fn get(self: *String, index: i32) -> char {
    return self.data[index];
}

fn set(self: *String, index: i32, value: char) {
    self.data[index] = value;
}

fn size(self: *String) -> i32 {
    return self.size;
}

fn empty(self: *String) -> bool {
    return self.size == 0;
}

fn print(self: *String) {
    let i = 0;
    while (i < self.size) {
        print(self.data[i]);
        i = i + 1;
    }
}

fn println(self: *String) {
    self.print();
    println();
}

fn c_str(self: *String) -> *char {
    let result = new char[self.size + 1];
    let i = 0;
    while (i < self.size) {
        result[i] = self.data[i];
        i = i + 1;
    }
    result[self.size] = 0 as char;
    return result;
}
)"
