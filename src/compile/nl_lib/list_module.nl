R"(
mod list;

import print;

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

fn delete(self: *ListI32) {
    free self.data;
    free self;
}

fn push_back(self: *ListI32, elem: i32) {
    if (self.size == self.capacity) {
        self.capacity = self.capacity * 2;
        let data = new i32[self.capacity];
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

fn at(self: *ListI32, index: i32) -> i32 {
    return self.data[index];
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
