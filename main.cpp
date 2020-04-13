/*
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
 
    chip8.loadROM("chip8.ch8");
    //chip8.loadApplication("pong.rom");

    //return 0;
    int i = 0;

    while(true)
    {
        i++;
        chip8.executeCycle();

        // Only draw to screen if draw flag is set
        if(chip8.getDrawFlag()) {
            drawGraphics(&chip8);
            std::cin.get();
            //chip8.resetDrawFlag();
        }
        
        chip8.setKeys();
        //std::cin.get();

        while (i % 100000 != 0) {
            i++;
        }
  	
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
            if (chip8->display[i * j + j] != 0) {
                std::cout << "██";
            } else {
                std::cout << "  ";
            }
        }
        std::cout << std::endl; 
        //for (auto i : chip8->display) {
        //    std::cout << (int) i << " ";
        //}
    }

    chip8->resetDrawFlag();
}
*/

#include <iostream>
#include <chrono>
#include <thread>
#include "stdint.h"
#include "SDL2/SDL.h"


#include "chip8.h"
#include "chip8.cpp"

using namespace std;

// Keypad keymap
uint8_t keymap[16] = {
    SDLK_x,
    SDLK_1,
    SDLK_2,
    SDLK_3,
    SDLK_q,
    SDLK_w,
    SDLK_e,
    SDLK_a,
    SDLK_s,
    SDLK_d,
    SDLK_z,
    SDLK_c,
    SDLK_4,
    SDLK_r,
    SDLK_f,
    SDLK_v,
};

int main(int argc, char **argv) {

    // Command usage
    if (argc != 2) {
        cout << "Usage: chip8 <ROM file>" << endl;
        return 1;
    }

    Chip8 chip8 = Chip8();          // Initialise Chip8

    int w = 1024;                   // Window width
    int h = 512;                    // Window height

    // The window we'll be rendering to
    SDL_Window* window = NULL;

    // Initialize SDL
    if ( SDL_Init(SDL_INIT_EVERYTHING) < 0 ) {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        exit(1);
    }
    // Create window
    window = SDL_CreateWindow(
            "CHIP-8 Emulator",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            w, h, SDL_WINDOW_SHOWN
    );
    if (window == NULL){
        printf( "Window could not be created! SDL_Error: %s\n",
                SDL_GetError() );
        exit(2);
    }

    // Create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_RenderSetLogicalSize(renderer, w, h);

    // Create texture that stores frame buffer
    SDL_Texture* sdlTexture = SDL_CreateTexture(renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            64, 32);

    // Temporary pixel buffer
    uint32_t pixels[2048];

    // Attempt to load ROM
    if (!chip8.loadROM(argv[1]))
        return 2;

    // Emulation loop
    while (true) {
        chip8.executeCycle();

        // Process SDL events
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) exit(0);

            // Process keydown events
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE)
                    exit(0);

                for (int i = 0; i < 16; ++i) {
                    if (e.key.keysym.sym == keymap[i]) {
                        chip8.keypad[i] = 1;
                        
                    }
                }
            }
            // Process keyup events
            if (e.type == SDL_KEYUP) {
                for (int i = 0; i < 16; ++i) {
                    if (e.key.keysym.sym == keymap[i]) {
                        chip8.keypad[i] = 0;
                    }
                }
            }
        }

        // If draw occurred, redraw SDL screen
        if (chip8.getDrawFlag()) {
            chip8.resetDrawFlag();

            // Store pixels in temporary buffer
            for (int i = 0; i < 2048; ++i) {
                unsigned char pixel = chip8.display[i];
                pixels[i] = (0x00FFFFFF * pixel) | 0xFF000000;
            }
            // Update SDL texture
            SDL_UpdateTexture(sdlTexture, NULL, pixels, 64 * sizeof(Uint32));
            // Clear screen and render
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, sdlTexture, NULL, NULL);
            SDL_RenderPresent(renderer);
        }

        // Sleep to slow down emulation speed
        std::this_thread::sleep_for(std::chrono::microseconds(1200));

    }
}