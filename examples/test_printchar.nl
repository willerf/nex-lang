
mod main;

import print;

fn main(x: i32, y: i32) -> i32 {
    
    printchar('H');
    let e: char = 'e';
    printchar(e);
    let c: i32 = 108;
    printchar(c as char);
    let ptr: *char = (&c) as (*char);
    printchar(*ptr);
    c = 'o' as i32;
    printchar(*ptr);
    printchar(10 as char);

    return 0;
}

