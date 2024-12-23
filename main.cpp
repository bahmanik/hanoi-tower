#include <SDL2/SDL.h>
#include <iostream>
#include "setting.h"
#include "tower.cpp"

void set_color (SDL_Renderer *, color);
void draw_tower(SDL_Renderer *, color, tower);
void move_ring (tower &A, tower &B, SDL_Renderer *);
void hanoi(int n, tower &A, tower &B, tower &C, SDL_Renderer *);
void show(SDL_Renderer *);
SDL_Rect *init_rings(int n);

//global variables
SDL_Rect *rings = init_rings(num);

tower towerA = {A_pos, TY_pos, rings, num, "A"};
tower towerB = {B_pos, TY_pos, nullptr, 0, "B"};
tower towerC = {C_pos, TY_pos, nullptr, 0, "C"};

int main(int argc, char *argv[])
{
    //inisializing sdl component 
    if (SDL_Init( SDL_INIT_VIDEO ) < 0){
        std::cout<<SDL_GetError();
    }

    // defining sdl variables
    SDL_Window   *window = SDL_CreateWindow("hanoi tower", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, hight, SDL_WINDOW_ALLOW_HIGHDPI);

    SDL_Renderer *render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Event windowEvent;

    //error cheaking
    if (NULL == render)
    {
        std::cout<<"could not create render";
        return 1;
    }

    if (NULL == window)
    {
        std::cout<<"could not create window";
        return 1;
    }

    //main loop
    while (true)
    {   
        show(render); 

        if (SDL_PollEvent(&windowEvent))
        {
            if (SDL_QUIT == windowEvent.type)
            {
                break;
            }
            if (SDL_KEYDOWN == windowEvent.type)
            {
                switch (windowEvent.key.keysym.sym)
                {
                case SDLK_DOWN:                 
                    hanoi(num, towerA, towerB, towerC, render);
                    break;
    
                case SDLK_UP:                 
                    break;
    
                default:
                    break;
                }
            }

        }
    }

    SDL_Quit();
    return EXIT_SUCCESS;
}

void set_color(SDL_Renderer *render, color Color){
    SDL_SetRenderDrawColor(render, Color.r, Color.g, Color.b, Color.alpha);
}

void draw_tower(SDL_Renderer *render, color toewr_color, tower tower_name){
    set_color(render, toewr_color);
    SDL_Rect temp = {tower_name.x_cor,tower_name.y_cor,tower_width,2*size};
    SDL_RenderFillRect(render, &temp);
    int SX_core = tower_name.x_cor + (tower_width/2) -size;
    int SY_core = tower_name.y_cor - 2*(num+1)*size;
    temp = {SX_core,SY_core,2*size, 2*(num+1)*size};
    SDL_RenderFillRect(render, &temp);
}

SDL_Rect *init_rings(int n){
    SDL_Rect *arr = new SDL_Rect[n];

    int rec_x = A_pos;
    int rec_y = TY_pos;
    int rec_w = tower_width;
    int rec_h = 2*size;
    int ratio = n>5?1:n>2?2:3;

    for (int i = 1; i < n+1; i++)
    {
        rec_x += ratio*size;
        rec_y -= 2*size;
        rec_w -= ratio*2*size;
        SDL_Rect temp = {rec_x, rec_y, rec_w, rec_h};
        arr[n-i] = temp;
    }
    return arr;
}

void move_ring(tower &A, tower &B, SDL_Renderer *render){

    if (A.len == 0)
    {
        return;
    }
     
    bool end = false;
    bool fn = true;
    int y_tow = A.y_cor - 2*(num+2)*size;
    int x_des = B.x_cor + (tower_width - A.rects[0].w)/2 ;
    int y_des = B.y_cor - (B.len+1)*2*size;
    bool fCompar = A.rects[0].x < x_des ? true : false;

    while (!end)
    {
        if (fn && A.rects[0].y>y_tow)
        {
            A.rects[0].y -= Aspeed;
            show(render);
            continue;
        }
        fn = false;
        if (A.rects[0].x<x_des)
        {
            A.rects[0].x += Aspeed;
            if (A.rects[0].x>x_des)
            {
                A.rects[0].x = x_des;
            }
            
            show(render);
            continue;
        }
        if (A.rects[0].x>x_des)
        {
            A.rects[0].x -= Aspeed;
            if (A.rects[0].x<x_des)
            {
                A.rects[0].x = x_des;
            }
            show(render);
            continue;
        }
        if (A.rects[0].y < y_des)
        {
            A.rects[0].y += Aspeed;
            show(render);
            continue;
        }
        end = true;
        SDL_Rect temp = A.pop();
        B.push(temp);
    }
    return;
}

void show(SDL_Renderer *render)
{
    set_color(render, fg);
    SDL_RenderClear(render);
    
    draw_tower(render, bg, towerA);
    draw_tower(render, bg, towerB);
    draw_tower(render, bg, towerC);

    set_color(render, rc);
    for (int i = 0; i < num; i++)
    for (int i = 0; i < towerA.len; i++)
    {
        SDL_RenderFillRect(render, &towerA.rects[i]);
    }
    
    for (int i = 0; i < towerB.len; i++)
    {
        SDL_RenderFillRect(render, &towerB.rects[i]);
    }
    
    for (int i = 0; i < towerC.len; i++)
    {
        SDL_RenderFillRect(render, &towerC.rects[i]);
    }
    
    SDL_RenderPresent(render);
}

void hanoi(int n, tower &A, tower &B, tower &C, SDL_Renderer *render){

    if (n == 1) {
        move_ring(A, B, render);
        return;
    }
    hanoi(n - 1, A, C, B, render);
    move_ring(A, B, render);
    hanoi(n - 1, C, B, A, render);
}