
mod main;

import print;

fn main(x: i32, y: i32) -> i32 {
    
    let newline: char = 10 as char;
    print("Hello World!");
    print(newline);
    let word: *char = "Hello World Again!";
    print(word);
    print(newline);

    return 0;
}

