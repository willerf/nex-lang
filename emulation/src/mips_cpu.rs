
use log::{debug, error};

use crate::mips_state::MIPSState;
use crate::mips_state::{PC, LO, HI};

macro_rules! FMT_ADDR { () => { "{:08X}\t{:<20} | {}" }; }

pub fn step(state: &mut MIPSState) {
    let pc: u32 = state.read_reg(PC);           // update uses of PC to use pc
    let instr: u32 = state.read_mem(pc);
    
    state.write_reg(PC, pc+4);

    let opcode: u32 = (instr >> 26) & 0b111111;
    let s_addr: u32 = (instr >> 21) & 0b11111;
    let t_addr: u32 = (instr >> 16) & 0b11111;
    let d_addr: u32 = (instr >> 11) & 0b11111;
    
    let s: u32 = state.read_reg(s_addr);
    let t: u32 = state.read_reg(t_addr);
    //let d: u32 = state.read_reg(d_addr);
    
    let func: u32 = instr & 0b11111111111;
    let imm: i16 = (instr & 0b1111111111111111) as i16;

    match opcode {

        // no imm
        0b000000 => {
            match func {

                // add
                0b00000100000 => {
                    state.write_reg(d_addr, ((s as i32) + (t as i32)) as u32);
                    if d_addr == 0 {
                        debug!(FMT_ADDR!(), pc, "nop", "*****************");
                    }
                    else {
                        debug!(FMT_ADDR!(), pc, format!("add ${}, ${}, ${}", d_addr, s_addr, t_addr), format!("${:<2} <- {} = {} + {}", d_addr, state.read_reg(d_addr), s, t));
                    }
                }

                // sub
                0b00000100010 => {
                    state.write_reg(d_addr, ((s as i32) - (t as i32)) as u32);
                    debug!(FMT_ADDR!(), pc, format!("sub ${}, ${}, ${}", d_addr, s_addr, t_addr), format!("${:<2} <- {} = {} - {}", d_addr, state.read_reg(d_addr), s, t));
                }

                // mult
                0b00000011000 => {
                    let result: u64 = ((s as i64) * (t as i64)) as u64;
                    state.write_reg(HI, ((result >> 32) & 0xFFFFFFFF) as u32);
                    state.write_reg(LO, (result & 0xFFFFFFFF) as u32);
                    debug!(FMT_ADDR!(), pc, format!("mult ${}, ${}", s_addr, t_addr), format!("${:<2} <- {} = {} * {} (63..32); ${:<2} <- {} = {} * {} (31..0)", "$hi", state.read_reg(HI), s, t, "$lo", state.read_reg(LO), s, t));
                }

                // multu
                0b00000011001 => {
                    let result: u64 = ((s as u64) * (t as u64)) as u64;
                    state.write_reg(LO, (result & 0xFFFFFFFF) as u32);
                    state.write_reg(HI, ((result >> 32) & 0xFFFFFFFF) as u32);
                    debug!(FMT_ADDR!(), pc, format!("multu ${}, ${}", s_addr, t_addr), format!("${:<2} <- {} = {} * {} (63..32); ${:<2} <- {} = {} * {} (31..0)", "$hi", state.read_reg(HI), s, t, "$lo", state.read_reg(LO), s, t));
                }

                // div
                0b00000011010 => {
                    state.write_reg(LO, ((s as i32) / (t as i32)) as u32);
                    state.write_reg(HI, ((s as i32) % (t as i32)) as u32);
                    debug!(FMT_ADDR!(), pc, format!("div ${}, ${}", s_addr, t_addr), format!("${:<2} <- {} = {} % {}; ${:<2} <- {} = {} / {}", "$hi", state.read_reg(HI), s, t, "$lo", state.read_reg(LO), s, t));
                }

                // divu
                0b00000011011 => {
                    state.write_reg(LO, ((s as u32) / (t as u32)) as u32);
                    state.write_reg(HI, ((s as u32) % (t as u32)) as u32);
                    debug!(FMT_ADDR!(), pc, format!("divu ${}, ${}", s_addr, t_addr), format!("${:<2} <- {} = {} % {}; ${:<2} <- {} = {} / {}", "$hi", state.read_reg(HI), s, t, "$lo", state.read_reg(LO), s, t));
                }

                // mfhi
                0b00000010000 => {
                    state.write_reg(d_addr, state.read_reg(HI));
                    debug!(FMT_ADDR!(), pc, format!("mfhi ${}", d_addr), format!("${:<2} <- {}", d_addr, state.read_reg(HI)));
                }

                // mflo
                0b00000010010 => {
                    state.write_reg(d_addr, state.read_reg(LO));
                    debug!(FMT_ADDR!(), pc, format!("mflo ${}", d_addr), format!("${:<2} <- {}", d_addr, state.read_reg(LO)));
                }

                // lis
                0b00000010100 => {
                    state.write_reg(d_addr, state.read_mem(pc+4));
                    state.write_reg(PC, pc+8);
                    debug!(FMT_ADDR!(), pc, format!("lis ${}", d_addr), format!("${:<2} <- {}", d_addr, state.read_mem(pc+4)));
                }

                // slt
                0b00000101010 => {
                    state.write_reg(d_addr, ((s as i32) < (t as i32)) as u32);
                    debug!(FMT_ADDR!(), pc, format!("slt ${}, ${}, ${}", d_addr, s_addr, t_addr), format!("${:<2} <- {} = {} < {}; signed", d_addr, state.read_reg(d_addr), s, t));
                }

                // sltu
                0b00000101011 => {
                    state.write_reg(d_addr, ((s as u32) < (t as u32)) as u32);
                    debug!(FMT_ADDR!(), pc, format!("sltu ${}, ${}, ${}", d_addr, s_addr, t_addr), format!("${:<2} <- {} = {} < {}; unsigned", d_addr, state.read_reg(d_addr), s, t));
                }

                // jr
                0b00000001000 => {
                    state.write_reg(PC, s);
                    debug!(FMT_ADDR!(), pc, format!("jr ${}", s_addr), format!("${:<2} <- {}", "PC", s));
                }

                // jalr
                0b00000001001 => {
                    state.write_reg(31, state.read_reg(PC));
                    state.write_reg(PC, s);
                    debug!(FMT_ADDR!(), pc, format!("jalr ${}", s_addr), format!("${:<2} <- {}; ${:<2} <- {}", "31", pc, "PC", s));
                    
                }

                // unknown instruction
                x => {
                    error!("Unknown Instruction: {:032b}", x);
                    panic!();
                }
            }
        }

        // yes imm

        // lw
        0b100011 => {
            state.write_reg(t_addr, state.read_mem(((s as i64) + (imm as i64)) as u32));
            debug!(FMT_ADDR!(), pc, format!("lw ${}, {}(${})", t_addr, imm, s_addr), format!("${:<2} <- {} = MEM[{} + {}]", t_addr, state.read_reg(t_addr), s, imm));
        }

        // sw
        0b101011 => {
            state.write_mem(((s as i64) + (imm as i64)) as u32, t);
            debug!(FMT_ADDR!(), pc, format!("sw ${}, {}(${})", t_addr, imm, s_addr), format!("MEM[{} + {}] <- {}", s, imm, state.read_reg(t_addr)));
        }

        // beq
        0b000100 => {
            if s == t  { 
                state.write_reg(PC, ((state.read_reg(PC) as i64) + 4*(imm as i64)) as u32)
            }
            debug!(FMT_ADDR!(), pc, format!("beq ${}, ${}, {}", s_addr, t_addr, imm), format!("if({} == {}) ${} <- {} = {} + 4*{}", s, t, "PC", pc, s, imm));
        }

        // bne
        0b000101 => {
            if s != t  {
                state.write_reg(PC, ((state.read_reg(PC) as i64) + 4*(imm as i64)) as u32)
            }
            debug!(FMT_ADDR!(), pc, format!("bne ${}, ${}, {}", s_addr, t_addr, imm), format!("if({} != {}) ${} <- {} = {} + 4*{}", s, t, "PC", pc, s, imm));
        }

        // unknown instruction
        x => {
            error!("Unknown Instruction: {:032b}", x);
            panic!();
        }
    }
}

pub fn run(state: &mut MIPSState) {
    let termination_pc: u32 = 0b11111110111000011101111010101101;

    state.write_reg(31, termination_pc);
    let mut cycles = 0;

    while state.read_reg(PC) != termination_pc && cycles < 10_000 {
        step(state);
        cycles += 1;
    }
}

