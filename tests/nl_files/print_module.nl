
mod print;

fn printchar(letter: char) {
    let print_addr: *char = (0 - 65524) as (*char);
    (*print_addr) = letter;
}

fn print(word: *char) {
    while ((*word) != (0 as char)) {
        printchar(*word);
        word = ((word as i32) + 4) as (*char);
    }
}

fn println(word: *char) {
    print(word);
    printchar(10 as char);
}

fn printnum(num: i32) {
    if (num == 0) {
        printchar('0');
    }
    else {
        if (num < 0) {
            printchar('-');
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
            printchar((dig + ('0' as i32)) as char);
            tens = tens / 10;
        }
    }
}
