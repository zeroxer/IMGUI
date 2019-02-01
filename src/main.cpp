#include <stdlib.h>
#include <stdio.h>
#include "SDL.h"

SDL_Window *window;
SDL_Surface *screen;

void render()
{
    // Get the window surface
    screen = SDL_GetWindowSurface(window);

    // Lock surface if needed
    if (SDL_MUSTLOCK(screen))
    {
        if (SDL_LockSurface(screen) < 0)
        {
            return;
        }
    }


    // Ask SDL for the time in milliseconds
    int tick = SDL_GetTicks();

    // Declare a couple of variables
    int i, j, yofs, ofs;

    // Draw to screen
    yofs = 0;
    for (int i = 0; i < 480; i++)
    {
        for (int j = 0, ofs = yofs; j < 640; j++, ofs++)
        {
            ((unsigned int*)screen->pixels)[ofs] = i * i + j * j + tick;
        }
        yofs += screen->pitch / 4;
    }

    // Unlock if needed
    if (SDL_MUSTLOCK(screen))
    {
        SDL_UnlockSurface(screen);
    }    

    // // 另外一种绘制方式
    // SDL_FillRect( screen, NULL, SDL_MapRGB( screen->format, 0x00, 0x00, 0xFF ) );


    // Tell SDL to update the whole screen
    SDL_UpdateWindowSurface(window);
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
                640, 480,
                // SDL_WINDOW_FULLSCREEN |  
                SDL_WINDOW_OPENGL);

    // If we fail, return error.
    if (window == NULL)
    {
        fprintf(stderr, "Unable to set 640*480 video: %s\n", SDL_GetError());
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