R"(
mod list;

import print;
import math;

struct ListI32 {
    data: *i32;
    size: i32;
    capacity: i32;
}

fn ListI32() -> *ListI32 {
    let list = new ListI32;
    list.data = new i32[1];
    list.size = 0;
    list.capacity = 1;
    return list;
}

fn destruct(self: *ListI32) {
    delete self.data;
    delete self;
}

fn realloc(self: *ListI32, new_capacity: i32) {
    let new_data = new i32[new_capacity];
    let i = 0;
    while (i < min(self.size, new_capacity)) {
        new_data[i] = self.data[i];
        i = i + 1;
    }
    delete self.data;
    self.data = new_data;
    self.capacity = new_capacity;
}

fn resize(self: *ListI32, size: i32) {
    self.size = size;
    if (self.capacity != size) {
        self.realloc(size);
    }
}

fn shrink_to_fit(self: *ListI32) {
    if (self.capacity != self.size) {
        self.realloc(self.size);
    }
}

fn push_back(self: *ListI32, elem: i32) {
    if (self.size == self.capacity) {
        let new_capacity = self.capacity * 2;
        self.realloc(new_capacity);
    }
    self.data[self.size] = elem;
    self.size = self.size + 1;
}

fn pop_back(self: *ListI32) -> i32 {
    let result = self.data[self.size - 1];
    self.size = self.size - 1;
    return result;
}

fn front(self: *ListI32) -> i32 {
    return self.data[0];
}

fn back(self: *ListI32) -> i32 {
    return self.data[self.size - 1];
}

fn at(self: *ListI32, index: i32) -> *i32 {
    return (self.data as i32 + 4 * index) as *i32;
}

fn get(self: *ListI32, index: i32) -> i32 {
    return self.data[index];
}

fn set(self: *ListI32, index: i32, value: i32) {
    self.data[index] = value;
}

fn size(self: *ListI32) -> i32 {
    return self.size;
}

fn empty(self: *ListI32) -> bool {
    return self.size == 0;
}

fn print(self: *ListI32) {
    if (self.size == 0) {
        print("[]");
    }
    else {
        let i = 0;
        print('[');
        while (i < self.size - 1) {
            print(self.data[i]);
            print(", ");
            i = i + 1;
        }
        print(self.back());
        print(']');
    }
}

fn println(self: *ListI32) {
    self.print();
    println();
}
)"
