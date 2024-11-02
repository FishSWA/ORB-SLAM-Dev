cmake_minimum_required(VERSION 3.10)
project(VSLAM_Dev)

# 查找包
find_package(realsense2 REQUIRED)
find_package(OpenCV REQUIRED)
find_package(OpenGL REQUIRED)

# 包含头文件路径
include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${realsense2_INCLUDE_DIR}
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/library/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/library/imgui/backends
    ${CMAKE_CURRENT_SOURCE_DIR}/library/ImGuiFileDialog-master
)

# 编译和链接ImGui
file(GLOB IMGUI_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/library/imgui/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/library/imgui/backends/imgui_impl_opengl3.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/library/imgui/backends/imgui_impl_glfw.cpp"
)
add_library(imgui STATIC ${IMGUI_SOURCES})
target_link_libraries(imgui PUBLIC glfw OpenGL::GL)

# 编译和链接ImGuiFileDialog
file(GLOB IMGUI_FILE_DIALOG_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/library/ImGuiFileDialog-master/*.cpp")
add_library(imgui_file_dialog STATIC ${IMGUI_FILE_DIALOG_SOURCES})
target_link_libraries(imgui_file_dialog PUBLIC imgui)

# 添加可执行文件
file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "src/*.hpp")
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# 将库链接到可执行文件
target_link_libraries(${PROJECT_NAME} 
    ${realsense2_LIBRARY}
    ${OpenCV_LIBS}
    imgui
    imgui_file_dialog
    glfw
    OpenGL::GL
    dl
)
