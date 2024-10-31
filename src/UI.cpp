#include "UI.hpp"

int UI_windows_task();

int UI_windows_task() {
    try {
        ImGuiApp app(800, 600, "vslam gui");
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

        // 渲染窗口内容
        ImGui::Begin("VSLAM GUI");
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
    ImGui::Text("Hello, world %d", 114514);
    if (ImGui::Button("Save"))
        printf("svae pressed, %s, %f\n", buf, f);
    ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));
    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);    
}