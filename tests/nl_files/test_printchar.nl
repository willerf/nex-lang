
mod main;

fn main(x: i32, y: i32) -> i32 {
    
    print::printchar('H');
    let e: char = 'e';
    print::printchar(e);
    let c: i32 = 108;
    print::printchar(c as char);
    let ptr: *char = (&c) as (*char);
    print::printchar(*ptr);
    c = 'o' as i32;
    print::printchar(*ptr);
    print::printchar(10 as char);

    return 0;
}

