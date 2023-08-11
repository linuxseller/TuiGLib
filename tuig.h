/* Game Tui Library Developed By Iurchenko Alexandr 2023
 * -----------------------------------------------------
 *
 *  USAGE:
 *  ```
 *  #define TUIG_IMPLEMEBTATION
 *  #include "tuig.h"
 *  ```
 *
 *  screen - outputed chars
 *  buffer - data stiored in memory
 *
 *  Library Structture
 *  +- Screen structure contains
 *  |  +- Output scren size (width, height)
 *  |  +- Pointer to a buffer, of integers needed to configure screendrawing
 *  |
 *  +- Game structure contains
 *  |  +- Game fps that set by tg_SetFPS() proc
 *  |  +- Actual fps of game (limited by IO delays, game loop slow down etc)
 *  |
 *  +- Functions such as
 *  |  +- tg_InitScreen(int32_t width, int32_t height) : sets up buffer, game struct and inits Ncurses
 *  |  +- tg_SetFPS(float fps) : sets expectes fps
 *  |  +- tg_GetFPS : returns actual fps
 *  |  +- tg_BeginDraw : does not do anything
 *  |  +- tg_EndDraw : prints to screen using 'ncurses' and waits to control FPS
 *  |  +- tg_ClearScreen : flashes buffer
 *  |  +- tg_DrawFPS(float x, float y) : actual fps
 *  |  +- tg_DrawText(char* str, float x, float y)
 *  |  +- tg_DrawLine(Vector2 src, Vector2 dst)
 *  |  +- tg_DrawTriangle(Vector2 dot1, Vector2 dot2, Vector2 dot3);
 *  |  +- tg_DrawRectangleRec(Rectangle rec)
 *  |  +- tg_GetTime : returns time in milliseconds
 *  |  +- tg_GetScreenHeight
 *  |  +- tg_GetScreenWidth
 *
 *
 *  How Does It work?
 *  +- Draw functions write arbitrary numbers to buffers, then when printing check happens
 *  |  +- If text is being drawn ascii char is stored as buf[idx] value 
 *  +- Printing
 *  |  +- if buf[idx]==1 print '#'
 *  |  |  else if buf[idx]==0 print ' '
 *  |  |  else print buf[idx]
 *  |  |
 *  |  +- This is made for printing text like in 'tg_DrawFPS' or 'tg_DrawText'
 *  |
 *  +-  
*/



#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <sys/time.h>
#include <curses.h>
#include <signal.h>

typedef struct Vector2 {
    float x;
    float y;
} Vector2;

typedef struct Rectangle {
    float x;                // Rectangle top-left corner position x
    float y;                // Rectangle top-left corner position y
    float width;            // Rectangle width
    float height;           // Rectangle height
} Rectangle;

typedef struct Screen {
    uint32_t width, height;
    uint8_t *screen;
} Screen;
typedef struct Game {
    float fps;
    uint32_t act_fps;
    char act_fps_cstr[10];

    uint64_t time_old;
} Game;

void tg_InitScreen(int32_t width, int32_t height); // Initialising library
void tg_SetFPS(float fps); // Set FPS program will slow up to
float tg_GetFPS(void); // Get actual FPS
void tg_EndDraw(void); // Ends draw
void tg_BeginDraw(void); // Begins draw
void tg_ClearScreen(void); // Empties draw buffer
void tg_DrawFPS(float x, float y); // Draws FPS at (X,Y)
void tg_DrawText(char* str, float x, float y); // Draw text at x y
void tg_DrawLine(Vector2 src, Vector2 dst); // Draws line from 'src' point to 'dst'
void tg_DrawTriangle(Vector2 dot1, Vector2 dot2, Vector2 dot3);
void tg_DrawRectangleRec(Rectangle rec); // Draws rectangle
uint64_t tg_GetTime(void); // Get time in milliseconds
float tg_GetScreenHeight(void);
float tg_GetScreenWidth(void);

#ifdef TUIG_IMPLEMEBTATION
Screen screen;
Game game;


