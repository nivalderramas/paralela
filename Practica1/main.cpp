#include <iostream>
#include <opencv2/opencv.hpp>
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

  // cv::VideoWriter outputVideo("nuevo_video.mp4",
  //                             cv::VideoWriter::fourcc('H', '2', '6', '4'),
  //                             fps, cv::Size(frameWidth, frameHeight), true);
  int compressedFrameWidth = frameWidth / 3;
  int compressedFrameHeight = frameHeight / 3;

  cv::VideoWriter outputVideo(
      "nuevo_video.mp4", cv::VideoWriter::fourcc('X', '2', '6', '4'), fps,
      cv::Size(compressedFrameWidth, compressedFrameHeight), true);

  if (!outputVideo.isOpened()) {
    cerr << "Error" << endl;
    return -1;
  }

  vector<cv::Mat> frames;
  cv::Mat frame;
  cv::Mat smallFrame(compressedFrameHeight, compressedFrameWidth,
                     CV_8UC3); // Create an empty frame

  while (cap.read(frame)) {
    frames.push_back(frame);
    for (int i = 0; i < compressedFrameHeight; i++) {
      for (int j = 0; j < compressedFrameWidth; j++) {
        int startI = 3 * i;
        int endI = 3 * (i + 1);
        int startJ = 3 * j;
        int endJ = 3 * (j + 1);
        int r, g, b;
        int cnt = 0;
        for (int ii = startI; ii < endI; ii++) {
          for (int jj = startJ; jj < endJ; jj++) {
            cv::Vec3b pixel = frame.at<cv::Vec3b>(ii, jj);
            cnt++;
            r += pixel[0];
            g += pixel[1];
            b += pixel[2];
          }
        }
        cout<<cnt<<endl;
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
