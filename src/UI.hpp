#ifndef UI_H
#define UI_H

#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include <opencv2/opencv.hpp>

/*用来把cv::Mat转换成OpenGL纹理的类，整理以避免内存泄漏*/
class Texture {
public:
    Texture() : textureID(0) {}

    // 构造函数：通过 cv::Mat 创建纹理
    explicit Texture(const cv::Mat& mat) {
        loadFromMat(mat);
    }

    // 析构函数：释放纹理资源
    ~Texture() {
        release();
    }

    // 从 cv::Mat 加载纹理数据
    void loadFromMat(const cv::Mat& mat) {
        // 如果已有纹理，先释放旧的资源
        release();
        
        // 生成并绑定新纹理
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // 上传纹理数据
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mat.cols, mat.rows, 0, 
                     mat.channels() == 3 ? GL_BGR : GL_LUMINANCE, GL_UNSIGNED_BYTE, mat.data);

        // 设置纹理参数
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0); // 解绑
    }

    // 获取纹理 ID
    GLuint getID() const {
        return textureID;
    }

    // 显式释放纹理资源
    void release() {
        if (textureID != 0) {
            glDeleteTextures(1, &textureID);
            textureID = 0;
        }
    }

private:
    GLuint textureID;  // 存储纹理 ID
};


class ImGuiApp {
public:
    Texture Cam_frame[2];   //双目相机捕获到的帧，0-左，1-右
    std::array<const char*, 5> uart_device{{"/dev/ttyACM0", "/dev/ttyACM1"}};
    int uart_device_id = 0;
    std::array<const char*, 5> real_sense_device{{"D430", "D435"}};
    int real_sense_device_id = 0;
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