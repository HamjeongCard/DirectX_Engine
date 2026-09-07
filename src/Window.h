#pragma once

#include <windows.h>
#include <string>

class Window {
public:
    Window(HINSTANCE hInstance, const wchar_t* title, int width, int height);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool ProcessMessages();
    HWND GetHWND() const { return m_hWnd; }
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    bool IsClosed() const { return m_isClosed; }

private:
    static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    HINSTANCE m_hInstance;
    HWND m_hWnd;
    int m_width;
    int m_height;
    bool m_isClosed = false;
    static constexpr const wchar_t* CLASS_NAME = L"DX11WindowClass";
};
