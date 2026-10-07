#include "RandomWalkGenerator.h"
#include "raylib.h"
#include <iostream>
#define CloseWindow CloseWindow_Win32
#define ShowCursor ShowCursor_Win32
#include <windows.h>
#undef ShowCursor
#undef CloseWindow

constexpr int MAP_WIDTH = 64;
constexpr int MAP_HEIGHT = 64;
constexpr int BRUSH_SIZE = 2;

int main(){
    // Set console output and input encoding to UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    RandomWalkGenerator gen;
    int seed;
    std::cout << "Enter a seed: ";
    std::cin >> seed;
    int** map = new int*[MAP_HEIGHT];
    for (int i = 0; i < MAP_HEIGHT; i++){
        map[i] = new int[MAP_WIDTH];
    }
    gen.generate(MAP_WIDTH, MAP_HEIGHT, MAP_WIDTH * MAP_HEIGHT / 4, BRUSH_SIZE, Vector2(MAP_WIDTH / 2, MAP_HEIGHT / 2), map, seed);
    for (int i = 0; i < MAP_HEIGHT; i++){
        for (int j = 0; j < MAP_WIDTH; j++){
            if (map[i][j] == 1){
                std::cout << "██";
            } else {
                std::cout << "  ";
            }
        }
        std::cout << std::endl;
    }
}