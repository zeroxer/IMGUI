#include <stdlib.h>
#include <stdio.h>
#include "SDL.h"

SDL_Window *window;
SDL_Surface *screen;

int screen_width = 640;
int screen_height = 480;

// Simplified interface to SDL_
void drawrect(int x, int y, int w, int h, int color)
{
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
    SDL_FillRect(screen, &r, color);
}

void render()
{
    // Get the window surface
    screen = SDL_GetWindowSurface(window);

    // Clear the screen.
    drawrect(0, 0, screen_width, screen_height, 0);

    // Draw a rectangle.
    drawrect(64, 48, 64, 48, 0xff);

    // Tell SDL to update the whole screen
    SDL_UpdateWindowSurface(window);

    // Don't take all the cpu time.
    SDL_Delay(10);
}

// Entry point
int main(int argc, char *argv[])
{
    //Initialize SDL's subsystems - in this case, only video.
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
        exit(1);
    }

    // Register SDL_Quit to be called at exit;
    // Make sure things are cleaned up when we quit.
    atexit(SDL_Quit);

    // Attempt to create a 640*480 window with 32bit pixels.
    window = SDL_CreateWindow("IMGUI - Learn 窗口", 
                SDL_WINDOWPOS_UNDEFINED, 
                SDL_WINDOWPOS_UNDEFINED, 
                screen_width, screen_height,
                // SDL_WINDOW_FULLSCREEN_DESKTOP |  
                SDL_WINDOW_OPENGL);

    // If we fail, return error.
    if (window == NULL)
    {
        fprintf(stderr, "Unable to set %d*%d video: %s\n", screen_width, screen_height, SDL_GetError());
        exit(1);
    }

    // Main Loop: loop forever.
    while(1)
    {
        // Render stuff
        render();

        // Poll for event, and handle the ones we care about.
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_KEYDOWN:
                {

                } break;

                case SDL_KEYUP:
                {
                    // If escape is pressed, return (and thus, quit)
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        return 0;
                    }
                } break;

                case SDL_QUIT:
                {
                    return 0;
                }
            }
        }
    }

    return 0;
}