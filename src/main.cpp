#include <stdlib.h>
#include <stdio.h>
#include "SDL.h"

// Generate unique id for each widget
// 把const char *（指针）强制转换成int类型。
// 因为指针的值是内存地址，每一个GEN_ID都有一个自己的内存地址，
// 所以GEN_ID可以说是唯一的。（但是，因为内存地址是64位，int是32位，所以其中会有一些有待商榷的问题。）
#define STRINGERMACRO(x) #x
#define GEN_ID (int) (__FILE__ STRINGERMACRO(__LINE__))

SDL_Window *window;
SDL_Surface *screen;

int screen_width = 640;
int screen_height = 480;

struct UIState
{
    int mousex;
    int mousey;
    int mousedown;

    int hotitem;
    int activeitem;

    // Keyboard process
    int kbditem;
    int keyentered;
    int keymod;

    int lastwidget;
}
uistate = {0, 0, 0, 0, 0, 0, 0, 0, 0};

// Simplified interface to SDL
void drawrect(int x, int y, int w, int h, int color)
{
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
    SDL_FillRect(screen, &r, color);
}

// Check whether current mouse position is within a rectangle.
int regionhit(int x, int y, int w, int h)
{
    if (uistate.mousex < x ||
        uistate.mousey < y ||
        uistate.mousex > x + w ||
        uistate.mousey > y + h)
        {
            return 0;
        }
    return 1;
}

// Simple button IMGUI widget
int button(int id, int x, int y)
{
    // Check whether the button should be hot
    if (regionhit(x, y, 64, 48))
    {
        uistate.hotitem = id;
        if (uistate.activeitem == 0 && uistate.mousedown)
        {
            uistate.activeitem = id;
        }
    }

    // If no widget has keyboard focus, take it
    if (uistate.kbditem == 0)
    {
        uistate.kbditem = id;
    }

    // If we have keyboard focus, show it
    if (uistate.kbditem == id)
    {
        drawrect(x-6, y-6, 84, 68, 0xff0000);
    }

    // Render button -- base button shadow
    drawrect(x+8, y+8, 64, 48, 0);

    // Button is hot.
    if (uistate.hotitem == id)
    {
        if (uistate.activeitem == id)
        {
            // Button is both 'hot' and 'active' -- change color and position
            drawrect(x+2, y+2, 64, 48, 0xffffff);
        } else {
            // Button is merely 'hot' -- change color
            drawrect(x, y, 64, 48, 0xffffff);
        }
    } else {
        // Button is not hot, but it may be active -- normal front color of button
        drawrect(x, y, 64, 48, 0xaaaaaa);
    }

    // If we have keyboard focus, we'll need to process the keys
    if (uistate.kbditem == id)
    {
        switch (uistate.keyentered)
        {
            case SDLK_TAB:
            {
                // If tab is pressed, lose keyboard focus.
                // Next widget will grab the focus.
                // 说明：为什么下一个控件会获取TAB键消息？因为代码的处理顺序。
                // 这一个button的判断执行完之后，程序会执行下一个button的判断程序。
                // 程序的循环处理逻辑使得TAB键可以实现按照一定的顺序循环切换。
                // 注意：下一个button获取焦点的逻辑为：如果没有控件已经获取了焦点，那我就获取焦点。
                uistate.kbditem = 0;

                // If shift was also pressed, we want to move focus
                // to the previous widget instead.
                if (uistate.keymod & KMOD_SHIFT)
                {
                    uistate.kbditem = uistate.lastwidget;
                }

                // Also clear the key so that next widget won't process it.
                uistate.keyentered = 0;
            } break;

            case SDLK_RETURN:
            {
                // Had keyboard focus, received return.
                // So we'll act as if we were clicked.
                return 1;
            } break;
        
            default:
                break;
        }
    }

    uistate.lastwidget = id;

    // Check whether user has click the button.

    // If button is hot and active, but mouse button is not
    // down, the user must have clicked the button.
    if (uistate.mousedown == 0 &&
        uistate.hotitem == id &&
        uistate.activeitem == id)
        {
            return 1;
        }

    // Otherwise, no clicky.
    return 0;

}

