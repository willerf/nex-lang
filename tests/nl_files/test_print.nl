
mod main;

fn main(x: i32, y: i32) -> i32 {
    
    let newline: char = 10 as char;
    print::print("Hello World!");
    print::printchar(newline);
    let word: *char = "Hello World Again!";
    print::print(word);
    print::printchar(newline);

    return 0;
}

