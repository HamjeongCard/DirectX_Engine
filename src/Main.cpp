#include <windows.h>
#include <chrono>
#include "Window.h"
#include "Graphics.h"
#include "Cube.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nCmdShow;

    try {
        Window window(hInstance, L"DirectX 11 3D Box Game Framework", 800, 600);
        Graphics graphics(window.GetHWND(), window.GetWidth(), window.GetHeight());
        Cube cube(graphics);

        auto lastTime = std::chrono::high_resolution_clock::now();

        while (window.ProcessMessages()) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            float dt = std::chrono::duration<float>(currentTime - lastTime).count();
            lastTime = currentTime;

            cube.Update(dt);

            graphics.BeginFrame(0.1f, 0.1f, 0.18f);
            cube.Draw(graphics);
            graphics.EndFrame();
        }
    }
    catch (const std::exception& e) {
        MessageBoxA(nullptr, e.what(), "Error", MB_OK | MB_ICONERROR);
        return -1;
    }

    return 0;
}
