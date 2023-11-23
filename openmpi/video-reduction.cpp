#include <bits/stdc++.h>
#include <chrono>
#include <mpi.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
using namespace std;

const int MAX_PROCESS = 10;

// Function to serialize cv::Mat to a vector of bytes
std::vector<uchar> serializeMat(const cv::Mat &image) {
  std::vector<uchar> buffer;
  cv::imencode(".png", image, buffer);
  return buffer;
}

// Function to deserialize vector of bytes to cv::Mat
cv::Mat deserializeMat(const std::vector<uchar> &buffer) {
  return cv::imdecode(buffer, cv::IMREAD_COLOR);
}

int main(int argc, char **argv) {

  MPI_Status status;
  MPI_Request req[MAX_PROCESS];
  int root = 0;

  MPI_Init(&argc, &argv);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int numProcesses;
  MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

  std::string inputVideoFileName = "video.mp4";
  cv::VideoCapture cap(inputVideoFileName);

  if (!cap.isOpened()) {
    std::cerr << "Error opening the video file." << std::endl;
    return -1;
  }

  // Set the starting frame number according to the process we are running
  int fps = cap.get(cv::CAP_PROP_FPS);
  int totalFrames = cap.get(cv::CAP_PROP_FRAME_COUNT);
  int startFrame = totalFrames / numProcesses * rank;
  cap.set(cv::CAP_PROP_POS_FRAMES, startFrame);

  int i = startFrame;
  // Main loop for processing frames
  vector<cv::Mat> newFrames;
  while (true) {
    cv::Mat frame;
    cap >> frame;

    if (frame.empty()) {
      std::cout << "End of video." << std::endl;
      break;
    }

    // create a smaller frame of one third the size of the original frame to
    // reduce the size of the video. Fill it using the average of pixel colors
    // of the original frame
    cv::Mat smallFrame(frame.rows / 3, frame.cols / 3, CV_8UC3);
    for (int i = 0; i < smallFrame.rows; i++) {
      for (int j = 0; j < smallFrame.cols; j++) {
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
        r /= cnt;
        r = min(r, 255);
        g /= cnt;
        g = min(g, 255);
        b /= cnt;
        b = min(b, 255);
        cv::Vec3b pixel;
        pixel[0] = r;
        pixel[1] = g;
        pixel[2] = b;
        smallFrame.at<cv::Vec3b>(i, j) = pixel;
      }
    }

    newFrames.push_back(smallFrame);

    i++;
    if (i == totalFrames / numProcesses * (rank + 1)) {
      break;
    }
  }

  // Gather serialized frames at the root process
  std::vector<std::vector<uchar>> serializedFrames(newFrames.size());
  if (rank == root) {
    serializedFrames.resize(totalFrames);
  }

  // Serialize frames
  for (size_t j = 0; j < newFrames.size(); ++j) {
    serializedFrames[j] = serializeMat(newFrames[j]);
  }

  // Gather serialized frames at the root process
  MPI_Gather(serializedFrames.data(),
             newFrames.size() * serializedFrames[0].size(), MPI_UNSIGNED_CHAR,
             serializedFrames.data(),
             newFrames.size() * serializedFrames[0].size(), MPI_UNSIGNED_CHAR,
             root, MPI_COMM_WORLD);

  if (rank == root) {
    // Deserialize received frames and create the final video
    std::vector<cv::Mat> allFrames;
    for (const auto &serializedFrame : serializedFrames) {
      allFrames.push_back(deserializeMat(serializedFrame));
    }

    // Create the final video using all received frames
    cv::VideoWriter videoWriter("output_video.mp4",
                                cv::VideoWriter::fourcc('X', '2', '6', '4'),
                                fps, cv::Size(1920 / 3, 1080 / 3));
    for (const auto &frame : allFrames) {
      videoWriter.write(frame);
    }
    videoWriter.release();
  }

  // Release the video capture object
  cap.release();

  MPI_Finalize();

  return 0;
}
