#include <conio.h>
#include <iostream>
#include <filesystem>
#include <string>
#include "utils.h"
namespace fs = std::experimental::filesystem;

#define CHESS_HEIGHT 9
#define CHESS_WIDTH 5

int collectImg(string path) {
    if (!fs::is_directory(path) || !fs::exists(path))
        fs::create_directory(path);

    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cout << "video not open." << std::endl;
        return 1;
    }

    double fps = cap.get(CV_CAP_PROP_FPS);
    cv::Mat frame;
    if (!cap.read(frame)) {
        std::cout << "no video frame" << std::endl;
        return 1;
    }

    char key = 0;
    int count = 0;
    cv::Size size = cv::Size(CHESS_WIDTH, CHESS_HEIGHT);
    while (key != 27) // press ESC to exit
    {
        cv::imshow("video", frame);
        key = cv::waitKey(30);
            if (!cap.read(frame)) {
                continue;
            }

        // Convert to grayscale
        cv::Mat gray;
        cv::cvtColor(frame, gray, CV_BGR2GRAY);
        std::vector<cv::Point2f> corners;
        bool found = cv::findChessboardCorners(gray, size, corners, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE | cv::CALIB_CB_FAST_CHECK);
        if (found) {
            cv::drawChessboardCorners(frame, size, corners, found);
        }

        // press Space to save frame
        if (key == 32 && found) {
            char name[128];
            sprintf_s(name, "%s%02d%s", path.c_str(), count, ".png");
            std::cout << name << std::endl;
            cv::imwrite(name, gray);
            key = 0;
            count++;
        }
    }
    cap.release();
    return 0;
}

int main()
{
    collectImg("data/");
    // cv::Mat frame = cv::imread("data/00.png", -1);
    // int * avgcolor = PerfectReflectionAlgorithm(frame, "data/");
    // cv::Mat show = convertColor(frame, avgcolor);
    // cv::imshow("White balance result", show);
    // cv::waitKey(0);
    int succ = calibration("data/", cv::Size(CHESS_WIDTH, CHESS_HEIGHT), 0.028); // 0.028 meter
    return 0;
}
