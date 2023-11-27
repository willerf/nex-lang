
mod test;

import math;
import print;

fn main(x: i32, y: i32) -> i32 {
    if (x == 1) {
        return (abs(y));
    } else {
        if (x == 2) {
            return (pow(y, 2));  // Testing the power function with a fixed exponent of 2
        } else {
            if (x == 3) {
                return (factorial(y));
            } else {
                if (x == 4) {
                    return (gcd(y, 10));  // Testing GCD with a fixed value of 10
                } else {
                    if (x == 5) {
                        return (lcm(y, 10));  // Testing LCM with a fixed value of 10
                    } else {
                        if (x == 6) {
                            return (sqrt(y));
                        } else {
                            if (x == 7) {
                                if (is_prime(y)) {
                                    println("true");
                                } else {
                                    println("false");
                                }
                            } else {
                                if (x == 8) {
                                    return (fibonacci(y));
                                } else {
                                    println("Invalid option");
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return 0;
}

