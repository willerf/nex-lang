
mod main;

import print;
import math;

fn main(x: i32, y: i32) -> i32 {

    let a = 10;
    let b = 17;
    let c = a + b;
    let d = max(10, 17);
    let e = min(10, 17);

    let f = 'f';
    print_char(f);
    let g = "hello world";
    println(g);
    let h = a < b;
    if (h) {
        let i = "ok";
        print(i);
    }


    return 0;
}

