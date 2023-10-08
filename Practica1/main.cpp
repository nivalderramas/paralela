#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

int main()
{
    VideoCapture cap("video.mp4");

    if (!cap.isOpened())
    {
        cerr << "Error" << endl;
        return -1;
    }

    int frameWidth = cap.get(CAP_PROP_FRAME_WIDTH);
    int frameHeight = cap.get(CAP_PROP_FRAME_HEIGHT);
    int fps = cap.get(CAP_PROP_FPS);

    int newWidth = 640;
    int newHeight = 360;

    VideoWriter outputVideo("nuevo_video.mp4", VideoWriter::fourcc('H', '2', '6', '4'), fps, Size(newWidth, newHeight));

    if (!outputVideo.isOpened())
    {
        cerr << "Error" << endl;
        return -1;
    }
    
    while (1)
    {
        Mat frame;
        cap >> frame;
        if (frame.empty())
            break;

        resize(frame, frame, Size(newWidth, newHeight));

        outputVideo.write(frame);
    }

    cout << "Se ha creado el nuevo video 'nuevo_video.mp4'." << endl;

    cap.release();
    outputVideo.release();
    destroyAllWindows();

    return 0;
}
