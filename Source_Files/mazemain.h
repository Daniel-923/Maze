#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
#include <time.h>
#include <stdio.h>
#include <Windows.h>
#include <math.h>
#include <stack>
#include <list>
#include <string>
#include <process.h>


/* enum Dir {
    UP, DOWN, LEFT, DOWN, NONE
}; */

#define NONE -1
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

#define EMPTY ' '
#define BOUNDARY '@'
#define WALL '#'
#define FINISH 'F'

using namespace std;

// struct
struct _SPlayerLoc
{
    float x;
    float y;
} _sPlayerLoc;

struct _SDestination
{
    float x;
    float y;
    int size;
} _sDestination;

stack<_SPlayerLoc> xyStack;
wstring map;



int screenWidth = 140; // Console Screen Size X (columns)
int screenHeight = 65; // Console Screen Size Y (rows)
int mapWidth = 30;     // World Dimensions
int mapHeight = 30;

int mapFloor = 1; //층수 변수

float fPlayerX = 1.00; // Player Start Position
float fPlayerY = 1.00;
float fPlayerA = 1.42f;       // Player Start Rotation
float fFOV = 3.14159f / 3.0f; // Field of View
float fDepth = 30.0f;         // Maximum rendering distance
float fSpeed = 3.0f;          // Walking Speed

bool showMinimap = true;
//bool nextFloor = true;



void initMap();
void createPath();
void createMap();
void configureCommandConsole();
void setPlayerXY(float x, float y);
void changeChar();
void drawMap();
void setDestination(int x, int y, int size);
void getDestination();

_SPlayerLoc setStructXY(float x, float y);

int getDirection();
int getDirX(int x, int i);
int getDirY(int y, int i);
int mapXY(int x, int y);
int run();
int inputUser(float fElapsedTime);