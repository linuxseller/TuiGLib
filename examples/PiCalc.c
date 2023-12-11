#include <stdlib.h>
#define TUIG_IMPLEMENTATION
#include "tuig.h"

#define SMAL_M_DEF 1
#define BIG_M_DEF 100
#define BIG_V_DEF -1

typedef struct Entity {
    Rectangle rec;
    int velx;
    int vely;
} Entity;

int random_int(int min, int max){
   return min + rand() % (max+1 - min);
}

int main(void){
    int32_t screenWidth = 70;
    int32_t screenHeight = 15;
    tg_InitScreen(screenWidth, screenHeight);
    tg_SetFPS(30);

    float size_unit = 2;    
    
    Rectangle ground =  {0, tg_GetScreenHeight()-size_unit, tg_GetScreenWidth(), size_unit};
    // configuring our booooys
    Rectangle big_boy =  {tg_GetScreenWidth()/2, ground.y-ground.height-size_unit*2, 5, 5};
    float big_boy_m = BIG_M_DEF;
    float big_boy_vel = BIG_V_DEF;
    Rectangle smal_boy = {tg_GetScreenWidth()/4, ground.y-ground.height, 2, 2};
    float smal_boy_vel = 0;
    float smal_boy_m = SMAL_M_DEF;
    int collisions = 0;
    char coll_str[15];
    char bbvels[30];
    char sbvels[30];
    while(!tg_ShouldExit()){
        size_unit = tg_GetScreenHeight()*0.1;
        // ground prop update
        ground.y = tg_GetScreenHeight()-2;
        ground.width = tg_GetScreenWidth();
        ground.height = size_unit;
        // big_boy props update
        
        if(big_boy.x<=smal_boy.x+smal_boy.width){
            float bbv = big_boy_vel;
            float sbv = smal_boy_vel;
            big_boy_vel = ((big_boy_m-smal_boy_m)/(big_boy_m+smal_boy_m))*bbv+(2*smal_boy_m/(big_boy_m+smal_boy_m))*sbv;
            smal_boy_vel = ((smal_boy_m-big_boy_m)/(big_boy_m+smal_boy_m))*sbv+(2*big_boy_m/(big_boy_m+smal_boy_m))*bbv;
            collisions++;
        }
        if(smal_boy.x<=0){
            smal_boy_vel = -smal_boy_vel;
            collisions++;
        }
        big_boy.y = ground.y-ground.height-size_unit*2;
        big_boy.x+=big_boy_vel;
        // smal_boy props update
        smal_boy.y = ground.y-ground.height;
        smal_boy.x+=smal_boy_vel;

        sprintf(coll_str, "PI: %d", collisions);
        sprintf(bbvels, "big boy vel: %.2f", big_boy_vel);
        sprintf(sbvels, "small boy vel: %.2f", smal_boy_vel);
        tg_BeginDraw();
            tg_ClearScreen();
            tg_DrawFPS(0, 0);
            tg_DrawText(coll_str, 0, 1);
            tg_DrawText(bbvels, 0, 2);
            tg_DrawText(sbvels, 0, 3);
            tg_DrawRectangleRec(big_boy);
            tg_DrawRectangleRec(smal_boy);
            tg_DrawLine((Vector2){0, ground.y+1}, (Vector2){tg_GetScreenWidth(), ground.y+1});
        tg_EndDraw();


    }
    tg_Exit();
    return 0;
}