// Simple scroll bar IMGUI widget
int slider(int id, int x, int y, int max, int &value)
{
    // Calculate mouse cursor's relative y offset
    // scrollbar高度：256像素
    int ypos = ((256 - 16) * value) / max;

    // Check for hotness
    // Scrollbar的热区
    if (regionhit(x+8, y+8, 16, 255))
    {
        uistate.hotitem = id;
        if (uistate.activeitem == 0 && uistate.mousedown)
        {
            uistate.activeitem = id;
        }
    }

    // If no widget has keyboard focus, take it
    if (uistate.kbditem == 0)
    {
        uistate.kbditem = id;
    }

    if (uistate.kbditem == id)
    {
        drawrect(x-4, y-4, 40, 280, 0xff0000);
    }

    // Render the scrollbar
    drawrect(x, y, 32, 256+16,0x777777);

    if (uistate.activeitem == id || uistate.hotitem == id)
    {
        drawrect(x+8, y+8+ypos, 16, 16, 0xffffff);
    } else {
        drawrect(x+8, y+8+ypos, 16, 16, 0xaaaaaa);
    }

    // If we have keyboard focus, we'll need to process the keys
    if (uistate.kbditem == id)
    {
        switch (uistate.keyentered)
        {
            case SDLK_TAB:
            {
                // If tab is pressed, lose keyboard focus
                // Next widget will grab the focus
                uistate.kbditem = 0;

                // If shift was also pressed, we want to move focus
                // to the previous widget instead.
                if (uistate.keymod & KMOD_SHIFT)
                {
                    uistate.kbditem = uistate.lastwidget;
                }

                // Also clear the key so that next widget won't process it.
                uistate.keyentered = 0;
            } break;

            case SDLK_UP:
            {
                // Slide slider up (if not at zero)
                if (value > 0)
                {
                    value--;
                    return 1;
                }
            } break;

            case SDLK_DOWN:
            {
                // Slide slider down (if not at max)
                if (value < max)
                {
                    value++;
                    return 1;
                }
            } break;
        }
    }

    uistate.lastwidget = id;

    // Update widget value
    if (uistate.activeitem == id)
    {
        int mousepos = uistate.mousey - (y + 8);
        if (mousepos < 0)
        {
            mousepos = 0;
        }
        if (mousepos > 255)
        {
            mousepos = 255;
        }
        // 计算当前滑块应该在的位置。如果位置不对，则更新位置。
        // int v = (mousepos / 255) * max;
        int v = (mousepos * max) / 255;
        if (v != value)
        {
            value = v;
            return 1;
        }
    }

    return 0;
}

// Prepare for IMGUI code
void imgui_prepare()
{
    uistate.hotitem = 0;
}

// Finish up after IMGUI code
void imgui_finish()
{
    // 如果鼠标左键松开，立刻取消Button的激活状态。
    if (uistate.mousedown == 0)
    {
        uistate.activeitem = 0;
    } else {
        // if (uistate.activeitem == 0)
        // {
        //     // ???
        //     uistate.activeitem = -1;
        // }
    }

    // If no widget grabbed tab, clear focus
    if (uistate.keyentered == SDLK_TAB)
    {
        uistate.kbditem = 0;
    }

    // Clear the entered key
    uistate.keyentered = 0;
}

void render()
{
    static int bgcolor = 0x77;

    // Get the window surface
    screen = SDL_GetWindowSurface(window);

    // Clear the screen.
    drawrect(0, 0, screen_width, screen_height, bgcolor);

    imgui_prepare();

    button(GEN_ID, 50, 50);    
    
    button(GEN_ID, 150, 50);

    if (button(3, 50, 150))
    {
        bgcolor = (SDL_GetTicks() * 0x0cac01a) | 0x77;
    }

    if (button(4, 150, 150))
    {
        exit(0);
    }

    // Scrollbar test

    // 通过控制移位数和slider的max值，来实现不同的范围控制。

    // Blue
    int slidervalue = bgcolor & 0xff;
    if (slider(GEN_ID, 500, 40, 255, slidervalue))
    {
        bgcolor = (bgcolor & 0xffff00) | slidervalue;
    }

    // Green
    // 其中：10, 0x3f, 63, 是根据对应关系计算出来的。
    slidervalue = ((bgcolor >> 10) & 0x3f);
    if (slider(GEN_ID, 550, 40, 63, slidervalue))
    {
        bgcolor = (bgcolor & 0xff00ff) | (slidervalue << 10);
    }

    //Red
    slidervalue = ((bgcolor >> 20) & 0xf);
    if (slider(GEN_ID, 600, 40, 15, slidervalue))
    {
        bgcolor = (bgcolor & 0x00ffff) | (slidervalue << 20);
    }

    imgui_finish();

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
                // Update mouse position.
                case SDL_MOUSEMOTION:
                {
                    uistate.mousex = event.motion.x;
                    uistate.mousey = event.motion.y;
                } break;

                // Update button down state if left-clicking.
                case SDL_MOUSEBUTTONDOWN:
                {
                    if (event.button.button == 1 )
                    {
                        uistate.mousedown = 1;
                    }
                } break;

                // Update button down state if left-clicking.
                case SDL_MOUSEBUTTONUP:
                {
                    if (event.button.button == 1)
                    {
                        uistate.mousedown = 0;
                    }
                } break;

                case SDL_KEYDOWN:
                {
                    // If a key if pressed, report it to the widgets
                    uistate.keyentered = event.key.keysym.sym;
                    uistate.keymod = event.key.keysym.mod;
                } break;

                case SDL_KEYUP:
                {
                    switch (event.key.keysym.sym)
                    {
                        // If escape is pressed, return (and thus, quit)
                        case SDLK_ESCAPE:
                        {
                            return 0;
                        }
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