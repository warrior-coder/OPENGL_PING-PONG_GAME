#include "game.h"


LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);


// функция входа приложения
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    HDC hdc;
    HGLRC hrc;
    MSG msg;


    // регестрируем класс окна
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = L"className";
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;

    RegisterClassEx(&wc);


    // создаем главное окно
    hwnd = CreateWindowEx(
        0,
        L"className",
        L"Ping-Pong Game",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        960,
        640,
        NULL,
        NULL,
        hInstance,
        NULL
    );


    // показываем окно
    ShowWindow(hwnd, nCmdShow);

    // подключаем OpenGL к окну
    EnableOpenGL(hwnd, &hdc, &hrc);

    
    gameInit();

    // основной цикл программы
    while (TRUE)
    {
        // проверка сообщения
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            // отправка или обработка сообщения
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT) break;
        }
        else
        {
            // двидение мячика
            ballMove(&ball);

            // движение игроков
            playerMove(&player1, 'A', 'D', -fWidth, 0.0);
            playerMove(&player2, VK_LEFT, VK_RIGHT, 0.0, fWidth);

            // отрисовка игры
            gameShow();
            SwapBuffers(hdc);


            Sleep(10);
        }
    }


    // отключаем OpenGL
    DisableOpenGL(hwnd, hdc, hrc);

    // уничтожаем окно
    DestroyWindow(hwnd);


    return 0;
}


// процедура обрабатки сообщений окна
LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_CLOSE) PostQuitMessage(0);
    else if (message == WM_KEYDOWN)
    {
        if (wParam == VK_ESCAPE) PostQuitMessage(0);
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

void EnableOpenGL(HWND hwnd, HDC* hdc, HGLRC* hrc)
{
    PIXELFORMATDESCRIPTOR pfd;
    int fmt;

    // получаем контекст устройства
    *hdc = GetDC(hwnd);

    // установливаем формат пикселей для контекста
    ZeroMemory(&pfd, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    fmt = ChoosePixelFormat(*hdc, &pfd);
    SetPixelFormat(*hdc, fmt, &pfd);


    // создаем и включаем контекст отрисовки
    *hrc = wglCreateContext(*hdc);
    wglMakeCurrent(*hdc, *hrc);
}

void DisableOpenGL(HWND hwnd, HDC hdc, HGLRC hrc)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hrc);
    ReleaseDC(hwnd, hdc);
}