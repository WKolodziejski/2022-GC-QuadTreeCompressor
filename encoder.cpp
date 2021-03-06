#include <cstdio>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include "tree.h"

using namespace cv;
using namespace std;

int sizeLog(int size) {
    int log = 0;

    while (size >>= 1)
        ++log;

    return 2 << log;
}

cv::Mat getSquareImage(const cv::Mat &img) {
    int width = img.cols, height = img.rows;
    int target = width > height ? sizeLog(width) : sizeLog(height);

    cv::Mat square = cv::Mat::zeros(target, target, img.type());

    int max_dim = (width >= height) ? width : height;
    float scale = ((float) target) / max_dim;

    cv::Rect roi;

    if (width >= height) {
        roi.width = target;
        roi.x = 0;
        roi.height = height * scale;
        roi.y = (target - roi.height) / 2;
    } else {
        roi.y = 0;
        roi.height = target;
        roi.width = width * scale;
        roi.x = (target - roi.width) / 2;
    }

    cv::resize(img, square(roi), roi.size());

    return square;
}

int main(int argc, char **argv) {
    Mat img = imread(argv[1], IMREAD_COLOR);
    img = getSquareImage(img);

    int threshold = atoi(argv[3]);
    int block = atoi(argv[4]);

    Tree *tree = Tree::compress(&img, threshold, block);
    FILE *file = fopen(argv[2], "w");
    Tree::save(tree, file);
    fclose(file);

    delete tree;
}
