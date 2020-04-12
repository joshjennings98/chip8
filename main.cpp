#include<iostream>
#include<fstream>
#include<string>
#include <stdlib.h>
#include"chip8.h"
#include"chip8.cpp"

void setupGraphics();
void setupInput();
void drawGraphics(Chip8 * chip8);

int main()
{
    setupGraphics();
    setupInput();

    Chip8 chip8;
 
    chip8.loadROM("pong.rom");

    //return 0;

    while(true)
    {
        chip8.executeCycle();

        // Only draw to screen if draw flag is set
        if(chip8.getDrawFlag()) {
            drawGraphics(&chip8);
            std::cin.get();
            chip8.resetDrawFlag();
        }
        
        chip8.setKeys();
        //std::cin.get();
  	
    }

    return 0;
}

void setupGraphics()
{

}

void setupInput()
{

}

void drawGraphics(Chip8 * chip8)
{
    //system("clear");
    for (int i = 0; i < 10; i++) {
        printf("\n\n\n\n\n\n\n\n");
    }
    
    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 64; j++) {
            if (chip8->display[i][j] != 0) {
                std::cout << "██";
            } else {
                std::cout << "  ";
            }
            //std::cout << (int) chip8->display[i][j] << " ";
        }
        std::cout << std::endl;
    }

    chip8->resetDrawFlag();
}
