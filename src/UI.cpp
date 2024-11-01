#include "UI.hpp"

int UI_windows_task();

int UI_windows_task() {
    try {
        ImGuiApp app(1024, 600, "vslam gui");
        return app.run();
    } catch (const std::exception& e) {
        fprintf(stderr, "Error: %s\n", e.what());
        return -1;
    }
}


ImGuiApp::ImGuiApp(int width, int height, const char* title) {
    // 初始化GLFW
    if (!glfwInit()) throw std::runtime_error("Failed to initialize GLFW");

    // 创建OpenGL上下文
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window);

    // 初始化ImGui
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    //读取图片（test)
    cv::Mat image = cv::imread("../test/test1.jpg", cv::IMREAD_COLOR);
    Cam_frame[0].loadFromMat(image);
    cv::Mat image2 = cv::imread("../test/test1.jpg", cv::IMREAD_COLOR);
    Cam_frame[1].loadFromMat(image2);
}

ImGuiApp::~ImGuiApp() {
    // 清理
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}

int ImGuiApp::run() {
    while (!glfwWindowShouldClose(window)) {
        // 渲染新的帧
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        //窗口属性设置
        ImGui::SetNextWindowPos(ImVec2(0, 0)); // 设置窗口位置为主窗口的左上角
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize); // 设置窗口大小为主窗口大小

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove | 
                                        ImGuiWindowFlags_NoResize | 
                                        ImGuiWindowFlags_NoCollapse | 
                                        ImGuiWindowFlags_NoTitleBar | 
                                        ImGuiWindowFlags_NoBringToFrontOnFocus;

        // 渲染窗口内容
        ImGui::Begin("VSLAM GUI", nullptr, window_flags);
        WindowsDefine();
        ImGui::End();

        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
    return 0;
}


void ImGuiApp::WindowsDefine() {
    // 创建两列布局
    ImGui::Columns(2, "main_split_layout", false);

    // 第一列：相机画面
    ImGui::SetColumnWidth(0, 400);
    ImGui::Image((ImTextureID)(intptr_t)Cam_frame[0].getID(), ImVec2(384, 288));
    ImGui::Image((ImTextureID)(intptr_t)Cam_frame[1].getID(), ImVec2(384, 288));

    // 第二列
    ImGui::NextColumn(); 
    ImGui::SetColumnWidth(1, 624); // 设置第二列宽度，剩余的宽度
    cv::Mat img; // vslam地图，临时占位
    Texture img_texture(img);
    ImGui::Image((ImTextureID)(intptr_t)img_texture.getID(), ImVec2(600, 400));

        // 创建子列
        ImGui::BeginChild("sub_area", ImVec2(0, 180), false);
        ImGui::Columns(2, "sub_split_layout", false); 
        ImGui::SetColumnWidth(0, 400); // 第一个子列宽度
        // 子列1内容
        if(ImGui::BeginCombo("IMU device", uart_device[uart_device_id]))
        {
            for (int i = 0; i < uart_device.size(); i++) {
                // 判断选项是否被选中
                bool is_selected = (uart_device_id == i);
                // 如果选项被点击，更新当前选中项索引
                if (ImGui::Selectable(uart_device[i], is_selected))
                    uart_device_id = i;

                // 设置选项为默认选中状态，以便在下拉清单中高亮显示
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        // 子列2内容：IMU数据
        ImGui::NextColumn(); 
        ImGui::SetColumnWidth(1, 200); 
        ImGui::Text("Yaw: %f", 1.0);
        ImGui::Text("Pitch: %f", 1.0);
        ImGui::Text("Roll: %f", 1.0);
        ImGui::Text("a_X: %f", 1.0);
        ImGui::Text("a_Y: %f", 1.0);
        ImGui::Text("a_Z: %f", 1.0);
        ImGui::Columns(1, "sub_split_layout");
        ImGui::EndChild();

    ImGui::Columns(1, "main_split_layout");
}