cmake_minimum_required(VERSION 3.10)
project(VSLAM_Dev)

# 找到 RealSense2 和 OpenCV 库
find_package(realsense2 REQUIRED)
find_package(OpenCV REQUIRED)

# 包含头文件路径
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${realsense2_INCLUDE_DIR} ${OpenCV_INCLUDE_DIRS})

# 添加可执行文件
file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "src/*.hpp")
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# 链接 RealSense2 和 OpenCV 库
target_link_libraries(${PROJECT_NAME} ${realsense2_LIBRARY} ${OpenCV_LIBS})

