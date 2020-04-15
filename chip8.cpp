// chip8.cpp

#include<iostream>
#include<random>
#include<fstream>

#include"chip8.h"

uint8_t fontset[80] = { 
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
    
Chip8::Chip8()
{
    pc = 0x200;  
    opcode = 0;
    I = 0;
    sp = 0;
 
    // Clear display	
    for (auto pixel : display) { 
        pixel = 0x0;
    }

    // Clear stack
    for (auto val : stack) { 
        val = 0;
    }

    // Clear registers
    for (auto vi : v) { 
        vi = 0;
    }

    // Clear keypad
    for (auto key : keypad) { 
        key = 0;
    }

    // Clear memory
    for (auto loc : memory) { 
        loc = 0;
    }

    // Load fontset
    for (int i = 0; i < 80; i++) {
        memory[i] = fontset[i];
    }

    // Reset timers
    delayTimer = 0;
    soundTimer = 0;

    // Reset Draw Flag
    drawFlag = false;
}

bool Chip8::getDrawFlag()
{
    return drawFlag;
}

bool Chip8::loadROM(std::string filename)
{    
    char c;
    uint8_t x;
    int i = 0;

    std::ifstream input(filename.c_str(), std::ios::in | std::ios::binary);    

    while (input.get(c)) {
        x = c;
        memory[512 + i] = x;
        i++;
    }

    return true;
}

void Chip8::resetDrawFlag() 
{
    drawFlag = false;
}

void Chip8::executeCycle()
{
    opcode = memory[pc] << 8 | memory[pc + 1];

    addr = opcode & 0x0FFF;
    byte = opcode & 0x00FF;
    x = (opcode & 0x0F00) >> 8;
    y = (opcode & 0x00F0) >> 4;

    switch(opcode & 0xF000) {    
        case 0x0000:
            switch(opcode) {
            case 0x00E0:
                CLS();
                break;
            case 0x00EE:
                RET();
                break;
            default:
                SYS_addr(addr);
                break;
            }
            break;
        case 0x1000:
            JP_addr(addr);
            break;
        case 0x2000:
            CALL_addr(addr);
            break;
        case 0x3000:
            SE_Vx_byte(x, byte);
            break;
        case 0x4000:
            SNE_Vx_byte(x, byte);
            break;
        case 0x5000:
            SNE_Vx_Vy(x, y);
            break;
        case 0x6000:
            LD_Vx_byte(x, byte);
            break;     
        case 0x7000:
            ADD_Vx_byte(x, byte);
            break; 
        case 0x8000:
            switch (opcode & 0xF00F) {
                case 0x8000:
                    LD_Vx_Vy(x, y);
                    break; 
                case 0x8001:
                    OR_Vx_Vy(x, y);
                    break; 
                case 0x8002:
                    AND_Vx_Vy(x, y);
                    break; 
                case 0x8003:
                    XOR_Vx_Vy(x, y);
                    break; 
                case 0x8004:
                    ADD_Vx_Vy(x, y);
                    break; 
                case 0x8005:
                    SUB_Vx_Vy(x, y);
                    break; 
                case 0x8006:
                    SHR_Vx_Vy(x, y);
                    break; 
                case 0x8007:
                    SUBN_Vx_Vy(x, y);
                    break; 
                case 0x800E:
                    SHL_Vx_Vy(x, y);
                    break;
                default:
                    break;
            }
            break; 
        case 0x9000:
            SNE_Vx_Vy(x, y);
            break; 
        case 0xA000:
            LD_I_addr(addr);
            break; 
        case 0xB000:
            JP_V0_addr(addr);
            break;      
        case 0xC000:
            RND_Vx_byte(x, byte);
            break; 
        case 0xD000:
            DRW_Vx_Vy_nibble(x, y, opcode & 0x000F);
            break; 
        case 0xE000: 
            switch (opcode & 0xF0FF) {
                case 0xE09E:
                    SKP_Vx(x);
                    break;
                case 0xE0A1:
                    SKNP_Vx(x);
                    break;
                default:
                    break;
            }
            break; 
        case 0xF000:
            switch (opcode & 0xF0FF) {
                case 0xF007:
                    LD_Vx_DT(x);
                    break;
                case 0xF00A:
                    LD_Vx_K(x);
                    break;
                case 0xF015:
                    LD_DT_Vx(x);
                    break;
                case 0xF018:
                    LD_ST_Vx(x);
                    break;
                case 0xF01E:
                    ADD_I_Vx(x);
                    break;
                case 0xF029:
                    LD_F_Vx(x);
                    break;
                case 0xF033:
                    LD_B_Vx(x);
                    break;
                case 0xF055:
                    LD_I_Vx(x);
                    break;
                case 0xF065:
                    LD_Vx_I(x);
                    break;
                default:
                    break;
            }
            break;    
        default:
            std::cout << "Invalid opcode: 0x" << opcode << std::endl;  
    } ;

    if (delayTimer > 0) {
        delayTimer--;
    }
    
    if (soundTimer > 0) {
        if (soundTimer == 1) {
            std::cout << "BEEP!" << std::endl;
        }
        soundTimer--;
    }  
}

void Chip8::CLS()
{
    for (auto pixel : display) { 
        pixel = 0x0;
    }

    drawFlag = true;
    pc += 2;
}

void Chip8::RET()
{
    sp--;
    pc = stack[sp];
    pc += 2;
}

void Chip8::SYS_addr(uint16_t addr)
{
    //  This instruction is only used on the old computers on which Chip-8 was originally implemented. It is ignored by modern interpreters.
    pc += 2;
}

void Chip8::JP_addr(uint16_t addr)
{
    pc = addr;
}

void Chip8::CALL_addr(uint16_t addr)
{
    stack[sp] = pc;
    sp++;
    pc = addr;
}

void Chip8::SE_Vx_byte(short x, uint8_t byte)
{
    pc += v[x] == byte ? 4 : 2;
}

void Chip8::SNE_Vx_byte(short x, uint8_t byte)
{
    pc += v[x] != byte ? 4 : 2;
}

void Chip8::SE_Vx_Vy(short x, short y)
{
    pc += v[x] == v[y] ? 4 : 2;
}

void Chip8::LD_Vx_byte(short x, uint8_t byte)
{
    v[x] = byte;
    pc += 2;
}

void Chip8::ADD_Vx_byte(short x, uint8_t byte)
{
    v[x] += byte;
    pc += 2;
}

void Chip8::LD_Vx_Vy(short x, short y)
{
    v[x] = v[y];
    pc += 2;
}

void Chip8::OR_Vx_Vy(short x, short y)
{
    v[x] |= v[y];
    pc += 2;
}

void Chip8::AND_Vx_Vy(short x, short y)
{
    v[x] &= v[y];
    pc += 2;
}

void Chip8::XOR_Vx_Vy(short x, short y)
{
    v[x] ^= v[y];
    pc += 2;
}

void Chip8::ADD_Vx_Vy(short x, short y)
{
    v[x] += v[y];
    pc += 2;

    v[0xF] = (int) v[x] + (int) v[y] > 255 ? 1 : 0;
}

void Chip8::SUB_Vx_Vy(short x, short y)
{
    v[x] -= v[y];
    pc += 2;

    v[0xF] = v[x] >= v[y] ? 1 : 0;
}

void Chip8::SHR_Vx_Vy(short x, short y)
{
    v[x] >>= 1;
    pc += 2;

    v[0xF] = v[x] % 2 == 1 ? 1 : 0;
}

void Chip8::SUBN_Vx_Vy(short x, short y)
{
    v[x] = v[y] - v[x];
    pc += 2;

    v[0xF] = v[y] >= v[x] ? 1 : 0;
}

void Chip8::SHL_Vx_Vy(short x, short y)
{
    v[x] <<= 1;
    pc += 2;

    v[0xF] = (v[x] & 0x80) == 0x80 ? 1 : 0;
}

void Chip8::SNE_Vx_Vy(short x, short y)
{
    pc += v[x] != v[y] ? 4 : 2;
}

void Chip8::LD_I_addr(uint16_t addr)
{
    I = addr;
    pc += 2;
}

void Chip8::JP_V0_addr(uint16_t addr)
{
    pc = v[0] + addr;
}

void Chip8::RND_Vx_byte(short x, uint8_t byte)
{
    char r = rand() % 255;

    v[x] = r & byte;
    pc += 2;
}

void Chip8::DRW_Vx_Vy_nibble(short x, short y, short n)
{
    v[0xF] = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 8; j++) {
            if ((memory[I + i] & (0x80 >> j)) != 0) {
                v[0xF] = display[(v[x] + j + ((v[y] + i) * 64))] == 1 ? 1 : 0;                                    
                display[v[x] + j + ((v[y] + i) * 64)] ^= 1;
            }
        }
    }
                
    drawFlag = true;			
    pc += 2;
}

