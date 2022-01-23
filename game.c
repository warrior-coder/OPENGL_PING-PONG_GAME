// game.с
// в данном файле реализована логика игры "Пинг-Понг"
#include "game.h"


// определение глобальных переменных
extern float fWidth, fHeight;
extern float netHeight;

extern BALL ball;
extern PLAYER player1, player2;


// реализация функций
void drawCircle(float x, float y, int vertexes)
{
    // чтобы создать эффект круга, нарисуем правильный моногоугольник с большим количеством вершин
    glBegin(GL_POLYGON);
    for (int i = 0; i < vertexes; i++)
    {
        glVertex2f(
            x + cos(M_PI * 2.0 / vertexes * i),
            y + sin(M_PI * 2.0 / vertexes * i)
        );
    }
    glEnd();
}

BOOL isCircleCross(float cx, float cy, float cr, float x, float y)
{
    return (cx - x) * (cx - x) + (cy - y) * (cy - y) < (cr) * (cr);
}

void ballInit(float x, float y, float dx, float dy, float r, float gravity)
{
    ball.x = x;
    ball.y = y;
    ball.dx = dx;
    ball.dy = dy;
    ball.r = r;
    ball.gravity = gravity;
}

void ballShow()
{
    glPushMatrix();

    glTranslatef(ball.x, ball.y, 0);
    glScalef(ball.r, ball.r, 0);
    drawCircle(0.0, 0.0, 16);

    glPopMatrix();
}

void ballMirrorDirection(float x, float y, float dPlayer)
{
    float crossAngle = atan2f(ball.x - x, ball.y - y);
    float resAngle;
    float dLength;

    if (dPlayer)
    {
        resAngle = crossAngle;
        dLength = dPlayer;
    }
    else
    {
        float ballAngle = atan2f(ball.dx, ball.dy);
        
        resAngle = M_PI - ballAngle + 2.0 * crossAngle;
        dLength = sqrtf((ball.dx) * (ball.dx) + (ball.dy) * (ball.dy));
    }

    ball.dx = dLength * sinf(resAngle);
    ball.dy = dLength * cosf(resAngle);
}

void ballReflect(float* a, float* da, BOOL condition, float reflectedPosition)
{
    if (condition)
    {
        *da *= -0.8; // при отражении мячик теряет часть скорости
        *a = reflectedPosition; // присваиваем отраженную позтцию, чтобы мячик не вышел за границу
    }
}

void ballMove()
{
    // изменяем координаты на величину скорости
    ball.x += ball.dx;
    ball.y += ball.dy;

    // столкновение с левой грницой
    ballReflect(&ball.x, &ball.dx, (ball.x < -fWidth + ball.r), -fWidth + ball.r);

    // столкновение с правой грницой
    ballReflect(&ball.x, &ball.dx, (ball.x > fWidth - ball.r), fWidth - ball.r);

    // столкновение с нижнией границой
    ballReflect(&ball.y, &ball.dy, (ball.y < -fHeight + ball.r), -fHeight + ball.r);

    // столкновение с сеткой
    if (ball.y < -fHeight + netHeight)
    {
        // с правой стороны
        if (ball.x > 0)
        {
            ballReflect(&ball.x, &ball.dx, (ball.x < ball.r), ball.r);
        }
        // с левой стороны 
        else
        {
            ballReflect(&ball.x, &ball.dx, (ball.x > -ball.r), -ball.r);
        }
    }
    // столкновение с верхней точкой сетки
    else if (isCircleCross(ball.x, ball.y, ball.r, 0.0, -fHeight + netHeight))
    {
        ballMirrorDirection(0.0, -fHeight + netHeight, 0);
    }

    // изменяем вертикальную скорость на величину гравитации, тем самым создаем эффект падения
    ball.dy -= ball.gravity;
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
    drawCircle(0.0, 0.0, 16);

    glPopMatrix();
}

void playerMove(PLAYER* player, char keyLeft, char keyRight, float borderLeft, float borderRight)
{
    // передвижение игрока по нажатию клавиш
    if (GetKeyState(keyLeft) < 0) player->x -= 0.04;
    else if (GetKeyState(keyRight) < 0) player->x += 0.04;

    // границы передвижения игрока
    if (player->x - player->r < borderLeft) player->x = borderLeft + player->r;
    if (player->x + player->r > borderRight) player->x = borderRight - player->r;

    // пересечение мячика с игроком
    if (isCircleCross(ball.x, ball.y, ball.r + player->r, player->x, player->y))
    {
        ballMirrorDirection(player->x, player->y, 0.07);
        
        if (ball.y <= -fHeight + ball.r)
        {
            ball.dy = 0.02; // поднятие мячика с пола
        }
    }
}

void gameInit()
{
    fHeight = 1.0; // чтобы сохраненить масштаба осей 1:1, оставим наименьшуую ось Y без изменений с границой 1.0
    fWidth = (float)W_WIDTH / W_HEIGHT; //для оси X введем новую границу fWidth, равную отношению размеров окна
    netHeight = 1.0; // высота сетки относительно нижней границы
    
    ballInit(0.5, 1, 0, 0, 0.15, 0.0015 );

    playerInit(&player1, -fWidth / 2.0, -fHeight + 0.2, 0.2);
    playerInit(&player2, fWidth / 2.0, -fHeight + 0.2, 0.2);

    glScalef(1 / fWidth, 1, 1); // изменяем масштаб по оси x для согранения масштаба осей
}

void gameShow()
{
    // отрисовка заднего фона
    glClearColor(0, 0.75, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.5, 0.9, 1.0);
    glBegin(GL_QUADS);
        glVertex2f(-fWidth, fHeight / 4.0);
        glVertex2f(fWidth, fHeight / 4.0);
        glVertex2f(fWidth, fHeight);
        glVertex2f(-fWidth, fHeight);
    glEnd();

    // отрисовка мячика
    glColor3f(1, 0.95, 0.1);
    ballShow();

    // отрисовка сетки
    glColor3f(1, 1, 1);
    glLineWidth(8);
    glBegin(GL_LINES);
        glVertex2f(0, -fHeight);
        glVertex2f(0, -fHeight + netHeight);
    glEnd();

    // отрисовка игроков
    glColor3f(0, 0, 1);
    playerShow(&player1);

    glColor3f(1, 0, 0);
    playerShow(&player2);
}