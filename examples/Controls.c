#define TUIG_IMPLEMENTATION 
#include "tuig.h"

int main(void){
    Rectangle player = {0,0,5,5};
    tg_InitScreen(70, 15);
    tg_SetFPS(30);
    while(!tg_ShouldExit()){
        char key = tg_GetKeyPressed();
        switch(key){
            case 'w':
                player.y--;
                break;
            case 's':
                player.y++;
                break;
            case 'a':
                player.x--;
                break;
            case 'd':
                player.x++;
        }
        printf("key: %c;", key);
        tg_BeginDraw();
            tg_ClearScreen();
            tg_DrawFPS(1, 1);
            tg_DrawRectangleRec(player);
        tg_EndDraw();
    }
    tg_Exit();
}
