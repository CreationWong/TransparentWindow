/**
 * @file TransparentWindow.cpp
 * @version V1.1.0.002
 * @author CreationWong
 * @copyright Copyright © 2024 CreationWong
 */

#include "TransparentWindow.h"
#include <windows.h>

// 窗口过程函数声明
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// 全局变量
HWND hwnd = nullptr;

void CreateTransparentWindow(const char title[], const char CLASS_NAME[], int x, int y, int width, int height) {

    // 初始化窗口类结构体
    WNDCLASS wc = {};
    wc.lpfnWndProc   = WndProc;         // 窗口过程函数
    wc.hInstance     = GetModuleHandle(nullptr);  // 实例句柄
    wc.lpszClassName = CLASS_NAME;      // 窗口类名

    // 注册窗口类
    if (!RegisterClass(&wc)) {
        return ;
    }

    // 创建窗口
    hwnd = CreateWindowEx(
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

    if (hwnd == nullptr) {
        return ;
    }

    // 设置窗口透明度
    // 不要将 bAlpha 设置为 0 ，否则窗口亲和性无效化！
    SetLayeredWindowAttributes(hwnd, 0, 1, LWA_ALPHA);

    // 设置窗口显示亲和性
    SetWindowDisplayAffinity(hwnd, WDA_MONITOR);

    // 设置窗口始终在最前面
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

}

bool DestroyTransparentWindow() {
    if (hwnd != nullptr) {
        DestroyWindow(hwnd);
        hwnd = nullptr;
        return true;
    }
    return false;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static bool isExcludedFromCapture = true;

    // 获取扩展样式
    LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);

    switch (uMsg) {
        case WM_DESTROY:
            // 发送退出消息
            PostQuitMessage(0);
            goto STOP;

        case WM_PAINT: {
            // 处理绘制消息
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
            EndPaint(hwnd, &ps);
        }
            goto STOP;

        case WM_DISPLAYCHANGE:
            // 检测显示模式变化
            if (isExcludedFromCapture) {
                // 取消窗口透明度
                SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);
                isExcludedFromCapture = false; // 重置标志
            }
            goto STOP;

        case WM_WINDOWPOSCHANGING:
            // 检查窗口是否设置了 WS_EX_LAYERED 样式
            if (exStyle & WS_EX_LAYERED) {
                isExcludedFromCapture = true;
            }
            goto STOP;

        default:
            // 默认处理其他消息
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    STOP:
        return 0;
}