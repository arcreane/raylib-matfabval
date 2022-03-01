/*******************************************************************************************
*
*   raylib [core] example - 2d camera platformer
*
*   This example has been created using raylib 2.5 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Example contributed by arvyy (@arvyy) and reviewed by Ramon Santamaria (@raysan5)
*
*   Copyright (c) 2019 arvyy (@arvyy)
*
********************************************************************************************/

#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <iostream>
#include <string>

//include de Class

#include "Game.h"

//include structures & fonctions
#include "structures.h"



using namespace std;


typedef enum GameMoment { DEBUT, CHOISIRPARTIE, ENJEU };

void UpdatePlayer(Player* player, EnvItem* envItems, int envItemsLength, float delta);
void UpdateMAPmonde1(Player* player, EnvItem* envItems, int envItemsLength, float delta, Game* g1);
void UpdateCameraCenter(Camera2D* camera, Player* player, EnvItem* envItems, int envItemsLength, float delta, int width, int height);
void UpdateCameraCenterInsideMAPmonde1(Camera2D* camera, Player* player, EnvItem* envItems, int envItemsLength, float delta, int width, int height);
void UpdateCameraCenterSmoothFollow(Camera2D* camera, Player* player, EnvItem* envItems, int envItemsLength, float delta, int width, int height);
void UpdateCameraEvenOutOnLanding(Camera2D* camera, Player* player, EnvItem* envItems, int envItemsLength, float delta, int width, int height);
void UpdateCameraPlayerBoundsPush(Camera2D* camera, Player* player, EnvItem* envItems, int envItemsLength, float delta, int width, int height);


