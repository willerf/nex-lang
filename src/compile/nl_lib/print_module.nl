R"(
mod print;

fn print(word: *char) {
    while ((*word) != (0 as char)) {
        print(*word);
        word = ((word as i32) + 4) as (*char);
    }
}

fn println(word: *char) {
    print(word);
    print(10 as char);
}

fn print(letter: char) {
    let print_addr: *char = (-65524) as *char;
    (*print_addr) = letter;
}

fn println(letter: char) {
    print(letter);
    print(10 as char);
}

fn print(num: i32) {
    if (num == 0) {
        print('0');
    }
    else {
        if (num < 0) {
            print('-');
            num = 0 - num;
        }
        let tens: i32 = 1;
        let copy: i32 = num;
        while (copy > 0) {
            tens = tens * 10;
            copy = copy / 10;
        }
        tens = tens / 10;

        while (tens > 0) {
            let dig: i32 = (num / tens) % 10;
            print((dig + ('0' as i32)) as char);
            tens = tens / 10;
        }
    }
}

fn println(num: i32) {
    print(num); 
    print(10 as char);
}

fn print(arr: *i32) {
    let size: i32 = arr[-1];
    let len: i32 = size / 4;
    let i: i32 = 0;
    while (i < len) {
        print(arr[i]);
        print(' ');
        i = i + 1;
    }
}

fn println(arr: *i32) {
    print(arr); 
    print(10 as char);
}
)"
