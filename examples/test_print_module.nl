mod test;

import print;

fn main(test_code: i32, test_value: i32) -> i32 {
    if (test_code == 1) {
        print(test_value as char);
    }
    if (test_code == 2) {
        println();
    }
    if (test_code == 3) {
        println(test_value as char);
    }
    if (test_code == 4) {
        print(test_value);
    }
    if (test_code == 5) {
        println(test_value);
    }
    if (test_code == 6) {
        let arr = new i32[3];
        arr[0] = 1; arr[1] = 2; arr[2] = 3;
        print(arr);
    }
    if (test_code == 7) {
        let arr = new i32[2];
        arr[0] = 1; arr[1] = 2;
        println(arr);
    }

    return 0;
}

