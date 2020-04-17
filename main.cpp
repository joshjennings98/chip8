// main.cpp

#include <iostream>
#include <chrono>
#include <thread>

#include "stdint.h"
#include "SDL2/SDL.h"
#include "chip8.h"
#include "chip8.cpp"

int main(int argc, char **argv) {

    Chip8 chip8 = Chip8();

    SDL_Window * window = NULL;
    SDL_Event e;

    int w, h;
    int screenBuffer[2048];
    uint8_t keys[16] = {SDLK_x, SDLK_1, SDLK_2, SDLK_3, SDLK_q, SDLK_w, SDLK_e, SDLK_a, SDLK_s, SDLK_d, SDLK_z, SDLK_c, SDLK_4, SDLK_r, SDLK_f, SDLK_v};

    if (argc < 2) {
        std::cout << "Need to provide rom file." << std::endl;
        return 1;
    } else if (argc == 2) {
        w = 1024;
        h = 512;
    } else if (argc == 4) {
        sscanf (argv[2],"%d", &w);
        sscanf (argv[3],"%d", &h);
    } else {
        std::cout << "Need to provide rom file (and optionally width and height)." << std::endl;
        return 1;
    }
    
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0 ) {
        std::cout << "SDL could not initialize: " << SDL_GetError() << std::endl;
        return 1;
    }

    window = SDL_CreateWindow("Josh's CHIP-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);

    if (window == NULL){
        std::cout << "Window could not be created: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0); // Create renderer
    SDL_RenderSetLogicalSize(renderer, w, h);

    // Create texture that stores frame buffer
    SDL_Texture* sdlTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
    
    if (!chip8.loadROM(argv[1])) {
        std::cout << "Failed to load ROM: " << argv[1] << std::endl;
        return 1;
    }

    while (true) {
        
        chip8.executeCycle();

        // Handle SDL events
        while (SDL_PollEvent(&e)) {
            
            // Handle key press events
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    return 0;
                }

                for (int i = 0; i < 16; i++) {
                    if (e.key.keysym.sym == keys[i]) {
                        chip8.keypad[i] = 1;
                    }
                }
            }

            // Handle key release events
            if (e.type == SDL_KEYUP) {
                for (int i = 0; i < 16; i++) {
                    if (e.key.keysym.sym == keys[i]) {
                        chip8.keypad[i] = 0;
                    }
                }
            }
        }

        // Only redraw if draw flag is enabled
        if (chip8.getDrawFlag()) {
            for (int i = 0; i < 2048; i++) {
                screenBuffer[i] = (0x00FFFFFF * chip8.display[i]) | 0xFF000000;
            }
            
            SDL_UpdateTexture(sdlTexture, NULL, screenBuffer, 64 * sizeof(Uint32)); // Update SDL texture
            
            // Clear screen and render
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, sdlTexture, NULL, NULL);
            SDL_RenderPresent(renderer);

            chip8.resetDrawFlag();
        }

        // Slow down emulation speed
        std::this_thread::sleep_for(std::chrono::microseconds(1200));
    }
}