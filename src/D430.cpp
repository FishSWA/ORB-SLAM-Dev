#include "D430.hpp"
#include <iostream>

//RealSense API
#include <librealsense2/rs.hpp>
//Opencv
#include <opencv2/opencv.hpp>


int D430_main(void);

int D430_main(void)
{

    // Create RealSense pipeline, encapsulating the actual device and sensors
    rs2::pipeline pipe;

    // Create a configuration object
    rs2::config cfg;

    // Enable both infrared streams (left and right) with specific parameters
    cfg.enable_stream(RS2_STREAM_INFRARED, 1, 640, 480, RS2_FORMAT_Y8, 15); // Infrared stream 1 (left)
    cfg.enable_stream(RS2_STREAM_INFRARED, 2, 640, 480, RS2_FORMAT_Y8, 15); // Infrared stream 2 (right)


    // Start streaming with default settings
    pipe.start(cfg);

    // Create an OpenCV window to display depth images
    cv::namedWindow("Depth Image", cv::WINDOW_AUTOSIZE);

    while (true) {
        // Wait for the next set of frames from the camera
        rs2::frameset frames = pipe.wait_for_frames();

        // Get depth frame
        rs2::frame frame = frames.get_infrared_frame(1);

        // Ensure the depth frame is valid
        if (!frame) {
            std::cerr << "No depth frame received!" << std::endl;
            continue;
        }

        // Create OpenCV matrix from depth frame
        const int depth_w = frame.as<rs2::video_frame>().get_width();
        const int depth_h = frame.as<rs2::video_frame>().get_height();
        cv::Mat depth(cv::Size(depth_w, depth_h), CV_16UC1, (void*)frame.get_data(), cv::Mat::AUTO_STEP);
        printf("w:%d, h:%d\n", depth_w, depth_h);

        // Display depth image
        cv::imshow("Depth Image", depth);

        // Break the loop if 'q' is pressed
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }

}