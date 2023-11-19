
mod fib;

import print;

fn calc_fibonacci(count: i32) {
    let found: i32 = 0;
    let curFib: i32 = 0;
    let nextFib: i32 = 1;
    while (found < count) {
        printnum(curFib);
        print(" ");
        let newNextFib: i32 = curFib + nextFib;
        curFib = nextFib;
        nextFib = newNextFib;
        found = found + 1;
    }
}
