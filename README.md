# Visual Image Retrieval

A collection of computer vision labs implementing image retrieval techniques using OpenCV and C++.

## Requirements

- CMake 3.10+
- OpenCV (installed at `C:/opencv/build/x64/vc16/lib`)
- C++20 compatible compiler

## Build

```bash
cd <Lab>/Source
cmake -B build
cmake --build build --config Release
```

## Projects

### Lab 1 - Video Capture & Histogram Visualization

Captures video from a file or webcam and displays a real-time color histogram (BGR channels) alongside each frame.

```bash
./21127469                      # from default camera
./21127469 -path=<video_path>   # from video file
./21127469 -camid=<id>          # specify camera ID
```

### Lab 2 - Feature Detection (SIFT & ORB)

Detects and visualizes keypoints in an image using SIFT and ORB algorithms. Draws keypoints on the image and saves results as `<image_path>_sift.jpg` and `<image_path>_orb.jpg`.

```bash
./21127469 -path=<image_path>               # detect and save
./21127469 -path=<image_path> -showOutput   # detect, save, and display
```

### Lab 3 - Visual Bag of Words

Implements a Visual Bag of Words (BoVW) pipeline for image retrieval. Extracts SIFT descriptors from a folder of images and clusters them using K-Means to build a visual vocabulary.

```bash
./21127469 -e=<image_folder>              # extract features
./21127469 -t=<feature_folder> -k=100     # train visual vocabulary
```

## Author

Student ID: 21127469
