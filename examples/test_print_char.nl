
mod main;

import print;

fn main(x: i32, y: i32) -> i32 {
    
    print_char('H');
    let e: char = 'e';
    print_char(e);
    let c: i32 = 108;
    print_char(c as char);
    let ptr: *char = (&c) as (*char);
    print_char(*ptr);
    c = 'o' as i32;
    print_char(*ptr);
    print_char(10 as char);

    return 0;
}

