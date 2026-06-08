#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>

SDL_Texture* LoadTexture(SDL_Renderer* renderer, const char* file)
{
    SDL_Texture* texture = IMG_LoadTexture(renderer, file);
    if (!texture)
        printf("Failed to load image '%s': %s\n", file, SDL_GetError());
    return texture;
}

void DrawMarbleWall(SDL_Renderer* renderer, int winW, int winH)
{
    // Base marble color (off-white/cream)
    SDL_SetRenderDrawColor(renderer, 220, 215, 205, 255);
    SDL_FRect bg = { 0, 0, (float)winW, (float)winH };
    SDL_RenderFillRect(renderer, &bg);

    // Marble veins
    for (int i = 0; i < 18; i++)
    {
        int x = (i * 137) % winW;
        int y = (i * 89) % winH;
        int len = 150 + (i * 73) % 300;
        int angle = (i * 47) % 60 - 30;

        if (i % 3 == 0)
            SDL_SetRenderDrawColor(renderer, 180, 175, 165, 120);
        else if (i % 3 == 1)
            SDL_SetRenderDrawColor(renderer, 200, 190, 160, 80);
        else
            SDL_SetRenderDrawColor(renderer, 160, 155, 145, 100);

        for (int t = -1; t <= 1; t++)
        {
            for (int j = 0; j < len; j++)
            {
                float fx = (float)(x + j + t + (j * angle) / 100);
                float fy = (float)(y + (j * angle) / 60 + (j % 20) - 10 + t);
                SDL_FRect dot = { fx, fy, 2, 2 };
                SDL_RenderFillRect(renderer, &dot);
            }
        }
    }
}

void DrawGoldFrame(SDL_Renderer* renderer, SDL_Texture* texture,
    float x, float y, float w, float h)
{
    // Drop shadow
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 160);
    SDL_FRect shadow = { x + 8, y + 8, w, h };
    SDL_RenderFillRect(renderer, &shadow);

    // Outer gold border
    SDL_SetRenderDrawColor(renderer, 180, 140, 30, 255);
    SDL_FRect outer = { x, y, w, h };
    SDL_RenderFillRect(renderer, &outer);

    // Gold highlight layer
    SDL_SetRenderDrawColor(renderer, 220, 185, 60, 255);
    SDL_FRect highlight = { x + 4, y + 4, w - 8, h - 8 };
    SDL_RenderFillRect(renderer, &highlight);

    // Dark gold inner edge
    SDL_SetRenderDrawColor(renderer, 150, 110, 20, 255);
    SDL_FRect inner = { x + 10, y + 10, w - 20, h - 20 };
    SDL_RenderFillRect(renderer, &inner);

    // Image fills the frame completely
    SDL_FRect image = { x + 14, y + 14, w - 28, h - 28 };
    SDL_RenderTexture(renderer, texture, NULL, &image);
}

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    int winW = 1200, winH = 750;
    SDL_Window* window = SDL_CreateWindow("Art Gallery", winW, winH, SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

    SDL_Texture* img1 = LoadTexture(renderer, "C:/programming/Graphics Gallery/images/Blender1.png");
    SDL_Texture* img2 = LoadTexture(renderer, "C:/programming/Graphics Gallery/images/Blender2.png");
    SDL_Texture* img3 = LoadTexture(renderer, "C:/programming/Graphics Gallery/images/opengl.png");

    // Frame sizes
    float frameW = 340, frameH = 240;
    float smallW = 300, smallH = 220;

    // Top row - centered together
    float totalTopW = frameW + frameW + 60;
    float topStartX = (winW - totalTopW) / 2.0f;
    float topY = 60;

    float x1 = topStartX;
    float x2 = topStartX + frameW + 60;

    // Bottom frame - centered
    float x3 = (winW - smallW) / 2.0f;
    float bottomY = topY + frameH + 60;

    bool running = true;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
            if (event.type == SDL_EVENT_QUIT)
                running = false;

        DrawMarbleWall(renderer, winW, winH);

        DrawGoldFrame(renderer, img1, x1, topY, frameW, frameH);
        DrawGoldFrame(renderer, img2, x2, topY, frameW, frameH);
        DrawGoldFrame(renderer, img3, x3, bottomY, smallW, smallH);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(img1);
    SDL_DestroyTexture(img2);
    SDL_DestroyTexture(img3);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}