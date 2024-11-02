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
    //预载选择标签（test）
    imu_device.addItem("/dev/ttyACM0");
    imu_device.addItem("/dev/ttyACM1");
    realsense_device.addItem("D430");
    realsense_device.addItem("D435");
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

//打开文件按钮
void ImGuiApp::openfile(const char *name)
{
    if (ImGui::Button(name)){
        ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".cpp,.h,.hpp");
        ImGui::SetNextWindowSize(ImVec2(700, 500), ImGuiCond_FirstUseEver);
    }

    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
            // Output the selected file path to the console
            std::cout << "Selected file: " << filePathName << std::endl;
            std::cout << "Directory: " << filePath << std::endl;
        }
        ImGuiFileDialog::Instance()->Close();
    }
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
    ImGui::SetColumnWidth(1, 624); 
    ImGui::BeginChild("slan_3d_area", ImVec2(0, 400), false);
    cv::Mat img; // vslam地图，临时占位
    Texture img_texture(img);
    ImGui::Image((ImTextureID)(intptr_t)img_texture.getID(), ImVec2(600, 400));
    ImGui::EndChild();

        // 创建子列
        ImGui::BeginChild("sub_area", ImVec2(0, 180), false);
        ImGui::Columns(2, "sub_split_layout", false); 
        ImGui::SetColumnWidth(0, 400); // 第一个子列宽度
        // 子列1内容
        imu_device.show("IMU Device");
        realsense_device.show("RealSense Device");
        if(ImGui::Button("Record")) printf("Reaord pressed, label%s\n", imu_device.getCurrentItem().c_str());
        if(ImGui::Button("Start")) printf("Start Pressed, label%s\n", realsense_device.getCurrentItem().c_str());
        openfile("open file");
        // 子列2内容：IMU数据
        ImGui::NextColumn(); 
        ImGui::SetColumnWidth(1, 200); 
        ImGui::Text("Yaw:   %f", 1.0);
        ImGui::Text("Pitch: %f", 1.0);
        ImGui::Text("Roll:  %f", 1.0);
        ImGui::Text("a_X:   %f", 1.0);
        ImGui::Text("a_Y:   %f", 1.0);
        ImGui::Text("a_Z:   %f", 1.0);
        ImGui::Columns(1, "sub_split_layout");
        ImGui::EndChild();

    ImGui::Columns(1, "main_split_layout");
}