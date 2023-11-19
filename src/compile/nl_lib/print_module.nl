R"(
mod print;

fn print(word: *char) {
    while ((*word) != (0 as char)) {
        print_char(*word);
        word = ((word as i32) + 4) as (*char);
    }
}

fn println(word: *char) {
    print(word);
    print_char(10 as char);
}

fn print_char(letter: char) {
    let print_addr: *char = (-65524) as *char;
    (*print_addr) = letter;
}

fn println_char(letter: char) {
    let print_addr: *char = (-65524) as *char;
    (*print_addr) = letter;
    (*print_addr) = 10 as char;
}

fn print_num(num: i32) {
    if (num == 0) {
        print_char('0');
    }
    else {
        if (num < 0) {
            print_char('-');
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
            print_char((dig + ('0' as i32)) as char);
            tens = tens / 10;
        }
    }
}

fn println_num(num: i32) {
    if (num == 0) {
        print_char('0');
    }
    else {
        if (num < 0) {
            print_char('-');
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
            print_char((dig + ('0' as i32)) as char);
            tens = tens / 10;
        }
    }
    print_char(10 as char);
}

fn print_arr(arr: *i32) {
    let size: i32 = arr[-1];
    let len: i32 = size / 4;
    let i: i32 = 0;
    while (i < len) {
        print_num(arr[i]);
        print_char(' ');
        i = i + 1;
    }
}

fn println_arr(arr: *i32) {
    let size: i32 = arr[-1];
    let len: i32 = size / 4;
    let i: i32 = 0;
    while (i < len) {
        print_num(arr[i]);
        print_char(' ');
        i = i + 1;
    }
    print_char(10 as char);
}
)"
