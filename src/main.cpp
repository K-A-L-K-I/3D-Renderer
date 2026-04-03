#include <SDL.h>
#include <cstdint>
#include <cstdlib>
#include <iostream>

constexpr int CANVAS_WIDTH  = 600;
constexpr int CANVAS_HEIGHT = 600;

static uint32_t framebuffer[CANVAS_WIDTH * CANVAS_HEIGHT];

void PutPixel(int x, int y, uint32_t color) {
    int sx = CANVAS_WIDTH  / 2 + x;
    int sy = CANVAS_HEIGHT / 2 - y;

    if (sx < 0 || sx >= CANVAS_WIDTH || sy < 0 || sy >= CANVAS_HEIGHT)
        return;

    framebuffer[sy * CANVAS_WIDTH + sx] = color;
}

uint32_t MakeColor(uint8_t r, uint8_t g, uint8_t b) {
    return (0xFF << 24) | (r << 16) | (g << 8) | b;
}

void ClearCanvas(uint32_t color) {
    for (int i = 0; i < CANVAS_WIDTH * CANVAS_HEIGHT; i++)
        framebuffer[i] = color;
}

void DrawDemoScene() {
    for (int x = 0; x <= 255; x++) {
        for (int y = 0; y <= 255; y++) {
            PutPixel(x, y, MakeColor(x, y, 128)); 
        }
    }
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "CG from Scratch",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        CANVAS_WIDTH, CANVAS_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        CANVAS_WIDTH, CANVAS_HEIGHT
    );

    ClearCanvas(MakeColor(15, 15, 15));
    DrawDemoScene();

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
                running = false;
        }

        SDL_UpdateTexture(texture, nullptr, framebuffer, CANVAS_WIDTH * sizeof(uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
