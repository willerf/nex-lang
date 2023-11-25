
mod main;

import print;

fn main(x: i32, y: i32) -> i32 {

    let arr: *i32 = new i32[10];
    arr[0] = 0;
    arr[1] = 1;

    let i: i32 = 2;
    while (i < 10) {
        arr[i] = arr[i-1] + arr[i-2];
        i = i + 1;
    }
    print(arr);
    println("");

    return 0;
}


