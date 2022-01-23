// game.h
// в данном файле описана логика игры "Пинг-Понг"
#ifndef _GAME_H_
#define _GAME_H_


#include <windows.h>
#include <gl/gl.h>
#define _USE_MATH_DEFINES
#include <math.h>

#pragma comment (lib, "opengl32.lib")


// определяем размеры окна
#define W_WIDTH 960
#define W_HEIGHT 640


// структура BALL хранит информацию о мячике
typedef struct _BALL
{
    float x, y;
    float dx, dy;
    float r;
    float gravity;
}BALL;

// структура PLAYER хранит информацию об игроке
typedef struct _PLAYER
{
    float x, y;
    float r;
}PLAYER;


// объявление глобальных переменных (выделение памяти)
float fWidth, fHeight;
float netHeight;

BALL ball;
PLAYER player1, player2;


// прототипы функций
void drawCircle(float, float, int);

BOOL isCircleCross(float, float, float, float, float);

void ballInit(float, float, float, float, float, float);

void ballShow();

void ballMirrorDirection(float, float, float);

void ballReflect(float*, float*, BOOL, float);

void ballMove();

void playerInit(PLAYER*, float, float, float);

void playerShow(PLAYER*);

void playerMove(PLAYER*, char, char, float, float);

void gameInit();

void gameShow();


#endif // _GAME_H_