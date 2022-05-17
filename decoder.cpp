#include <cstdio>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include "tree.h"

using namespace std;

int main(int argc, char **argv) {
    FILE *file = fopen(argv[1], "r");
    Tree *tree = new Tree();
    Tree::read(tree, file);
    fclose(file);

    Mat output = Tree::decompress(tree, argv[3]);

    vector<int> params;
    params.push_back(cv::IMWRITE_PNG_COMPRESSION);
    params.push_back(0);
    imwrite(argv[2], output, params);

    delete tree;
}