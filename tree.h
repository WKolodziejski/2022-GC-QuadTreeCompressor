#ifndef QUAD_TREE_H

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

#define QUAD_TREE_H

using namespace cv;

class Tree {
private:
    unsigned char rgb[3];
    unsigned int cols, rows; //can be stored as char if value >> 2, since it's a power of 2
    unsigned int x, y;

    Tree *tl = nullptr;
    Tree *tr = nullptr;
    Tree *bl = nullptr;
    Tree *br = nullptr;

    static Tree *_compress(Mat *img, int x, int y, int cols, int rows, int threshold, int block);
    static void _decompress(Mat *img, Tree *tree, bool grid);

public:
    static Tree *compress(Mat *img, int threshold, int block);
    static Mat decompress(Tree *tree, bool grid);
    static void save(Tree *tree, FILE *file);
    static void read(Tree *tree, FILE *file);

    Tree();
};

#endif