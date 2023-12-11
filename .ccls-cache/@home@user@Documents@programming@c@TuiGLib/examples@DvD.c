#define TUIG_IMPLEMENTATION 
#include "tuig.h"

int main(void){
    float dvdx = 0;
    float dvdy = 0;
    float dvdvelx = 0.5;
    float dvdvely = 0.5;
    tg_InitScreen(70, 15);
    tg_SetFPS(30);
    while(1){
        if(dvdx+3>tg_GetScreenWidth() || dvdx<0){
            dvdvelx = -dvdvelx;
        }
        if(dvdy>tg_GetScreenHeight() || dvdy<0){
            dvdvely = -dvdvely;
        }
        dvdx+=dvdvelx;
        dvdy+=dvdvely;
        tg_BeginDraw();
            tg_ClearScreen();
            tg_DrawFPS(1, 1);
            tg_DrawText("DvD", dvdx, dvdy);
        tg_EndDraw();
    }
}
