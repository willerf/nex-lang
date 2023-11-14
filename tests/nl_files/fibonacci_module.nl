
mod fib;

fn calc_fibonacci(count: i32) {
    let found: i32 = 0;
    let curFib: i32 = 0;
    let nextFib: i32 = 1;
    while (found < count) {
        print::printnum(curFib);
        print::print(" ");
        let newNextFib: i32 = curFib + nextFib;
        curFib = nextFib;
        nextFib = newNextFib;
        found = found + 1;
    }
}
