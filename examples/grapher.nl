mod main;

import print;
import math;

fn main(x: i32, y: i32) -> i32 {
  graph_it(x, x);
  return 0;
}

fn my_func(x: i32) -> i32 {
    return x*x;
}

fn graph_it(a: i32, b: i32) {
    let x_width = 2 * a + 1;
    let y_width = 2 * b + 1;

    let x_start = 0-a;
    let x_end = a;

    let y_start = 0-b;
    let y_end = b;



    let arr = new *char[x_width];
    let i = 0;
    while (i < x_width) {
        arr[i] = new char[y_width];
        let j = 0;
        while (j < y_width) {
            arr[i][j] = ' ';
            j = j + 1;
        }
        i = i + 1;
    }

    i = 0;
    while (i < x_width) {
        let j = 0;
        while (j < y_width) {
            arr[i][j] = ' ';
            if (i + x_start == 0 && j + y_start == 0) {
                arr[i][j] = '+';
            }
            if (i + x_start == 0 && j + y_start != 0) {
                arr[i][j] = '-';
            }
            if (i + x_start != 0 && j + y_start == 0) {
                arr[i][j] = '|';
            }
            
            j = j + 1;
        }
        i = i + 1;
    }

    

    let x = x_start;

    while (x <= x_end) {
        let y = 0-my_func(x);
        if (y >= y_start && y <= y_end) {
            arr[y - y_start][x - x_start] = '*';
        }
        x = x + 1;
    }

    i = 0;
    while (i < x_width) {
        let j = 0;
        while (j < y_width) {
            print(arr[i][j]);
            if (i + x_start == 0) {
                print('-');
            }
            else {
                print(' ');
            }
            j = j + 1;
        }
        println("");
        i = i + 1;
    }

    i = 0;
    while (i < x_width) {
        delete arr[i];
        i = i + 1;
    }
    delete arr;
}
