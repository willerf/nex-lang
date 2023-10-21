
use crate::memory::Memory;

pub const PC: u32 = 32;
pub const LO: u32 = 33;
pub const HI: u32 = 34;

pub const PRINT_ADDR: u32 = 0b11111111111111110000000000001100;

pub struct MIPSState {
    mem: Memory,
    regs: Vec<u32>,
}

impl MIPSState {
    pub fn new(mem: Memory) -> MIPSState {
        MIPSState {
            mem: mem,
            regs: vec![0; 35],
        }
    }

    pub fn read_reg(&self, addr: u32) -> u32 {
        assert!(addr < 35);
        self.regs[addr as usize]
    }

    pub fn write_reg(&mut self, addr: u32, word: u32) {
        assert!(addr < 35);
        if addr != 0 {
            self.regs[addr as usize] = word;
        }
    }

    pub fn read_mem(&self, addr: u32) -> u32 {
        ((self.mem.read(addr) as u32) << 24) 
            | ((self.mem.read(addr+1) as u32) << 16)
            | ((self.mem.read(addr+2) as u32) << 8)
            | ((self.mem.read(addr+3) as u32) << 0)
    }

    pub fn write_mem(&mut self, addr: u32, word: u32) {
        if addr == PRINT_ADDR {
            print!("{}", char::from_u32(word).expect("Error"));
        }
        else {
            self.mem.write(addr, (word >> 24) as u8);
            self.mem.write(addr+1, (word >> 16) as u8);
            self.mem.write(addr+2, (word >> 8) as u8);
            self.mem.write(addr+3, (word >> 0) as u8);
        }
    }

    pub fn size(&self) -> u32 {
        self.mem.size()
    }
}

