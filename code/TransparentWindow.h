/**
 * TransparentWindow 是一个创建防截屏和录屏的隐私防护窗口的 DLL .
 *
 * @file TransparentWindow.h
 * @version V1.1.0
 * @author CreationWong
 * @copyright Copyright © 2024 CreationWong
 *
 * 该库使用 GNU General Public License V3.0 开源协议，开源 TransparentWindow 源代码
 */

#ifndef TRANSPARENTWINDOW_H
#define TRANSPARENTWINDOW_H

// 勿动 勿改
#ifdef LIBRARY_EXPORTS
#define TRANSPARENTWINDOW_API __declspec(dllexport)
#else
#define TRANSPARENTWINDOW_API __declspec(dllimport)
#endif

extern "C" {

/**
 * @brief CreateTransparentWindow 创建窗口函数
 * @version V1.1.1
 *
 * 更新内容：
 * 1.添加窗口类修改变量
 * 2.修改 title 、CLASS_NAME 为 const
 *
 * @param title 窗口名称
 * @param CLASS_NAME 窗口类名称
 * @param x 窗口位置 X 轴
 * @param y 窗口位置 Y 轴
 * @param width 窗口宽度
 * @param height 窗口高度
 *
 */
TRANSPARENTWINDOW_API void
CreateTransparentWindow(const char title[], const char CLASS_NAME[], int x, int y, int width, int height);

/**
 * @brief DestroyTransparentWindow 关闭由 CreateTransparentWindow 创建的窗口
 * @version V1.1.0
 * @return 返回 bool 值    如果关闭失败则返回 false ，否则返回 true
 */
TRANSPARENTWINDOW_API bool DestroyTransparentWindow();

}

#endif // TRANSPARENTWINDOW_H