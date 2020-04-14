// chip8.h
	
#ifndef CHIP8_H
#define CHIP8_H

#include<iostream>
#include<string>

class Chip8 {
    private:
        short opcode;
        
        uint16_t addr;
        uint8_t byte;
        short x;
        short y;

        bool drawFlag;

        uint8_t memory[4096];
        uint8_t v[16];

        short I;
        short pc;

        uint8_t delayTimer;
        uint8_t soundTimer;

        short stack[16];
        short sp;

        void CLS();
        void RET();
        void SYS_addr(uint16_t addr);
        void JP_addr(uint16_t addr);
        void CALL_addr(uint16_t addr);
        void SE_Vx_byte(short x, uint8_t byte);
        void SNE_Vx_byte(short x, uint8_t byte);
        void SE_Vx_Vy(short x, short y);
        void LD_Vx_byte(short x, uint8_t byte);
        void ADD_Vx_byte(short x, uint8_t byte);
        void LD_Vx_Vy(short x, short y);
        void OR_Vx_Vy(short x, short y);
        void AND_Vx_Vy(short x, short y);
        void XOR_Vx_Vy(short x, short y);
        void ADD_Vx_Vy(short x, short y);
        void SUB_Vx_Vy(short x, short y);
        void SHR_Vx_Vy(short x, short y);
        void SUBN_Vx_Vy(short x, short y);
        void SHL_Vx_Vy(short x, short y);
        void SNE_Vx_Vy(short x, short y);
        void LD_I_addr(uint16_t addr);
        void JP_V0_addr(uint16_t addr);
        void RND_Vx_byte(short x, uint8_t byte);
        void DRW_Vx_Vy_nibble(short x, short y, short n);
        void SKP_Vx(short x);
        void SKNP_Vx(short x);
        void LD_Vx_DT(short x);
        void LD_Vx_K(short x);
        void LD_DT_Vx(short x);
        void LD_ST_Vx(short x);
        void ADD_I_Vx(short x);
        void LD_F_Vx(short x);
        void LD_B_Vx(short x);
        void LD_I_Vx(short x);
        void LD_Vx_I(short x);

    public:
        uint8_t display[2048];
        uint8_t keypad[16];

        Chip8(); 
        void executeCycle();
        bool loadROM(std::string filename);
        bool getDrawFlag();
        void resetDrawFlag();
};

#endif