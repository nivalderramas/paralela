#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <cstdlib>
#include <omp.h>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        cerr << "Uso: " << argv[0] << " <video_entrada.mp4> <video_salida.mp4> <num_hilos>" << endl;
        return -1;
    }

    string videoInputFilename = argv[1];
    string videoOutputFilename = argv[2];
    int numThreads = atoi(argv[3]);

    omp_set_num_threads(numThreads);

    cv::VideoCapture cap(videoInputFilename);

    if (!cap.isOpened())
    {
        cerr << "Error al abrir el video de entrada." << endl;
        return -1;
    }

    int frameWidth = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int frameHeight = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    int fps = cap.get(cv::CAP_PROP_FPS);

    int compressedFrameWidth = frameWidth / 3;
    int compressedFrameHeight = frameHeight / 3;

    cv::VideoWriter outputVideo(
        videoOutputFilename, cv::VideoWriter::fourcc('X', '2', '6', '4'), fps,
        cv::Size(compressedFrameWidth, compressedFrameHeight), true);

    if (!outputVideo.isOpened())
    {
        cerr << "Error al crear el video de salida." << endl;
        return -1;
    }

    vector<cv::Mat> frames;
    cv::Mat frame;
    cv::Mat smallFrame(compressedFrameHeight, compressedFrameWidth, CV_8UC3);

    while (cap.read(frame))
    {
        frames.push_back(frame);
        #pragma omp parallel for collapse(2)
        for (int i = 0; i < compressedFrameHeight; i++)
        {
            for (int j = 0; j < compressedFrameWidth; j++)
            {
                int startI = 3 * i;
                int endI = 3 * (i + 1);
                int startJ = 3 * j;
                int endJ = 3 * (j + 1);
                int r, g, b;
                int cnt = 0;
                for (int ii = startI; ii < endI; ii++)
                {
                    for (int jj = startJ; jj < endJ; jj++)
                    {
                        cv::Vec3b pixel = frame.at<cv::Vec3b>(ii, jj);
                        cnt++;
                        r += pixel[0];
                        g += pixel[1];
                        b += pixel[2];
                    }
                }
                r /= 9;
                g /= 9;
                b /= 9;
                smallFrame.at<cv::Vec3b>(i, j) = cv::Vec3b(r, g, b);
            }
        }
        outputVideo.write(smallFrame);
    }

    cout << "Se ha creado el nuevo video 'nuevo_video.mp4'." << endl;

    cap.release();
    outputVideo.release();
    cv::destroyAllWindows();

    return 0;
}