void Chip8::SKP_Vx(short x)
{
    pc += keypad[v[x]] != 0 ? 4 : 2;
} 

void Chip8::SKNP_Vx(short x)
{
    pc += keypad[v[x]] == 0 ? 4 : 2;
}

void Chip8::LD_Vx_DT(short x)
{
    v[x] = delayTimer;
    pc += 2;
}

void Chip8::LD_Vx_K(short x)
{
    bool keyPress = false;

    for(int i = 0; i < 16; i++) {
        if(keypad[i] != 0) {
            v[x] = i;
            keyPress = true;
        }
    }

    // wait for keypress
    if(keyPress) {					
        pc += 2;
    }
}

void Chip8::LD_DT_Vx(short x)
{
    delayTimer = v[x];
    pc += 2;
}

void Chip8::LD_ST_Vx(short x)
{
    soundTimer = v[x];
    pc += 2;
}

void Chip8::ADD_I_Vx(short x)
{
    I += v[x];
    pc += 2;

    v[0xF] = I + v[x] > 0xFFF ? 1 : 0;
}

void Chip8::LD_F_Vx(short x)
{
    I = 5 * v[x];
    pc += 2;
}

void Chip8::LD_B_Vx(short x)
{
    memory[I] = v[x] / 100;
    memory[I + 1] = (v[x] / 10) % 10;
    memory[I + 2] = (v[x] % 100) % 10;	

    pc += 2;
}

void Chip8::LD_I_Vx(short x)
{
    for (int i = 0; i <= x; i++) {
        memory[i + I] = v[i];
    }

    pc += 2;
}

void Chip8::LD_Vx_I(short x)
{
    for (int i = 0; i <= x; i++) {
        v[i] = memory[i + I];
    }
    
    pc += 2;
}