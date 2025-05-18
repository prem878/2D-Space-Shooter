#include "raylib.h"
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

#define FPS 60
#define MAX_ROCKS 20
#define MAX_BULLETS 69
#define px 128
#define MAX_ENEMYS 10

const int width=600;
const int height=800;

typedef struct Rock {
    float speed;
    bool active;
    Vector2 pos;
    Texture2D tex;
} Rock;

typedef struct Player {
    Texture2D tex;
    float speed;
    Vector2 pos;
} Player;

typedef struct Bullet {
    Texture2D tex;
    Vector2 pos;
    bool active;
    float speed;
} Bullet;

typedef struct Enemy {
    Texture2D s_tex;
    Texture2D b_tex;
    Vector2 pos;
    bool active;
    float speed;
} Enemy;

int main() {
    InitWindow(width,height,"Shoot them up");

    int frame_count=0;
    bool game_over = false;  // FIXED: Start with game_over = false
    SetTargetFPS(FPS);
    
    
    // -- Backgounds
    const char* rock_file[]= {"assets/128px/Asteroid 01_png_processed.png",
                          "assets/128px/Asteroid 02_png_processed.png",
                          "assets/128px/Asteroid 03_png_processed.png",
                          "assets/128px/Asteroid 04_png_processed.png"
    };

    Texture2D bullet= LoadTexture("assets/128px/Minigun_Small_png_processed.png");

    Texture2D space     = LoadTexture("assets/space.jpeg");

    // ---- Rocks  Begin---- //
    Texture2D rock_tex[4];
    srand(time(NULL));
    for(int i=0;i<4;++i)
        rock_tex[i]=LoadTexture(rock_file[i]);

    Rock rocks[MAX_ROCKS]={0};
    for(int i=0;i<MAX_ROCKS;++i) 
        rocks[i].active = false;
    // ---- Rocks end ---- //

    // ---- Bullets Begin ---- //
    Bullet bullets[MAX_BULLETS];
    for(size_t x=0;x<MAX_BULLETS;++x) {
        bullets[x].tex=bullet;
        bullets[x].active=false;
    }
    // ---- Bullets End ------//
    
    // ---- Enemy Begin-----//
    const char* enemy_ships[] ={
        "assets/128px/Enemy02_Teal_Frame_2_png_processed.png",
        "assets/128px/Enemy02Green_Frame_1_png_processed.png",
        "assets/128px/Enemy02Red_Frame_1_png_processed.png"
    };
    const char* enemy_bulletsf[] ={
        "assets/128px/Explosion02_Frame_02_png_processed.png",
        "assets/128px/Laser_Medium_png_processed.png",
        "assets/128px/Explosion01_Frame_02_png_processed.png"
    };
    Texture2D enemy_s_tex[3];
    Texture2D enemy_b_tex[3];
    
    // FIXED: Load enemy ship textures
    for(size_t i=0;i<3;++i) {
        enemy_s_tex[i]=LoadTexture(enemy_ships[i]);
        enemy_b_tex[i]=LoadTexture(enemy_bulletsf[i]);
    } 
    
    Enemy enemys[MAX_ENEMYS]; // FIXED: Use MAX_ENEMYS instead of MAX_BULLETS
    for(unsigned int x=0;x<MAX_ENEMYS;++x) {
        enemys[x].active=false;
        enemys[x].speed = 6.0f;
    }
    
    Bullet enemy_bullets[MAX_BULLETS];
    for(size_t x=0;x<MAX_BULLETS;++x) {
        enemy_bullets[x].tex=LoadTexture(enemy_bulletsf[rand()%3]);
        enemy_bullets[x].active=false;
        enemy_bullets[x].speed=4.0f;
    }

    // ---- Enemy End -----//

    //Background Speed 
    float space_speed =0.0f;

    Player player;
    player.tex      =  LoadTexture("assets/128px/PlayerRed_Frame_02_png_processed.png");
    player.pos      =  (Vector2) {width/2.0f-player.tex.width/2.0f,height-player.tex.height-120};
    player.speed    =  5.0f;

    while(!WindowShouldClose()) {
        frame_count++;
        //-- Updates 
        space_speed -=0.3f;
        if(space_speed <= -space.width) space_speed = 0;
        /*   Drawing  */
        BeginDrawing();
            
        ClearBackground(BLACK);
        DrawTextureEx(space,(Vector2){20,space_speed},0.0f,2.0f,WHITE);
        DrawTextureEx(space,(Vector2){20,space.width*2+space_speed},0.0f,2.0f,WHITE);

        /*Rocks Falling*/
        
        if(frame_count%70==0) {
            for(int x=0;x<MAX_ROCKS;++x) {
                if(rocks[x].active==false){
                    rocks[x].pos.x  = rand()%(width-px);
                    rocks[x].pos.y  = -px;
                    rocks[x].speed  = 1.5 + rand()%7;
                    rocks[x].tex    = rock_tex[rand()%4];
                    rocks[x].active = true;
                    break;
                }
            }
        }

        for(int id=0;id<MAX_ROCKS;++id) {
            if(rocks[id].active==true) {
                rocks[id].pos.y += rocks[id].speed;
                DrawTexture(rocks[id].tex,
                            (int) rocks[id].pos.x,
                            (int) rocks[id].pos.y,
                            WHITE );
                if( rocks[id].pos.y > height)
                    rocks[id].active= false;
                
            }
        }

        /* Enemy Ships - Reduced spawn frequency */
        if(frame_count % 180 ==0) {  // Changed from 60 to 180 (3 seconds between spawns)
            for(size_t x=0;x<MAX_ENEMYS;++x) {
                if(enemys[x].active!=true){
                    enemys[x].pos.x  = rand()%(width-px);
                    enemys[x].pos.y  = -px;
                    enemys[x].speed  = 0.3+ rand()%4;
                    size_t _i        = rand()%3;
                    enemys[x].s_tex  = enemy_s_tex[_i];
                    enemys[x].b_tex  = enemy_b_tex[_i];
                    enemys[x].active = true;
                    break;
                }
            }
            
        }

        for(size_t x=0;x<MAX_ENEMYS;++x) {
            if(enemys[x].active==true) {
                enemys[x].pos.y+=enemys[x].speed;
                DrawTexture(enemys[x].s_tex,
                            (int) enemys[x].pos.x,
                            (int) enemys[x].pos.y,
                            WHITE);
                if( enemys[x].pos.y > height ) 
                    enemys[x].active = false;
            }
        }


        /*Player Movement*/
        DrawTexture(player.tex,(int)player.pos.x,(int)player.pos.y,WHITE);
        if(IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W))  player.pos.y -= player.speed;
        if(IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S))  player.pos.y += player.speed;
        if(IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A))  player.pos.x -= player.speed;
        if(IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))  player.pos.x += player.speed;
        
        if(player.pos.x < 0) player.pos.x=0;
        if(player.pos.y < 0) player.pos.y=0;
        
        if(player.pos.x > width-player.tex.width)   player.pos.x=width-player.tex.width;
        if(player.pos.y > height-player.tex.height) player.pos.y=height-player.tex.height;

        if(IsKeyPressed(KEY_SPACE)) {
            for(size_t x=0 ; x <MAX_BULLETS ;++x) {
                if(bullets[x].active == false ){
                    bullets[x].pos.x  = player.pos.x + player.tex.width/2.0f-bullet.width/2.0f;
                    bullets[x].pos.y  = player.pos.y;
                    bullets[x].active = true;
                    bullets[x].speed  = -8.0f;
                    break;
                }
            }
        }

        // Draw and update player bullets
        for(size_t x=0;x<MAX_BULLETS;++x) {
            if(bullets[x].active == true) {
                bullets[x].pos.y += bullets[x].speed;
                DrawTexture(bullets[x].tex,(int)bullets[x].pos.x,(int)bullets[x].pos.y,WHITE);

                if(bullets[x].pos.y < -bullets[x].tex.height) {
                    bullets[x].active = false;
                }
            }
        }
       
        // Collision detection for player bullets and rocks
        for(size_t x=0;x<MAX_BULLETS;++x) {
            if(bullets[x].active == true) {
                Rectangle b_rect = {
                    bullets[x].pos.x,
                    bullets[x].pos.y,
                    bullets[x].tex.width,
                    bullets[x].tex.height
                };

                for(size_t y=0;y<MAX_ROCKS;++y) {
                    if(rocks[y].active == true) {
                        Rectangle r_rect = {
                            rocks[y].pos.x,
                            rocks[y].pos.y,
                            rocks[y].tex.width,
                            rocks[y].tex.height
                        };
                        if(CheckCollisionRecs(b_rect,r_rect)) {
                            rocks[y].active = false;
                            bullets[x].active = false;
                            break;
                        }
                    }
                }
            }
        }

        // Collision detection for rocks and player
        for(size_t x=0;x<MAX_ROCKS;++x) {
            if(rocks[x].active == true) {
                Rectangle p_rect = {
                    player.pos.x,
                    player.pos.y,
                    player.tex.width,
                    player.tex.height
                };
                Rectangle r_rect = {
                    rocks[x].pos.x,
                    rocks[x].pos.y,
                    rocks[x].tex.width,
                    rocks[x].tex.height
                };
                if(CheckCollisionRecs(p_rect,r_rect)){
                    game_over=true;
                }
            }
        }
        
        // Collision detection for player bullets and enemies
        for(size_t x=0;x<MAX_BULLETS;++x) {
            if(bullets[x].active == true) {
                Rectangle b_rect={
                    bullets[x].pos.x,
                    bullets[x].pos.y,
                    bullets[x].tex.width,
                    bullets[x].tex.height
                };
                for(size_t y=0;y<MAX_ENEMYS;++y){
                    if(enemys[y].active==true){
                        Rectangle e_rect = {
                            enemys[y].pos.x,
                            enemys[y].pos.y,
                            enemys[y].s_tex.width,
                            enemys[y].s_tex.height
                        };
                        if(CheckCollisionRecs(b_rect,e_rect)){
                            enemys[y].active=false;
                            bullets[x].active=false; 
                            break;
                        }
                    }
                }
            }
        }
        
        // Collision detection for enemies and player
        for(size_t x=0;x<MAX_ENEMYS;++x){
            if(enemys[x].active == true) {
                Rectangle e_rect = {
                    enemys[x].pos.x,
                    enemys[x].pos.y,
                    enemys[x].s_tex.width,
                    enemys[x].s_tex.height
                };
                Rectangle p_rect = {
                    player.pos.x,
                    player.pos.y,
                    player.tex.width,
                    player.tex.height
                };
                if(CheckCollisionRecs(p_rect,e_rect)){
                    game_over=true;
                    break;
                }
            }
        }

        if(game_over) {
            Rectangle popup = { width/2 - 100, height/2 - 80, 200, 160 };
            Rectangle replayBtn = { popup.x + 25, popup.y + 40, 150, 40 };
            Rectangle exitBtn   = { popup.x + 25, popup.y + 100, 150, 40 };

            Vector2 mouse = GetMousePosition();

            Color replayColor = CheckCollisionPointRec(mouse, replayBtn) ? GRAY : LIGHTGRAY;
            Color exitColor = CheckCollisionPointRec(mouse, exitBtn) ? GRAY : LIGHTGRAY;

            DrawRectangleRec(popup, DARKGRAY);
            DrawText("Game Over", popup.x + 30, popup.y + 10, 20, WHITE);
            DrawRectangleRec(replayBtn, replayColor);
            DrawText("Replay", replayBtn.x + 35, replayBtn.y + 10, 20, BLACK);
            DrawRectangleRec(exitBtn, exitColor);
            DrawText("Exit", exitBtn.x + 50, exitBtn.y + 10, 20, BLACK);

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (CheckCollisionPointRec(mouse, replayBtn)) {
                    game_over = false;
                    frame_count = 0;
                    for (int i = 0; i < MAX_ROCKS; ++i) rocks[i].active = false;
                    for (int i = 0; i < MAX_BULLETS; ++i) {
                        bullets[i].active = false;
                        enemy_bullets[i].active = false;
                    }
                    for (int i = 0; i < MAX_ENEMYS; ++i) enemys[i].active = false;
                    player.pos = (Vector2){ width / 2.0f - player.tex.width / 2.0f, height - player.tex.height - 120 };
                }
                if (CheckCollisionPointRec(mouse, exitBtn)) {
                    CloseWindow();
                    exit(0);
                }
            }
        }
        EndDrawing();
        /*   Ending */

    }

    // Cleanup resources
    for(int i=0;i<4;++i) 
        UnloadTexture(rock_tex[i]);

    for(int i=0;i<3;++i) {
        UnloadTexture(enemy_s_tex[i]);
        UnloadTexture(enemy_b_tex[i]);
    }

    // Unload bullets
    for(int i=0;i<MAX_BULLETS;++i) {
        if(enemy_bullets[i].tex.id > 0)
            UnloadTexture(enemy_bullets[i].tex);
    }

    // Deletion Stuff
    UnloadTexture(space);
    UnloadTexture(player.tex);
    UnloadTexture(bullet);
    CloseWindow();
    return 0;
}
