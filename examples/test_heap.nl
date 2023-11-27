
mod main;

import print;

fn main(x: i32, y: i32) -> i32 {
    let a: *i32 = new i32[x];
    let b: *i32 = new i32;
    let c: *i32 = new i32[10];

    let cond: bool = ((a as i32 + 4 * (x + 1)) == (b as i32));
    print(cond as i32);
    println("");

    cond = ((b as i32 + 4 * (1 + 1)) == (c as i32));
    print(cond as i32);
    println("");

    delete a;
    let d: *i32 = new i32[y];
    
    cond = d == a;
    print(cond as i32);
    println("");

    return 0;
}


