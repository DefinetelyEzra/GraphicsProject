#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Unable to initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Side-by-Side Images", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 600, 0);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "Failed to initialize SDL_image: " << IMG_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    return true;
}

bool loadAndRenderImages(const char* imagePath1, const char* imagePath2) {
    SDL_Surface* surface1 = IMG_Load(imagePath1);
    SDL_Surface* surface2 = IMG_Load(imagePath2);
    if (!surface1 || !surface2) {
        std::cerr << "Failed to load images: " << IMG_GetError() << std::endl;
        return false;
    }

    SDL_Texture* texture1 = SDL_CreateTextureFromSurface(renderer, surface1);
    SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer, surface2);
    SDL_FreeSurface(surface1);
    SDL_FreeSurface(surface2);
    if (!texture1 || !texture2) {
        std::cerr << "Failed to create textures: " << SDL_GetError() << std::endl;
        return false;
    }

    // Clear the renderer
    SDL_RenderClear(renderer);

    // Calculate the position and size for each image
    SDL_Rect rect1 = { 100, 100, 800, 474 };
    SDL_Rect rect2 = { 600, 100, 700, 474 };

    // Render the textures to the renderer
    SDL_RenderCopy(renderer, texture1, NULL, &rect1);
    SDL_RenderCopy(renderer, texture2, NULL, &rect2);

    // Render to the window
    SDL_RenderPresent(renderer);

    return true;
}

void closeSDL() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    if (!initSDL()) {
        return -1;
    }

    if (!loadAndRenderImages("image1.png", "image2.png")) {
        closeSDL();
        return -1;
    }

    SDL_Delay(50000);  // Show images for 5 seconds

    closeSDL();
    return 0;
}