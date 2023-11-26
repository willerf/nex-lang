R"(
mod math;

fn max(x: i32, y: i32) -> i32 {
    if (x > y) {
        return x;
    }
    else {
        return y;
    }
}

fn min(x: i32, y: i32) -> i32 {
    if (x < y) {
        return x;
    }
    else {
        return y;
    }
}

fn abs(x: i32) -> i32 {
    if (x < 0) {
        return 0-x;
    } else {
        return x;
    }
}

fn pow(base: i32, exponent: i32) -> i32 {
    if (exponent == 0) {
        return 1;
    } else {
        let result = 1;
        let count = 0;
        while (count < exponent) {
            result = result * base;
            count = count + 1;
        }
        return result;
    }
}

fn factorial(n: i32) -> i32 {
    if (n == 0) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

fn gcd(a: i32, b: i32) -> i32 {
    if (b == 0) {
        return a;
    } else {
        return gcd(b, a % b);
    }
}

fn lcm(a: i32, b: i32) -> i32 {
    return (a / gcd(a, b)) * b;
}

fn sqrt(n: i32) -> i32 {
    let x = n;
    let y = (x + 1) / 2;
    while (y < x) {
        x = y;
        y = (x + n / x) / 2;
    }
    return x;
}

fn is_prime(n: i32) -> bool {
    if (n <= 1) {
        return false;
    } else {
        let i = 2;
        while (i * i <= n) {
            if (n % i == 0) {
                return false;
            }
            i = i + 1;
        }
        return true;
    }
}

fn fibonacci(n: i32) -> i32 {
    if (n <= 1) {
        return n;
    } else {
        return fibonacci(n - 1) + fibonacci(n - 2);
    }
}
)"