int main(void)
{
    Game g1;
    g1.start();



    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1300;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Mario & DK Bros");

    //s'occupe du cube dans ENJEU
    Player player = { 0 };
    player.position = { 20 , 0 };
    player.speed = 0;
    player.canJump = false;

    //s'occupe du cube dans CHOISIRPARTIE
    Player playerMENU = { 0 };
    playerMENU.position = { 20 , 0 };
    playerMENU.speed = 0;
    playerMENU.canJump = false;

    //créer la MAPmonde1
    EnvItem envItems[] = {
        // 1 :X    -- 2:Y    --- 3:Distance    --- 4 : Hauteur
        // 1 : => + -- vers le bas +
        {{ -1000, -1000, 2000, 0 }, 0, LIGHTGRAY },
        {{ 0, 0, 10000, 200 }, 1, GRAY },
        {{ 300, 200, 400, 10 }, 1, GRAY },
        {{ 250, 300, 100, 10 }, 1, GRAY },
        {{ 650, 300, 100, 10 }, 1, GRAY }
    };

    EnvItem envItems2[] = {
    {{ -1000, -1000, 2000, 400 }, 0, LIGHTGRAY },
    {{ 0, 400, 10000, 200 }, 1, BLUE },
    {{ 300, 200, 400, 10 }, 1, BLUE},
    {{ 250, 300, 100, 10 }, 1, BLUE},
    {{ 650, 300, 100, 10 }, 1, BLUE}
    };

    EnvItem MAPmonde1[] = {
    {{ -1000, -1000, 2000, 400 }, 0, LIGHTGRAY },
    {{ 0,0, 10000, 200 }, 1, DARKBROWN },
    {{ 0, 0, 40 , 40 }, 1, GREEN },
    {{ 300, 0, 40 , 40 }, 1, GREEN },
    {{ 600, 0, 40 , 40 }, 1, GREEN },
    {{ 900, 0, 40 , 40 }, 1, GREEN },
    {{ 1200, 0, 40 , 40 }, 1, GREEN },
    {{ 1500, 0, 40 , 40 }, 1, GREEN }
    };

    int envItemsLength = sizeof(envItems) / sizeof(envItems[0]);
    int envItemsLengthMAPmonde1 = sizeof(MAPmonde1) / sizeof(MAPmonde1[0]);


    Camera2D camera = { 0 };


    Camera2D cameraMENU = { 0 };



    // Store pointers to the multiple update camera functions
    void (*cameraUpdaters[])(Camera2D*, Player*, EnvItem*, int, float, int, int) = {
        UpdateCameraCenter,
        UpdateCameraCenterInsideMAPmonde1,
        UpdateCameraCenterSmoothFollow,
        UpdateCameraEvenOutOnLanding,
        UpdateCameraPlayerBoundsPush
    };

    int cameraOption = 0;
    int cameraUpdatersLength = sizeof(cameraUpdaters) / sizeof(cameraUpdaters[0]);


    //char a;
    SetTargetFPS(60);
    GameMoment currentScreen = DEBUT;

   
   /* printf("%d", g1.GetTotalLevel());
    g1.SetWorld(1);
    g1.SetCurrentLevel(1);
    g1.SetUnlockLevel(4);
    */

    //Gestion de l'écran 1


    Texture2D button = LoadTexture("../LeProjet/LeProjet/files/img/play.png"); // Load button texture

    // Define frame rectangle for drawing
    float frameHeight = (float)button.height;
    Rectangle sourceRec = { 0, 0, (float)button.width, frameHeight };

    // Define button bounds on screen
    Rectangle btnBounds = { screenWidth / 2.0f - button.width / 2.0f, screenHeight / 2.0f - button.height  / 2.0f, (float)button.width, frameHeight };

    bool btnAction = false;         // Button action should be activated

    Vector2 mousePoint = { 0.0f, 0.0f };
    btnAction = false;

   
    while (!WindowShouldClose())
    {
        //Goal : 3 Screen - mby 4 later about victory 
        
        switch (currentScreen)
        {
        //Page that ask user to press SPACE bar in ordre to go in screen 2
        case DEBUT:
        {

            mousePoint = GetMousePosition();


            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) btnAction = true;
            // Draw
            //----------------------------------------------------------------------------------
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawTexture(button, btnBounds.x, btnBounds.y , WHITE); // Draw button frame
            if (btnAction)
            {
                currentScreen = CHOISIRPARTIE;
                cameraMENU.target = playerMENU.position;
                cameraMENU.offset = { screenWidth / 2.0f, screenHeight / 2.0f };
                cameraMENU.rotation = 0.0f;
                cameraMENU.zoom = 1.0f;
            }
            EndDrawing();
        }
        break;
        //Page that ask user to cloose a LEVEL
        case CHOISIRPARTIE:
        {


            float deltaTime = GetFrameTime();
            UpdateMAPmonde1(&playerMENU, MAPmonde1, envItemsLengthMAPmonde1, deltaTime, &g1);
            cameraUpdaters[cameraOption](&cameraMENU, &playerMENU, MAPmonde1, envItemsLengthMAPmonde1, deltaTime, screenWidth, screenHeight);

            if (IsKeyPressed(KEY_B))
            {
                printf("Position de X: %f \nPosition de Y: %f \n ", playerMENU.position.x, playerMENU.position.y);
            }

            //----------------------------------------------------------------------------------
            // Draw
            //----------------------------------------------------------------------------------
            BeginDrawing();
            ClearBackground(LIGHTGRAY);

            //Affichage des données sur l'écran ( chiant car *char )
            std::string DispCurrentWorld = "Monde : " + std::to_string(g1.GetWorld());
            char const* pchar = DispCurrentWorld.c_str();  //use char const* as target 
            std::string DispCurrentLevel = "Niveau : " + std::to_string(g1.GetCurrentLevel());
            char const* pchar2 = DispCurrentLevel.c_str();  //use char const* as target type

            //printf("%d", g1.GetTotalLevel
            DrawText(pchar, 5, 0, 30, BLUE);
            DrawText(pchar2, 5, 40, 30, BLUE);

            BeginMode2D(cameraMENU);
            for (int i = 0; i < envItemsLengthMAPmonde1; i++) DrawRectangleRec(MAPmonde1[i].rect, MAPmonde1[i].color);
            Rectangle playerRect = { playerMENU.position.x - 20, playerMENU.position.y - 40, 40, 40 };
            DrawRectangleRec(playerRect, RED);
            EndMode2D();
            if (IsKeyPressed(KEY_ENTER))
            {
                currentScreen = ENJEU;
                camera.target = playerMENU.position;
                camera.offset = { screenWidth / 2.0f, screenHeight / 2.0f };
                camera.rotation = 0.0f;
                camera.zoom = 1.0f;
            }
            EndDrawing();
        }
        break;
        //PLAY the choosen level
        case ENJEU:
        {
            float deltaTime = GetFrameTime();
            UpdatePlayer(&player, envItems, envItemsLength, deltaTime);
            cameraUpdaters[cameraOption](&camera, &player, envItems, envItemsLength, deltaTime, screenWidth, screenHeight);

            if (IsKeyPressed(KEY_R))
            {
                camera.zoom = 1.0f;
                player.position = { 20, 0 };
            }
            if (player.position.y > 200)
            {
                DrawText("LOST", 100, 100, 100, DARKGRAY);
            }

            if (IsKeyPressed(KEY_B))
            {
                printf("Position de X: %f \nPosition de Y: %f \n ", player.position.x, player.position.y);
            }
            if (IsKeyPressed(KEY_N))
            {
                currentScreen = CHOISIRPARTIE;
                cameraMENU.target = playerMENU.position;
                cameraMENU.offset = { screenWidth / 2.0f, screenHeight / 2.0f };
                cameraMENU.rotation = 0.0f;
                cameraMENU.zoom = 1.0f;
            }
            //----------------------------------------------------------------------------------
            // Draw
            //----------------------------------------------------------------------------------
            BeginDrawing();
            ClearBackground(LIGHTGRAY);
            BeginMode2D(camera);
            for (int i = 0; i < envItemsLength; i++) DrawRectangleRec(envItems[i].rect, envItems[i].color);
            Rectangle playerRect = { player.position.x - 20, player.position.y - 40, 40, 40 };
            DrawRectangleRec(playerRect, RED);
            EndMode2D();
            DrawText("Controls:", 20, 20, 10, BLACK);
            DrawText("- Right/Left to move", 40, 40, 10, DARKGRAY);
            DrawText("- Space to jump", 40, 60, 10, DARKGRAY);
            DrawText("- Mouse Wheel to Zoom in-out, R to reset zoom", 40, 80, 10, DARKGRAY);
            EndDrawing();


        }
        break;
        }


    }

    CloseWindow();        // Close window and OpenGL context
    return 0;
}


void UpdateMAPmonde1(Player* player, EnvItem* envItems, int envItemsLength, float delta, Game* g1)
{
    int framesCounter = 0;


    if (IsKeyPressed(KEY_LEFT))
    {
        if (g1->GetCurrentLevel() > 1)
        {

            player->position.x = player->position.x - 300;
            g1->SetCurrentLevel(g1->GetCurrentLevel() - 1);
            printf("%d \n", g1->GetCurrentLevel());
        }
        else printf("\n Tamer \n");
    }


    if (IsKeyPressed(KEY_RIGHT))
    {

        if (std::exp(g1->GetCurrentLevel()) < std::exp(g1->GetUnlockLevel()))
        {

            player->position.x = 20 + (g1->GetCurrentLevel() - 1) * 300 + 300;
            g1->SetCurrentLevel(g1->GetCurrentLevel() + 1);
            printf("%d \n", g1->GetCurrentLevel());
        }
    }
}