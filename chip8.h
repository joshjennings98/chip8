// chip8.h
	
#ifndef CHIP8_H
#define CHIP8_H

#include<iostream>
#include<string>

class Chip8 {
    private:
        unsigned short opcode;
        unsigned short addr;
        unsigned char byte;
        unsigned short x;
        unsigned short y;

        bool drawFlag;

        unsigned char memory[4096];
        unsigned char v[16];

        unsigned short I;
        unsigned short pc;

        unsigned char delayTimer;
        unsigned char soundTimer;

        unsigned short stack[16];
        unsigned short sp;

        void CLS();
        void RET();
        void SYS_addr(unsigned short addr);
        void JP_addr(unsigned short addr);
        void CALL_addr(unsigned short addr);
        void SE_Vx_byte(unsigned short x, unsigned char byte);
        void SNE_Vx_byte(unsigned short x, unsigned char byte);
        void SE_Vx_Vy(unsigned short x, unsigned short y);
        void LD_Vx_byte(unsigned short x, unsigned char byte);
        void ADD_Vx_byte(unsigned short x, unsigned char byte);
        void LD_Vx_Vy(unsigned short x, unsigned short y);
        void OR_Vx_Vy(unsigned short x, unsigned short y);
        void AND_Vx_Vy(unsigned short x, unsigned short y);
        void XOR_Vx_Vy(unsigned short x, unsigned short y);
        void ADD_Vx_Vy(unsigned short x, unsigned short y);
        void SUB_Vx_Vy(unsigned short x, unsigned short y);
        void SHR_Vx_Vy(unsigned short x, unsigned short y);
        void SUBN_Vx_Vy(unsigned short x, unsigned short y);
        void SHL_Vx_Vy(unsigned short x, unsigned short y);
        void SNE_Vx_Vy(unsigned short x, unsigned short y);
        void LD_I_addr(unsigned short addr);
        void JP_V0_addr(unsigned short addr);
        void RND_Vx_byte(unsigned short x, unsigned char byte);
        void DRW_Vx_Vy_nibble(unsigned short x, unsigned short y, unsigned short n);
        void SKP_Vx(unsigned short x);
        void SKNP_Vx(unsigned short x);
        void LD_Vx_DT(unsigned short x);
        void LD_Vx_K(unsigned short x);
        void LD_DT_Vx(unsigned short x);
        void LD_ST_Vx(unsigned short x);
        void ADD_I_Vx(unsigned short x);
        void LD_F_Vx(unsigned short x);
        void LD_B_Vx(unsigned short x);
        void LD_I_Vx(unsigned short x);
        void LD_Vx_I(unsigned short x);

    public:
        unsigned char display[2048];
        unsigned char keypad[16];

        Chip8(); 
        void executeCycle();
        bool loadROM(std::string filename);
        bool getDrawFlag();
        void resetDrawFlag();
};

#endif