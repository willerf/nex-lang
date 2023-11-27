mod test;

import list;
import print;
import math;

fn main(test_code: i32, test_value: i32) -> i32 {
    let list = ListI32();
    if (test_code == 1) {
        list.println();
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        list.println();
    }

    if (test_code == 2) {
        list.push_back(1);
        list.push_back(2);
        list.println();
        println(list.pop_back());
        list.println();
        println(list.pop_back());
        list.println();
    }

    if (test_code == 3) {
        list.push_back(4);
        list.push_back(5);
        list.push_back(6);
        list.println();
        println(list.front());
        println(list.back());
    }

    if (test_code == 4) {
        list.push_back(7);
        list.push_back(8);
        list.push_back(5);
        list.push_back(12);
        list.println();
        println(list.get(0));
        println(list.get(1));
        println(list.get(2));
        println(list.get(3));
        list.set(2, 17);
        list.println();
        list.set(0, 25);
        list.println();
    }

    if (test_code == 5) {
        list.push_back(5);
        list.push_back(7);
        list.push_back(9);
        list.println();
        let ptr = list.at(0);
        *ptr = 8;
        list.println();
        *(list.at(2)) = 12;
        list.println();
    }

    if (test_code == 6) {
        println(list.empty());
        println(list.size());
        list.push_back(2);
        println(list.empty());
        println(list.size());
        list.push_back(3);
        println(list.empty());
        println(list.size());
        list.push_back(5);
        println(list.empty());
        println(list.size());
        list.pop_back();
        println(list.empty());
        println(list.size());
        list.pop_back();
        println(list.empty());
        println(list.size());
        list.pop_back();
        println(list.empty());
        println(list.size());
    }

    if (test_code == 7) {
        println(list.capacity);
        list.push_back(0);
        println(list.capacity);
        list.push_back(0);
        println(list.capacity);
        list.push_back(0);
        println(list.capacity);
        list.push_back(0);
        println(list.capacity);
        list.push_back(0);
        println(list.capacity);
        list.shrink_to_fit();
        println(list.capacity);
    }

    if (test_code == 8) {
        println(list.capacity);
        list.println();
        list.push_back(3);
        println(list.capacity);
        list.println();
        list.push_back(5);
        println(list.capacity);
        list.println();
        list.push_back(4);
        println(list.capacity);
        list.println();
        list.resize(9, 0);
        println(list.capacity);
        list.println();
        list.resize(1, 0);
        println(list.capacity);
        list.println();
    }

    list.destruct();
    return 0;
}

