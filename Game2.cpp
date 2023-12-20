//
// Created by Will&Carm on 24/11/2023.
// Submission ID is: 66aba218-9f6b-42f7-aa21-14d516ac4fb1

#include <Windows.h>
#include "Game2.h"
#include "stimulateTheGame.h"
#include "PlatformCommon.h"
#include <iostream>


global_variable bool running = true;

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


RenderState renderState;

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
// Create a window class
    const char CLASS_NAME[] = "Sample Window Class";

    WNDCLASS window_class = {};
//    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpszClassName = CLASS_NAME;
    window_class.lpfnWndProc = WndProc;
    window_class.hInstance = hInstance;

// register class
    RegisterClass(&window_class);

// create window
    HWND window = CreateWindow(
            window_class.lpszClassName,
            CLASS_NAME,
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT, CW_USEDEFAULT, 1280 , 720,
            nullptr,
            nullptr,
            hInstance,
            nullptr
            );
//    if (window == nullptr) {
//        return 0;
//    }
//
        HDC hdc = GetDC(window);

    Input input = {};

    f32 deltaTime = 0.016666f;
    LARGE_INTEGER frameBeginTime;
    QueryPerformanceCounter(&frameBeginTime);

    f32 performaceFreq;
    {
        LARGE_INTEGER perf;
        QueryPerformanceFrequency(&perf);
        performaceFreq = (f32)perf.QuadPart;
    }

    while (running) {
        // input
        MSG message = {};

        for (u32 i = 0; i< BUTTON_COUNT; i++) {
            input.buttons[i].change = false;
        }

        while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {

            switch (message.message) {
                case WM_KEYUP:
                case WM_KEYDOWN: {
                    u32 vkCode = (u32)message.wParam;
                    bool isDown = ((message.lParam & (1 << 31)) == 0);

#define process_button(b, vk) \
case vk: {\
input.buttons[b].change = isDown != input.buttons[b].isDown;\
input.buttons[b].isDown = isDown;\
} break; // this is a macro to condense the code so there will not be as many if statements

                    switch (vkCode) {
                        case 0: {
                            process_button(BUTTON_Up, VK_UP)
                            process_button(BUTTON_DOWN, VK_DOWN)
                            process_button(BUTTON_LEFT, VK_LEFT)
                            process_button(BUTTON_RIGHT, VK_RIGHT)
                            process_button(BUTTON_W, 'W')
                            process_button(BUTTON_S, 'S')
                            process_button(BUTTON_ENTER, VK_RETURN)
                            process_button(BUTTON_ESC, VK_ESCAPE)
                        }
                        default: {
                            std::cerr << "Process buttons is broken" << std::endl;
                        }
                    }
                } break;

                default: {
                    TranslateMessage(&message);
                    DispatchMessage(&message);
                }
            }
        }

        // simulate
        simulateGame (&input, deltaTime);


        //render
        StretchDIBits(hdc, 0,0, renderState.bufferWidth, renderState.bufferHeight, 0 , 0,
                      renderState.bufferWidth, renderState.bufferHeight, renderState.bufferMemory,
                      &renderState.bufferBitMapInfo,
                      DIB_RGB_COLORS, SRCCOPY);
        // sending the memory to windows so it can render it

        LARGE_INTEGER frameEndTime;
        QueryPerformanceCounter(&frameEndTime);
        deltaTime = (f32)(frameEndTime.QuadPart - frameBeginTime.QuadPart) / performaceFreq;
//        std::cout << deltaTime << std::endl;
        frameBeginTime = frameEndTime;
        // getting the fps Will print to the screen
    }
    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    // the brains of the gui. this is what keeps the program running
    LRESULT result = 0;
    switch (uMsg) {
        case WM_CLOSE:
        case WM_DESTROY: {
            running = false;
        // used to close the program
        } break;

        case WM_SIZE : {
            // used for the size of the window. keeps the game in the center even if it is changed in size
            RECT rect;
            GetClientRect(hwnd, &rect);
            renderState.bufferWidth = rect.right - rect.left;
            renderState.bufferHeight = rect.bottom - rect.top;

            s32 buffer_size = renderState.bufferWidth * renderState.bufferHeight * sizeof(u32);

            if(renderState.bufferMemory) VirtualFree(renderState.bufferMemory, 0, MEM_RELEASE);
            // what if play changes size again, checking if the pointer has memory in it, so it can free it
            renderState.bufferMemory = VirtualAlloc(nullptr, buffer_size, MEM_COMMIT | MEM_RESERVE,
                                                    PAGE_READWRITE);
            // can use malloc

            renderState.bufferBitMapInfo.bmiHeader.biSize = sizeof (renderState.bufferBitMapInfo.bmiHeader);
            renderState.bufferBitMapInfo.bmiHeader.biWidth = renderState.bufferWidth;
            renderState.bufferBitMapInfo.bmiHeader.biHeight = renderState.bufferHeight;
            renderState.bufferBitMapInfo.bmiHeader.biPlanes = 1; // must be 1 for some reason
            renderState.bufferBitMapInfo.bmiHeader.biBitCount = 32; // 32 bit
            renderState.bufferBitMapInfo.bmiHeader.biCompression = BI_RGB;
            // can be found on the windows gui page bufferbitmapINFO
        } break;

        default: {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
            // if there is a problem it will just return it
        }
    }
    return result;
}