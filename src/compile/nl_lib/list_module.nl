R"(
mod list;

import math;
import print;

fn list_init() -> **i32 {
    let list: **i32 = new *i32[2];
    list[0] = new i32[1];
    list[1] = new i32;
    (*list[1]) = 1;
    return list;
}

fn list_append(list: **i32, elem: i32) {
    let size: i32 = list[0][-1] / 4;
    let len: i32 = (*list[1]);
    if (len >= size) {
        list_resize(list, size * 2);
    }
    list[0][len] = elem;
    (*list[1]) = len + 1;
}

fn list_resize(list: **i32, size: i32) {
    let arr: *i32 = new i32[size];
    let i = 0;
    let end = min(size, *list[1]);
    while (i < end) {
        arr[i] = list[0][i];
        i = i + 1;
    }
    delete list[0];
    list[0] = arr;
}

fn list_ptr(list: **i32) -> *i32 {
    return list[0];
}

fn list_delete(list: **i32) {
    delete list[0];
    delete list[1];
    delete list;
}

fn list_print(list: **i32) {
    let len: i32 = (*list[1]);
    let i = 0;
    while (i < len) {
        print_num(list[0][i]);
        print_char(' ');
        i = i + 1;
    }
    println("");
}
)"
