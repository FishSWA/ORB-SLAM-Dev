cmake_minimum_required(VERSION 3.10)
project(VSLAM_Dev)

# 找到 RealSense2 和 OpenCV 库
find_package(realsense2 REQUIRED)
find_package(OpenCV REQUIRED)

# 包含头文件路径
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${realsense2_INCLUDE_DIR} ${OpenCV_INCLUDE_DIRS})

# IMGUI库
# 设置 ImGui 路径，根据实际位置调整
set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/library/imgui")

# 添加 ImGui 源文件
file(GLOB IMGUI_SOURCES
    "${IMGUI_DIR}/*.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"  # 或者其他后端实现
    "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"     # 或者其他平台实现
)

# 将 ImGui 源文件加入工程中
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC "${IMGUI_DIR}")
target_include_directories(imgui PUBLIC "${IMGUI_DIR}/backends")
include_directories(${IMGUI_DIR})
include_directories(${IMGUI_DIR}/backends)



# 链接其他依赖的库（如 OpenGL、GLFW）
find_package(OpenGL REQUIRED)
target_link_libraries(imgui PUBLIC glfw OpenGL::GL)


# 添加可执行文件
file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "src/*.hpp")
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# 链接 RealSense2 和 OpenCV 库
target_link_libraries(${PROJECT_NAME} ${realsense2_LIBRARY} ${OpenCV_LIBS} imgui dl)
