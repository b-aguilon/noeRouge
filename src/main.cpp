// noeRouge - Terminal rouge-like ascii dungeon crawler game

// Includes
#include <iostream>
#include <vector>
#include <cstdlib>
#include <raylib.h>

// Local includes
#include "object.h"
#include "mapGen.h"

/*
// TODO list (class voted it to be here)
// This list is worse than a todo at each thing because you have to just figure
// out where to work on each item listed here
//
// Doing it this way is objectivly worse in every way and has only
// disadvantages. I will comply but I beg that everyone agrees to 
// do it the other way instead
//
// 00 - Create a "game" class that holds deltatime, the map, the handlers, and
//      the game loop isRunning flag. This object will be global
// 01 - Game loop with time code
*/

typedef enum ObjectTypes { PLAYER, NPC };

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 450;

constexpr int FPS = 60;

constexpr int OBJECT_SPEED = 300;

constexpr Vector2 OBJECT_SIZE = {30, 30};

//this is bad
void spawnObjectOnMap(ObjectHandler& handler, Floor& floor, ObjectTypes type);

int main() 
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "noeRouge alpha v0.1");
    SetTargetFPS(FPS);

    // Print version info
    std::cout << "noeRouge alpha v0.1\n";

    srand(time(0));

        // Create the objectHandler
    class ObjectHandler objectHandler;

        // Create floor, spawn objects in relation to it
    Floor floor = Floor('r');
    std::vector<Rectangle> collidables = floor.getNearCollidables();
    spawnObjectOnMap(objectHandler, floor, PLAYER);
    spawnObjectOnMap(objectHandler, floor, NPC);

        // driver code - just for testing before real driver and drawing code
    while (!WindowShouldClose())
    {
        objectHandler.tickAll(floor);

        BeginDrawing();

        ClearBackground(BLACK);

        objectHandler.renderAll();
            
            //temporary until render code
        for (Rectangle rect : collidables)
        {
            DrawRectangle(rect.x, rect.y, rect.width, rect.height, BLUE);
        }

        EndDrawing();
    }

        //TODO - Deallocate memory

    CloseWindow();

    return 0;
}

//very, very temporary. Goes through map data and as soon is it sees floor, it spawns an object.
void spawnObjectOnMap(ObjectHandler& handler, Floor& floor, ObjectTypes type)
{
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            if (floor.data[x][y] == FLOOR)
            {
                switch (type)
                {
                case PLAYER:
                    handler.createPlayer({ (float)x * TILE_SIZE, (float)y * TILE_SIZE }, OBJECT_SIZE, OBJECT_SPEED);
                    break;
                case NPC:
                    handler.createNPC({ (float)x * TILE_SIZE, (float)y * TILE_SIZE }, OBJECT_SIZE, OBJECT_SPEED);
                    break;
                default:
                    break;
                }

                return;
            }
        }
    }
}