#include "library.h"
#include <windows.h>

// 窗口过程函数声明
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// 全局变量
HWND g_hwnd = nullptr;

void CreateTransparentWindow(char title[],int x, int y, int width, int height) {
    const char CLASS_NAME[] = "TransparentWindowClass";

    // 初始化窗口类结构体
    WNDCLASS wc = {};
    wc.lpfnWndProc   = WndProc;         // 窗口过程函数
    wc.hInstance     = GetModuleHandle(nullptr);  // 实例句柄
    wc.lpszClassName = CLASS_NAME;      // 窗口类名

    // 注册窗口类
    if (!RegisterClass(&wc)) {
        return;
    }

    // 创建窗口
    g_hwnd = CreateWindowEx(
            WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW,                  // 扩展样式
            CLASS_NAME,                                                        // 窗口类名
            title,                                          // 窗口标题
            WS_POPUP | WS_VISIBLE,                                              // 窗口样式
            x, y,                                                                      // 初始位置
            width, height,                                             // 初始大小
            nullptr,                                                       // 父窗口句柄
            nullptr,                                                          // 菜单句柄
            GetModuleHandle(nullptr),                         // 应用程序实例句柄
            nullptr                                                         // 创建参数
    );

    if (g_hwnd == nullptr) {
        return;
    }

    // 设置窗口透明度
    SetLayeredWindowAttributes(g_hwnd, 0, 1, LWA_ALPHA);

    // 设置窗口显示亲和性
    SetWindowDisplayAffinity(g_hwnd, WDA_MONITOR);

    // 设置窗口始终在最前面
    SetWindowPos(g_hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void DestroyTransparentWindow() {
    if (g_hwnd != nullptr) {
        DestroyWindow(g_hwnd);
        g_hwnd = nullptr;
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static bool isExcludedFromCapture = true;

    // 获取扩展样式
    LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);

    switch (uMsg) {
        case WM_DESTROY:
            // 发送退出消息
            PostQuitMessage(0);
            return 0;

        case WM_PAINT: {
            // 处理绘制消息
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
            EndPaint(hwnd, &ps);
        }
            return 0;

        case WM_DISPLAYCHANGE:
            // 检测显示模式变化
            if (isExcludedFromCapture) {
                // 取消窗口透明度
                SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);
                isExcludedFromCapture = false; // 重置标志
            }
            return 0;

        case WM_WINDOWPOSCHANGING:
            // 检查窗口是否设置了 WS_EX_LAYERED 样式
            if (exStyle & WS_EX_LAYERED) {
                isExcludedFromCapture = true;
            }
            return 0;

        default:
            // 默认处理其他消息
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}