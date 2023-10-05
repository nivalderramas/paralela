//
//  main.cpp
//  VideoDowngrader
//
//  Created by Juan Pablo Garzon Parra on 5/10/23.
//

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;

int main() {
    cv::VideoCapture cap("video.mp4"); // Reemplaza "nombre_del_video.mp4" por la ruta de tu archivo de video.
    
    if (!cap.isOpened()) {
        cerr << "Error al abrir el video." << endl;
        return -1;
    }
    // i need to sabe all the frames in a vector


    cv::Mat frame;
    int frameWidth = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int frameHeight = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    int fps = cap.get(cv::CAP_PROP_FPS);
    int frameCount = cap.get(cv::CAP_PROP_FRAME_COUNT);

    cout << "El video tiene " << frameCount << " frames en total." << endl;
    cout << "El video tiene " << frameHeight << " pixeles de alto." << endl;

    vector<cv::Mat> frames;
    while (cap.read(frame)) {
        frames.push_back(frame);
    }

    cout << "Se han leido " << frames.size() << " frames." << endl;

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
