R"(
mod string;

import print;

struct String {
    data: *char;
    size: i32;
    capacity: i32;
}

fn String() -> *String {
    let str = new String;
    str.data = new char[1];
    str.size = 0;
    str.capacity = 1;
    return str;
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

fn delete(self: *String) {
    free self.data;
    free self;
}

fn push_back(self: *String, elem: char) {
    if (self.size == self.capacity) {
        self.capacity = self.capacity * 2;
        let data = new char[self.capacity];
        let i = 0;
        while (i < self.size) {
            data[i] = self.data[i];
            i = i + 1;
        }
        free self.data;
        self.data = data;
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

fn at(self: *String, index: i32) -> char {
    return self.data[index];
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
