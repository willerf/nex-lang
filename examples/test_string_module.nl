mod test;

import string;
import print;
import math;

fn main(test_code: i32, test_value: i32) -> i32 {
    let str = String("Hello");
    if (test_code == 1) {
        str.println();
        str.push_back('W');
        str.push_back('o');
        str.push_back('r');
        str.println();
    }

    if (test_code == 2) {
        str.push_back('W');
        str.push_back('o');
        str.println();
        println(str.pop_back());
        str.println();
        println(str.pop_back());
        str.println();
    }

    if (test_code == 3) {
        str.push_back('W');
        str.push_back('o');
        str.push_back('r');
        str.println();
        println(str.front());
        println(str.back());
    }

    if (test_code == 4) {
        str.push_back('W');
        str.push_back('o');
        str.push_back('r');
        str.push_back('l');
        str.println();
        println(str.get(0));
        println(str.get(1));
        println(str.get(2));
        println(str.get(5));
        str.set(2, 'm');
        str.println();
        str.set(7, 'p');
        str.println();
    }

    if (test_code == 5) {
        str.push_back('W');
        str.push_back('o');
        str.push_back('r');
        str.println();
        let ptr = str.at(0);
        *ptr = 'k';
        str.println();
        *(str.at(2)) = 'f';
        str.println();
    }

    if (test_code == 6) {
        str.destruct();
        str = String();
        println(str.empty());
        println(str.size());
        str.push_back('W');
        println(str.empty());
        println(str.size());
        str.push_back('o');
        println(str.empty());
        println(str.size());
        str.push_back('r');
        println(str.empty());
        println(str.size());
        str.pop_back();
        println(str.empty());
        println(str.size());
        str.pop_back();
        println(str.empty());
        println(str.size());
        str.pop_back();
        println(str.empty());
        println(str.size());
    }

    if (test_code == 7) {
        str.push_back('W');
        str.push_back('o');
        str.push_back('r');
        str.push_back('l');
        str.push_back('d');
        str.println();
        let c = str.c_str();
        println(c);
        delete c;
    }

    if (test_code == 8) {
        println(str.capacity);
        str.destruct();
        str = String();
        println(str.capacity);
        str.push_back('a');
        println(str.capacity);
        str.push_back('a');
        println(str.capacity);
        str.push_back('a');
        println(str.capacity);
        str.push_back('a');
        println(str.capacity);
        str.push_back('a');
        println(str.capacity);
        str.shrink_to_fit();
        println(str.capacity);
    }

    if (test_code == 9) {
        str.println();
        str.resize(3, 'a');
        str.println();
        str.resize(8, 'a');
        str.println();
    }

    str.destruct();
    return 0;
}
