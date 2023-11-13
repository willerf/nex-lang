
mod main;

fn main(x: i32, y: i32) -> i32 {
    fib::calc_fibonacci(x);
    print::printchar(10 as char);
    return 0;
}