void tg_InitScreen(int32_t width, int32_t height){
    initscr(); // ncurses init
    nonl();
    cbreak();
    screen.width = width;
    screen.height = height;
    game.time_old = tg_GetTime();
    game.fps = 30;
    screen.screen = malloc(width*height*sizeof(uint8_t));
    refresh();
}


void tg_SetFPS(float fps){
    game.fps = fps;
}

uint64_t tg_GetTime(void){
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    uint64_t milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    return milliseconds;
}

void tg_BeginDraw(void){
}

void tg_EndDraw(void){
    for(uint32_t y=0; y<screen.height; y++){
        move(y, 0);
        for(uint32_t x=0; x<screen.width; x++){
            char currcell = screen.screen[y*screen.width + x];
            switch (currcell){
                case 1:
                    addch('#');
                    break;
                case 0:
                    addch(' ');
                    break;
                default:
                    addch(currcell);
            }
        }
    }
    refresh();
    uint64_t tempTimeC = tg_GetTime();
    while(tempTimeC-game.time_old<(float)1/game.fps*1000){
        tempTimeC = tg_GetTime();
    }
    game.act_fps = (float)1/(tg_GetTime()-game.time_old)*1000;
    game.time_old = tg_GetTime();
}

float tg_GetScreenHeight(void){return screen.height; }
float tg_GetScreenWidth(void){return screen.width; }


void tg_ClearScreen(void){
    for(uint32_t i=0; i<screen.width*screen.height; i++){
        screen.screen[i]=0;
    }
}

void tg_DrawFPS(float x, float y){
    sprintf(game.act_fps_cstr, "FPS: %d", game.act_fps);
    for(int i=0; game.act_fps_cstr[i]!=0; i++){
        screen.screen[(int)(floor(y*screen.width)+floor(x)+i)] = game.act_fps_cstr[i];
    }
}

void tg_DrawText(char *str, float x, float y){
    for(int i=0; str[i]!=0; i++){
        screen.screen[(int)(round(y)*screen.width+round(x)+i)] = str[i];
    }
}

float sgn(float x){
    return (x==0) ? 0 : (x>0) ? 1 : -1; 
} 

// Bresenham's line algorithm implementation
void tg_DrawLine(Vector2 src, Vector2 dst){
    float dx,dy,sdx,sdy,px,py,dxabs,dyabs,i;
    float slope;

    float x1 = src.x;
    float x2 = dst.x;
    float y1 = src.y;
    float y2 = dst.y;

    dx = x2-x1;
    dy = y2-y1;
    dxabs = fabs(dx);
    dyabs = fabs(dy);
    sdx = sgn(dx);
    sdy = sgn(dy);
    if (dxabs>=dyabs){
        slope=(float)dy / (float)dx;
        for(i=0;i<dx;i+=sdx){
            px=i+x1;
            py=slope*i+y1;
            unsigned int tempy = floor(py)*screen.width;
            unsigned int tempx = floor(px);
            if(px>screen.width || py > screen.height || px<0 || py < 0){continue;}
            screen.screen[tempx+tempy]=1;
        }
    } else {
        slope=(float)dx / (float)dy;
        for(i=0;i<dy;i+=sdy){
            px=slope*i+x1;
            py=i+y1;
            unsigned int tempy = floor(py)*screen.width;
            unsigned int tempx = floor(px);
            if(px>screen.width || py > screen.height || px<0 || py < 0){continue;}
            screen.screen[tempx+tempy]=1;
        }
    }
}
void tg_DrawRectangleRec(Rectangle rec){
    Vector2 dot1 = {rec.x, rec.y};
    Vector2 dot2 = {rec.x+rec.width, rec.y};
    Vector2 dot3 = {rec.x+rec.width, rec.y+rec.height};
    Vector2 dot4 = {rec.x, rec.y+rec.height};
    tg_DrawLine(dot1, dot2);
    tg_DrawLine(dot2, dot3);
    tg_DrawLine(dot3, dot4);
    tg_DrawLine(dot4, dot1);
}

void tg_DrawTriangle(Vector2 dot1, Vector2 dot2, Vector2 dot3){
    tg_DrawLine(dot1, dot2);
    tg_DrawLine(dot2, dot3);
    tg_DrawLine(dot3, dot1);
}
#endif
