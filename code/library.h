#ifndef LIBRARY_H
#define LIBRARY_H

// 定义宏来区分导出和导入
#ifdef LIBRARY_EXPORTS
#define TRANSPARENTWINDOW_API __declspec(dllexport)
#else
#define TRANSPARENTWINDOW_API __declspec(dllimport)
#endif

extern "C" {
// 声明导出函数
TRANSPARENTWINDOW_API void CreateTransparentWindow(char title[],int x, int y, int width, int height);
TRANSPARENTWINDOW_API void DestroyTransparentWindow();
}

#endif // LIBRARY_H