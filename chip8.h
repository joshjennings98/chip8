// chip8.h
	
#ifndef CHIP8_H
#define CHIP8_H

#include<iostream>

class Chip8 {
    private:
        unsigned short opcode;
        unsigned char addr;
        unsigned char byte;
        unsigned char x;
        unsigned char y;

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

#endif