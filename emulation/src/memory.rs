use std::fs;

pub struct Memory {
    mem: Vec<u8>,
}

impl Memory {
    pub fn new(num_words: u32) -> Memory {
        Memory { 
            mem: vec![0; num_words as usize],
        }
    }

    pub fn from_vec(mem: Vec<u8>) -> Memory {
        Memory {
            mem: mem,
        }
    }

    pub fn from_binary(file_path: &str) -> Memory {
        let file_in: Vec<u8> = fs::read(file_path).expect("File not found!");
        Memory {
            mem: file_in,
        }
    }

    pub fn read(&self, address: u32) -> u8 {
        self.mem[address as usize]
    }

    pub fn write(&mut self, address: u32, value: u8) {
        self.mem[address as usize] = value;
    }

    pub fn resize(&mut self, new_size: u32) {
        self.mem.extend(vec![0; (new_size - self.size()) as usize]);
    }

    pub fn concat(&mut self, other: Memory) {
        self.mem.extend(other.mem);
    }

    pub fn size(&self) -> u32 {
        self.mem.len() as u32
    }
}
