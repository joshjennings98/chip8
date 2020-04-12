// chip8.cpp
#include<iostream>
#include<random>
#include<fstream>
#include"chip8.h"

int k = 0;

Chip8::Chip8()
{
    pc = 0x200;  
    opcode = 0;
    I = 0;
    sp = 0;
 
    // Clear display	
    for (int i = 0; i < 32; i++) { 
        for (int j = 0; j < 64; j++) {
            display[i][j] = 0;
        }
    }

    // Clear stack
    for (int i = 0; i < 16; i++) { 
        stack[i] = 0;
    }

    // Clear registers
    for (int i = 0; i < 16; i++) { 
        v[i] = 0;
    }
    // Clear memory
    for (int i = 0; i < 4096; i++) { 
        memory[i] = 0;
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

void Chip8::loadROM(std::string filename)
{
    std::ifstream input(filename.c_str(), std::ios::in | std::ios::binary);    char c;
    unsigned char x;
    int i = 0;
    while (input.get(c)) {
        x = c;
        memory[0x200 + i] = x;
        //std::cout << (int) x << std::endl;
        i++;
    }
}

void Chip8::setKeys()
{

}

void Chip8::resetDrawFlag() 
{
    drawFlag = false;
}

void Chip8::executeCycle()
{
    opcode = memory[pc] << 8 | memory[pc + 1];
    //std::cout << k << std::endl;
    k++;

    addr = opcode & 0x0FFF;
    byte = opcode & 0x00FF;
    x = opcode & 0x0F00 >> 8;
    y = opcode & 0x00F0 >> 4;

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
    for (int i = 0; i < 32; i++) { 
        for (int j = 0; j < 64; j++) {
            display[i][j] = 0;
        }
    }
}

void Chip8::RET()
{
    pc = stack[sp];
    sp--;
}

void Chip8::SYS_addr(unsigned char addr)
{
    //  This instruction is only used on the old computers on which Chip-8 was originally implemented. It is ignored by modern interpreters.
    pc += 2;
}

void Chip8::JP_addr(unsigned char addr)
{
    pc = addr;
}

void Chip8::CALL_addr(unsigned char addr)
{
    stack[sp] = pc;
    sp++;
    pc = addr;
}

void Chip8::SE_Vx_byte(unsigned short x, unsigned char byte)
{
    if (v[x] == byte) {
        pc += 4;
    } else {
        pc += 2;
    }
}

void Chip8::SNE_Vx_byte(unsigned short x, unsigned char byte)
{
    if (v[x] != byte) {
        pc += 4;
    } else {
        pc += 2;
    }
}

void Chip8::SE_Vx_Vy(unsigned short x, unsigned short y)
{
    if (v[x] == v[y]) {
        pc += 4;
    } else {
        pc += 2;
    }
}

void Chip8::LD_Vx_byte(unsigned short x, unsigned char byte)
{
    v[x] = byte;
    pc += 2;
}

void Chip8::ADD_Vx_byte(unsigned short x, unsigned char byte)
{
    v[x] += byte;
    pc += 2;
}

void Chip8::LD_Vx_Vy(unsigned short x, unsigned short y)
{
    v[x] = v[y];
    pc += 2;
}

void Chip8::OR_Vx_Vy(unsigned short x, unsigned short y)
{
    v[x] |= v[y];
    pc += 2;
}

void Chip8::AND_Vx_Vy(unsigned short x, unsigned short y)
{
    v[x] &= v[y];
    pc += 2;
}

void Chip8::XOR_Vx_Vy(unsigned short x, unsigned short y)
{
    v[x] ^= v[y];
    pc += 2;
}

void Chip8::ADD_Vx_Vy(unsigned short x, unsigned short y)
{
    v[x] += v[y];
    pc += 2;

    if ((int) v[x] + (int) v[y] > 255) {
        v[0xF] = 1;
    } else {
        v[0xF] = 0;
    }
}

void Chip8::SUB_Vx_Vy(unsigned short x, unsigned short y)
{
    if (v[x] > v[y]) {
        v[0xF] = 1;
    } else {
        v[0xF] = 0;
    }

    v[x] -= v[y];
    pc += 2;
}

void Chip8::SHR_Vx_Vy(unsigned short x, unsigned short y)
{
    if (v[x] % 2 == 1) {
        v[0xF] = 1;
    } else {
        v[0xF] = 0;
    }

    v[x] >>= 1;
    pc += 2;
}

void Chip8::SUBN_Vx_Vy(unsigned short x, unsigned short y)
{
    if (v[y] > v[x]) {
        v[0xF] = 1;
    } else {
        v[0xF] = 0;
    }

    v[x] = v[y] - v[x];
    pc += 2;
}

void Chip8::SHL_Vx_Vy(unsigned short x, unsigned short y)
{
    if ((v[x] & 0x80) == 0x80) {
        v[0xF] = 1;
    } else {
        v[0xF] = 0;
    }

    v[x] = v[x] << 1 & 0xFF;
    pc += 2;
}

void Chip8::SNE_Vx_Vy(unsigned short x, unsigned short y)
{
    if (v[x] != v[y]) {
        pc += 4;
    } else {
        pc += 2;
    }
}

void Chip8::LD_I_addr(unsigned char addr)
{
    I = memory[addr];
    pc += 2;
}

void Chip8::JP_V0_addr(unsigned char addr)
{
    pc = v[0] + addr;
}

void Chip8::RND_Vx_byte(unsigned short x, unsigned char byte)
{
    char r = rand() % 255;
    v[x] = r & byte;
    pc += 2;
}

void Chip8::DRW_Vx_Vy_nibble(unsigned short x, unsigned short y, unsigned short n)
{
    v[0xF] = 0;

    unsigned char curr, prev;
    /*
    
    for (int i = 0; i < n; ++i) {
        curr = memory[I + i];
        for (int j = 0; j < 8; ++j) {
            if ((curr & (0x80 >> j)) != 0) {
                display[v[x] + j][v[y] + i] ^= curr;
                
                v[0xF] = 1;
            }
        }
    }
    */
    // /*
    for (unsigned short i = 0; i < n; i++) {
        for (int j = 7; j >= 0; j--) {
            if (memory[i + I] & (1 << j)) {
                curr = 0xFF;
            } else {
                curr = 0x00;
            }
            //std::cout << curr << std::endl;
            prev = display[v[x] + i][v[y] + j];
            display[v[x] + i][v[y] + j] ^= curr;

            if (curr & prev) {
                v[0xF] = 1;
            }
        }
    }
    // */
    pc += 2;
    drawFlag = true;
}

void Chip8::SKP_Vx(unsigned short x)
{
    if (keypad[x] != 0) {
        pc += 4;
    } else {
        pc += 2;
    }
} 

void Chip8::SKNP_Vx(unsigned short x)
{
    if (keypad[x] == 0) {
        pc += 4;
    } else {
        pc += 2;
    }
}

void Chip8::LD_Vx_DT(unsigned short x)
{
    v[x] = delayTimer;
    pc += 2;
}

void Chip8::LD_Vx_K(unsigned short x)
{
    v[x] = soundTimer;
    pc += 2;
}

void Chip8::LD_DT_Vx(unsigned short x)
{
    delayTimer = v[x];
    pc += 2;
}

void Chip8::LD_ST_Vx(unsigned short x)
{
    soundTimer = v[x];
    pc += 2;
}

void Chip8::ADD_I_Vx(unsigned short x)
{
    I += v[x];
    pc += 2;
}

void Chip8::LD_F_Vx(unsigned short x)
{
    I += 5 * v[x];
    pc += 2;
}

void Chip8::LD_B_Vx(unsigned short x)
{
    memory[I]     = v[x] / 100;
    memory[I + 1] = (v[x] % 100 / 10);
    memory[I + 2] = (v[x] % 10);
    pc += 2;
}

void Chip8::LD_I_Vx(unsigned short x)
{
    for (int i = 0; i <= x; i++) {
        memory[i + I] = v[i];
    }
    pc += 2;
}

void Chip8::LD_Vx_I(unsigned short x)
{
    printf("test");
    for (int i = 0; i <= x; i++) {
        v[i] = memory[i + I];
    }
    pc += 2;
}
