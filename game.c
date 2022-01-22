// game.с
// в данном файле реализована логика игры
#include "game.h"


/*
дл€ сохранени€ масштаба осей 1:1, оставим наименьшуую ось Y без изменений [-1.0, 1.0]
дл€ оси X введем нувые границы коэфициентом fWidth, котторый равен отношению размеров окна
*/
extern float fWidth;
extern float fHeight;

extern BALL ball;
extern float netHeight;
extern PLAYER player1, player2;


void drawCircle()
{
    const int n = 16;
    float x, y;

    glBegin(GL_POLYGON);
    for (int i = 0; i < n; i++)
    {
        x = cos(M_PI * 2.0 / n * i);
        y = sin(M_PI * 2.0 / n * i);
        glVertex2f(x, y);
    }
    glEnd();
}

void ballInit(BALL* ball, float x, float y, float dx, float dy, float r, float gravity)
{
    ball->x = x;
    ball->y = y;
    ball->dx = dx;
    ball->dy = dy;
    ball->r = r;
    ball->gravity = gravity;
}

void ballShow(BALL* ball)
{
    glPushMatrix();

    glTranslatef(ball->x, ball->y, 0);
    glScalef(ball->r, ball->r, 0);
    drawCircle();

    glPopMatrix();
}

BOOL isBallCross(BALL* ball, float x, float y)
{
    return (ball->x - x) * (ball->x - x) + (ball->y - y) * (ball->y - y) < (ball->r) * (ball->r);
}

void mirrorDirection(BALL* ball, float x, float y, float dPlayer)
{
    float ballAngle = atan2f(ball->dx, ball->dy);
    float crossAngle = atan2f(ball->x - x, ball->y - y);

    float resAngle = dPlayer ? crossAngle : M_PI - ballAngle + 2.0 * crossAngle;
    float dLength = dPlayer ? dPlayer : sqrtf((ball->dx) * (ball->dx) + (ball->dy) * (ball->dy));

    ball->dx = dLength * sinf(resAngle);
    ball->dy = dLength * cosf(resAngle);
}

void ballReflect(float* a, float* da, BOOL condition, float border)
{
    if (!condition) return;

    *da *= -0.85;
    *a = border;
}

void ballMove(BALL* ball)
{
    ball->x += ball->dx;
    ball->y += ball->dy;

    // столкновение с левой грницой
    ballReflect(&ball->x, &ball->dx, (ball->x < -fWidth + ball->r), -fWidth + ball->r);

    // столкновение с правой грницой
    ballReflect(&ball->x, &ball->dx, (ball->x > fWidth - ball->r), fWidth - ball->r);

    // столкновение с нижнией границой
    ballReflect(&ball->y, &ball->dy, (ball->y < -fHeight + ball->r), -fHeight + ball->r);
    ball->dy -= ball->gravity;

    // столкновение с сеткой
    if (ball->y < -fHeight + netHeight)
    {
        // столкновение справа
        if (ball->x > 0)
        {
            ballReflect(&ball->x, &ball->dx, (ball->x < ball->r), ball->r);
        }
        // столкновение слева
        else
        {
            ballReflect(&ball->x, &ball->dx, (ball->x > -ball->r), -ball->r);
        }
    }
    // столкновение с верхней точкой сетки
    else if (isBallCross(ball, 0.0, -fHeight + netHeight))
    {
        mirrorDirection(ball, 0.0, -fHeight + netHeight, 0);
    }
}

void playerInit(PLAYER* player, float x, float y, float r)
{
    player->x = x;
    player->y = y;
    player->r = r;
}

void playerShow(PLAYER* player)
{
    glPushMatrix();

    glTranslatef(player->x, player->y, 0);
    glScalef(player->r, player->r, 0);
    drawCircle();

    glPopMatrix();
}

void playerMove(PLAYER* player, char keyLeft, char keyRight, float borderLeft, float borderRight)
{
    if (GetKeyState(keyLeft) < 0) player->x -= 0.04;
    else if (GetKeyState(keyRight) < 0) player->x += 0.04;

    if (player->x - player->r < borderLeft) player->x = borderLeft + player->r;
    if (player->x + player->r > borderRight) player->x = borderRight - player->r;

    ball.r += player->r;
    if (isBallCross(&ball, player->x, player->y))
    {
        mirrorDirection(&ball, player->x, player->y, 0.07);
        ball.dy += 0.01;
    }
    ball.r -= player->r;

}

void gameInit()
{
    fWidth = (float)W_WIDTH / W_HEIGHT;
    fHeight = 1.0;

    ballInit(&ball, 0.5, 1, 0, 0, 0.2, 0.002);

    netHeight = 1.0;

    playerInit(&player1, -fWidth / 2.0, -fHeight + 0.2, 0.2);
    playerInit(&player2, fWidth / 2.0, -fHeight + 0.2, 0.2);

    glScalef(1 / fWidth, 1, 1); // измен€ем масштаб по оси x
}

void gameShow()
{
    // задний фон
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0, 0.75, 0);
    glBegin(GL_POLYGON);
    glVertex2f(-fWidth, -fHeight);
    glVertex2f(fWidth, -fHeight);
    glVertex2f(fWidth, fHeight / 4.0);
    glVertex2f(-fWidth, fHeight / 4.0);
    glEnd();


    // м€чик
    glColor3f(1, 1, 0.1);
    ballShow(&ball);


    // сетка
    glColor3f(1, 1, 1);
    glLineWidth(8);
    glBegin(GL_LINES);
    glVertex2f(0, -fHeight);
    glVertex2f(0, -fHeight + netHeight);
    glEnd();


    // игроки
    glColor3f(0, 0, 1);
    playerShow(&player1);

    glColor3f(1, 0, 0);
    playerShow(&player2);
}