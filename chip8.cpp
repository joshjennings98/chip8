// chip8.cpp
#include<iostream>
#include"chip8.h"

Chip8::Chip8()
{
    pc = 0x200;  
    opcode = 0;
    I = 0;
    sp = 0;
 
    // Clear display	
    for (int i = 0; i < 64; i++) { 
        for (int j = 0; j < 32; j++) {
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

}

void Chip8::setKeys()
{

}

void Chip8::executeCycle()
{
    opcode = memory[pc] << 8 | memory[pc + 1];
    
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
    for (int i = 0; i < 64; i++) { 
        for (int j = 0; j < 32; j++) {
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

void Chip8::SE_Vx_byte(unsigned char x, unsigned char byte)
{

}

void Chip8::SNE_Vx_byte(unsigned char x, unsigned char byte)
{

}

void Chip8::SE_Vx_Vy(unsigned char x, unsigned char y)
{

}

void Chip8::LD_Vx_byte(unsigned char x, unsigned char byte)
{

}

void Chip8::ADD_Vx_byte(unsigned char x, unsigned char byte)
{

}

void Chip8::LD_Vx_Vy(unsigned char x, unsigned char y)
{

}

void Chip8::OR_Vx_Vy(unsigned char x, unsigned char y)
{

}

void Chip8::AND_Vx_Vy(unsigned char x, unsigned char y)
{

}

void Chip8::XOR_Vx_Vy(unsigned char x, unsigned char y)
{

}

void Chip8::ADD_Vx_Vy(unsigned char x, unsigned char y)
{

}

void Chip8::SUB_Vx_Vy(unsigned char x, unsigned char y)
{

}

void Chip8::SHR_Vx_Vy(unsigned char x, unsigned char y)
{

}

void Chip8::SUBN_Vx_Vy(unsigned char x, unsigned char y)
{

}

void Chip8::SHL_Vx_Vy(unsigned char x, unsigned char y)
{

}

void Chip8::SNE_Vx_Vy(unsigned char x, unsigned char y)
{

}

void Chip8::LD_I_addr(unsigned char addr)
{

}

void Chip8::JP_V0_addr(unsigned char addr)
{

}

void Chip8::RND_Vx_byte(unsigned char x, unsigned char byte)
{

}

void Chip8::DRW_Vx_Vy_nibble(unsigned char x, unsigned char y, unsigned char nibble)
{

}

void Chip8::SKP_Vx(unsigned char x)
{

}

void Chip8::SKNP_Vx(unsigned char x)
{

}

void Chip8::LD_Vx_DT(unsigned char x)
{

}

void Chip8::LD_Vx_K(unsigned char x)
{

}

void Chip8::LD_DT_Vx(unsigned char x)
{

}

void Chip8::LD_ST_Vx(unsigned char x)
{

}

void Chip8::ADD_I_Vx(unsigned char x)
{

}

void Chip8::LD_F_Vx(unsigned char x)
{

}

void Chip8::LD_B_Vx(unsigned char x)
{

}

void Chip8::LD_I_Vx(unsigned char x)
{

}

void Chip8::LD_Vx_I(unsigned char x)
{

}
