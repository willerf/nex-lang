
use std::env;

use crate::memory::Memory;
use crate::mips_state::MIPSState;

pub mod memory;
pub mod mips_state;
pub mod mips_cpu;

fn main() {
    env_logger::init();
    let args: Vec<String> = env::args().collect();
    // ignore first argument
    if args.len() != 4 {
        panic!("Expected 3 arguments, found {}", args.len() - 1);
    }
    let file_name = &args[1];
    let input1 = args[2].parse::<i32>().unwrap();
    let input2 = args[3].parse::<i32>().unwrap();

    let mut mem: Memory = Memory::from_binary(&file_name);
    mem.resize(16 * 1024);

    let mut state: MIPSState = MIPSState::new(mem);

    state.write_reg(1, input1 as u32);
    state.write_reg(2, input2 as u32);
    state.write_reg(30, 16 * 1024);

    mips_cpu::run(&mut state);
    std::process::exit(state.read_reg(3) as i32);
}
