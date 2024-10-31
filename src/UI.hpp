#ifndef UI_H
#define UI_H

#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

class ImGuiApp {
public:
    ImGuiApp(int width, int height, const char* title);
    ~ImGuiApp();
    int run();

private:
    void WindowsDefine();  // 用于定义窗口内容的函数
    GLFWwindow* window;

    //test
    float f;
    char buf[128];
};


extern int UI_windows_task();

#endif