// chip8.cpp
#include<iostream>
#include<random>
#include<fstream>
#include<assert.h>
#include"chip8.h"

int k = 0;

Chip8::Chip8()
{
    pc = 0x200;  
    opcode = 0;
    I = 0;
    sp = 0;
 
    // Clear display	
    for (int i = 0; i < 2048; i++) { 
        display[i] = 0x0;
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

bool Chip8::loadROM(std::string filename)
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
    return true;
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

    
    //assert(pc < 0x210);

    
    addr = opcode & 0x0FFF;
    byte = opcode & 0x00FF;
    x = (opcode & 0x0F00) >> 8;
    y = (opcode & 0x00F0) >> 4;
   
    //printf("%X %X \naddr=%d byte=%d x=%d y=%d ", pc, opcode, addr, byte, x, y);


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

     /*
    for (int i = 0; i < 16; i++)
        std::cout << "v" << i  << "=" << (int) v[i] << " ";
    printf("I=%d", I);
    std::cout << std::endl << std::endl;

    std::cin.get();
    // */
}

void Chip8::CLS()
{
    for (int i = 0; i < 2048; i++) { 
        display[i] = 0x0;
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

void Chip8::SYS_addr(unsigned short addr)
{
    //  This instruction is only used on the old computers on which Chip-8 was originally implemented. It is ignored by modern interpreters.
    pc += 2;
}

void Chip8::JP_addr(unsigned short addr)
{
    pc = addr;
}

void Chip8::CALL_addr(unsigned short addr)
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
    if (v[x] >= v[y]) {
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
    if (v[y] >= v[x]) {
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

    v[x] <<= 1;
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

void Chip8::LD_I_addr(unsigned short addr)
{
    I = addr;
    pc += 2;
}

void Chip8::JP_V0_addr(unsigned short addr)
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
    bool keyPress = false;

    for(int i = 0; i < 16; ++i)
    {
        if(keypad[i] != 0)
        {
            v[x] = i;
            keyPress = true;
            
        }

    }

    // If we didn't received a keypress, skip this cycle and try again.
    if(!keyPress)						
        return;

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
    if(I + v[x] > 0xFFF)	// VF is set to 1 when range overflow (I+VX>0xFFF), and 0 when there isn't.
        v[0xF] = 1;
    else
        v[0xF] = 0;
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
    memory[I] = v[x] / 100;
    memory[I + 1] = (v[x] / 10) % 10;
    memory[I + 2] = (v[x] % 100) % 10;					
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
    for (int i = 0; i <= x; i++) {
        v[i] = memory[i + I];
    }
    pc += 2;
}

bool Chip8::loadApplication(const char * filename)
{
	//init();
	printf("Loading: %s\n", filename);
		
	// Open file
	FILE * pFile = fopen(filename, "rb");
	if (pFile == NULL)
	{
		fputs ("File error", stderr); 
		return false;
	}

	// Check file size
	fseek(pFile , 0 , SEEK_END);
	long lSize = ftell(pFile);
	rewind(pFile);
	printf("Filesize: %d\n", (int)lSize);
	
	// Allocate memory to contain the whole file
	char * buffer = (char*)malloc(sizeof(char) * lSize);
	if (buffer == NULL) 
	{
		fputs ("Memory error", stderr); 
		return false;
	}

	// Copy the file into the buffer
	size_t result = fread (buffer, 1, lSize, pFile);
	if (result != lSize) 
	{
		fputs("Reading error",stderr); 
		return false;
	}

	// Copy buffer to Chip8 memory
	if((4096-512) > lSize)
	{
		for(int i = 0; i < lSize; ++i)
			memory[i + 512] = buffer[i];
	}
	else
		printf("Error: ROM too big for memory");
	
	// Close file, free buffer
	fclose(pFile);
	free(buffer);

	return true;
}
