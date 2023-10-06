#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    cv::VideoCapture cap("video.mp4");

    if (!cap.isOpened()) {
        cerr << "Error" << endl;
        return -1;
    }

    int frameWidth = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int frameHeight = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    int fps = cap.get(cv::CAP_PROP_FPS);

    cv::VideoWriter outputVideo("nuevo_video.mp4", cv::VideoWriter::fourcc('H', '2', '6', '4'), fps, cv::Size(frameWidth, frameHeight), true);


    if (!outputVideo.isOpened()) {
        cerr << "Error" << endl;
        return -1;
    }

    vector<cv::Mat> frames;
    cv::Mat frame;

    while (cap.read(frame)) {
        frames.push_back(frame);
    }

    // cout << "Se han leído " << frames.size() << " fotogramas." << endl;

    for (const cv::Mat& frame : frames) {
        outputVideo.write(frame);
    }

    cout << "Se ha creado el nuevo video 'nuevo_video.mp4'." << endl;

    cap.release();
    outputVideo.release();
    cv::destroyAllWindows();

    return 0;
}
