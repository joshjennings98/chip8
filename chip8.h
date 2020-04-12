// chip8.h

#include<iostream>

class Chip8 {
    private:
        unsigned short opcode;

        unsigned char fontset[80] = { 
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

        unsigned char memory[4096];
        unsigned char v[16];

        unsigned short I;
        unsigned short pc;

        unsigned char display[64][32];

        unsigned char delayTimer;
        unsigned char soundTimer;

        unsigned short stack[16];
        unsigned short sp;

        unsigned char keypad[16];

        void CLS();
        void RET();
        void SYS_addr(unsigned char addr);
        void JP_addr(unsigned char addr);
        void CALL_addr(unsigned char addr);
        void SE_Vx_byte(unsigned char x, unsigned char byte);
        void SNE_Vx_byte(unsigned char x, unsigned char byte);
        void SE_Vx_Vy(unsigned char x, unsigned char y);
        void LD_Vx_byte(unsigned char x, unsigned char byte);
        void ADD_Vx_byte(unsigned char x, unsigned char byte);
        void LD_Vx_Vy(unsigned char x, unsigned char y);
        void OR_Vx_Vy(unsigned char x, unsigned char y);
        void AND_Vx_Vy(unsigned char x, unsigned char y);
        void XOR_Vx_Vy(unsigned char x, unsigned char y);
        void ADD_Vx_Vy(unsigned char x, unsigned char y);
        void SUB_Vx_Vy(unsigned char x, unsigned char y);
        void SHR_Vx_Vy(unsigned char x, unsigned char y);
        void SUBN_Vx_Vy(unsigned char x, unsigned char y);
        void SHL_Vx_Vy(unsigned char x, unsigned char y);
        void SNE_Vx_Vy(unsigned char x, unsigned char y);
        void LD_I_addr(unsigned char addr);
        void JP_V0_addr(unsigned char addr);
        void RND_Vx_byte(unsigned char x, unsigned char byte);
        void DRW_Vx_Vy_nibble(unsigned char x, unsigned char y, unsigned char nibble);
        void SKP_Vx(unsigned char x);
        void SKNP_Vx(unsigned char x);
        void LD_Vx_DT(unsigned char x);
        void LD_Vx_K(unsigned char x);
        void LD_DT_Vx(unsigned char x);
        void LD_ST_Vx(unsigned char x);
        void ADD_I_Vx(unsigned char x);
        void LD_F_Vx(unsigned char x);
        void LD_B_Vx(unsigned char x);
        void LD_I_Vx(unsigned char x);
        void LD_Vx_I(unsigned char x);

    public:
        Chip8(); 
        void executeCycle();
};

Chip8::Chip8()
{
    pc = 0x200;  
    opcode = 0;
    I = 0;
    sp = 0;
 
    // Clear display	
    for (unsigned char i = 0; i < 64; i++) { 
        for (unsigned char j = 0; j < 32; j++) {
            display[i][j] = 0;
        }
    }

    // Clear stack
    for (unsigned char i = 0; i < 16; i++) { 
        stack[i] = 0;
    }

    // Clear registers
    for (unsigned char i = 0; i < 16; i++) { 
        v[i] = 0;
    }
    // Clear memory
    for (unsigned char i = 0; i < 4096; i++) { 
        memory[i] = 0;
    }

    // Load fontset
    for (unsigned char i = 0; i < 80; i++) {
        memory[i] = fontset[i];
    }

    // Reset timers
    delayTimer = 0;
    soundTimer = 0;
}

void Chip8::executeCycle()
{
    opcode = memory[pc] << 8 | memory[pc + 1];

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
                SYS_addr(opcode & 0x0FFF);
                break;
            }
            break;
        case 0x1000:
            JP_addr(opcode & 0x0FFF);
            break;
        case 0x2000:
            CALL_addr(opcode & 0x0FFF);
            break;
        case 0x3000:
            SE_Vx_byte(opcode & 0x0F00 >> 8, opcode & 0x00FF);
            break;
        case 0x4000:
            SNE_Vx_byte(opcode & 0x0F00 >> 8, opcode & 0x00FF);
            break;
        case 0x5000:
            SNE_Vx_Vy(opcode & 0x0F00 >> 8, opcode & 0x00F0 >> 4);
            break;
        case 0x6000:
            LD_Vx_byte(opcode & 0x0F00 >> 8, opcode & 0x00FF);
            break;     
        case 0x7000:
            ADD_Vx_byte(opcode & 0x0F00 >> 8, opcode & 0x00FF);
            break; 
        case 0x8000:
            switch (opcode & 0xF00F) {
                case 0x8000:
                    LD_Vx_Vy(opcode & 0x0F00 >> 8, opcode & 0x00F0 >> 4);
                    break; 
                case 0x8001:
                    OR_Vx_Vy(opcode & 0x0F00 >> 8, opcode & 0x00F0 >> 4);
                    break; 
                case 0x8002:
                    AND_Vx_Vy(opcode & 0x0F00 >> 8, opcode & 0x00F0 >> 4);
                    break; 
                case 0x8003:
                    XOR_Vx_Vy(opcode & 0x0F00 >> 8, opcode & 0x00F0 >> 4);
                    break; 
                case 0x8004:
                    ADD_Vx_Vy(opcode & 0x0F00 >> 8, opcode & 0x00F0 >> 4);
                    break; 
                case 0x8005:
                    SUB_Vx_Vy(opcode & 0x0F00 >> 8, opcode & 0x00F0 >> 4);
                    break; 
                case 0x8006:
                    SHR_Vx_Vy(opcode & 0x0F00 >> 8, opcode & 0x00F0 >> 4);
                    break; 
                case 0x8007:
                    SUBN_Vx_Vy(opcode & 0x0F00 >> 8, opcode & 0x00F0 >> 4);
                    break; 
                case 0x800E:
                    SHL_Vx_Vy(opcode & 0x0F00 >> 8, opcode & 0x00F0 >> 4);
                    break;
                default:
                    break;
            }
            break; 
        case 0x9000:
            SNE_Vx_Vy(opcode & 0x0F00 >> 8, opcode & 0x00F0 >> 4);
            break; 
        case 0xA000:
            LD_I_addr(opcode & 0x0FFF);
            break; 
        case 0xB000:
            JP_V0_addr(opcode & 0x0FFF);
            break;      
        case 0xC000:
            RND_Vx_byte(opcode & 0x0F00 >> 8, opcode & 0x00FF);
            break; 
        case 0xD000:
            DRW_Vx_Vy_nibble(opcode & 0x0F00 >> 8, opcode & 0x00F0 >> 4, opcode & 0x000F);
            break; 
        case 0xE000: 
            switch (opcode & 0xF0FF) {
                case 0xE09E:
                    SKP_Vx(opcode & 0x0F00 >> 8);
                    break;
                case 0xE0A1:
                    SKNP_Vx(opcode & 0x0F00 >> 8);
                    break;
                default:
                    break;
            }
            break; 
        case 0xF000:
            switch (opcode & 0xF0FF) {
                case 0xF007:
                    LD_Vx_DT(opcode & 0xF000);
                    break;
                case 0xF00A:
                    LD_Vx_K(opcode & 0xF000);
                    break;
                case 0xF015:
                    LD_DT_Vx(opcode & 0xF000);
                    break;
                case 0xF018:
                    LD_ST_Vx(opcode & 0xF000);
                    break;
                case 0xF01E:
                    ADD_I_Vx(opcode & 0xF000);
                    break;
                case 0xF029:
                    LD_F_Vx(opcode & 0xF000);
                    break;
                case 0xF033:
                    LD_B_Vx(opcode & 0xF000);
                    break;
                case 0xF055:
                    LD_I_Vx(opcode & 0xF000);
                    break;
                case 0xF065:
                    LD_Vx_I(opcode & 0xF000);
                    break;
                default:
                    break;
            }
            break;    
        default:
            std::cout << "Invalid opcode: 0x" << opcode << std::endl;  
    } ;

    if(delayTimer > 0) {
        delayTimer--;
    }
    
    if(soundTimer > 0) {
        if(soundTimer == 1) {
            std::cout << "BEEP!" << std::endl;
        }
        soundTimer--;
    }  
}

void Chip8::CLS()
{

}

void Chip8::RET()
{

}

void Chip8::SYS_addr(unsigned char addr)
{

}

void Chip8::JP_addr(unsigned char addr)
{

}

void Chip8::CALL_addr(unsigned char addr)
{

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