#include<iostream>
#include"chip8.h"
#include"chip8.cpp"

void setupGraphics();
void setupInput();
void drawGraphics();

int main()
{
    setupGraphics();
    setupInput();

    Chip8 chip8;
 
    chip8.loadROM("pong");

    while(true)
    {
        chip8.executeCycle();

        // Only draw to screen if draw flag is set
        if(chip8.getDrawFlag()) {
            drawGraphics();
        }
        
        chip8.setKeys();	
    }

    return 0;
}

void setupGraphics()
{

}

void setupInput()
{

}

void drawGraphics()
{

}
