// game.h
// в данном файле описана логика игры
#ifndef _GAME_H_
#define _GAME_H_


#include <windows.h>
#include <gl/gl.h>
#define _USE_MATH_DEFINES
#include <math.h>

#pragma comment (lib, "opengl32.lib")


#define W_WIDTH 960
#define W_HEIGHT 640


typedef struct _BALL
{
    float x, y;
    float dx, dy;
    float r;
    float gravity;
}BALL;

typedef struct _PLAYER
{
    float x, y;
    float r;
}PLAYER;


float fWidth;
float fHeight;

BALL ball;
float netHeight;
PLAYER player1, player2;


void drawCircle();

void ballInit(BALL* ball, float x, float y, float dx, float dy, float r, float gravity);

void ballShow(BALL* ball);

BOOL isBallCross(BALL* ball, float x, float y);

void mirrorDirection(BALL* ball, float x, float y, float dPlayer);

void ballReflect(float* a, float* da, BOOL condition, float border);

void ballMove(BALL* ball);

void playerInit(PLAYER* player, float x, float y, float r);

void playerShow(PLAYER* player);

void playerMove(PLAYER* player, char keyLeft, char keyRight, float borderLeft, float borderRight);

void gameInit();

void gameShow();


#endif // _GAME_H_