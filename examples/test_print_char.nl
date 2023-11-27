
mod main;

import print;

fn main(x: i32, y: i32) -> i32 {
    
    print('H');
    let e: char = 'e';
    print(e);
    let c: i32 = 108;
    print(c as char);
    let ptr: *char = (&c) as (*char);
    print(*ptr);
    c = 'o' as i32;
    print(*ptr);
    print(10 as char);

    return 0;
}

